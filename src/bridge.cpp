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

#include "bridge.hpp"

bool bridge::isCredInit;
CredHandle bridge::hClientCreds;
int bridge::count = 0;

void bridge::start(const std::string& upstream_host, unsigned short upstream_port)
{
  BLOG(trace) << VIEW_FUNCTION << number_string;
  BLOG(info) << "start new connection with number " << number; 
  upstream_host_ = upstream_host;
  try {
    isSSL = downstream_handshake();
  }
  catch (const std::exception& e) {
    BLOG(error) << e.what() << number_string;
    close();
  }

  ip::tcp::resolver::query q(upstream_host, std::to_string(upstream_port));
  resolver_.async_resolve(q,
                boost::bind(&bridge::handle_resolve, shared_from_this(),
                      boost::asio::placeholders::error,
                      boost::asio::placeholders::iterator, true));
}

void bridge::handle_resolve(const boost::system::error_code& err,
                      ip::tcp::resolver::iterator endpoint_iterator, bool first_time)
{
  BLOG(trace) << VIEW_FUNCTION << number_string;
  if (!err && !first_time) {
    handle_upstream_connect(err);
  } else if (endpoint_iterator != ip::tcp::resolver::iterator()) {
    BLOG(trace) << "handle connect first_time" << number_string;
    ip::tcp::endpoint endpoint = *endpoint_iterator;
    BLOG(debug) << "start connect to " << endpoint.address() << number_string;
    upstream_socket_.async_connect(endpoint,
                 boost::bind(&bridge::handle_resolve, shared_from_this(),
                       boost::asio::placeholders::error,
                       ++endpoint_iterator, 
                       false));
  } else {
    BLOG(error) << "error to resolve " << upstream_host_ 
                << ": " << err.message() << number_string;
    std::cerr << err.message();
    close();
  }
}

void bridge::handle_upstream_connect(const boost::system::error_code& error)
{
  BLOG(trace) << VIEW_FUNCTION << number_string;
  if (!error) {
    try {
      upstream_handshake();
    }
    catch (const bridge::exception& e) {
      BLOG(error) << e.what() << number_string;
      std::cerr << e.what();
      close();
    }

    upstream_socket_.async_read_some(
         boost::asio::buffer(upstream_data_,max_data_length),
         boost::bind(&bridge::handle_upstream_read,
              shared_from_this(),
              boost::asio::placeholders::error,
              boost::asio::placeholders::bytes_transferred));

    downstream_async_read_some(downstream_data_,max_data_length,
         boost::bind(&bridge::handle_downstream_read,
              shared_from_this(),
              boost::asio::placeholders::error,
              boost::asio::placeholders::bytes_transferred));
  } else {
    close();
  }
}

void bridge::handle_downstream_write(const boost::system::error_code& error/*,
                                     const size_t& bytes_transferred*/)
{
  BLOG(trace) << VIEW_FUNCTION << number_string;
  if (!error) {
    upstream_socket_.async_read_some(
         boost::asio::buffer(upstream_data_, max_data_length),
         boost::bind(&bridge::handle_upstream_read,
              shared_from_this(),
              boost::asio::placeholders::error,
              boost::asio::placeholders::bytes_transferred));
  } else {
    close();
  }
}

void bridge::handle_downstream_read(const boost::system::error_code& error,
                                    const size_t& bytes_transferred)
{
  BLOG(trace) << VIEW_FUNCTION << number_string;
  if (!error) {
    std::string s(downstream_data_, bytes_transferred);
    std::size_t pos = s.find("\r\n");
    if (pos != std::string::npos) {
      BLOG(debug) << s.substr(0, pos) << number_string;
    }
    // encrypt
    std::size_t length;
    try {
      length = encrypt_message(downstream_data_, bytes_transferred);
    }
    catch (...) {
      BLOG(error) << "Error encryption data" << number_string;
    }
    BLOG(debug) << "Encrypt " << bytes_transferred << " to " << length;
    async_write(upstream_socket_,
            boost::asio::buffer(downstream_data_, length),
            boost::bind(&bridge::handle_upstream_write,
                  shared_from_this(),
                  boost::asio::placeholders::error));
  } else {
    close();
  }
}

void bridge::handle_upstream_write(const boost::system::error_code& error)
{
  if (!error) {
    BLOG(trace) << VIEW_FUNCTION << number_string;
    downstream_async_read_some(downstream_data_, max_data_length,
         boost::bind(&bridge::handle_downstream_read,
              shared_from_this(),
              boost::asio::placeholders::error,
              boost::asio::placeholders::bytes_transferred));
  } else {
    close();
  }
}

