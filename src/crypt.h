// Copyright (c) 2009-2012 The Bitcoin developers
// Copyright (c) 2019 The SorachanCoin Developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
//
// OpenSSL
// This product includes software developed by the OpenSSL Project for use in the OpenSSL Toolkit (http://www.openssl.org/). 

#ifndef SORACHANCOIN_CRYPT_H
#define SORACHANCOIN_CRYPT_H
#pragma warning(disable: 4355)

#include <windows.h>
#include <limits>
#include <openssl/crypto.h>
#include <openssl/rand.h>
#include "common.h"
#include "sync.h"
#undef max

//
// referrence: the bitcoin util.cpp
// OpenSSL library multithreading support (Leave ALL memory allocation to the 'OPENSSL_malloc'.)
//
class openssl_multithreading_support
{
private:
    openssl_multithreading_support(const openssl_multithreading_support &); // {}
    openssl_multithreading_support &operator=(const openssl_multithreading_support &); // {}

    static sync *pmutexOpenSSL;
    static openssl_multithreading_support instance_of_oms;

    static void __cdecl locking_callback(int mode, int i, const char *, int) {
        if (mode & CRYPTO_LOCK) {
            pmutexOpenSSL[i].enter();
        } else {
            pmutexOpenSSL[i].leave();
        }
    }

    void RandAddSeed() const {
        //
        // Seed with CPU performance counter
        //
        LARGE_INTEGER li;
        if(! ::QueryPerformanceCounter(&li)) {
            throw std::runtime_error("openssl_multithreading_support QueryPerformanceCounter failure.");
        }
        __int64 nCounter = li.QuadPart;
        ::RAND_add(&nCounter, sizeof(nCounter), 1.5);
        ::FillMemory(&nCounter, sizeof(nCounter), 0x00);
    }

private:
    openssl_multithreading_support() {
        pmutexOpenSSL = (sync *)::OPENSSL_malloc(::CRYPTO_num_locks() * sizeof(sync));
        if(! pmutexOpenSSL) {
            throw std::runtime_error("openssl_multithreading_support OPENSSL_malloc allocate failure.");
        }

        for (int i=0; i < CRYPTO_num_locks(); ++i)
        {
            new(pmutexOpenSSL + i) sync;
        }
        ::CRYPTO_set_locking_callback(locking_callback);

        //
        // Seed random number generator with screen scrape and other hardware sources
        //
        ::RAND_screen(); // OpenSSL
        ::RAND_poll();

        //
        // Seed random number generator with performance counter
        //
        this->RandAddSeed();
    }
    ~openssl_multithreading_support() {
        //
        // Shutdown OpenSSL library multithreading support
        //
        ::CRYPTO_set_locking_callback(nullptr);
        for (int i=0; i < ::CRYPTO_num_locks(); ++i)
        {
            (pmutexOpenSSL + i)->~sync();
        }
        ::OPENSSL_free(pmutexOpenSSL);
    }
};

template <typename C, typename T>
class randrangebuffer final
{
private:
    randrangebuffer(); // {}
    randrangebuffer(const randrangebuffer &); // {}
    randrangebuffer &operator=(const randrangebuffer &); //{}

    int (C::*__RAND_bytes)(unsigned char *&buf, int num);
    C *self;
    T buf;
public:
    T getrand(T nMax = std::numeric_limits<T>::max()) {
        // referrence: the bitcoin util.cpp ////////////////////////////////
        // The range of the random source must be a multiple of the modulus
        // to give every possible output value an equal possibility.
        ////////////////////////////////////////////////////////////////////
        const T nRange = (std::numeric_limits<T>::max() / nMax) * nMax;
        T nRand = 0;
        T *pnRand = &nRand;
        do
        {
            (void)((self->*(__RAND_bytes))((unsigned char *&)pnRand, sizeof(nRand)));
        } while (*pnRand >= nRange);

        buf = (*pnRand) % nMax;
        buf = (buf != 0) ? buf: nRange;
        return buf;
    }
    T getbuf() const {
        return buf;
    }

    explicit randrangebuffer(int (C::*__RAND_bytes_func)(unsigned char *&buf, int num), C *__self) {
        buf = std::numeric_limits<T>::max();
        __RAND_bytes = __RAND_bytes_func;
        self = __self;
    }
    ~randrangebuffer() {}
};

template <typename T>
class mcrypt final
{
private:
    mcrypt(const mcrypt &); // {}
    mcrypt &operator=(const mcrypt &); // {}
    randrangebuffer<mcrypt<T>, T> randrange;

    int __RAND_bytes(unsigned char *&buf, int num) {
        return ::RAND_bytes(buf, num);
    }
public:
    mcrypt() : randrange(&mcrypt<T>::__RAND_bytes, this) {}
    ~mcrypt() {}

    mcrypt &operator>>=(const mcrypt &obj) {
        randrange.getrand(obj.randrange.getbuf());
        return *this;
    }
    operator T() const {
        return randrange.getbuf();
    }
};

#endif SORACHANCOIN_CRYPT_H
