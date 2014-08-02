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
#include "common.hpp"
#include "settings.hpp"

#include <boost/filesystem.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>

namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;

void init_log(logging::trivial::severity_level log_level, std::string& log_filename) {
  logging::core::get()->set_filter(logging::trivial::severity >= log_level);
  logging::core::get()->add_global_attribute(
        "TimeStamp", 
        boost::log::attributes::local_clock());
  logging::register_simple_formatter_factory<logging::trivial::severity_level, char>("Severity");
  if (log_filename.size() != 0) {
    logging::add_file_log
    (
      keywords::file_name = log_filename,
      keywords::rotation_size = 5 * 1024 * 1024,
      keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0),
      keywords::format = "[%TimeStamp%] (%Severity%)\t: %Message%"
    );
  }
}

void print_setting(const settings& st) {
  BLOG(debug) << "config: " << st.config_filename;
  BLOG(debug) << "section: " << st.section;
  BLOG(debug) << "log: " << st.log_filename;
  BLOG(debug) << "level: " << st.log_level;
  BLOG(debug) << "remote: " << st.remote.host << ":" << st.remote.port;
  BLOG(debug) << "cname: " << st.cn;
  BLOG(debug) << "local: " << st.local.host << ":" << st.local.port;
  BLOG(debug) << "cert: " << st.cert_filename;
  BLOG(debug) << "key: " << st.key_filename;
  BLOG(debug) << "dh: " << st.dh_filename;
}

int main(int argc, char* argv[])
{
  setlocale(LC_ALL, "");

  settings setting;
  try {
    if (!setting.load(argc, argv)) {
      return 0;
    }
  }
  catch (const boost::property_tree::ptree_error& error) {
    std::cerr << "Error: " << error.what() << std::endl;
    return 1;
  }
  catch (const boost::program_options::error& error) {
    std::cerr << "Error: " << error.what() << std::endl;
    return 1;
  }
  catch (const std::exception& error) {
    std::cerr << "Error: " << error.what() << std::endl;
    return 1;
  }

  print_setting(setting);

  if (setting.log_filename.size() != 0 && 
      !boost::filesystem::exists(setting.log_filename)) {
    std::cerr << "Log file path error " + setting.log_filename;
    setting.log_filename = "logs/" + setting.section + ".log";
  }

  init_log(setting.log_level, setting.log_filename);

  boost::asio::io_service ios;

  try {
    acceptor acceptor(ios, setting.local.host, setting.local.port,
                      setting.remote.host, setting.remote.port,
                      setting.cn,
                      certs_path(setting.cert_filename,
                      setting.key_filename,
                      setting.dh_filename));
    BLOG(info) << "accept connections on " 
               << setting.local.host << ":" << setting.local.port 
               << ", remote host " 
               << setting.remote.host << ":" << setting.remote.port;
    acceptor.accept_connections();
    ios.run();
  }
  catch(std::exception& e) {
    std::cerr << std::string("Error: ") + e.what() << std::endl;
    return 1;
  }

  return 0;
}
