#ifndef _WINCRYPTEX_H_INCLUDED
#define _WINCRYPTEX_H_INCLUDED

#ifndef _WINCRYPTEX_USE_EXTERNAL_TYPES

#ifdef UNIX
#include<CPROCSP/CSP_WinCrypt.h>
#else // UNIX
#include <wincrypt.h>
#endif // UNIX

#endif // _WINCRYPTEX_USE_EXTERNAL_TYPES

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
#define CP_DEF_PROV_A "Crypto-Pro Cryptographic Service Provider"
#define CP_DEF_PROV_W L"Crypto-Pro Cryptographic Service Provider"
#ifdef UNICODE
#define CP_DEF_PROV CP_DEF_PROV_W
#else //!UNICODE
#define CP_DEF_PROV CP_DEF_PROV_A
#endif //!UNICODE

// ����� ����������� ��� � CSP 2.0
#define CP_GR3410_94_PROV_A "Crypto-Pro GOST R 34.10-94 Cryptographic Service Provider"
#define CP_GR3410_94_PROV_W L"Crypto-Pro GOST R 34.10-94 Cryptographic Service Provider"
#ifdef UNICODE
#define CP_GR3410_94_PROV CP_GR3410_94_PROV_W
#else //!UNICODE
#define CP_GR3410_94_PROV CP_GR3410_94_PROV_A
#endif //!UNICODE

#define CP_GR3410_2001_PROV_A "Crypto-Pro GOST R 34.10-2001 Cryptographic Service Provider"
#define CP_GR3410_2001_PROV_W L"Crypto-Pro GOST R 34.10-2001 Cryptographic Service Provider"
#ifdef UNICODE
#define CP_GR3410_2001_PROV CP_GR3410_2001_PROV_W
#else //!UNICODE
#define CP_GR3410_2001_PROV CP_GR3410_2001_PROV_A
#endif //!UNICODE

#define CP_GR3410_2001_HSM_LOCAL_PROV_A "Crypto-Pro GOST R 34.10-2001 HSM Local CSP"
#define CP_GR3410_2001_HSM_LOCAL_PROV_W L"Crypto-Pro GOST R 34.10-2001 HSM Local CSP"
#ifdef UNICODE
#define CP_GR3410_2001_HSM_LOCAL_PROV CP_GR3410_2001_HSM_LOCAL_PROV_W
#else //!UNICODE
#define CP_GR3410_2001_HSM_LOCAL_PROV CP_GR3410_2001_HSM_LOCAL_PROV_A
#endif //!UNICODE

#define CP_KC1_GR3410_94_PROV_A "Crypto-Pro GOST R 34.10-94 KC1 CSP"
#define CP_KC1_GR3410_94_PROV_W L"Crypto-Pro GOST R 34.10-94 KC1 CSP"
#ifdef UNICODE
#define CP_KC1_GR3410_94_PROV CP_KC1_GR3410_94_PROV_W
#else //!UNICODE
#define CP_KC1_GR3410_94_PROV CP_KC1_GR3410_94_PROV_A
#endif //!UNICODE

#define CP_KC1_GR3410_2001_PROV_A "Crypto-Pro GOST R 34.10-2001 KC1 CSP"
#define CP_KC1_GR3410_2001_PROV_W L"Crypto-Pro GOST R 34.10-2001 KC1 CSP"
#ifdef UNICODE
#define CP_KC1_GR3410_2001_PROV CP_KC1_GR3410_2001_PROV_W
#else //!UNICODE
#define CP_KC1_GR3410_2001_PROV CP_KC1_GR3410_2001_PROV_A
#endif //!UNICODE

#define CP_KC2_GR3410_94_PROV_A "Crypto-Pro GOST R 34.10-94 KC2 CSP"
#define CP_KC2_GR3410_94_PROV_W L"Crypto-Pro GOST R 34.10-94 KC2 CSP"
#ifdef UNICODE
#define CP_KC2_GR3410_94_PROV CP_KC2_GR3410_94_PROV_W
#else //!UNICODE
#define CP_KC2_GR3410_94_PROV CP_KC2_GR3410_94_PROV_A
#endif //!UNICODE

#define CP_KC2_GR3410_2001_PROV_A "Crypto-Pro GOST R 34.10-2001 KC2 CSP"
#define CP_KC2_GR3410_2001_PROV_W L"Crypto-Pro GOST R 34.10-2001 KC2 CSP"
#ifdef UNICODE
#define CP_KC2_GR3410_2001_PROV CP_KC2_GR3410_2001_PROV_W
#else //!UNICODE
#define CP_KC2_GR3410_2001_PROV CP_KC2_GR3410_2001_PROV_A
#endif //!UNICODE

#define PH_GR3410_94_PROV_A "Phoenix-CS GOST R 34.10-94 Cryptographic Service Provider"
#define PH_GR3410_94_PROV_W L"Phoenix-CS GOST R 34.10-94 Cryptographic Service Provider"
#ifdef UNICODE
#define PH_GR3410_94_PROV PH_GR3410_94_PROV_W
#else //!UNICODE
#define PH_GR3410_94_PROV PH_GR3410_94_PROV_A
#endif //!UNICODE

#define PH_GR3410_2001_PROV_A "Phoenix-CS GOST R 34.10-2001 Cryptographic Service Provider"
#define PH_GR3410_2001_PROV_W L"Phoenix-CS GOST R 34.10-2001 Cryptographic Service Provider"
#ifdef UNICODE
#define PH_GR3410_2001_PROV PH_GR3410_2001_PROV_W
#else //!UNICODE
#define PH_GR3410_2001_PROV PH_GR3410_2001_PROV_A
#endif //!UNICODE

#define CP_KC1_GR3410_2001_MAGISTRA_PROV_A "GOST R 34.10-2001 Magistra CSP"
#define CP_KC1_GR3410_2001_MAGISTRA_PROV_W L"GOST R 34.10-2001 Magistra CSP"
#ifdef UNICODE
#define CP_KC1_GR3410_2001_MAGISTRA_PROV CP_KC1_GR3410_2001_MAGISTRA_PROV_W
#else //!UNICODE
#define CP_KC1_GR3410_2001_MAGISTRA_PROV CP_KC1_GR3410_2001_MAGISTRA_PROV_A
#endif //!UNICODE

