/*
 * GOSTunnel SSL tunnel with CryptoAPI support
 * Copyright (c) 2014 Dostovalov Vitaly <mail@dostovalov.tk>
 *               All Rights Reserved
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
 
#include <windows.h>
#include <winsock.h>
#include <schannel.h>
#include <prsht.h>
#include <Cryptuiapi.h>
#define SECURITY_WIN32
#include <security.h>
#include <sspi.h>
#ifndef _WIN32
#include <CSP_SChannel.h>
#endif
#include "WinCryptEx.h"

#include "common.hpp"

#define IO_BUFFER_SIZE  0x10000

#ifndef SEC_I_CONTEXT_EXPIRED
# define SEC_I_CONTEXT_EXPIRED ((HRESULT)0x00090317L)
#endif
#ifndef SEC_E_CONTEXT_EXPIRED
# define SEC_E_CONTEXT_EXPIRED ((HRESULT)0x80090317L)
#endif

static SECURITY_STATUS CreateCredentials(PCredHandle phCreds);

static SECURITY_STATUS PerformClientHandshake(boost::asio::ip::tcp::socket& socket, 
																							PCredHandle phCreds, const std::string&	serverName, 
																							CtxtHandle *phContext);

static SECURITY_STATUS ClientHandshakeLoop(boost::asio::ip::tcp::socket& socket, 
																					 PCredHandle phCreds, CtxtHandle *phContext,
																					 BOOL fDoInitialRead);

static SECURITY_STATUS AuthenticateServerCertificate(CtxtHandle *phContext, 
																										 const std::string& serverName);

static void DisplayCertChain(PCCERT_CONTEXT pServerCert, BOOL fLocal);

static DWORD VerifyServerCertificate(PCCERT_CONTEXT  pServerCert, char *pszServerName, DWORD dwCertFlags);

static void DisplayConnectionInfo(CtxtHandle *phContext);

static void DisplayWinVerifyTrustError(DWORD Status);

static void GetNewClientCredentials(CredHandle *phCreds, CtxtHandle *phContext);

static LONG DisconnectFromServer(boost::asio::ip::tcp::socket& socket, PCredHandle	phCreds, CtxtHandle *phContext);

//-------------------------------------------------------------
// Функция создания мандатов.
static SECURITY_STATUS CreateCredentials(PCredHandle phCreds/*out*/)
{
	TimeStamp tsExpiry;
	SECURITY_STATUS Status;
	HCERTSTORE	hMyCertStore = NULL;
	SCHANNEL_CRED   SchannelCred;

	PCCERT_CONTEXT pCertContext = NULL; // Здесь: клиентский сертификат

	// Открытие хранилища сертификатов "MY"
	hMyCertStore = CertOpenSystemStore(0, "MY");
  if(!hMyCertStore) {
    return SEC_E_CANNOT_INSTALL;
  }

	// Диалог выбора сертификата из списка в хранилище hMyCertStore
	pCertContext = CryptUIDlgSelectCertificateFromStore(
                        hMyCertStore,
                        NULL,
                        NULL,
                        NULL,
                        CRYPTUI_SELECT_LOCATION_COLUMN,
                        0,
                        NULL);
	if(hMyCertStore) {
		CertCloseStore(hMyCertStore, 0);
  }
  if (!pCertContext) {
  	return SEC_E_NOT_SUPPORTED;
  }
	// Построение структуры Schannel мандатов.
	// В данном примере определяются используемые протокол и сертификат.
	ZeroMemory(&SchannelCred, sizeof(SchannelCred));
	SchannelCred.dwVersion = SCHANNEL_CRED_VERSION; // так нужно
	SchannelCred.cCreds = 1; // The number of structures in the paCred array
  SchannelCred.paCred = &pCertContext;
	/* SchannelCred.grbitEnabledProtocols = {
			SP_PROT_PCT1_SERVER, SP_PROT_PCT1_CLIENT,
			SP_PROT_SSL2_SERVER, SP_PROT_SSL2_CLIENT, SP_PROT_SSL3_SERVER,
			SP_PROT_SSL3_CLIENT, SP_PROT_TLS1_SERVER, SP_PROT_TLS1_CLIENT,
			SP_PROT_TLS1_0_SERVER, SP_PROT_TLS1_SERVER, SP_PROT_TLS1_0_CLIENT,
			SP_PROT_TLS1_CLIENT, SP_PROT_TLS1_1_SERVER, SP_PROT_TLS1_1_CLIENT,
			SP_PROT_TLS1_2_SERVER, SP_PROT_TLS1_2_CLIENT } */
	SchannelCred.grbitEnabledProtocols = 0;

	SchannelCred.dwFlags |= SCH_CRED_NO_DEFAULT_CREDS; // Prevent Schannel from attempting to automatically supply a certificate chain for client authentication.

	// Флаг SCH_CRED_MANUAL_CRED_VALIDATION установлен, поскольку
	// в данном примере сертификат сервера проверяется "вручную".
	// TODO: change to SCH_CRED_AUTO_CRED_VALIDATION
	SchannelCred.dwFlags |= SCH_CRED_MANUAL_CRED_VALIDATION;
	
	// Создание SSPI мандата.
  // TODO: add ASC_REQ_MUTUAL_AUTH.
	Status = AcquireCredentialsHandleA(
						NULL, 			 // Name of principal
						UNISP_NAME_A, 	 // Name of package
						SECPKG_CRED_OUTBOUND,   // SECPKG_CRED_INBOUND - для обработки входящих сообщений (сервер),
                                    // SECPKG_CRED_OUTBOUND - для подготовки исходящих сообщений (клиент).
						NULL, 			 // Pointer to logon ID
						&SchannelCred, 	// Package specific data
						NULL, 			 // Pointer to GetKey() func
						NULL, 			 // Value to pass to GetKey()
						phCreds, 			// (out) Cred Handle
						&tsExpiry); 		 // (out) Lifetime (optional) 
	if(Status != SEC_E_OK) {
		return SEC_E_NO_CREDENTIALS;
  }

  CertFreeCertificateContext(pCertContext);
	return Status;
} //CreateCredentials

