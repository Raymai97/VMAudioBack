#pragma once
#include <Windows.h>

#define VMAUDIOBACK_SVC_NAME  L"VMAudioBackSvc"

void WINAPI VMAudioBackSvcMain(DWORD nArg, WCHAR **pszArgs);