#define CP_KC1_GR3410_2001_RUTOKEN_PROV_A "GOST R 34.10-2001 Rutoken CSP"
#define CP_KC1_GR3410_2001_RUTOKEN_PROV_W L"GOST R 34.10-2001 Rutoken CSP"
#ifdef UNICODE
#define CP_KC1_GR3410_2001_RUTOKEN_PROV CP_KC1_GR3410_2001_RUTOKEN_PROV_W
#else //!UNICODE
#define CP_KC1_GR3410_2001_RUTOKEN_PROV CP_KC1_GR3410_2001_RUTOKEN_PROV_A
#endif //!UNICODE

#define CP_KC1_GR3410_2001_ETOKEN_PROV_A "GOST R 34.10-2001 eToken CSP"
#define CP_KC1_GR3410_2001_ETOKEN_PROV_W L"GOST R 34.10-2001 eToken CSP"
#ifdef UNICODE
#define CP_KC1_GR3410_2001_ETOKEN_PROV CP_KC1_GR3410_2001_ETOKEN_PROV_W
#else //!UNICODE
#define CP_KC1_GR3410_2001_ETOKEN_PROV CP_KC1_GR3410_2001_ETOKEN_PROV_A
#endif //!UNICODE

#define CP_KC1_GR3410_2001_FLASH_PROV_A "Crypto-Pro Flash Drive KC1 CSP"
#define CP_KC1_GR3410_2001_FLASH_PROV_W L"Crypto-Pro Flash Drive KC1 CSP"
#ifdef UNICODE
#define CP_KC1_GR3410_2001_FLASH_PROV CP_KC1_GR3410_2001_FLASH_PROV_W
#else //!UNICODE
#define CP_KC1_GR3410_2001_FLASH_PROV CP_KC1_GR3410_2001_FLASH_PROV_A
#endif //!UNICODE

#define CP_KC1_GR3410_2001_REGISTRY_PROV_A "Crypto-Pro Registry KC1 CSP"
#define CP_KC1_GR3410_2001_REGISTRY_PROV_W L"Crypto-Pro Registry KC1 CSP"
#ifdef UNICODE
#define CP_KC1_GR3410_2001_REGISTRY_PROV CP_KC1_GR3410_2001_REGISTRY_PROV_W
#else //!UNICODE
#define CP_KC1_GR3410_2001_REGISTRY_PROV CP_KC1_GR3410_2001_REGISTRY_PROV_A
#endif //!UNICODE

#define PROV_GOST_DH 2
#define PROV_GOST_94_DH 71
#define PROV_GOST_2001_DH 75
#define CRYPT_XER_ENCODING (8)
#define CRYPT_GENERAL				0x00004000
#define CRYPT_TOKEN_SHARED			0x00008000
#define CRYPT_NOSERIALIZE			0x00010000
#define CRYPT_REBOOT				0x00020000
#define CPCMSG_CADES_STRICT		    (0x00000100)
#define CPCMSG_CADES_DISABLE                (0x00000200)
#define CPCMSG_CADES_DISABLE_CERT_SEARCH    (0x00000400)
#define CPCRYPT_MESSAGE_CADES_STRICT	    (CPCMSG_CADES_STRICT)
#define CPCRYPT_MESSAGE_CADES_DISABLE	    (CPCMSG_CADES_DISABLE)
#define CRYPT_ECCNEGATIVE	0x00000400
#define CRYPT_MODE_EKEXOR	11
#define CRYPT_MODE_EKEECADD	12
#define USERKEY_KEYEXCHANGE			AT_KEYEXCHANGE
#define USERKEY_SIGNATURE			AT_SIGNATURE

/* Algorithm types */
#define ALG_TYPE_GR3410				(7 << 9)
/* GR3411 sub-ids */
#define ALG_SID_GR3411				30
#define ALG_SID_GR3411_HASH			39
#define ALG_SID_GR3411_HASH34			40
/* G28147 sub_ids */
#define ALG_SID_G28147				30
#define ALG_SID_PRODIVERS			38
#define ALG_SID_RIC1DIVERS			40
/* Export Key sub_id */
#define ALG_SID_PRO_EXP				31
#define ALG_SID_SIMPLE_EXP			32
/* Hash sub ids */
#define ALG_SID_G28147_MAC			31
#define ALG_SID_TLS1_MASTER_HASH		32

/* GOST_DH sub ids */
#define ALG_SID_DH_EX_SF			30
#define ALG_SID_DH_EX_EPHEM			31
#define ALG_SID_PRO_AGREEDKEY_DH		33
#define ALG_SID_PRO_SIMMETRYKEY			34
#define ALG_SID_GR3410				30
#define ALG_SID_GR3410EL			35
#define ALG_SID_DH_EL_SF			36
#define ALG_SID_DH_EL_EPHEM			37
#define ALG_SID_GR3410_94_ESDH			39
#define ALG_SID_GR3410_01_ESDH			40
/* EKE sub ids*/
#define ALG_SID_EKE_CIPHER			41
#define ALG_SID_EKE_EXPORTPUBLIC		42
#define ALG_SID_EKEVERIFY_HASH			43
#define ALG_SID_AGREED_DEMASK			44

#define AT_KEYEXCHANGE_FKC		   0x80000001
#define AT_SIGNATURE_FKC		   0x80000002

#define CALG_GR3411 \
    (ALG_CLASS_HASH | ALG_TYPE_ANY | ALG_SID_GR3411)

#define CALG_GR3411_HMAC \
    (ALG_CLASS_HASH | ALG_TYPE_ANY | ALG_SID_GR3411_HASH)
#define CALG_GR3411_HMAC34 \
    (ALG_CLASS_HASH | ALG_TYPE_ANY | ALG_SID_GR3411_HASH34)

#define CALG_G28147_MAC \
    (ALG_CLASS_HASH | ALG_TYPE_ANY | ALG_SID_G28147_MAC)

#define CALG_G28147_IMIT \
    CALG_G28147_MAC

#define CALG_GR3410 \
    (ALG_CLASS_SIGNATURE | ALG_TYPE_GR3410 | ALG_SID_GR3410)