static SECURITY_STATUS PerformClientHandshake(
	boost::asio::ip::tcp::socket& socket, 	 // in
	PCredHandle phCreds, 	// in
	const std::string& serverName,  // in
	CtxtHandle *phContext) // out
{
	SecBufferDesc OutBuffer;
	SecBuffer OutBuffers[1];
	DWORD	dwSSPIFlags;
	DWORD	dwSSPIOutFlags;
	TimeStamp tsExpiry;
	SECURITY_STATUS scRet;
	DWORD	cbData;

    /*   ISC_REQ_REPLAY_DETECT	        Защита от навязывания повторных пакетов
     *   ISC_REQ_SEQUENCE_DETECT	    Защита от навязывания перестановок пакетов
     *   ISC_REQ_CONFIDENTIALITY	    Конциденциальность (шифрование)
     *   ISC_REQ_STREAM	                Соединение потокового типа
     *   ISC_REQ_ALLOCATE_MEMORY	    SSP отводит память для буферов. Ее необходимо освободить вызовом FreeContextBuffer
     *   ISC_REQ_EXTENDED_ERROR	        Уведомлять другую сторону об ошибках (SSL alerts)
     *   ISC_REQ_CONNECTION	            Неформатированные сообщения
     *   ISC_REQ_MUTUAL_AUTH	        Взаимная аутентификация
     *   ISC_REQ_USE_SUPPLIED_CREDS	    SSP не будет пытаться искать удостоверение, если оно не предоставлено приложением
     *   ISC_REQ_MANUAL_CRED_VALIDATION	SSP не будет проверять удостоверение другой стороны
     */
	dwSSPIFlags = ISC_REQ_SEQUENCE_DETECT   |
				ISC_REQ_REPLAY_DETECT	 |
				ISC_REQ_CONFIDENTIALITY   |
				ISC_RET_EXTENDED_ERROR	|
				ISC_REQ_ALLOCATE_MEMORY   |
				ISC_REQ_STREAM;

	//
	//  Инициализация сообщения ClientHello и генерация токена
	//

	OutBuffers[0].pvBuffer   = NULL; // A pointer to a buffer
	OutBuffers[0].BufferType = SECBUFFER_TOKEN; // This buffer type is used to indicate the security token portion of the message
	OutBuffers[0].cbBuffer   = 0; // Specifies the size, in bytes, of the buffer pointed to by the pvBuffer member

	OutBuffer.cBuffers = 1;
	OutBuffer.pBuffers = OutBuffers;
	OutBuffer.ulVersion = SECBUFFER_VERSION;

	char *pServerName = new char[serverName.length()];
	strcpy(pServerName, serverName.c_str());
    // Создает клиентский контекст безопасности, используемый для защищенного обмена между клиентом и сервером
	scRet = InitializeSecurityContextA(
						phCreds, // Указатель на дескриптор удостоверения клиента. Указанный дескриптор получается через запрос к функции AcquireCredentialsHandle
						NULL, // Указатель на Ctxt. При первом вызове, укажите NULL. При последующих - указатель на дескриптор, возвращенный в phNewContext после первого вызова к этой функции
						pServerName, // Строка, являющаяся уникальным идентификатором сервера. Используется при поиске сессии в кеше сессий при восстановлении соединения.
						dwSSPIFlags,
						0,
						SECURITY_NATIVE_DREP, // В доке == 0
						NULL,
						0,
						phContext, // Указатель на CtxtHandle. При первом вызове сюда будет помещен дескриптор нового контекста. При последующих вызовах полученный дескриптор следует передавать через параметр phContext, а в phNewContext передавать NULL
						&OutBuffer, // Указатель на структуру SecBufferDesc, содержащую SecBuffer с типом SECBUFFER_TOKEN. На выходе в этот буфер будет помещен пакет, который нужно переслать серверу.
						&dwSSPIOutFlags, // Указатель на ULONG, куда будут записаны флаги, описывающие свойства устанавливаемого контекста. Список возможных значений приведен в описании параметра fContextReq.
						&tsExpiry); // Указатель на структуру TimeStamp. При наличии сертификата клиента, этот параметр получает время истечения этого сертификата.
	delete []pServerName;
	if(scRet != SEC_I_CONTINUE_NEEDED) {
		return SEC_I_CONTINUE_NEEDED;
  }

	// Посылка отвера серверу.
	if(OutBuffers[0].cbBuffer != 0 && OutBuffers[0].pvBuffer != NULL)
	{
	    // Client Hello
		bool err = false;
		try {
			cbData = socket.write_some(boost::asio::buffer((CHAR *)OutBuffers[0].pvBuffer, OutBuffers[0].cbBuffer));
		}
		catch(std::exception& e) 
		{
			BLOG(error) << e.what();
			err = true;
		}
		if(err || cbData == 0)
		{
			BLOG(error) << "Error sending data to server (1)";
			FreeContextBuffer(OutBuffers[0].pvBuffer);
			DeleteSecurityContext(phContext);
			return SEC_E_INTERNAL_ERROR;
    }

		BLOG(debug) << cbData << " bytes of handshake data sent";


		// Освобождение выходного буфера.
		FreeContextBuffer(OutBuffers[0].pvBuffer);
		OutBuffers[0].pvBuffer = NULL;
  }

	return ClientHandshakeLoop(socket, phCreds, phContext, TRUE);
} // PerformClientHandshake

