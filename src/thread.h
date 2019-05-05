// Copyright (c) 2019 The SorachanCoin Developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef SORACHANCOIN_THREAD_H
#define SORACHANCOIN_THREAD_H

#include <process.h>
#include "common.h"
#include "debug.h"

template <typename T>
class cla_thread
{
public:
    typedef struct _thread_data
    {
        void *p;
        bool exit_flag;
    } thread_data;
private:
    struct thread_param : public thread_data
    {
        T *self;
        unsigned int (T::*func)(thread_data *pdata);
    } param;

    HANDLE hHandle;
    cla_thread(); // {}
    cla_thread(const cla_thread &); // {}
    cla_thread &operator=(const cla_thread &); // {}

    static unsigned int _stdcall _thread(void *p) {
        struct thread_param *tp = reinterpret_cast<struct thread_param *>(p);
        unsigned int ret = (tp->self->*(tp->func))(static_cast<thread_data *>(tp));
        ::_endthreadex(0);
        return ret;
    }
public:
    explicit cla_thread(unsigned int (T::*_func)(thread_data *pdata)) {
        param.p = nullptr;
        param.exit_flag = false;
        param.self = nullptr;
        param.func = _func;
        hHandle = nullptr;
    }
    ~cla_thread() {
        stop();
        waitclose();
    }

    bool start(void *_p, T *_self) {
        param.p = _p;
        param.exit_flag = false;
        param.self = _self;
        hHandle = (HANDLE)::_beginthreadex(nullptr, 0, _thread, &param, 0, nullptr);
        return hHandle != nullptr;
    }
    void stop() {
        param.exit_flag = true;
    }
    bool signal() const {
        if(hHandle) {
            return (::WaitForSingleObject(hHandle, 0) == WAIT_OBJECT_0) ? true: false;
        } else {
            return true;
        }
    }
    void waitclose() {
        if(hHandle) {
            ::WaitForSingleObject(hHandle, INFINITE);
            ::CloseHandle(hHandle);
            hHandle = nullptr;
        }
    }
};

#endif SORACHANCOIN_THREAD_H
