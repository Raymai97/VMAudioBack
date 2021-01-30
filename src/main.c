#include <Windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

static void set_cwd_to_exe_dir(void)
{
	WCHAR szPath[MAX_PATH], *p = szPath, *pSlash = p;
	GetModuleFileNameW(NULL, szPath, MAX_PATH);
	for (; *p; ++p)
	{
		if (*p == '\\') pSlash = p;
	}
	*pSlash = 0;
	SetCurrentDirectory(szPath);
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE a_, LPSTR a__, int nCmdShow)
{
	DWORD mciErr = 0;
	PCSTR pszIng = NULL;
	set_cwd_to_exe_dir();
	pszIng = "opening";
	mciErr = mciSendStringA("open a.mp3", NULL, 0, NULL);
	if (mciErr) goto eof;
	pszIng = "playing";
	mciErr = mciSendStringA("play a.mp3 repeat", NULL, 0, NULL);
	if (mciErr) goto eof;
	/* Keep process alive to make playback loop persists */
	/* To stop, just terminate this process. */
	for (;;)
	{
		Sleep(1000);
	}
eof:
	if (mciErr)
	{
		char szErr[99];
		wsprintfA(szErr, "MCI error %lu when %s a.mp3", mciErr, pszIng);
		MessageBoxA(NULL, szErr, NULL, MB_ICONHAND);
	}
	(void)a_;(void)a__;
	return 0;
}
