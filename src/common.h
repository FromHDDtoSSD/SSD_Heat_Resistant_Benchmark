// Copyright (c) 2019 The SorachanCoin Developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef SORACHANCOIN_COMMON_H
#define SORACHANCOIN_COMMON_H

#define ARRAY_SIZE(X) (sizeof(X)/(sizeof(X[0])))

//
// When after C++11 can not be used ...
//
#define final
#define override
const class nullptr_t
{
public:
    template<typename T> operator T*() const { return 0; }
    template<typename C, typename T> operator T C::*() const { return 0; }
private:
    void operator&() const;
} nullptr = {};

#endif SORACHANCOIN_COMMON_H