void bridge::handle_upstream_read(const boost::system::error_code& error,
                                  const size_t& bytes_transferred)
{
  BLOG(trace) << VIEW_FUNCTION << number_string;
  if (!error) {
    // decrypt
    std::size_t length = bytes_transferred;
    length = decrypt_message(upstream_data_, bytes_transferred);
    BLOG(debug) << "Decrypt " << bytes_transferred << " bytes to " << length << number_string;
    // std::cout << std::string(upstream_data_, length) << std::endl;
    downstream_async_write(upstream_data_, length,
         boost::bind(&bridge::handle_downstream_write,
              shared_from_this(),
              boost::asio::placeholders::error));
  } else {
    BLOG(error) << error.message() << number_string;
    close();
  }
}

void bridge::close()
{
  BLOG(trace) << VIEW_FUNCTION << number_string;
  if (!isClosed) {
    BLOG(warning) << "close downstream" << number_string;
    downstream_socket().close();
    BLOG(warning) << "close upstream" << number_string;
    upstream_shutdown();
    upstream_socket_.close();
    isClosed = true;
  }
}

bool bridge::downstream_handshake()
{
  char handshake_data[1024];
  bool recv = false;
  try {
    size_t size = downstream_socket().receive(boost::asio::buffer(handshake_data),
                                              ip::tcp::socket::message_peek);
    recv = true;
  }
  catch(const std::exception& e) {
    BLOG(warning) << "Error receive message peek" << number_string;
    // no need to throwing, because if error then http
  }
  if (recv && !isalpha(handshake_data[0])) { //TODO
    try {
      downstream_socket_.handshake(boost::asio::ssl::stream_base::server);
    } 
    catch(const std::exception& e) {
      BLOG(error) << "Error handshake downstream";
      throw e;
    }
    return true;
  } else {
    return false;
  }
}

void bridge::downstream_async_write(char *buffer, size_t size, handler_type handler)
{
  if (isSSL) {
    async_write(downstream_socket_, boost::asio::buffer(buffer, size), handler);
  } else {
    async_write(downstream_socket_.next_layer(), boost::asio::buffer(buffer, size), handler);
  }
}

void bridge::downstream_async_read_some(char *buffer, size_t size, handler_type handler)
{
  isSSL;
  if (isSSL) {
    downstream_socket_.async_read_some(boost::asio::buffer(buffer, size), handler);
  } else {
    downstream_socket_.next_layer().async_read_some(boost::asio::buffer(buffer, size), handler);
  }
}

void bridge::upstream_handshake()
{
  if(!isCredInit) {
    long res = CreateCredentials(&hClientCreds);
    if(!res) { // complete
      isCredInit = true;
    } else {
      isCredInit = false;
      std::string err;
      switch (res) {
        case SEC_E_CANNOT_INSTALL: err = "Error opening the system store";
          break;
        case SEC_E_NOT_SUPPORTED: err = "Do not select a certificate";
          break;
        default: err = "Error creating credentials";
      }
      throw bridge::exception(err);
    }
  }
  if(!PerformClientHandshake(upstream_socket_, &hClientCreds, cname_, &hContext)) {
    if(!AuthenticateServerCertificate(&hContext, cname_)) {
      isContextInit = true;
      try {
        setBuffers();
      }
      catch (const bridge::exception& e) {
        throw e;
      }
    } else {
      // очстить мандат и флаг в false
      isContextInit = false;
      throw bridge::exception("Error authenticate " + cname_); // TODO: server true
    }
  } else {
    throw bridge::exception("Error handshake to " + upstream_host_);
  }
}

void bridge::upstream_shutdown()
{
  BLOG(info) << "isContextInit: " << isContextInit;
  if (isContextInit) {
    DisconnectFromServer(upstream_socket_, &hClientCreds, &hContext);
    isContextInit = false;
  }
}

std::size_t bridge::encrypt_message(char *message, std::size_t length)
{
  BLOG(trace) << VIEW_FUNCTION << number_string;

  SecBuffer Buffers[4];
  SecBufferDesc Message;

  Buffers[0].BufferType = SECBUFFER_STREAM_HEADER;
  Buffers[0].cbBuffer = sizes.cbHeader;
  Buffers[0].pvBuffer = downstream_data_io;

  memcpy(downstream_data_io + sizes.cbHeader, message, length);

  Buffers[1].BufferType = SECBUFFER_DATA;
  Buffers[1].cbBuffer = length;
  Buffers[1].pvBuffer = downstream_data_io + sizes.cbHeader;

  Buffers[2].BufferType = SECBUFFER_STREAM_TRAILER;
  Buffers[2].cbBuffer = sizes.cbTrailer;
  Buffers[2].pvBuffer = downstream_data_io + sizes.cbHeader + length;

  Buffers[3].BufferType = SECBUFFER_EMPTY;

  Message.ulVersion = SECBUFFER_VERSION;
  Message.cBuffers = 4;
  Message.pBuffers = Buffers;

  long reslt = EncryptMessage(&hContext, 0, &Message, 0);

  // обработать ошибки (отдельная функция -> текст) и вызвать искючения

  std::size_t size = Buffers[0].cbBuffer + Buffers[1].cbBuffer + Buffers[2].cbBuffer;
  memcpy(message, downstream_data_io, size);

  return size;
}

