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

#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#define VERSION "GOSTunnel 1.0"
#define COPYRIGHT "Copyright (C) 2014 Dostovalov Vitaly\nEmail: mail[a]dostovalov.tk"

#include <string>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/program_options.hpp>
#include <boost/program_options/value_semantic.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>

namespace po = boost::program_options;
namespace bl = boost::log::trivial;

class settings {
public:

  std::string section;
  std::string config_filename;
  std::string log_filename;
  boost::log::trivial::severity_level log_level;
  struct point {
    std::string host;
    unsigned short port;
  } local, remote;
  std::string cn;
  std::string cert_filename;
  std::string key_filename;
  std::string dh_filename;

  bool load(int argc, char *argv[]);

private:

  void fill_points(const boost::property_tree::ptree& p);
  void fill_log(const boost::property_tree::ptree& p);
};

#endif
