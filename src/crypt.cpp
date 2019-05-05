// Copyright (c) 2009-2012 The Bitcoin developers
// Copyright (c) 2019 The SorachanCoin Developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
//
// OpenSSL
// This product includes software developed by the OpenSSL Project for use in the OpenSSL Toolkit (http://www.openssl.org/). 

#include "sync.h"
#include "rand.h"
#include "crypt.h"

sync *openssl_multithreading_support::pmutexOpenSSL = nullptr;
openssl_multithreading_support openssl_multithreading_support::instance_of_oms;
