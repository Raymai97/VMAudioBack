#pragma once
#include <Windows.h>

extern WCHAR x_szLogPath[MAX_PATH];
extern WCHAR x_szLogTag[100];

void logging_init(void);
void logd(WCHAR const *pszText);
void logdf(WCHAR const *pszFmt, ...);

void set_cwd_to_exe_dir(void);

void MessageBoxError(WCHAR const *pszErr);

BOOL try_open_SET_TIMER_RES_INSTEAD(void);
