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

#ifndef ACCEPTOR
#define ACCEPTOR

#include "bridge.hpp"

class acceptor
{
public:
  typedef boost::shared_ptr<ip::tcp::acceptor> acceptor_ptr;

  acceptor(boost::asio::io_service& io_service,
            const std::string& local_host, unsigned short local_port,
            const std::string& upstream_host, unsigned short upstream_port,
            const std::string& cname, certs_path cert)
   : io_service_(io_service),
     resolver_(io_service),
     local_host_(local_host),
     local_port_(local_port),
     upstream_port_(upstream_port),
     upstream_host_(upstream_host),
     cname_(cname)
  {
    BLOG(info) << "starting gostunnel...";
    ip::tcp::resolver::query q(local_host_, std::to_string(local_port_));
    ip::tcp::resolver::iterator iter;
    try {
      iter = resolver_.resolve(q);
    }
    catch (boost::system::error_code & ec) {
      BLOG(error) << "error resolve " << local_host_;
    }
    ip::tcp::endpoint ep = *iter;
    std::cout << ep.address() << std::endl;
    acceptor_ = boost::shared_ptr<ip::tcp::acceptor>(new ip::tcp::acceptor(io_service_, ep));
    isInitContext = init_context(cert);
  }

   bool accept_connections();

private:

   void handle_accept(const boost::system::error_code& error);

   bool init_context(certs_path& crt);

   boost::asio::io_service& io_service_;
   ip::tcp::resolver resolver_;
   acceptor_ptr acceptor_;
   bridge::ptr_type session_;

   std::string local_host_;
   unsigned short local_port_;
   std::string upstream_host_;
   unsigned short upstream_port_;
   std::string cname_;

   bool isInitContext;
   bridge::context_ptr context;
};

#endif