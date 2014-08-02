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

#ifndef BRIDGE_HPP
#define BRIDGE_HPP

#include <cstdlib>

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/date_time/posix_time/posix_time_io.hpp>

#include "common.hpp"
#include "sspi_win.hpp"

namespace ip = boost::asio::ip;
namespace tcp_proxy
{
   class bridge;
}

class bridge : public boost::enable_shared_from_this<bridge>
{
public:

  typedef ip::tcp::socket socket_type;
  typedef boost::shared_ptr<bridge> ptr_type;
  typedef std::shared_ptr<boost::asio::ssl::context> context_ptr;
  typedef boost::asio::ssl::stream<ip::tcp::socket> ssl_socket;
  typedef boost::function<void(const boost::system::error_code& error)> handler_connect_type;
  typedef boost::function<void(const boost::system::error_code& error, std::size_t bytes_transferred)> handler_type;

  bridge(boost::asio::io_service& ios, const std::string& cname, context_ptr ctx)
    : isClosed(false),
      resolver_(ios),
      isSSL(true),
      downstream_socket_(ios, *ctx),
      upstream_socket_(ios),
      number(++count),
      number_string(" (" + std::to_string(number) + ")"),
      isContextInit(false),
      cname_(cname)
  {
    upstream_data_ = new char[max_data_length];
    downstream_data_ = new char[max_data_length];
    upstream_data_io = nullptr;
    downstream_data_io = nullptr;
  }

  ~bridge()
  {
    BLOG(trace) << VIEW_FUNCTION;
    delete []upstream_data_;
    delete []downstream_data_;
    if (downstream_data_io != nullptr) {
      delete []downstream_data_io;
    }
    if (upstream_data_io != nullptr) {
      delete []upstream_data_io;
    }
    if (isCredInit) {
      //FreeCredentialsHandle(&hClientCreds);
    }
    if (isContextInit) {
      DeleteSecurityContext(&hContext);
      isContextInit = false;
    }
  }

  socket_type& downstream_socket() {
    return downstream_socket_.next_layer();
  }

  socket_type& upstream_socket() {
    return upstream_socket_;
  }

  void start(const std::string& upstream_host, unsigned short upstream_port);

  void handle_resolve(const boost::system::error_code& err,
                      ip::tcp::resolver::iterator endpoint_iterator, bool first_time);

  void handle_upstream_connect(const boost::system::error_code& error);

  struct exception : public std::exception {
    std::string s;
    exception(std::string ss) : s(ss) {}
    ~exception() throw () {}
    const char* what() const throw() { return s.c_str(); }
  };

private:

  void handle_downstream_write(const boost::system::error_code& error/*,
                               const size_t& bytes_transferred*/);
  void handle_downstream_read(const boost::system::error_code& error,
                              const size_t& bytes_transferred);
  void handle_upstream_write(const boost::system::error_code& error);
  void handle_upstream_read(const boost::system::error_code& error,
                            const size_t& bytes_transferred);
  // к/от браузера
  bool downstream_handshake();
  void downstream_async_write(char *data, size_t size, handler_type handler);
  void downstream_async_read_some(char *data, size_t size, handler_type handler);

  void upstream_handshake();
  void upstream_shutdown();
  std::size_t encrypt_message(char *message, std::size_t length);
  std::size_t decrypt_message(char *message, std::size_t length);
  void setBuffers();

  void close();

  std::string cname_;

  bool isClosed;

  std::string upstream_host_;

  ip::tcp::resolver resolver_;

  bool isSSL;
  ssl_socket downstream_socket_;
  socket_type upstream_socket_;

  static bool isCredInit;
  static CredHandle hClientCreds;
  bool isContextInit;
  CtxtHandle hContext;

  SecPkgContext_StreamSizes sizes;
  enum {max_data_length = 9009}; // Размер, который может расшифровать DecryptMessage за раз

  char *downstream_data_io;
  char *upstream_data_io;

  char *downstream_data_;
  char *upstream_data_;

  static int count;
  int number;
  std::string number_string;

  // boost::mutex mutex_;
};
#endif