static SECURITY_STATUS ClientHandshakeLoop(
	boost::asio::ip::tcp::socket& socket, 	 // in
	PCredHandle	phCreds, 	// in
	CtxtHandle *phContext, // in, out
	BOOL fDoInitialRead)	 // in
{
	SecBufferDesc InBuffer;
	SecBuffer InBuffers[2];
	SecBufferDesc OutBuffer;
	SecBuffer OutBuffers[1];
	SecBuffer *pExtraData;
	DWORD dwSSPIFlags;
	DWORD dwSSPIOutFlags;
	TimeStamp tsExpiry;
	SECURITY_STATUS scRet;
	DWORD	cbData;

	char*	IoBuffer;
	DWORD	cbIoBuffer;
	BOOL fDoRead;

	dwSSPIFlags = ISC_REQ_SEQUENCE_DETECT   |
				ISC_REQ_REPLAY_DETECT	 |
				ISC_REQ_CONFIDENTIALITY   |
				ISC_RET_EXTENDED_ERROR	|
				ISC_REQ_ALLOCATE_MEMORY   |
				ISC_REQ_STREAM;

	// Размещение буфера данных.
	IoBuffer = new char[IO_BUFFER_SIZE];
	if(IoBuffer == nullptr) {
		BLOG(error) << "Out of memory (1)";
		return SEC_E_INTERNAL_ERROR;
  }
	cbIoBuffer = 0;

	fDoRead = fDoInitialRead;

	// Цикл до тех пока, пока не закончится обмен сообщениями,
	// либо не произойдет ошибка.
	scRet = SEC_I_CONTINUE_NEEDED;

	while(scRet == SEC_I_CONTINUE_NEEDED    ||
        scRet == SEC_E_INCOMPLETE_MESSAGE ||
        scRet == SEC_I_INCOMPLETE_CREDENTIALS)
  {
		// Чтение данных от сервера.
		if(cbIoBuffer == 0 || scRet == SEC_E_INCOMPLETE_MESSAGE) {
			if(fDoRead) {
				try {
					cbData = socket.read_some(boost::asio::buffer(IoBuffer + cbIoBuffer,	IO_BUFFER_SIZE - cbIoBuffer));
				}
				catch(std::exception& e) 
				{
					BLOG(error) << "Error reading data from server: " << e.what();
					scRet = SEC_E_INTERNAL_ERROR;
					break;
				}
				if(cbData == 0) {
					BLOG(error) << "Server unexpectedly disconnected";
					scRet = SEC_E_INTERNAL_ERROR;
					break;
				}

				BLOG(debug) << cbData << " bytes of handshake data received";

				cbIoBuffer += cbData;
      }	else {
				fDoRead = TRUE;
      }
    }

		// Установка входных буферов. Buffer 0 содержит данные, получаемые
		// от сервера. Schannel поглощает некоторые или все из них.
		// Оставшиеся данные (в любом случае) располагаются в  buffer 1 и
		// получают тип буфера SECBUFFER_EXTRA.

		InBuffers[0].pvBuffer   = IoBuffer;
		InBuffers[0].cbBuffer   = cbIoBuffer;
		InBuffers[0].BufferType = SECBUFFER_TOKEN;

		InBuffers[1].pvBuffer   = NULL;
		InBuffers[1].cbBuffer   = 0;
		InBuffers[1].BufferType = SECBUFFER_EMPTY;

		InBuffer.cBuffers	 = 2;
		InBuffer.pBuffers	 = InBuffers;
		InBuffer.ulVersion	= SECBUFFER_VERSION;

		// Установка выходных буферов. Инициализация производится таким образом,
		// чтобы pvBuffer содержал NULL.
		// Это сделано для того, чтобы в случае неудачи не было необходимости выполнять
		// освобождение памяти.

		OutBuffers[0].pvBuffer  = NULL;
		OutBuffers[0].BufferType= SECBUFFER_TOKEN;
		OutBuffers[0].cbBuffer  = 0;

		OutBuffer.cBuffers	= 1;
		OutBuffer.pBuffers	= OutBuffers;
		OutBuffer.ulVersion	 = SECBUFFER_VERSION;

		// BLOG(debug) << __LINE__;
		// try {
		scRet = InitializeSecurityContextA(phCreds,
										phContext,
										NULL,
										dwSSPIFlags,
										0,
										SECURITY_NATIVE_DREP,
										&InBuffer,
										0,
										NULL,
										&OutBuffer,
										&dwSSPIOutFlags,
										&tsExpiry);
		// }
		// catch (...) {BLOG(error) << "Init";}
		// BLOG(debug) << __LINE__;

		// Если InitializeSecurityContext успешно выполнена (или если произошла
		// одна из распространенных ошибок), то выполняется посылка выходного
		// буфера серверу.
		if(scRet == SEC_E_OK				||
		 	 scRet == SEC_I_CONTINUE_NEEDED   ||
			 FAILED(scRet) && (dwSSPIOutFlags & ISC_RET_EXTENDED_ERROR))
		{
			if(OutBuffers[0].cbBuffer != 0 && OutBuffers[0].pvBuffer != NULL) {
				bool err = false;
				try {
					cbData = socket.write_some(boost::asio::buffer((CHAR *)OutBuffers[0].pvBuffer, 
																												OutBuffers[0].cbBuffer));
				}
				catch(std::exception& e) 
				{
					BLOG(error) << e.what();
					err = true;
				}
				if(err || cbData == 0) {
					BLOG(error) << "Error sending data to server (2)";
					FreeContextBuffer(OutBuffers[0].pvBuffer);
					DeleteSecurityContext(phContext);
					return SEC_E_INTERNAL_ERROR;
        }

				BLOG(debug) << cbData << " bytes of handshake data sent";

				// Освобождение выходного буфера.
				FreeContextBuffer(OutBuffers[0].pvBuffer);
				OutBuffers[0].pvBuffer = NULL;
      }
    }

		// Если InitializeSecurityContext вернула ошибку SEC_E_INCOMPLETE_MESSAGE,
		// тогда необходимо прочитать большее количество данных от сервера и повторить
		// попытку снова.
		if(scRet == SEC_E_INCOMPLETE_MESSAGE) {
			continue;
    }

		// Если InitializeSecurityContext возвратила SEC_E_OK, тогда
		// обмен данными завершился успешно.
		if(scRet == SEC_E_OK) {
			// Если "extra" буфер содержит данные, то они являются данными
			// протокола зашифрования. Их необходимо сохранить.
			// Приложение позже расшифрует их при помощи DecryptMessage.
			BLOG(debug) << "Handshake was successful";

			if(InBuffers[1].BufferType == SECBUFFER_EXTRA) {
				BLOG(debug) << "SECBUFFER_EXTRA";
				pExtraData->pvBuffer = LocalAlloc(LMEM_FIXED, InBuffers[1].cbBuffer);
				if(pExtraData->pvBuffer == nullptr) {
					BLOG(error) << "Out of memory (2)";
					return SEC_E_INTERNAL_ERROR;
        }

				MoveMemory(pExtraData->pvBuffer,
						 IoBuffer + (cbIoBuffer - InBuffers[1].cbBuffer),
						 InBuffers[1].cbBuffer);

				pExtraData->cbBuffer = InBuffers[1].cbBuffer;
				pExtraData->BufferType = SECBUFFER_TOKEN;

				BLOG(debug) << pExtraData->cbBuffer << " bytes of app data was bundled with handshake data";
      } else {
				pExtraData->pvBuffer = NULL;
				pExtraData->cbBuffer = 0;
				pExtraData->BufferType = SECBUFFER_EMPTY;
				// TODO: delete extrabuffer
      }
			// Выход
			break;
    }

		// Проверка на ошибки.
		if(FAILED(scRet)) {
			BLOG(error) << "Error 0x" << scRet << " returned by InitializeSecurityContext (2)";
			break;
    }

		// Если InitializeSecurityContext возвратила SEC_I_INCOMPLETE_CREDENTIALS,
		// то сервер только что запросил аутентификацию клиента.
		if(scRet == SEC_I_INCOMPLETE_CREDENTIALS) {
			BLOG(warning) << "SEC_I_INCOMPLETE_CREDENTIALS";
			// Ошибка. Сервер запросил аутентификацию клиента, но переданный
			// мандат не содержит сертификата клиента.

			// Эта функция читает список доверенных сертификатов авторов
			// ("issuers", полученный от сервера и пытается найти
			// подходящий сертификат клиента. Если эта функция выполнена успешно,
			// то происходит соединение при помощи нового сертификата.
			// В противном случае осуществляется попытка произвести
			// соединение анонимно (используя текущий мандат).
			GetNewClientCredentials(phCreds, phContext);

			// Повторная попытка.
			fDoRead = false;
			scRet = SEC_I_CONTINUE_NEEDED;
			continue;
    }

		// Копирование всех данных из "extra" буфера и повторная попытка.
		if ( InBuffers[1].BufferType == SECBUFFER_EXTRA ) {
			MoveMemory(IoBuffer, IoBuffer + (cbIoBuffer - InBuffers[1].cbBuffer),
					 			 InBuffers[1].cbBuffer);
			cbIoBuffer = InBuffers[1].cbBuffer;
    } else {
			cbIoBuffer = 0;
    }
  }
	// Уничтожение закрытого контекста в случае непоправимой ошибки.
	if(FAILED(scRet))
	{
		DeleteSecurityContext(phContext);
  }
	delete []IoBuffer;
	return scRet;
} // ClientHandshakeLoop