#define CALG_GR3410EL \
    (ALG_CLASS_SIGNATURE | ALG_TYPE_GR3410 | ALG_SID_GR3410EL)

#define CALG_G28147 \
    (ALG_CLASS_DATA_ENCRYPT | ALG_TYPE_BLOCK | ALG_SID_G28147)

#define CALG_DH_EX_SF \
    (ALG_CLASS_KEY_EXCHANGE | ALG_TYPE_DH | ALG_SID_DH_EX_SF)

#define CALG_DH_EX_EPHEM \
    (ALG_CLASS_KEY_EXCHANGE | ALG_TYPE_DH | ALG_SID_DH_EX_EPHEM)

#define CALG_DH_EX \
    CALG_DH_EX_SF

#define CALG_DH_EL_SF \
    (ALG_CLASS_KEY_EXCHANGE | ALG_TYPE_DH | ALG_SID_DH_EL_SF)

#define CALG_DH_EL_EPHEM \
    (ALG_CLASS_KEY_EXCHANGE | ALG_TYPE_DH | ALG_SID_DH_EL_EPHEM)

#define CALG_GR3410_94_ESDH \
    (ALG_CLASS_KEY_EXCHANGE | ALG_TYPE_DH | ALG_SID_GR3410_94_ESDH)

#define CALG_GR3410_01_ESDH \
    (ALG_CLASS_KEY_EXCHANGE | ALG_TYPE_DH | ALG_SID_GR3410_01_ESDH)

#define CALG_PRO_AGREEDKEY_DH \
    (ALG_CLASS_KEY_EXCHANGE | ALG_TYPE_BLOCK | ALG_SID_PRO_AGREEDKEY_DH)

#define CALG_PRO_EXPORT \
    (ALG_CLASS_DATA_ENCRYPT | ALG_TYPE_BLOCK | ALG_SID_PRO_EXP)

#define CALG_SIMPLE_EXPORT \
    (ALG_CLASS_DATA_ENCRYPT | ALG_TYPE_BLOCK | ALG_SID_SIMPLE_EXP)

#define CALG_SIMMETRYKEY \
    CALG_G28147
    /* (ALG_CLASS_DATA_ENCRYPT | ALG_TYPE_BLOCK | ALG_SID_SIMMETRYKEY) */

#define CALG_TLS1_MASTER_HASH \
    (ALG_CLASS_HASH | ALG_TYPE_ANY | ALG_SID_TLS1_MASTER_HASH)

#define CALG_TLS1_MAC_KEY \
    (ALG_CLASS_DATA_ENCRYPT | ALG_TYPE_SECURECHANNEL | ALG_SID_SCHANNEL_MAC_KEY)

#define CALG_TLS1_ENC_KEY \
    (ALG_CLASS_DATA_ENCRYPT | ALG_TYPE_SECURECHANNEL | ALG_SID_SCHANNEL_ENC_KEY)

#define CALG_PRO_DIVERS \
    (ALG_CLASS_DATA_ENCRYPT | ALG_TYPE_BLOCK | ALG_SID_PRODIVERS)
#define CALG_RIC_DIVERS \
    (ALG_CLASS_DATA_ENCRYPT | ALG_TYPE_BLOCK | ALG_SID_RIC1DIVERS)
#define CALG_OSCAR_DIVERS CALG_RIC_DIVERS

#define CALG_EKE_CIPHER \
    (ALG_CLASS_KEY_EXCHANGE | ALG_TYPE_BLOCK | ALG_SID_EKE_CIPHER)
#define CALG_EKEVERIFY_HASH \
    (ALG_CLASS_HASH | ALG_TYPE_ANY | ALG_SID_EKEVERIFY_HASH)

#define CALG_AGREED_DEMASK \
    (ALG_CLASS_KEY_EXCHANGE | ALG_TYPE_BLOCK | ALG_SID_AGREED_DEMASK)



#define CRYPT_ALG_PARAM_OID_GROUP_ID            20


#define CRYPT_PROMIX_MODE	0x00000001
#define CRYPT_SIMPLEMIX_MODE	0x00000000
#define CRYPT_MIXDUPLICATE	0x00000002

/*��� ��������� ����� ��� �������������� ������ � �������
    ������� CPImportKey � ������ ����� ������� CALG_PRO_EXPORT*/
#define DIVERSKEYBLOB	0x70

/*��� ��������� ����� ��� �������� ���������� � ��������� ������� FKC*/
#define HASHPUBLICKEYEXBLOB 0x71

/* �������������� ��������� ���������������� */
#if !(defined(CPCSP_C_SOURCE ) && CPCSP_C_SOURCE - 0 < 36)
#define PP_LAST_ERROR 90 //deprecated
#endif
#define PP_ENUMOIDS_EX 91
#define PP_HASHOID 92
#define PP_CIPHEROID 93
#define PP_SIGNATUREOID 94
#define PP_DHOID 95
#define PP_RESERVED1 96
#define PP_BIO_STATISTICA_LEN 97
//#pragma deprecated("PP_REBOOT")
#define PP_REBOOT 98
/*��������� �������� ������������ ��� ������� �� ���������, �������� �� WIN32*/
/*#define PP_ANSILASTERROR 99*/
#define PP_RANDOM 100
/*#define PP_DRVCONTAINER	101*/
#define PP_MUTEX_ARG	102
#define PP_ENUM_HASHOID 103
#define PP_ENUM_CIPHEROID 104
#define PP_ENUM_SIGNATUREOID 105
#define PP_ENUM_DHOID	106
#define PP_SET_PIN 107
#define PP_CHANGE_PIN 108
#define PP_HCRYPTPROV 109
#define PP_SELECT_CONTAINER 110
#define PP_FQCN 111
#define PP_CHECKPUBLIC 112
#define PP_ADMIN_CHECKPUBLIC 113
#define PP_ENUMREADERS 114
#define PP_CACHE_SIZE 115
#define PP_NK_SYNC 117
#define PP_INFO 118
#define PP_PIN_INFO 120
#define PP_PASSWD_TERM 123
#define PP_SAME_MEDIA 124
#define PP_DELETE_KEYSET 125
#define PP_DELETE_SAVED_PASSWD 126
#define PP_VERSION_TIMESTAMP 127
#define PP_SECURITY_LEVEL 129
#define PP_FAST_CODE_FUNCS 131
#define PP_CONTAINER_EXTENSION 132
#define PP_ENUM_CONTAINER_EXTENSION 133
#define PP_CONTAINER_EXTENSION_DEL 134
#define PP_CONTAINER_DEFAULT 135
#define PP_LCD_QUERY 136
#define PP_ENUM_LOG 137
#define PP_VERSION_EX 138
#define PP_FAST_CODE_FLAGS 139
#define PP_ENCRYPTION_CARRIER 140
#define PP_FKC				141
#define PP_FRIENDLY_NAME		142
#define PP_FKC_DH_CHECK			143
#define PP_DELETE_SHORTCUT 144