std::size_t bridge::decrypt_message(char *message, std::size_t length)
{
  BLOG(trace) << VIEW_FUNCTION << number_string;

  SecBuffer Buffers[4];
  SecBuffer *pDataBuffer, *pExtraBuffer;
  SecBufferDesc Message;
  std::size_t size = 0,
              count = length;

  memcpy(upstream_data_io, message, length);

  while(true) {
    pDataBuffer = pExtraBuffer = nullptr;

    Buffers[0].pvBuffer = upstream_data_io;
    Buffers[0].cbBuffer = count;
    Buffers[0].BufferType = SECBUFFER_DATA;

    Buffers[1].BufferType = SECBUFFER_EMPTY;
    Buffers[2].BufferType = SECBUFFER_EMPTY;
    Buffers[3].BufferType = SECBUFFER_EMPTY;

    Message.ulVersion = SECBUFFER_VERSION;
    Message.cBuffers = 4;
    Message.pBuffers = Buffers;

    long result = DecryptMessage(&hContext, &Message, 0, NULL);

    if (result == SEC_E_MESSAGE_ALTERED) {
      BLOG(warning) << "SEC_E_MESSAGE_ALTERED" << number_string;
    }
    if (result == SEC_I_CONTEXT_EXPIRED) {
      BLOG(warning) << "SEC_I_CONTEXT_EXPIRED" << number_string;
      close();
    }
    if (result == SEC_I_RENEGOTIATE) {
      BLOG(warning) << "SEC_I_RENEGOTIATE" << number_string;
    }

    for(int i = 0; i < 4; i++) {
      if(pDataBuffer == nullptr && Buffers[i].BufferType == SECBUFFER_DATA) {
        pDataBuffer = &Buffers[i];
        BLOG(debug) << "SECBUFFER_DATA = " << i;
      }
      if(pExtraBuffer == nullptr && Buffers[i].BufferType == SECBUFFER_EXTRA) {
        pExtraBuffer = &Buffers[i];
        BLOG(warning) << "SECBUFFER_EXTRA = " << i << number_string;
      }
      BLOG(trace) << "Buffers[" << i << "] = " << Buffers[i].cbBuffer;
    }

    if (result == SEC_E_INCOMPLETE_MESSAGE) {
      BLOG(warning) << "SEC_E_INCOMPLETE_MESSAGE: was readed " << count << number_string;
      memcpy(upstream_data_io, message, count);
      try {
        count += upstream_socket_.read_some(boost::asio::buffer(upstream_data_io + count, sizes.cbHeader + max_data_length + sizes.cbTrailer - count));
      } 
      catch (const std::exception& e) {
        BLOG(error) << e.what();
      }
      continue;
    }

    if (pDataBuffer != nullptr) {
      memcpy(upstream_data_ + size, pDataBuffer->pvBuffer, pDataBuffer->cbBuffer);
      size += pDataBuffer->cbBuffer;
    }
    if (pExtraBuffer != nullptr) {
      count = pExtraBuffer->cbBuffer;
      memcpy(upstream_data_io, pExtraBuffer->pvBuffer, count);
    } else {
      break;
    }
  }

  return size;
}

void bridge::setBuffers()
{
  long scRet = QueryContextAttributes(&hContext, SECPKG_ATTR_STREAM_SIZES, &sizes);
  if(scRet != SEC_E_OK) {
    throw bridge::exception("Error 0x" + std::to_string(scRet) + " reading SECPKG_ATTR_STREAM_SIZES");
  }
  // Выделение рабочего буфера. Открытый текст, передаваемый EncryptMessage,
  // долженен провосходить по размерам 'Sizes.cbMaximumMessage', поэтому
  // размер буфера равен этой величене, сложеной с размерами заголовка и
  // заключительной части.
  // TODO: message must be no greater than cbMaximumMessage
  BLOG(debug) << "Header: " << sizes.cbHeader << " byte, Trailer: " << sizes.cbTrailer << 
                 " byte, MaxMessage: " << sizes.cbMaximumMessage << " byte";

  downstream_data_io = new char[sizes.cbHeader + max_data_length /*sizes.cbMaximumMessage*/ + sizes.cbTrailer];
  upstream_data_io = new char[sizes.cbHeader + max_data_length /* sizes.cbMaximumMessage*/ + sizes.cbTrailer];
  //pbIoBuffer = new char[max_data_length];
  //if(pbIoBuffer == nullptr) {
  //  throw bridge::exception("Out of memory");
  //}
}