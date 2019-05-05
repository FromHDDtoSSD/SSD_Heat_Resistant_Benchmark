// Copyright (c) 2019 The SorachanCoin Developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef SORACHANCOIN_SECTOR_H
#define SORACHANCOIN_SECTOR_H

#include <windows.h>
#include <new>
#include "common.h"
#include "rand.h"
#include "drive.h"

class sector_base
{
private:
    sector_base(const sector_base &); // {}
    sector_base &operator=(const sector_base &); // {}
public:
    sector_base() {}
    virtual ~sector_base() {}

    //
    // Method
    //
    virtual bool create() = 0;
    virtual void stop() = 0;
    virtual void waitclose() = 0;
    virtual bool signal() const = 0;
    virtual void destroy() = 0;
    virtual double getprog() const = 0;
};

class sector_randbuffer final : public sector_base
{
private:
    sector_randbuffer(const sector_randbuffer &); // {}
    sector_randbuffer &operator=(const sector_randbuffer &); // {}

    rand_base *target;
public:
    enum RAND_TYPE {
        RAND_GENE_STANDARD = 0,
        RAND_GENE_XORSHIFT,
        RAND_GENE_OPENSSL,
        RAND_GENE_MAX,
    };

    sector_randbuffer() {
        target = nullptr;
    }
    ~sector_randbuffer() {
        destroy();
    }

    bool settype(RAND_TYPE type) {
        destroy();
        switch (type)
        {
        case RAND_GENE_STANDARD:
            target = new(std::nothrow) rand_standard;
            break;
        case RAND_GENE_XORSHIFT:
            target = new(std::nothrow) rand_xorshift;
            break;
        case RAND_GENE_OPENSSL:
            target = new(std::nothrow) rand_openssl;
            break;
        default:
            return false;
        }
        return target != nullptr;
    }
    void destroy() final override {
        if(target) {
            delete target;
            target = nullptr;
        }
    }
    bool create() final override {
        if(target) {
            target->srand();
            return target->create();
        } else {
            return false;
        }
    }
    void stop() final override {
        if(target) {
            target->stop();
        }
    }
    void waitclose() final override {
        if(target) {
            target->waitclose();
        }
    }
    bool signal() const final override {
        if(target) {
            return target->signal();
        } else {
            return true;
        }
    }
    double getprog() const final override {
        if(target) {
            return target->getprog();
        } else {
            return 0.0;
        }
    }
    const std::vector<unsigned __int64> *getbuf() const {
        if(target) {
            return &target->getbuf();
        } else {
            return nullptr;
        }
    }
    void setgenecount(int count) {
        if(target) {
            target->setgenecount(count);
        }
    }
};

class sector_io final : public sector_base
{
private:
    sector_io(const sector_io &); // {}
    sector_io &operator=(const sector_io &); // {}

    static const DWORD SECTORS_SIZE_SEQ = 100 * 1024 * 1024;
    static const DWORD SECTORS_SIZE_8192KB = 8192 * 1024;
    static const DWORD SECTORS_SIZE_512KB = 512 * 1024;
    static const DWORD SECTORS_SIZE_4KB = 4 * 1024;

    drive_base *drive;
public:
    sector_io() {
        drive = nullptr;
    }
    ~sector_io() {
        destroy();
    }

    enum ACC_TYPE {
        ACC_TYPE_SEQ_READ = 0,
        ACC_TYPE_SEQ_WRITE,
        ACC_TYPE_RANDOM_READ_8192KB,
        ACC_TYPE_RANDOM_WRITE_8192KB,
        ACC_TYPE_RANDOM_READ_512KB,
        ACC_TYPE_RANDOM_WRITE_512KB,
        ACC_TYPE_RANDOM_READ_4KB,
        ACC_TYPE_RANDOM_WRITE_4KB,
        ACC_TYPE_RANDOM_MAX,
    };

    bool settype_io(ACC_TYPE type, int drive_target, const sector_io *instanced = nullptr) {
        if(instanced) {
            return settype(type, drive_target, instanced->drive);
        } else {
            return settype(type, drive_target);
        }
    }