#define CRYPT_MEDIA 0x20
#define CRYPT_FQCN 0x10
#define CRYPT_UNIQUE 0x08
#define CRYPT_FINISH 0x04
#define CRYPT_DELETEKEYSET_PART 0x1
#define ERR_CRYPT_MEDIA_NO_MEDIA "NO_MEDIA"
#define ERR_CRYPT_MEDIA_NO_FKC "NO_FKC"
#define ERR_CRYPT_MEDIA_FKC "IS_FKC"
#define ERR_CRYPT_MEDIA_NO_UNIQUE "NO_UNIQUE"
#define HP_HASHSTARTVECT 0x0008
#define HP_HASHCOPYVAL	 0x0009
#define HP_OID 0x000a
#define HP_OPEN 0x000B
#define HP_KEYMIXSTART 0x0011
#define HP_OPAQUEBLOB 0x000C

#define HP_R2_SIGN 0x000D
#define HP_R_SIGN 0x000E
#define HP_S2_SIGN 0x000F
#define HP_KEYSPEC_SIGN 0x0010

#define KP_SV		KP_IV
#define KP_MIXMODE	101
#define KP_MIXSTART	0x800000e0
#define KP_OID		102
#define KP_HASHOID	103
#define KP_CIPHEROID	104
#define KP_SIGNATUREOID 105
#define KP_DHOID	106
#define KP_KC1EXPORT	0x800000f0
/* Token Interfase NEW */
#define KP_MULX		0x800000f1
#define KP_MULX_INVERS  0x800000f2
#define KP_ADDX		0x800000f3
#define KP_SUBX		0x800000f4
#define KP_ECADD	0x800000f5
#define KP_ECSUB	0x800000f6
#define KP_SYNCRO	0x800000f7
#define KP_DELTA	0x800000f8

#define KP_DEMASKPUBLIC	0x800000f9

/* FKC KP_...  to delete */
#define KP_TOKENRECOVERY	0x800001fb
/* End FKC KP_...  to delete */

/* CRYPT_PRIVATE_KEYS_ALG_OID_GROUP_ID */
#define szOID_CP_GOST_PRIVATE_KEYS_V1 "1.2.643.2.2.37.1"
#define szOID_CP_GOST_PRIVATE_KEYS_V2 "1.2.643.2.2.37.2"
#define szOID_CP_GOST_PRIVATE_KEYS_V2_FULL "1.2.643.2.2.37.2.1"
#define szOID_CP_GOST_PRIVATE_KEYS_V2_PARTOF "1.2.643.2.2.37.2.2"

/* CRYPT_HASH_ALG_OID_GROUP_ID */
#define szOID_CP_GOST_R3411 "1.2.643.2.2.9"

/* CRYPT_ENCRYPT_ALG_OID_GROUP_ID */
#define szOID_CP_GOST_28147 "1.2.643.2.2.21"

/* CRYPT_PUBKEY_ALG_OID_GROUP_ID */
#define szOID_CP_GOST_R3410 "1.2.643.2.2.20"
#define szOID_CP_GOST_R3410EL "1.2.643.2.2.19"
#define szOID_CP_DH_EX "1.2.643.2.2.99"
#define szOID_CP_DH_EL "1.2.643.2.2.98"
#define szOID_CP_GOST_R3410_94_ESDH "1.2.643.2.2.97"
#define szOID_CP_GOST_R3410_01_ESDH "1.2.643.2.2.96"

/* CRYPT_SIGN_ALG_OID_GROUP_ID */
#define szOID_CP_GOST_R3411_R3410 "1.2.643.2.2.4"
#define szOID_CP_GOST_R3411_R3410EL "1.2.643.2.2.3"

/* CRYPT_ENHKEY_USAGE_OID_GROUP_ID */
#define szOID_KP_TLS_PROXY "1.2.643.2.2.34.1"
#define szOID_KP_RA_CLIENT_AUTH "1.2.643.2.2.34.2"
#define szOID_KP_WEB_CONTENT_SIGNING "1.2.643.2.2.34.3"
#define szOID_KP_RA_ADMINISTRATOR "1.2.643.2.2.34.4"
#define szOID_KP_RA_OPERATOR "1.2.643.2.2.34.5"

/* Our well-known policy ID */
#define szOID_CEP_BASE_PERSONAL	"1.2.643.2.2.38.1"
#define szOID_CEP_BASE_NETWORK	"1.2.643.2.2.38.2"

/* OIDs for HASH */
#define szOID_GostR3411_94_TestParamSet			"1.2.643.2.2.30.0"
#define szOID_GostR3411_94_CryptoProParamSet		"1.2.643.2.2.30.1"	/* ���� � 34.11-94, ��������� �� ��������� */
#define szOID_GostR3411_94_CryptoPro_B_ParamSet		"1.2.643.2.2.30.2"
#define szOID_GostR3411_94_CryptoPro_C_ParamSet		"1.2.643.2.2.30.3"
#define szOID_GostR3411_94_CryptoPro_D_ParamSet		"1.2.643.2.2.30.4"

