// Copyright (c) 2019 The SorachanCoin Developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef SORACHANCOIN_RESOURCE_H
#define SORACHANCOIN_RESOURCE_H

#include <windows.h>

const INT_PTR IDC_BUTTON_START        = 1000;
const INT_PTR IDC_BUTTON_STOP         = 1001;
const INT_PTR IDC_COMBO_DRIVE         = 1100;
const INT_PTR IDC_COMBO_THREAD        = 1101;
const INT_PTR IDC_COMBO_LOOP          = 1200;
const INT_PTR IDC_COMBO_RAND          = 1300;
const INT_PTR IDC_THREAD_TIMER        = 1400;
const int LOOP_BENCHMARK_ON           = 1;
const int RAND_SELECT_MIX             = 0;
const int RAND_SELECT_MT19937         = 1;
const int RAND_SELECT_XORSHIFT        = 2;
const int RAND_SELECT_OPENSSL         = 3;
const int BENCH_SELECT_ON             = 0;
const int BENCH_SELECT_OFF            = 1;
const int RAND_STRENGTH_LOW           = 0;
const int RAND_STRENGTH_MID           = 1;
const int RAND_STRENGTH_HIGH          = 2;

#define ICO_MIMI                      100
#define PROGRESS_ID(X)                ((INT_PTR)X)
#define BENCH_ONOFF_ID(X)             ((INT_PTR)X + 1500)
#define WM_SET_PROGRESS               (WM_APP + 0)
#define IDM_BENCH_LOG                 1600
#define IDM_FILE_EXIT                 1601
#define IDM_INFO_VERSION              1602
#define IDM_BENCH_START               1700
#define IDM_BENCH_STOP                1701
#define IDM_LANG_ENGLISH              1800
#define IDM_LANG_JAPANESE             1801

#define IDS_APP_TITLE                 2000
#define IDS_APP_WINDOWCLASSNAME       2016
#define IDS_APP_COPYRIGHT             2032
#define IDS_APP_OPENSSLRIGHT          2048

#define IDS_ERROR_CREATEWINDOW        4000
#define IDS_ERROR_CLASSREGISTER       4016
#define IDS_ERROR_BENCHMARK_START     4032
#define IDS_ERROR_BENCHMARK_FAILURE   4048
#define IDS_ERROR_FONT                4064

#define IDS_MESSAGEBOX_OK             6000
#define IDS_MESSAGEBOX_ERROR          6016
#define IDS_MESSAGEBOX_QUESTION       6032
#define IDS_MESSAGEBOX_INFO           6048

#define IDS_START                     8000
#define IDS_STOP                      8016
#define IDS_DISK                      8032
#define IDS_THREADS                   8048
#define IDS_ONCE_BENCHMARK            8064
#define IDS_LOOP_BENCHMARK            8080
#define IDS_BENCHMARK_RAND_MIX        8096
#define IDS_BENCHMARK_RAND_MT19937    8112
#define IDS_BENCHMARK_RAND_XORSHIFT   8128
#define IDS_BENCHMARK_RAND_OPENSSL    8144
#define IDS_BENCHMARK_ON              8160
#define IDS_BENCHMARK_OFF             8176
#define IDS_RAND_LOW                  8192
#define IDS_RAND_MID                  8208
#define IDS_RAND_HIGH                 8224
#define IDS_DISK_NONE                 8240

#define IDS_PROGRESSBAR_0             10016
#define IDS_PROGRESSBAR_1             10032
#define IDS_PROGRESSBAR_2             10048
#define IDS_PROGRESSBAR_3             10064
#define IDS_PROGRESSBAR_4             10080
#define IDS_PROGRESSBAR_5             10096
#define IDS_PROGRESSBAR_6             10112
#define IDS_PROGRESSBAR_7             10128
#define IDS_PROGRESSBAR_8             10144

#define IDR_MENU_ENGLISH              12000
#define IDR_MENU_JAPANESE             12016

#define IDS_BENCHMARK_START           14000
#define IDS_BENCHMARK_STOP            14016
#define IDS_BENCHMARK_INFO            14032
#define IDS_BENCHMARK_OK              14048

#define IDS_BENCHMARK_RESULT_ERROR    16000
#define IDS_BENCHMARK_NO_CLOSE        16016
#define IDS_LANG_NO_CLOSE             16032
#define IDS_BENCH_LOGSET_NO_CLOSE     16048
#define IDS_BENCH_START_NO_CLOSE      16064

#define IDS_BENCHMARK_COMPLETED       18016
#define IDS_BENCHMARK_DOING           18032
#define IDS_BENCHMARK_RESULT          18048
#define IDS_BENCHMARK_WAITING         18064
#define IDS_BENCHMARK_GENERATING      18080

#define IDS_MESSAGEBOX_COPYRIGHT      20000

#define IDS_LOG_YEAR                  22000
#define IDS_LOG_MON                   22016
#define IDS_LOG_DAY                   22032
#define IDS_LOG_HOUR                  22048
#define IDS_LOG_MIN                   22064
#define IDS_LOG_SEC                   22080
#define IDS_LOG_DIR_DIALOG            22096
#define IDS_LOG_DRIVEINFO             22112
#define IDS_LOG_PARAM                 22128
#define IDS_LOG_THREAD                22144
#define IDS_LOG_RAND                  22160

#endif