static SECURITY_STATUS AuthenticateServerCertificate(PCtxtHandle phContext, const std::string& serverName)
{
	SECURITY_STATUS Status;
	PCCERT_CONTEXT pRemoteCertContext;
	Status = QueryContextAttributes(phContext,
												  SECPKG_ATTR_REMOTE_CERT_CONTEXT,
											 		(PVOID)&pRemoteCertContext);
	if(Status != SEC_E_OK) {
		BLOG(error) << "Error 0x" << Status << " querying remote certificate";
		CertFreeCertificateContext(pRemoteCertContext);
		return Status;
	}

	// Вывод цепочки сертификатов сервера.
	// DisplayCertChain(pRemoteCertContext, FALSE); // TODO

	// Проверка действительности сертификата сервера.
	char *pServerName = new char[serverName.length()];
	strcpy(pServerName, serverName.c_str());
	Status = VerifyServerCertificate(pRemoteCertContext, pServerName, 0);
	if(Status) {
		// Сертификат сервера не действителен. Возможно было осуществлено
		// соединение с недопустимым сервером, либо производилась атака
		// "противник посередине". Лучше всего прервать соединение.
		BLOG(error) << "Error 0x" << Status << " authenticating server credentials!";
		CertFreeCertificateContext(pRemoteCertContext);
		return Status;
  }
  delete []pServerName;

	// DisplayConnectionInfo(phContext); // TODO

	if(pRemoteCertContext) {
		CertFreeCertificateContext(pRemoteCertContext);
		pRemoteCertContext = nullptr;
  }
	return Status;
}