/* OIDs for Crypt */
#define szOID_Gost28147_89_TestParamSet			"1.2.643.2.2.31.0"
#define szOID_Gost28147_89_CryptoPro_A_ParamSet		"1.2.643.2.2.31.1"	/* ���� 28147-89, ��������� �� ��������� */
#define szOID_Gost28147_89_CryptoPro_B_ParamSet		"1.2.643.2.2.31.2"	/* ���� 28147-89, ��������� ���������� 1 */
#define szOID_Gost28147_89_CryptoPro_C_ParamSet		"1.2.643.2.2.31.3" 	/* ���� 28147-89, ��������� ���������� 2 */
#define szOID_Gost28147_89_CryptoPro_D_ParamSet		"1.2.643.2.2.31.4"	/* ���� 28147-89, ��������� ���������� 3 */
#define szOID_Gost28147_89_CryptoPro_Oscar_1_1_ParamSet	"1.2.643.2.2.31.5"	/* ���� 28147-89, ��������� ����� 1.1 */
#define szOID_Gost28147_89_CryptoPro_Oscar_1_0_ParamSet	"1.2.643.2.2.31.6"	/* ���� 28147-89, ��������� ����� 1.0 */
#define szOID_Gost28147_89_CryptoPro_RIC_1_ParamSet	"1.2.643.2.2.31.7"	/* ���� 28147-89, ��������� ��� 1 */

/* OID for Signature 1024*/
#define szOID_GostR3410_94_CryptoPro_A_ParamSet		"1.2.643.2.2.32.2" 	/*VerbaO*/
#define szOID_GostR3410_94_CryptoPro_B_ParamSet		"1.2.643.2.2.32.3"
#define szOID_GostR3410_94_CryptoPro_C_ParamSet		"1.2.643.2.2.32.4"
#define szOID_GostR3410_94_CryptoPro_D_ParamSet		"1.2.643.2.2.32.5"

/* OID for Signature 512*/
#define szOID_GostR3410_94_TestParamSet			"1.2.643.2.2.32.0" 	/*Test*/

/* OID for DH 1024*/
#define szOID_GostR3410_94_CryptoPro_XchA_ParamSet	"1.2.643.2.2.33.1"
#define szOID_GostR3410_94_CryptoPro_XchB_ParamSet	"1.2.643.2.2.33.2"
#define szOID_GostR3410_94_CryptoPro_XchC_ParamSet	"1.2.643.2.2.33.3"

/* OID for EC signature */
#define szOID_GostR3410_2001_TestParamSet		"1.2.643.2.2.35.0"      /* ���� � 34.10-2001, �������� ��������� */
#define szOID_GostR3410_2001_CryptoPro_A_ParamSet	"1.2.643.2.2.35.1"	/* ���� � 34.10-2001, ��������� �� ��������� */
#define szOID_GostR3410_2001_CryptoPro_B_ParamSet	"1.2.643.2.2.35.2"	/* ���� � 34.10-2001, ��������� ����� 2.x */
#define szOID_GostR3410_2001_CryptoPro_C_ParamSet	"1.2.643.2.2.35.3"	/* ���� � 34.10-2001, ��������� ������� 1 */

/* OID for EC DH */
#define szOID_GostR3410_2001_CryptoPro_XchA_ParamSet	"1.2.643.2.2.36.0"	/* ���� � 34.10-2001, ��������� ������ �� ��������� */
#define szOID_GostR3410_2001_CryptoPro_XchB_ParamSet 	"1.2.643.2.2.36.1"	/* ���� � 34.10-2001, ��������� ������ 1 */

/* OIDs for private key container extensions */
/* ���������� ����������. �������������� ������� � CSP 3.6 */
#define szOID_CryptoPro_private_keys_extension_intermediate_store "1.2.643.2.2.37.3.1"
#define szOID_CryptoPro_private_keys_extension_signature_trust_store "1.2.643.2.2.37.3.2"
#define szOID_CryptoPro_private_keys_extension_exchange_trust_store "1.2.643.2.2.37.3.3"
#define szOID_CryptoPro_private_keys_extension_container_friendly_name "1.2.643.2.2.37.3.4"

/* OIDs for certificate and CRL extensions */
/* ����� ������������� CRL � ������������ ��������. */
#define szOID_CryptoPro_extensions_certificate_and_crl_matching_technique "1.2.643.2.2.49.1"

#define szCPOID_RSA_SMIMEaaSigningCertificate "1.2.840.113549.1.9.16.2.12"
#define szCPOID_RSA_SMIMEaaSigningCertificateV2 "1.2.840.113549.1.9.16.2.47"
#define szCPOID_RSA_SMIMEaaETSotherSigCert "1.2.840.113549.1.9.16.2.19"
#define szCPGUID_RSA_SMIMEaaSigningCertificateEncode "{272ED084-4C55-42A9-AD88-A1502D9ED755}"
#define szCPGUID_RSA_SMIMEaaSigningCertificateV2Encode "{42AB327A-BE56-4899-9B81-1BF2F3C5E154}"
#define szCPGUID_RSA_SMIMEaaETSotherSigCertEncode "{410F6306-0ADE-4485-80CC-462DEB3AD109}"
#define szCPGUID_PRIVATEKEY_USAGE_PERIOD_Encode "{E36FC6F5-4880-4CB7-BA51-1FCD92CA1453}"

#define CPCERT_CHAIN_POLICY_PRIVATEKEY_USAGE_PERIOD "{C03D5610-26C8-4B6F-9549-245B5B3AB743}"
#define CPCERT_CHAIN_POLICY_SIGNATURE "{B52FF66F-13A5-402C-B958-A3A6B5300FB6}"
#define CPCERT_CHAIN_POLICY_TIMESTAMP_SIGNING "{AF74EE92-A059-492F-9B4B-EAD239B22A1B}"
#define CPCERT_CHAIN_POLICY_OCSP_SIGNING "{A4CC781E-04E9-425C-AAFD-1D74DA8DFAF6}"

#define OID_HashVar_Default	szOID_GostR3411_94_CryptoProParamSet
#define OID_HashTest		szOID_GostR3411_94_TestParamSet
#define OID_HashVerbaO		szOID_GostR3411_94_CryptoProParamSet
#define OID_HashVar_1		szOID_GostR3411_94_CryptoPro_B_ParamSet
#define OID_HashVar_2		szOID_GostR3411_94_CryptoPro_C_ParamSet
#define OID_HashVar_3		szOID_GostR3411_94_CryptoPro_D_ParamSet

