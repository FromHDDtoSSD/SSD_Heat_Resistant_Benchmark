// Copyright (c) 2019 The SorachanCoin Developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef FHS_SYNC_H
#define FHS_SYNC_H

#include <windows.h>
#include <stdexcept>
#include "common.h"

class sync
{
private:
    sync(const sync &); // {}
    sync &operator=(const sync &); // {}
    mutable CRITICAL_SECTION cs;
public:
    sync() {::InitializeCriticalSection(&cs);}
    ~sync() {::DeleteCriticalSection(&cs);}
    void enter() const {::EnterCriticalSection(&cs);}
    void leave() const {::LeaveCriticalSection(&cs);}
};

class cevent
{
private:
    cevent(const cevent &); // {}
    cevent &operator=(const cevent &); // {}
    HANDLE hEvent;
public:
    cevent() {
        hEvent = ::CreateEventW(nullptr, TRUE, FALSE, L"");
        if(! hEvent) {
            std::runtime_error("Event object create failure.");
        }
    }
    ~cevent() {
        if(hEvent) {
            ::CloseHandle(hEvent);
        }
    }

    void set() const {
        ::SetEvent(hEvent);
    }
    void reset() const {
        ::ResetEvent(hEvent);
    }
    void wait() const {
        ::WaitForSingleObject(hEvent, INFINITE);
    }
};

#endif