static void DisplayCertChain(
	PCCERT_CONTEXT  pServerCert,
	BOOL			fLocal)
{
	CHAR szName[1000];
	PCCERT_CONTEXT pCurrentCert;
	PCCERT_CONTEXT pIssuerCert;
	DWORD dwVerificationFlags;
	DWORD result;

	// Вывод листа цеточки
	if(!(result = CertNameToStr(pServerCert->dwCertEncodingType,
					&pServerCert->pCertInfo->Subject,
					CERT_X500_NAME_STR | CERT_NAME_STR_NO_PLUS_FLAG,
					szName, sizeof(szName))))
	{
		BLOG(error) << "Error 0x" << result << " building subject name";
  }
	if(fLocal) {
		BLOG(debug) << "Client subject: " << szName;
  }	else {
		BLOG(debug) << "Server subject: " << szName;
  }
	if(!(result = CertNameToStr(pServerCert->dwCertEncodingType,
					&pServerCert->pCertInfo->Issuer,
					CERT_X500_NAME_STR | CERT_NAME_STR_NO_PLUS_FLAG,
					szName, sizeof(szName))))
	{
		BLOG(error) << "Error 0x" << result << " building issuer name";
  }
	if(fLocal) {
		BLOG(debug) << "Client issuer: " << szName;
  } else {
		BLOG(debug) << "Server issuer: " << szName;
  }
	// Вывод цепочки сертификатов
	pCurrentCert = pServerCert;
	while(pCurrentCert != nullptr)	{
		dwVerificationFlags = 0;
		pIssuerCert = CertGetIssuerCertificateFromStore(pServerCert->hCertStore,
														pCurrentCert,
														NULL,
														&dwVerificationFlags);
		if(pIssuerCert == nullptr) {
			if(pCurrentCert != pServerCert)	{
				CertFreeCertificateContext(pCurrentCert);
			}
			break;
    }

		if(!(result = CertNameToStr(pIssuerCert->dwCertEncodingType,
						&pIssuerCert->pCertInfo->Subject,
						CERT_X500_NAME_STR | CERT_NAME_STR_NO_PLUS_FLAG,
						szName, sizeof(szName))))
		{
			BLOG(error) << "Error 0x" << result << " building subject name";
    }
		BLOG(debug) << "CA subject: " << szName;
		if(!(result = CertNameToStr(pIssuerCert->dwCertEncodingType,
						&pIssuerCert->pCertInfo->Issuer,
						CERT_X500_NAME_STR | CERT_NAME_STR_NO_PLUS_FLAG,
						szName, sizeof(szName))))
		{
			BLOG(error) << "Error 0x" << result << " building issuer name";
    }
		BLOG(debug) << "CA issuer: " << szName;

		if(pCurrentCert != pServerCert) {
			CertFreeCertificateContext(pCurrentCert);
    }
		pCurrentCert = pIssuerCert;
		pIssuerCert = NULL;
  }
} // DisplayCertChain

