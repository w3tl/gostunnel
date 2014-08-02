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

#ifndef COMMON_HPP
#define COMMON_HPP

#include <iostream>
#include <string>

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/filesystem.hpp>

namespace logging = boost::log;

struct certs_path {
    certs_path(std::string& c, std::string& k, std::string& d) :
        cert(c), key(k), dh(d) {}
    std::string cert;
    std::string key;
    std::string dh;
};

#define BLOG(log) BOOST_LOG_TRIVIAL( log )

#ifdef __MINGW__
    #define MINGW_STUFF
#endif
#ifdef __MINGW32__
    #define MINGW_STUFF
#endif
#if defined(__GNUC__) || defined(MINGW_STUFF)
    #define GCC_COMPILER
#endif
#if (_MSC_VER)
    #define VISUAL_STUDIO
#endif

#ifdef GCC_COMPILER
    #undef VIEW_FUNCTION
    #define VIEW_FUNCTION __PRETTY_FUNCTION__
#else
    #ifdef VISUAL_STUDIO
        #undef VIEW_FUNCTION
        #define VIEW_FUNCTION __FUNCSIG__
    #else
        #undef VIEW_FUNCTION
        #define VIEW_FUNCTION __FUNCTION__
    #endif
#endif

#endif