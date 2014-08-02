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
 
#include "acceptor.hpp"

bool acceptor::accept_connections()
{
  BLOG(trace) << VIEW_FUNCTION;
  try
  {
    session_ = boost::shared_ptr<bridge>(new bridge(io_service_, cname_, context));
    acceptor_->async_accept(session_->downstream_socket(),
      boost::bind(&acceptor::handle_accept,
                   this,
                   boost::asio::placeholders::error));
  }
  catch(std::exception& e)
  {
    BLOG(error) << "acceptor exception: " << e.what();
    return false;
  }
  return true;
}

void acceptor::handle_accept(const boost::system::error_code& error)
{
  BLOG(trace) << VIEW_FUNCTION;
  if (!error) {
    session_->start(upstream_host_, upstream_port_);
    if (!accept_connections()) {
      BLOG(error) << "Failure during call to accept.";
    }
  } else {
    BLOG(error) << "Error: " << error.message();
  }
}

bool acceptor::init_context(certs_path& crt)
{
  BLOG(trace) << VIEW_FUNCTION;
  try {
    context = std::make_shared<boost::asio::ssl::context>(boost::asio::ssl::context::sslv23);
    context->set_options(
            boost::asio::ssl::context::default_workarounds
            | boost::asio::ssl::context::no_sslv2
            | boost::asio::ssl::context::single_dh_use);
    context->use_certificate_chain_file(crt.cert);
    context->use_private_key_file(crt.key, boost::asio::ssl::context::pem);
    context->use_tmp_dh_file(crt.dh);
  }
  catch(std::exception& e) {
    BLOG(error) << e.what();
    std::cerr << e.what();
    return false;
  }
  return true;
}