//-------------------------------------------------------------
// Функция проверки сертификата сервера.
static DWORD VerifyServerCertificate(
	PCCERT_CONTEXT  pServerCert,
	char *pszServerName,
	DWORD		 dwCertFlags)
{
	HTTPSPolicyCallbackData  polHttps;
	CERT_CHAIN_POLICY_PARA   PolicyPara;
	CERT_CHAIN_POLICY_STATUS PolicyStatus;
	CERT_CHAIN_PARA		ChainPara;
	PCCERT_CHAIN_CONTEXT	 pChainContext = NULL;

	// char - change
	LPSTR rgszUsages[] = {  szOID_PKIX_KP_SERVER_AUTH,
							szOID_SERVER_GATED_CRYPTO,
							szOID_SGC_NETSCAPE };
	DWORD cUsages = sizeof(rgszUsages) / sizeof(LPSTR);

	PWSTR   pwszServerName = NULL;
	DWORD   cchServerName;
	HRESULT   Status;

	if(pServerCert == nullptr) {
		Status = SEC_E_WRONG_PRINCIPAL;
		goto cleanup;
  }
	// Преобразование имени сервера в unicode.
	if(pszServerName == nullptr || strlen(pszServerName) == 0) {
		Status = SEC_E_WRONG_PRINCIPAL;
		goto cleanup;
  }

	cchServerName = MultiByteToWideChar(CP_ACP, 0, pszServerName, -1, NULL, 0);
	pwszServerName = (wchar_t *)LocalAlloc(LMEM_FIXED, cchServerName * sizeof(WCHAR));
	if(pwszServerName == NULL) {
		Status = SEC_E_INSUFFICIENT_MEMORY;
		goto cleanup;
  }
	cchServerName = MultiByteToWideChar(CP_ACP, 0, pszServerName, -1, pwszServerName, cchServerName);
	if(cchServerName == 0) {
		Status = SEC_E_WRONG_PRINCIPAL;
		goto cleanup;
  }
	// Построение цепочки сертификатов.
	ZeroMemory(&ChainPara, sizeof(ChainPara));
	ChainPara.cbSize = sizeof(ChainPara);
	ChainPara.RequestedUsage.dwType = USAGE_MATCH_TYPE_OR;
	ChainPara.RequestedUsage.Usage.cUsageIdentifier	 = cUsages;
	ChainPara.RequestedUsage.Usage.rgpszUsageIdentifier = rgszUsages;

	if(!CertGetCertificateChain(
							NULL,
							pServerCert,
							NULL,
							pServerCert->hCertStore,
							&ChainPara,
							0,
							NULL,
							&pChainContext))
	{
		Status = GetLastError();
		BLOG(error) << "Error 0x" << Status << " returned by CertGetCertificateChain!";
		goto cleanup;
  }

	// Проверка цепочки сертификатов.
	ZeroMemory(&polHttps, sizeof(HTTPSPolicyCallbackData));
	polHttps.cbStruct = sizeof(HTTPSPolicyCallbackData);
	polHttps.dwAuthType = AUTHTYPE_SERVER;
	polHttps.fdwChecks = dwCertFlags;
	polHttps.pwszServerName = pwszServerName;

	memset(&PolicyPara, 0, sizeof(PolicyPara));
	PolicyPara.cbSize	= sizeof(PolicyPara);
	PolicyPara.pvExtraPolicyPara = &polHttps;

	memset(&PolicyStatus, 0, sizeof(PolicyStatus));
	PolicyStatus.cbSize = sizeof(PolicyStatus);

	if(!CertVerifyCertificateChainPolicy(
							CERT_CHAIN_POLICY_SSL,
							pChainContext,
							&PolicyPara,
							&PolicyStatus))
	{
		Status = GetLastError();
		BLOG(error) << "Error 0x" << Status << " returned by CertVerifyCertificateChainPolicy!";
		goto cleanup;
  }

	if(PolicyStatus.dwError)
	{
		Status = PolicyStatus.dwError;
		DisplayWinVerifyTrustError(Status);
		goto cleanup;
  }

	Status = SEC_E_OK;

cleanup:

	if(pChainContext)	{
		CertFreeCertificateChain(pChainContext);
  }
	if(pwszServerName) {
		LocalFree(pwszServerName);
  }

	return Status;
} // VerifyServerCertificate