    bool settype(ACC_TYPE type, int drive_target, const drive_base *instanced = nullptr) {
        destroy();
        DWORD sectors_size = 0;
        switch (type)
        {
        case ACC_TYPE_SEQ_READ:
            drive = new(std::nothrow) drive_seqread(drive_target);
            sectors_size = SECTORS_SIZE_SEQ;
            break;
        case ACC_TYPE_SEQ_WRITE:
            drive = new(std::nothrow) drive_seqwrite(drive_target);
            sectors_size = SECTORS_SIZE_SEQ;
            break;
        case ACC_TYPE_RANDOM_READ_8192KB:
            drive = new(std::nothrow) drive_randomread(drive_target);
            sectors_size = SECTORS_SIZE_8192KB;
            break;
        case ACC_TYPE_RANDOM_READ_512KB:
            drive = new(std::nothrow) drive_randomread(drive_target);
            sectors_size = SECTORS_SIZE_512KB;
            break;
        case ACC_TYPE_RANDOM_READ_4KB:
            drive = new(std::nothrow) drive_randomread(drive_target);
            sectors_size = SECTORS_SIZE_4KB;
            break;
        case ACC_TYPE_RANDOM_WRITE_8192KB:
            drive = new(std::nothrow) drive_randomwrite(drive_target);
            sectors_size = SECTORS_SIZE_8192KB;
            break;
        case ACC_TYPE_RANDOM_WRITE_512KB:
            drive = new(std::nothrow) drive_randomwrite(drive_target);
            sectors_size = SECTORS_SIZE_512KB;
            break;
        case ACC_TYPE_RANDOM_WRITE_4KB:
            drive = new(std::nothrow) drive_randomwrite(drive_target);
            sectors_size = SECTORS_SIZE_4KB;
            break;
        default:
            return false;
        }

        if(drive) {
            bool ret = (instanced ? drive->openhandle(instanced): drive->openhandle());
            if(ret) {
                drive->setstep(sectors_size);
            }
            return ret;
        } else {
            return false;
        }
    }
    bool settype_file(ACC_TYPE type, int drive_target, const drive_base *instanced = nullptr) {
        destroy();
        DWORD sectors_size = 0;
        switch (type)
        {
        case ACC_TYPE_SEQ_WRITE:
            drive = new(std::nothrow) drive_datawritelimit(drive_target);
            sectors_size = SECTORS_SIZE_SEQ;
            break;
        case ACC_TYPE_RANDOM_WRITE_8192KB:
            drive = new(std::nothrow) drive_datawritelimit(drive_target);
            sectors_size = SECTORS_SIZE_8192KB;
            break;
        case ACC_TYPE_RANDOM_WRITE_512KB:
            drive = new(std::nothrow) drive_datawritelimit(drive_target);
            sectors_size = SECTORS_SIZE_512KB;
            break;
        case ACC_TYPE_RANDOM_WRITE_4KB:
            drive = new(std::nothrow) drive_datawritelimit(drive_target);
            sectors_size = SECTORS_SIZE_4KB;
            break;
        default:
            return false;
        }

        if(drive) {
            bool ret = (instanced ? drive->openhandle(instanced): drive->openhandle());
            if(ret) {
                drive->setstep(sectors_size);
            }
            return ret;
        } else {
            return false;
        }
    }
    std::wstring getdriveinfo() const {
        if(drive) {
            return drive->getdriveinfo();
        } else {
            return L"";
        }
    }
    void destroy() final override {
        if(drive) {
            delete drive;
            drive = nullptr;
        }
    }
    void setparam(const sector_randbuffer &randbuf) {
        const std::vector<unsigned __int64> *buf = randbuf.getbuf();
        if(buf && drive) {
            drive->setrand(*buf);
            drive->clearaccpoint();
        }
    }
    // void checkpartition() const {}

    bool create() final override {
        if(drive) {
            drive->clearfailure();
            return drive->scan();
        } else {
            return false;
        }
    }
    void stop() final override {
        if(drive) {
            drive->stop();
        }
    }
    void waitclose() final override {
        if(drive) {
            drive->waitclose();
        }
    }
    bool signal() const final override {
        if(drive) {
            return drive->signal();
        } else {
            return true;
        }
    }
    bool getfailure() const {
        if(drive) {
            return drive->getfailure();
        } else {
            return false;
        }
    }
    double getprog() const final override {
        if(drive) {
            return drive->getprog();
        } else {
            return 0.0;
        }
    }
    double getspeed(double ti) const {
        if(drive) {
            return drive->getspeed(ti);
        } else {
            return 0.0;
        }
    }
};

#endif SORACHANCOIN_SECTOR_H