#define OID_CipherVar_Default	szOID_Gost28147_89_CryptoPro_A_ParamSet
#define OID_CryptTest		szOID_Gost28147_89_TestParamSet
#define OID_CipherVerbaO	szOID_Gost28147_89_CryptoPro_A_ParamSet
#define OID_CipherVar_1		szOID_Gost28147_89_CryptoPro_B_ParamSet
#define OID_CipherVar_2		szOID_Gost28147_89_CryptoPro_C_ParamSet
#define OID_CipherVar_3		szOID_Gost28147_89_CryptoPro_D_ParamSet
#define OID_CipherOSCAR		szOID_Gost28147_89_CryptoPro_Oscar_1_1_ParamSet
#define OID_CipherTestHash	szOID_Gost28147_89_CryptoPro_Oscar_1_0_ParamSet
#define OID_CipherRIC1		szOID_Gost28147_89_CryptoPro_RIC_1_ParamSet

#define OID_SignDH128VerbaO	szOID_GostR3410_94_CryptoPro_A_ParamSet
#define OID_Sign128Var_1	szOID_GostR3410_94_CryptoPro_B_ParamSet
#define OID_Sign128Var_2	szOID_GostR3410_94_CryptoPro_C_ParamSet
#define OID_Sign128Var_3	szOID_GostR3410_94_CryptoPro_D_ParamSet
#define OID_DH128Var_1		szOID_GostR3410_94_CryptoPro_XchA_ParamSet
#define OID_DH128Var_2		szOID_GostR3410_94_CryptoPro_XchB_ParamSet
#define OID_DH128Var_3		szOID_GostR3410_94_CryptoPro_XchC_ParamSet

#define OID_Sg64_Test		szOID_GostR3410_94_TestParamSet

#define OID_ECCTest3410		szOID_GostR3410_2001_TestParamSet
#define OID_ECCSignDHPRO	szOID_GostR3410_2001_CryptoPro_A_ParamSet
#define OID_ECCSignDHOSCAR	szOID_GostR3410_2001_CryptoPro_B_ParamSet
#define OID_ECCSignDHVar_1	szOID_GostR3410_2001_CryptoPro_C_ParamSet

#define OID_ECCDHPRO		szOID_GostR3410_2001_CryptoPro_XchA_ParamSet
#define OID_ECCDHPVar_1		szOID_GostR3410_2001_CryptoPro_XchB_ParamSet

/* �������� ��� ������������� � ������� 1.1*/
#define OID_SipherVerbaO	szOID_Gost28147_89_CryptoPro_A_ParamSet
#define OID_SipherVar_1		szOID_Gost28147_89_CryptoPro_B_ParamSet
#define OID_SipherVar_2		szOID_Gost28147_89_CryptoPro_C_ParamSet
#define OID_SipherVar_3		szOID_Gost28147_89_CryptoPro_D_ParamSet
#define OID_SipherVar_Default	szOID_Gost28147_89_CryptoPro_A_ParamSet

#define X509_GR3410_PARAMETERS ((LPCSTR) 5001)
#define OBJ_ASN1_CERT_28147_ENCRYPTION_PARAMETERS ((LPCSTR) 5007)

#define CP_GOST_R3411_ALGORITHM "GOST R 34.11-94"
#define CP_GOST_28147_ALGORITHM "GOST 28147-89"
#define CP_GOST_R3410_ALGORITHM "GOST R 34.10-94"
#define CP_GOST_R3410EL_ALGORITHM "GOST R 34.10-2001"
#define CP_GOST_HMAC_ALGORITHM "HMAC GOST 28147-89"

#define CP_GOST_R3410_ALGORITHM "GOST R 34.10-94"
#define CP_GOST_R3410EL_ALGORITHM "GOST R 34.10-2001"


#define CP_PRIMITIVE_PROVIDER   L"Crypto-Pro Primitive Provider"
#define BCRYPT_CP_GOST_R3411_ALGORITHM L#CP_GOST_R3411_ALGORITHM
#define BCRYPT_CP_GOST_28147_ALGORITHM L#CP_GOST_28147_ALGORITHM
#define BCRYPT_CP_GOST_R3410_ALGORITHM L#CP_GOST_R3410_ALGORITHM
#define BCRYPT_CP_GOST_R3410EL_ALGORITHM L#CP_GOST_R3410EL_ALGORITHM

#define CRYPT_MODE_CBCSTRICT	1

#define CRYPT_MODE_CBCRFC4357	31

#define CRYPT_MODE_CNT          3      // GOST 28147-89 in "gammirovanie" (counter) mode

#define CRYPT_MODE_CNTPACKET       0x80000100 | CRYPT_MODE_CNT

#define SECRET_KEY_LEN		32

#define G28147_KEYLEN        SECRET_KEY_LEN

#define EXPORT_IMIT_SIZE		4

#define SEANCE_VECTOR_LEN		8

#define MAX_CONTAINER_NAME_LEN		260

#define GR3410_1_MAGIC			0x3147414D
#define GR3410_2_MAGIC			GR3410_1_MAGIC//0x3145474a

#define DH_1_MAGIC			GR3410_1_MAGIC
#define DH_2_MAGIC			GR3410_1_MAGIC
#define DH_3_MAGIC			GR3410_1_MAGIC

#define G28147_MAGIC			0x374a51fd
#define SIMPLEBLOB_MAGIC		G28147_MAGIC
#define G28147_OPAQUEMAGIC		0x374a51fe

#define DIVERS_MAGIC			0x31564944

#define BLOB_VERSION			(BYTE)0x20


/* ����������� ��� */
/*! \ingroup ProCSPData
 * \brief ���������� ������ ������������.
 */
#define VER_TYPE_DEBUG 1
/*! \ingroup ProCSPData
* \brief �������� ������ ������������.
*/
#define VER_TYPE_RELEASE 0

