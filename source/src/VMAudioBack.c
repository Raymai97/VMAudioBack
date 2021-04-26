#undef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#include "VMAudioBack.h"
#include "util.h"

#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

static BOOL VMAudioBack_PlayMP3Loop(BOOL fUI)
{
	BOOL ok = FALSE;
	DWORD mciErr = 0;
	WCHAR const *pszIng = NULL;
	int nTryOpen = 0;
	set_cwd_to_exe_dir();
	pszIng = L"opening";
	for (nTryOpen = 1;; ++nTryOpen) {
		mciErr = mciSendStringW(L"open a.mp3", NULL, 0, NULL);
		if (mciErr != MCIERR_INTERNAL) break;
		logdf(L"MCI internal error on attempt %d", nTryOpen);
		Sleep(100);
	}
	if (mciErr) goto eof;
	pszIng = L"playing";
	mciErr = mciSendStringW(L"play a.mp3 repeat", NULL, 0, NULL);
	if (mciErr) goto eof;
	logd(L"MCI play OK, entering loop");
	/* Keep process alive to make playback loop persists */
	/* To stop, just terminate this process. */
	for (ok = TRUE;;)
	{
		Sleep(1000);
	}
eof:
	if (mciErr)
	{
		WCHAR szErr[99];
		wsprintfW(szErr, L"MCI error %lu when %s a.mp3", mciErr, pszIng);
		logd(szErr);
		if (fUI)
		{
			MessageBoxError(szErr);
		}
	}
	return ok;
}

static BOOL VMAudioBack_SetTimerResLoop(BOOL fUI)
{
	typedef LONG(NTAPI *NtQueryTimerResolution_t)(ULONG *minRes, ULONG *maxRes, ULONG *currRes);
	typedef LONG(NTAPI *NtSetTimerResolution_t)(ULONG desiredRes, BOOL wantSet, ULONG *currRes);
	BOOL ok = FALSE;
	HANDLE hNtdll = GetModuleHandleW(L"ntdll");
	FARPROC pfnQueryTimerRes = !hNtdll ? 0 : GetProcAddress(hNtdll, "NtQueryTimerResolution");
	FARPROC pfnSetTimerRes = !hNtdll ? 0 : GetProcAddress(hNtdll, "NtSetTimerResolution");
	LONG ntErr = 0;
	ULONG minRes = 0, maxRes = 0, currRes = 0, newRes = 0;
	if (!pfnQueryTimerRes || !pfnSetTimerRes)
	{
		logd(L"GetProcAddress(TimerRes) error");
		goto eof;
	}
	ntErr = ((NtQueryTimerResolution_t)pfnQueryTimerRes)(&minRes, &maxRes, &currRes);
	logdf(L"NtQueryTimerRes return %ld, minRes[%lu], maxRes[%lu], currRes[%lu]"
		, ntErr, minRes, maxRes, currRes);
	if (ntErr < 0)
	{
		goto eof;
	}
	ntErr = ((NtSetTimerResolution_t)pfnSetTimerRes)(5000, TRUE, &newRes);
	logdf(L"NtSetTimerRes(0.5ms) return %ld, newRes[%lu]", ntErr, newRes);
	if (ntErr < 0)
	{
		goto eof;
	}
	for (ok = TRUE;;)
	{
		Sleep(1000);
	}
eof:
	if (!ok && fUI)
	{
		MessageBoxError(L"SetTimerRes hit unexpected error. Enable logging for detail.");
	}
	return ok;
}

BOOL VMAudioBack_Loop(BOOL fUI)
{
	logd(L"VMAudioBack_Loop()...");
	if (try_open_SET_TIMER_RES_INSTEAD())
	{
		return VMAudioBack_SetTimerResLoop(fUI);
	}
	return VMAudioBack_PlayMP3Loop(fUI);
}
