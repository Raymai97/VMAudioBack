#undef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#include "VMAudioBack.h"
#include "util.h"

#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

BOOL VMAudioBack_Loop(BOOL fUI)
{
	DWORD mciErr = 0;
	WCHAR const *pszIng = NULL;
	int nTryOpen = 0;
	logd(L"VMAudioBack_Loop()...");
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
	for (;;)
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
	return 0;
}