//-------------------------------------------------------------
// Функция вывода информации о соединении.
static void DisplayConnectionInfo(CtxtHandle *phContext)
{
	SECURITY_STATUS Status;
	SecPkgContext_ConnectionInfo ConnectionInfo;

	Status = QueryContextAttributes(phContext,
									SECPKG_ATTR_CONNECTION_INFO,
									(PVOID)&ConnectionInfo);
	if(Status != SEC_E_OK) {
		BLOG(debug) << "Error 0x" << Status << "querying connection info";
		return;
  }

	switch(ConnectionInfo.dwProtocol) {
		case SP_PROT_TLS1_CLIENT:
			BLOG(debug) << "Protocol: TLS1";
			break;

		default:
			BLOG(debug) << "Protocol: 0x" << ConnectionInfo.dwProtocol;
  }

	switch(ConnectionInfo.aiCipher)	{
		case CALG_G28147:
			BLOG(debug) << "Cipher: Gost 28147-89";
			break;

		default:
			BLOG(debug) << "Cipher: 0x" << ConnectionInfo.aiCipher;
  }

	BLOG(debug) << "Cipher strength: " << ConnectionInfo.dwCipherStrength;

	switch(ConnectionInfo.aiHash) {
		case CALG_GR3411:
			BLOG(debug) << "Hash: Gost R 34.11-94";
			break;

		default:
			BLOG(debug) << "Hash: 0x" << ConnectionInfo.aiHash;
  }

	BLOG(debug) << "Hash strength: " << ConnectionInfo.dwHashStrength;

	switch(ConnectionInfo.aiExch) {
		case CALG_DH_EX_EPHEM:
			BLOG(debug) << "Key exchange: DH Ephemeral (Gost R 34.10-94)";
			break;

		default:
			BLOG(debug) << "Key exchange: 0x" << ConnectionInfo.aiExch;
  }

	BLOG(debug) << "Key exchange strength: " << ConnectionInfo.dwExchStrength;
} //DisplayConnectionInfo

//-------------------------------------------------------------
// Функция вывода ошибки полученной при проверке цепочки сертификатов.
static void DisplayWinVerifyTrustError(DWORD Status)
{
	std::string name;

	switch(Status) {
		case CERT_E_EXPIRED: name = "CERT_E_EXPIRED"; break;
		case CERT_E_VALIDITYPERIODNESTING: name = "CERT_E_VALIDITYPERIODNESTING";   break;
		case CERT_E_ROLE:	name = "CERT_E_ROLE"; 				break;
		case CERT_E_PATHLENCONST: name = "CERT_E_PATHLENCONST"; 		break;
		case CERT_E_CRITICAL:	name = "CERT_E_CRITICAL"; 			break;
		case CERT_E_PURPOSE: name = "CERT_E_PURPOSE"; 			 break;
		case CERT_E_ISSUERCHAINING:	name = "CERT_E_ISSUERCHAINING"; 	break;
		case CERT_E_MALFORMED: name = "CERT_E_MALFORMED"; 		 break;
		case CERT_E_UNTRUSTEDROOT: name = "CERT_E_UNTRUSTEDROOT"; 	 break;
		case CERT_E_CHAINING:	name = "CERT_E_CHAINING"; 			break;
		case TRUST_E_FAIL: name = "TRUST_E_FAIL"; 			 break;
		case CERT_E_REVOKED: name = "CERT_E_REVOKED"; 			 break;
		case CERT_E_UNTRUSTEDTESTROOT: name = "CERT_E_UNTRUSTEDTESTROOT";  break;
		case CERT_E_REVOCATION_FAILURE: name = "CERT_E_REVOCATION_FAILURE"; break;
		case CERT_E_CN_NO_MATCH: name = "CERT_E_CN_NO_MATCH"; 		 break;
		case CERT_E_WRONG_USAGE: name = "CERT_E_WRONG_USAGE"; 		 break;
		default: name = "(unknown)"; 				break;
  }

	BLOG(error) << "Error 0x" << Status << " (" << name << ") returned by CertVerifyCertificateChainPolicy!";
} //DisplayWinVerifyTrustError

static void GetNewClientCredentials(
	CredHandle *phCreds,
	CtxtHandle *phContext)
{
	CredHandle hCreds;
	SecPkgContext_IssuerListInfoEx IssuerListInfo;
	PCCERT_CHAIN_CONTEXT pChainContext;
	CERT_CHAIN_FIND_BY_ISSUER_PARA FindByIssuerPara;
	TimeStamp	 tsExpiry;
	SECURITY_STATUS Status;
	SCHANNEL_CRED SchannelCred;
	HCERTSTORE	hMyCertStore = nullptr;

	PCCERT_CONTEXT pCertContext = nullptr; // Здесь: клиентский сертификат

	// Открытие хранилища сертификатов "MY"
	hMyCertStore = CertOpenSystemStore(0, "MY");
  if(!hMyCertStore) {
  	BLOG(error) << "Error SEC_E_NO_CREDENTIALS while opened store";
  	return;
  }

	// Чтение спсика доверенных издателей из schannel.
	Status = QueryContextAttributes(phContext, 
																	SECPKG_ATTR_ISSUER_LIST_EX,
																  (PVOID)&IssuerListInfo);
	if(Status != SEC_E_OK) {
		BLOG(error) << "Error 0x" << Status <<" querying issuer list info";
		if(hMyCertStore) {
			CertCloseStore(hMyCertStore, 0);
  	}
		return;
  }

	// Перечисление сертификатов клиента

	ZeroMemory(&FindByIssuerPara, sizeof(FindByIssuerPara));

	FindByIssuerPara.cbSize = sizeof(FindByIssuerPara);
	FindByIssuerPara.pszUsageIdentifier = szOID_PKIX_KP_CLIENT_AUTH;
	FindByIssuerPara.dwKeySpec = 0;
	FindByIssuerPara.cIssuer = IssuerListInfo.cIssuers;
	FindByIssuerPara.rgIssuer = IssuerListInfo.aIssuers;

	pChainContext = NULL;

	for(;;)
	{
		// Поиск цепочки сертификатов.
		pChainContext = CertFindChainInStore(hMyCertStore,
											 X509_ASN_ENCODING,
											 0,
											 CERT_CHAIN_FIND_BY_ISSUER,
											 &FindByIssuerPara,
											 pChainContext);
		if(pChainContext == nullptr) {
			BLOG(error) << "Error 0x" << GetLastError() << " finding cert chain";
			break;
    }
		BLOG(debug) << "Certificate chain found";

		// Получение указателя на контекс сертифика-листа.
		pCertContext = pChainContext->rgpChain[0]->rgpElement[0]->pCertContext;

		// Создание schannel мандата.
		SchannelCred.dwVersion = SCHANNEL_CRED_VERSION;
		SchannelCred.cCreds = 1;
		SchannelCred.paCred = &pCertContext;

		Status = AcquireCredentialsHandleA(
							NULL, 			 // Имя администратора
							UNISP_NAME_A, 	 // Имя пакета
							SECPKG_CRED_OUTBOUND,   // Флаг, определяющий использование
							NULL, 			 // Указатель на идентификатор пароля
							&SchannelCred, 	// Данные пакета
							NULL, 			 // Указатель на функицю GetKey()
							NULL, 			 // Значения, передаваемые функции GetKey()
							&hCreds, 			// (out) Даскриптор мандата
							&tsExpiry); 		 // (out) Период актуальности (необязательно)
		if(Status != SEC_E_OK) {
			BLOG(error) << "Error 0x" << Status << " returned by AcquireCredentialsHandle";
			continue;
		}
		BLOG(debug) << "New schannel credential created";

		// Уничтожение старых мандатов.
		FreeCredentialsHandle(phCreds);

		*phCreds = hCreds;

		break;
  }
  if(hMyCertStore) {
		CertCloseStore(hMyCertStore, 0);
  }
} // Конец GetNewClientCredentials