/*! \ingroup ProCSPData
* \brief ����������� IA32.
*/
#define VER_ARCH_IA32	    0
/*! \ingroup ProCSPData
* \brief ����������� IA64.
*/
#define VER_ARCH_IA64	    1
/*! \ingroup ProCSPData
* \brief ����������� Sparc32.
*/
#define VER_ARCH_SPARC32    2
/*! \ingroup ProCSPData
* \brief ����������� Sparc64.
*/
#define VER_ARCH_SPARC64    3
/*! \ingroup ProCSPData
* \brief ����������� AMD64.
*/
#define VER_ARCH_AMD64	    4
/*! \ingroup ProCSPData
* \brief ����������� ARM.
*/
#define VER_ARCH_ARM	    5
/*! \ingroup ProCSPData
* \brief ����������� PowerPC32.
*/
#define VER_ARCH_PPC32      6
/*! \ingroup ProCSPData
* \brief ����������� PowerPC64.
*/
#define VER_ARCH_PPC64      7


/*! \ingroup ProCSPData
* \brief �� Windows.
*/
#define VER_OS_WINDOWS 0
/*! \ingroup ProCSPData
* \brief �� Solaris.
*/
#define VER_OS_SOLARIS 1
/*! \ingroup ProCSPData
* \brief �� FreeBSD.
*/
#define VER_OS_FREEBSD 2
/*! \ingroup ProCSPData
* \brief �� Linux.
*/
#define VER_OS_LINUX   3
/*! \ingroup ProCSPData
* \brief �� AIX.
*/
#define VER_OS_AIX     4

/*! \ingroup ProCSPData
* \brief �� Mac OS X.
*/
#define VER_OS_DARWIN  5
/*! \ingroup ProCSPData
* \brief Apple iOS */
#define VER_OS_IOS  6
#define CRYPT_PIN_PASSWD 0
/*! \brief ��� ������: ��� ���������� ������������
     ������������ ��� ����������. */
#define CRYPT_PIN_ENCRYPTION 1
/*! \brief ��� ������: �������� ���������� �� ����� �� HANDLE.
     ������������ ����� �����������. */
#define CRYPT_PIN_NK 2
/*! \brief ��� ������: ���������� */
#define CRYPT_PIN_UNKNOWN 3
/*! \brief ��� ������: ��� � �������� ���������� � ����. */
#define CRYPT_PIN_QUERY 4
/*! \brief ��� ������: �������� ������. */
#define CRYPT_PIN_CLEAR 5
/*! \brief ��� ������: ������������ ���������� �������. */
#define CRYPT_PIN_HARDWARE_PROTECTION 6
/*! \brief ��� ������: ������ ��� FKC ����������, ��� �������������� �� EKE */
#define CRYPT_PIN_FKC_EKE 	7

#define CSP_INFO_SIZE sizeof(CSP_INFO)

#define PROVIDER_TYPE_FKC_MAGISTRA 1

#define CSP_INFO_FREE_SPACE	(0)	/* ��������� ����� �� /var � bytes */
#define CSP_INFO_NUMBER_UL	(1)	/* "\\local\\number_UL" --- ���������� ���������� ������ �� */
#define CSP_INFO_NUMBER_SIGNS	(2)     /* "\\local\\number_signs" --- ���������� �������� ������� */
#define CSP_INFO_KCARDS_CHANGES	(3)     /* "\\local\\Kcard_changes" --- ���������� ���� ���� ������ "�" */
#define CSP_INFO_NUMBER_KCARDS	(4)     /* "\\local\\number_Kcard_sessions" --- ���������� ���������� � ��������� ��� ���� ������ "�" */
#define CSP_INFO_NUMBER_KEYS	(5)     /* "\\local\\number_keys" --- ���������� ����������  */
#define CSP_INFO_FUTURE_SIZE	(10)

#define CPC_FAST_CODE_DEFAULT	0
#define CPC_FAST_CODE_NO	1
#define CPC_FAST_CODE_USER	2

#ifdef UNIX
#if defined(__GNUC__) && !defined(IOS)
#define CPCAPI __attribute__((regparm(0)))
#else // __GNUC__
#define CPCAPI
#endif // __GNUC__
#else // UNIX
#define CPCAPI __cdecl
#endif // UNIX

#define CSP_FAST_CODE_GET_FN	(1<<28)

#define CSP_FAST_CODE_GET_SETFN	(1<<27)

/*!
 *  \brief ����, ������������ �������� \ref CPGetProvParam (PP_FAST_CODE_FLAGS).
 *  ������������ ��� ����, ��� ��������, ����� ���
 *  ����������� � ������� ������������ �� ECB
 *  ����� 1 � ������ �������� ���� ������ �������,
 *  � 0 �����.
 */
#define CSP_FAST_CODE_E_ECB	(1)

/*!
 *  \brief ����, ������������ �������� \ref CPGetProvParam (PP_FAST_CODE_FLAGS).
 *  ������������ ��� ����, ��� ��������, ����� ���
 *  ����������� � ������� ������������ �� CBC
 *  ����� 1 � ������ �������� ���� ������ �������,
 *  � 0 �����.
 */
#define CSP_FAST_CODE_E_CBC	(1<<1)

/*!
 *  \brief ����, ������������ �������� \ref CPGetProvParam (PP_FAST_CODE_FLAGS).
 *  ������������ ��� ����, ��� ��������, ����� ���
 *  ����������� � ������� ������������ �� CNT
 *  ����� 1 � ������ �������� ���� ������ �������,
 *  � 0 �����.
 */
#define CSP_FAST_CODE_E_CNT	(1<<2)

/*!
 *  \brief ����, ������������ �������� \ref CPGetProvParam (PP_FAST_CODE_FLAGS).
 *  ������������ ��� ����, ��� ��������, ����� ���
 *  ����������� � ������� ������������� �� CNT
 *  ����� 1 � ������ �������� ���� ������ �������,
 *  � 0 �����.
 */
#define CSP_FAST_CODE_D_CNT	(1<<2)

/*!
 *  \brief ����, ������������ �������� \ref CPGetProvParam (PP_FAST_CODE_FLAGS).
 *  ������������ ��� ����, ��� ��������, ����� ���
 *  ����������� � ������� ������������ �� CFB
 *  ����� 1 � ������ �������� ���� ������ �������,
 *  � 0 �����.
 */
