// Copyright (c) 2019 The SorachanCoin Developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef FHS_CPUTIME_H
#define FHS_CPUTIME_H

#include <windows.h>

class cputime
{
private:
    cputime(const cputime &); // {}
    cputime &operator=(const cputime &); // {}
    LARGE_INTEGER qf, qc;
public:
    cputime() {
        (void)::QueryPerformanceFrequency(&qf);
    }
    ~cputime() {}

    double operator()() {
        (void)::QueryPerformanceCounter(&qc);
        return (double)qc.QuadPart / (double)qf.QuadPart;
    }
};

#endif FHS_CPUTIME_H