static LONG DisconnectFromServer(
	boost::asio::ip::tcp::socket& socket,
	PCredHandle	phCreds,
	CtxtHandle *phContext)
{
	DWORD dwType;
	char* pbMessage;
	DWORD cbMessage;
	DWORD cbData;

	SecBufferDesc OutBuffer;
	SecBuffer OutBuffers[1];
	DWORD dwSSPIFlags;
	DWORD dwSSPIOutFlags;
	TimeStamp tsExpiry;
	DWORD	Status;

	// Уведомление schannel о закрытии соединения.

	dwType = SCHANNEL_SHUTDOWN;

	OutBuffers[0].pvBuffer   = &dwType;
	OutBuffers[0].BufferType = SECBUFFER_TOKEN;
	OutBuffers[0].cbBuffer   = sizeof(dwType);

	OutBuffer.cBuffers  = 1;
	OutBuffer.pBuffers  = OutBuffers;
	OutBuffer.ulVersion = SECBUFFER_VERSION;

	Status = ApplyControlToken(phContext, &OutBuffer);

	if(FAILED(Status)) {
		BLOG(error) << "Error 0x" << Status << " returned by ApplyControlToken";
		return Status;
  }

	// Построение SSL сообщения, являющегося уведомлением о закрытии.

	dwSSPIFlags = ISC_REQ_SEQUENCE_DETECT |
								ISC_REQ_REPLAY_DETECT	 	|
								ISC_REQ_CONFIDENTIALITY |
								ISC_RET_EXTENDED_ERROR	|
								ISC_REQ_ALLOCATE_MEMORY |
								ISC_REQ_STREAM;

	OutBuffers[0].pvBuffer   = NULL;
	OutBuffers[0].BufferType = SECBUFFER_TOKEN;
	OutBuffers[0].cbBuffer   = 0;

	OutBuffer.cBuffers  = 1;
	OutBuffer.pBuffers  = OutBuffers;
	OutBuffer.ulVersion = SECBUFFER_VERSION;

	Status = InitializeSecurityContextA(
					phCreds,
					phContext,
					NULL,
					dwSSPIFlags,
					0,
					SECURITY_NATIVE_DREP,
					NULL,
					0,
					phContext,
					&OutBuffer,
					&dwSSPIOutFlags,
					&tsExpiry);

	if(FAILED(Status)) {
		BLOG(error) << "Error 0x" << Status << " returned by InitializeSecurityContext";
  }

	pbMessage = (CHAR *)OutBuffers[0].pvBuffer;
	cbMessage = OutBuffers[0].cbBuffer;

	// Посылка этого сообщения серверу.

	if(pbMessage != nullptr && cbMessage != 0) {
		bool error = false;
		try {
			cbData = socket.write_some(boost::asio::buffer(pbMessage, cbMessage));
		}
		catch (const std::exception& e) {
			error = true;
		}
		if(error || cbData == 0) {
			Status = WSAGetLastError();
			BLOG(error) << "Error " << Status << " sending close notify";
			return Status;
    }

		BLOG(debug) << "Sending Close Notify. " << cbData << " bytes of handshake data sent";

		// Освобождение выходного буфера.
		FreeContextBuffer(pbMessage);
  }

	return Status;
} // DisconnectFromServer