#define CSP_FAST_CODE_E_CFB	(1<<3)
#define CSP_FAST_CODE_D_ECB	(1<<4)
#define CSP_FAST_CODE_D_CBC	(1<<5)
#define CSP_FAST_CODE_D_CFB	(1<<6)
#define CSP_FAST_CODE_MD_ECB	(1<<7)
#define CSP_FAST_CODE_GR3411SP	(1<<8)
#define CSP_FAST_CODE_GR3411H	(1<<9)
#define CSP_FAST_CODE_GR3411HV	(1<<10)
#define CSP_FAST_CODE_HASH	(1<<11)
#define CSP_FAST_CODE_IMIT	(1<<12)
#define CSP_FAST_CODE_MULT	(1<<13)
#define CSP_FAST_CODE_MULT_ATT	(1<<13)
#define CSP_FAST_CODE_ALL_ENCRYPT (CSP_FAST_CODE_E_ECB|CSP_FAST_CODE_E_CBC|CSP_FAST_CODE_E_CNT|CSP_FAST_CODE_E_CFB)
#define CSP_FAST_CODE_ALL_DECRYPT (CSP_FAST_CODE_D_ECB|CSP_FAST_CODE_D_CBC|CSP_FAST_CODE_D_CNT|CSP_FAST_CODE_D_CFB)
#define CSP_FAST_CODE_ALL_HASH (CSP_FAST_CODE_HASH|CSP_FAST_CODE_GR3411SP|CSP_FAST_CODE_GR3411H|CSP_FAST_CODE_GR3411HV)
#define CSP_FAST_CODE_ALL_MULT (CSP_FAST_CODE_MULT|CSP_FAST_CODE_MULT_ATT)
#define CSP_FAST_CODE_ALL_CRYPT (CSP_FAST_CODE_ALL_ENCRYPT|CSP_FAST_CODE_ALL_DECRYPT|CSP_FAST_CODE_MD_ECB|CSP_FAST_CODE_IMIT)
#define CSP_FAST_CODE_ALL_FUNCTIONS (CSP_FAST_CODE_ALL_CRYPT|CSP_FAST_CODE_ALL_HASH|CSP_FAST_CODE_ALL_MULT)
#define CSP_FAST_CODE_ALL_KERNEL_FUNCTIONS (CSP_FAST_CODE_ALL_CRYPT|CSP_FAST_CODE_ALL_HASH)
#define CSP_FAST_CODE_ALL_USER_FUNCTIONS (CSP_FAST_CODE_ALL_CRYPT|CSP_FAST_CODE_HASH|CSP_FAST_CODE_ALL_MULT)
#define CRYPT_FAST_CODE_ALL_KERNEL_FUNCTIONS	1

#define CRYPT_FAST_CODE_ALL_USER_FUNCTIONS	2

#define CRYPT_FAST_CODE_ALL_FUNCTIONS		(CRYPT_FAST_CODE_ALL_KERNEL_FUNCTIONS|CRYPT_FAST_CODE_ALL_USER_FUNCTIONS)

#define CRYPT_FAST_CODE_GET_SETFN		8

#define CSP_OPERATION_CIPHER1	(CSP_FAST_CODE_E_CFB | CSP_FAST_CODE_E_CBC)

#define CSP_OPERATION_CIPHER2	(CSP_FAST_CODE_E_ECB | CSP_FAST_CODE_E_CNT | CSP_FAST_CODE_D_ECB | CSP_FAST_CODE_D_CBC | CSP_FAST_CODE_D_CNT | CSP_FAST_CODE_D_CFB | CSP_FAST_CODE_MD_ECB)

#define CSP_OPERATION_IMIT	(CSP_FAST_CODE_IMIT)

#define CSP_OPERATION_HASH	(CSP_FAST_CODE_ALL_HASH)

#define CSP_OPERATION_MULT	(CSP_FAST_CODE_ALL_MULT)

#define CSP_OPERATION_ALL	(CSP_OPERATION_MULT | CSP_OPERATION_HASH | CSP_OPERATION_IMIT | CSP_OPERATION_CIPHER2 | CSP_OPERATION_CIPHER1)

#define CSP_OPERATION_UNDEF	(0xFFFFFFFF)


#if !(defined(CPCSP_C_SOURCE ) && CPCSP_C_SOURCE - 0 < 36)
#undef CP_GR3410_94_PROV
#undef CP_KC1_GR3410_94_PROV
#undef CP_KC2_GR3410_94_PROV
#undef PH_GR3410_94_PROV

#undef PROV_GOST_DH
#undef PROV_GOST_94_DH
#endif

#if !defined(CPCSP_USE_NON_STANDART_OIDS) && !(defined(CPCSP_C_SOURCE ) && CPCSP_C_SOURCE - 0 < 36)

/* Deprecated cipher mode*/
#undef CRYPT_MODE_CBC

/* OIDs for HASH */
#undef szOID_GostR3411_94_TestParamSet
#undef szOID_GostR3411_94_CryptoPro_B_ParamSet
#undef szOID_GostR3411_94_CryptoPro_C_ParamSet
#undef szOID_GostR3411_94_CryptoPro_D_ParamSet

/* OIDs for Crypt */
#undef szOID_Gost28147_89_TestParamSet
#undef szOID_Gost28147_89_CryptoPro_Oscar_1_1_ParamSet
#undef szOID_Gost28147_89_CryptoPro_Oscar_1_0_ParamSet
#undef szOID_Gost28147_89_CryptoPro_RIC_1_ParamSet

/* OID for Signature 1024*/
#undef szOID_GostR3410_94_CryptoPro_A_ParamSet
#undef szOID_GostR3410_94_CryptoPro_B_ParamSet
#undef szOID_GostR3410_94_CryptoPro_C_ParamSet
#undef szOID_GostR3410_94_CryptoPro_D_ParamSet

/* OID for Signature 512*/
#undef szOID_GostR3410_94_TestParamSet

/* OID for DH 1024*/
#undef szOID_GostR3410_94_CryptoPro_XchA_ParamSet
#undef szOID_GostR3410_94_CryptoPro_XchB_ParamSet
#undef szOID_GostR3410_94_CryptoPro_XchC_ParamSet

/* OID for EC signature */
#undef szOID_GostR3410_2001_TestParamSet

#endif

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _WINCRYPTEX_H_INCLUDED
