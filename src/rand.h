// Copyright (c) 2019 The SorachanCoin Developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
//
// mt19937
// BOOST Library
//
// xorshift
// https://www.jstatsoft.org/article/view/v008i14
//
// OpenSSL
// This product includes software developed by the OpenSSL Project for use in the OpenSSL Toolkit (http://www.openssl.org/). 

#ifndef SORACHANCOIN_RAND_H
#define SORACHANCOIN_RAND_H

#include <vector>
#include <stdlib.h>
#include <assert.h>
#pragma warning(disable: 4244)
#include <boost/random.hpp>
#include "common.h"
#include "thread.h"
#include "crypt.h" // #undef max

const int RAND_GENE_MAX_DEFAULT = 30;

class rand_base
{
private:
    rand_base(const rand_base &); // {}
    rand_base &operator=(const rand_base &); // {}

    std::vector<unsigned __int64> buf;
    cla_thread<rand_base> thread;
    rand_base *obj;
    int gene_count;

    virtual unsigned __int64 r_func(int i) = 0;
    unsigned int r_thread(cla_thread<rand_base>::thread_data *pdata) {
        if(! obj) { return 1; }
        for(int i=0; i < gene_count; ++i)
        {
            buf.push_back(obj->r_func(i));
            if(pdata->exit_flag) { break; }
        }
        return 1;
    }
protected:
    typedef union _RAND_INTEGER
    {
        struct {
            unsigned long LowPart;
            unsigned long HighPart;
        } u;
        unsigned __int64 QuadPart; 
    } RAND_INTEGER;

    bool start(rand_base *_obj) {
        clear();
        obj = _obj;
        return thread.start(nullptr, this);
    }
public:
    rand_base() : thread(&rand_base::r_thread) {
        obj = nullptr;
        gene_count = RAND_GENE_MAX_DEFAULT;
    }
    virtual ~rand_base() {}

    //
    // Method
    //
    void init() {buf.clear();}
    void waitclose() {thread.waitclose();}
    void stop() {thread.stop();}
    bool signal() const {return thread.signal();}
    double getprog() const {return (double)size() / gene_count;}
    size_t size() const {return buf.size();}
    void clear() { buf.clear(); obj = nullptr; }
    void setgenecount(int count) {if(0 < count) {gene_count = count;}}
    unsigned __int64 get(int i) const {return buf[i];}
    const std::vector<unsigned __int64> &getbuf() const {return buf;}
    virtual void srand() = 0;
    virtual bool create() = 0;
};

class rand_standard final : public rand_base
{
private:
    rand_standard(const rand_standard &); // {}
    rand_standard &operator=(const rand_standard &); // {}
    randrangebuffer<rand_standard, unsigned __int64> randrange;

    boost::random::mt19937 gen;
    boost::random::uniform_real_distribution<> urd;

    RAND_INTEGER ri;
    int __RAND_bytes(unsigned char *&buf, int num_unused) { // num is 8.(sizeof(unsigned __int64))
        assert(num_unused == 8);
        boost::random::variate_generator<boost::random::mt19937 &, boost::random::uniform_real_distribution<> > mtrand(gen, urd);
        this->ri.u.LowPart = (unsigned long)mtrand();
        this->ri.u.HighPart = (unsigned long)mtrand();
        buf = (unsigned char *)&this->ri.QuadPart;
        return num_unused;
    }
    unsigned __int64 r_func(int) final override {
        unsigned __int64 rr = randrange.getrand();
        // OK debugcs::instance() << rr;
        return rr;
    }
public:
    rand_standard() : randrange(&rand_standard::__RAND_bytes, this), urd(1, (boost::uint32_t)std::numeric_limits<boost::uint32_t>::max()) {
        ::memset(&ri, 0x00, sizeof(RAND_INTEGER));
    }
    ~rand_standard() {}

    //
    // Method
    //
    void srand() final override {
        mcrypt<boost::uint32_t> crypt;
        gen.seed((boost::uint32_t)(crypt >>= crypt));
        ::memset(&ri, 0x00, sizeof(RAND_INTEGER));
        debugcs::instance() << L"[standard Seed Renge]" << (boost::uint32_t)crypt;
    }
    bool create() final override {
        return this->start(this);
    }
};

class rand_xorshift final : public rand_base
{
private:
    rand_xorshift(const rand_xorshift &); // {}
    rand_xorshift &operator=(const rand_xorshift &); // {}
    randrangebuffer<rand_xorshift, unsigned __int64> randrange;

    unsigned long x, y, z, w;
    RAND_INTEGER ri;
    unsigned long xor128() {
        unsigned long t;
        t=(x^(x<<11));x=y;y=z;z=w;
        return( w=(w^(w>>19))^(t^(t>>8)) );
    }

    int __RAND_bytes(unsigned char *&buf, int num_unused) { // num is 8.(sizeof(unsigned __int64))
        assert(num_unused == 8);
        this->ri.u.LowPart = xor128();
        this->ri.u.HighPart = xor128();
        buf = (unsigned char *)&this->ri.QuadPart;
        return num_unused;
    }
    unsigned __int64 r_func(int) final override {
        unsigned __int64 rr = randrange.getrand();
        // OK debugcs::instance() << rr;
        return rr;
    }
public:
    rand_xorshift() : randrange(&rand_xorshift::__RAND_bytes, this) {
        x = 123456789;
        y = 362436069;
        z = 521288629;
        w = 88675123;
        ::memset(&ri, 0x00, sizeof(RAND_INTEGER));
    }
    explicit rand_xorshift(unsigned long _x, unsigned long _y, unsigned long _z, unsigned long _w) : randrange(&rand_xorshift::__RAND_bytes, this) {
        x = _x;
        y = _y;
        z = _z;
        w = _w;
        ::memset(&ri, 0x00, sizeof(RAND_INTEGER));
    }
    ~rand_xorshift() {}

    //
    // Method
    //
    void srand() final override {
        mcrypt<unsigned long> crypt;
        x = crypt >>= crypt;
        y = crypt >>= crypt;
        z = crypt >>= crypt;
        w = crypt >>= crypt;
        ::memset(&ri, 0x00, sizeof(RAND_INTEGER));
        debugcs::instance() << L"[xorshift Seed]" << x << y << z << w;
    }
    bool create() final override {
        return this->start(this);
    }
};

class rand_openssl final : public rand_base
{
private:
    rand_openssl(const rand_openssl &); // {}
    rand_openssl &operator=(const rand_openssl &); // {}

    unsigned __int64 r_func(int) final override {
        mcrypt<unsigned __int64> crypt;
        unsigned __int64 rr = crypt >>= crypt;
        // OK debugcs::instance() << rr;
        return rr;
    }
public:
    rand_openssl() {}
    ~rand_openssl() {}

    //
    // Method
    //
    void srand() final override {}
    bool create() final override {
        return this->start(this);
    }
};

#endif SORACHANCOIN_RAND_H
