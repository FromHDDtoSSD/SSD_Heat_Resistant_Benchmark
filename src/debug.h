// Copyright (c) 2019 The SorachanCoin Developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef SORACHANCOIN_DEBUG_H
#define SORACHANCOIN_DEBUG_H

#include <windows.h>
#include <string>
#include <sstream>
#include "sync.h"

class debugcs
{
private:
    debugcs(const debugcs &); // {}
    debugcs &operator=(const debugcs &); // {}

    HANDLE hStdout;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    sync cs;

    debugcs() {
# ifdef _DEBUG
        ::AllocConsole();
        hStdout = ::GetStdHandle(STD_OUTPUT_HANDLE);
        ::GetConsoleScreenBufferInfo(hStdout, &csbi);
# else
        hStdout = nullptr;
# endif
    }
    ~debugcs() {
        if(hStdout) {
            ::FreeConsole();
            ::CloseHandle(hStdout);
        }
    }

public:
    static const debugcs &instance() {
        static debugcs dcs;
        return dcs;
    }
    template <typename T> const debugcs &operator<<(const T &obj) const {
# ifdef _DEBUG
        cs.enter();
        std::wostringstream stream;
        stream << obj << L"\n";
        DWORD dwRet;
        ::WriteConsoleW(hStdout, stream.str().c_str(), (DWORD)stream.str().length(), &dwRet, nullptr);
        cs.leave();
# else
        static_cast<const T &>(obj);
# endif
        return *this;
    }
};

#endif
