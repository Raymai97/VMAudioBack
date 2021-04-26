#undef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#include "util.h"

WCHAR x_szLogPath[MAX_PATH];
WCHAR x_szLogTag[100] = L"VMAudioBack";

static BOOL try_open_this_exe_dir_file(WCHAR const *pszName)
{
	WCHAR szPath[MAX_PATH], *p = szPath, *pSlash = NULL;
	GetModuleFileNameW(NULL, szPath, MAX_PATH);
	for (; *p; ++p)
	{
		if (*p == '\\') pSlash = p;
	}
	if (pSlash)
	{
		HANDLE hFile = 0;
		{
			WCHAR const *q = pszName;
			for (p = &pSlash[1]; *p = *q, *q; ++p, ++q);
		}
		hFile = CreateFileW(szPath, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
		if (hFile != INVALID_HANDLE_VALUE)
		{
			CloseHandle(hFile);
			return TRUE;
		}
	}
	return FALSE;
}

static BOOL try_open_DISABLE_LOGGING(void)
{
	return try_open_this_exe_dir_file(L"DISABLE_LOGGING");
}

BOOL try_open_SET_TIMER_RES_INSTEAD(void)
{
	return try_open_this_exe_dir_file(L"SET_TIMER_RES_INSTEAD");
}

void logging_init(void)
{
	if (!try_open_DISABLE_LOGGING())
	{
		GetModuleFileNameW(NULL, x_szLogPath, MAX_PATH);
		lstrcatW(x_szLogPath, L".log");
	}
}

void logd(WCHAR const *pszText)
{
	HANDLE hFile = 0;
	DWORD cbWri = 0;
	hFile = CreateFileW(x_szLogPath, FILE_APPEND_DATA, FILE_SHARE_READ, NULL, OPEN_ALWAYS, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		hFile = 0;
		goto eof;
	}
	if (GetFileSize(hFile, NULL) > 0)
	{
		SetFilePointer(hFile, 0, NULL, FILE_END);
	}
	else
	{
		WriteFile(hFile, "\xFF\xFE", 2, &cbWri, NULL);
	}
	{
		WCHAR szHead[99];
		SYSTEMTIME t;
		GetLocalTime(&t);
		wsprintfW(szHead, L"[%04u-%02u-%02u %02u:%02u:%02u.%03u][%s] ",
			t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond, t.wMilliseconds, x_szLogTag);
		WriteFile(hFile, szHead, lstrlenW(szHead) * sizeof(WCHAR), &cbWri, NULL);
	}
	WriteFile(hFile, pszText, lstrlenW(pszText) * sizeof(WCHAR), &cbWri, NULL);
	WriteFile(hFile, L"\r\n", 2 * sizeof(WCHAR), &cbWri, NULL);
eof:
	if (hFile) { CloseHandle(hFile); }
}

void logdf(WCHAR const *pszFmt, ...)
{
	WCHAR szBuf[1024];
	va_list ap;
	va_start(ap, pszFmt);
	if (wvsprintfW(szBuf, pszFmt, ap) >= 0)
	{
		logd(szBuf);
	}
	va_end(ap);
}

void set_cwd_to_exe_dir(void)
{
	WCHAR szPath[MAX_PATH], *p = szPath, *pSlash = NULL;
	GetModuleFileNameW(NULL, szPath, MAX_PATH);
	for (; *p; ++p)
	{
		if (*p == '\\') pSlash = p;
	}
	if (pSlash)
	{
		*pSlash = 0;
		SetCurrentDirectoryW(szPath);
	}
}

void MessageBoxError(WCHAR const *pszErr)
{
	MessageBoxW(NULL, pszErr, NULL, MB_ICONHAND | MB_SYSTEMMODAL);
}
