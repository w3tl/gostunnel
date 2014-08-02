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

#include "settings.hpp"

bool settings::load(int argc, char *argv[]) 
{
	po::variables_map conf_vm, cmd_vm;
  boost::property_tree::ptree pt;
	po::options_description command("Command options");
	po::options_description program("Config file options");
	command.add_options()
  	("help,h", "produce help message")
    ("version,v", "print program info")
    ("config", po::value<std::string>(&config_filename)->default_value("gostunnel.conf")->implicit_value(""), "path to config file")
    ("section,s", po::value<std::string>(&section)->default_value("General")->implicit_value(""), "section in config file")
    ("log", po::value<std::string>(&log_filename)->default_value("log.txt"), "path to log file")
    ("trace,t", po::value<bool>()->zero_tokens()->default_value(false), "debug level")
    ("debug,d", po::value<bool>()->zero_tokens()->default_value(false), "debug level")
    ("config -h", "display help on the settings file");
 	program.add_options()
 		("log-file", po::value<std::string>(&log_filename)->default_value("logs/General.log"), "path to log file")
 		("log-level", po::value<unsigned short>()->default_value(2), "= log level (0 - error only, 4 - trace log)")
 		("remote-host", "remote host (ip|uri)")
 		("remote-port", "remote port")
 		("local-host", "local host (ip only)")
 		("local-port", "local port")
    ("cn", po::value<std::string>()->implicit_value(""), 
      "field \"Common Name\" server certificate, by default, equal to the parameter \"remote-host\"")
 		("cert-filename", po::value<std::string>()->default_value("certs/localhost.crt"), "certificate chain file")
 		("key-filename", po::value<std::string>()->default_value("certs/localhost.key"), "private key file")
 		("dh-filename", po::value<std::string>()->default_value("certs/dh.pem"), "the specified file to obtain the temporary Diffie-Hellman parameters");	    
    
    po::store(po::command_line_parser(argc, argv).options(command).run(), cmd_vm);
		po::notify(cmd_vm);
		if (cmd_vm.count("help")) {
			if (cmd_vm["config"].as<std::string>().length() == 0) {
        std::cout << "The program gostunnel supports the syntax INI-file settings. "
          "Each individual connection must begin with the [Name], "
          "should go after setting type \"parameter = value\". "
          "The name of the connection, which will run the program, "
          "the key is \"-s NameOfConnection.\" If you do not specify "
          "this key, the program will use the default value (section [General]).\n" << std::endl;
				std::cout << program << std::endl;
			} else {
   			std::cout << command << std::endl;
   		}
   		return false;
  	}
  	if (cmd_vm.count("version")) {
   		std::cout << VERSION << "\n"
                << COPYRIGHT << std::endl;
   		return false;
  	}
    boost::property_tree::ini_parser::read_ini(config_filename, pt);
    fill_points(pt);
  	if (cmd_vm.count("debug") && cmd_vm["debug"].as<bool>()) {
  		log_filename.clear();
  		log_level = bl::debug;
  	} else { 
  		if (cmd_vm.count("trace") && cmd_vm["trace"].as<bool>()) {
  			log_filename.clear();
  			log_level = bl::trace;
  		} else {
  			fill_log(pt);
  		}
  	}
  	return true;
}

void settings::fill_points(const boost::property_tree::ptree& p) {
  log_filename = p.get<std::string>(section + ".log-file", 
                                    "logs/" + section + ".log");
  if (log_filename.size() == 0) {
    log_filename = "logs/" +  section + ".log";
  }
	remote.host = p.get<std::string>(section + ".remote-host");
	remote.port = p.get<unsigned short>(section + ".remote-port");
	local.host = p.get<std::string>(section + ".local-host");
	local.port = p.get<unsigned short>(section + ".local-port");
  cn = p.get<std::string>(section + ".cn", remote.host);
  cert_filename = p.get<std::string>(section + ".cert-filename", 
                                     "certs/localhost.crt");
  key_filename = p.get<std::string>(section + ".key-filename", 
                                    "certs/localhost.key");
  dh_filename = p.get<std::string>(section + ".dh-filename", 
                                   "certs/dh.pem");
}

void settings::fill_log(const boost::property_tree::ptree& p) {
	switch (p.get<unsigned short>(section + ".log-level", 2)) {
		case 0: log_level = bl::error; break;
		case 1: log_level = bl::warning; break;
		case 2: log_level = bl::info; break;
		case 3: log_level = bl::debug; break;
		case 4: log_level = bl::trace; break;
		default: log_level = bl::error; break;
		}
}
