#undef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "VMAudioBack.h"
#include "VMAudioBackSvc.h"
#include "util.h"

int Main_Install_VMAudioBackSvc(void)
{
	int ret = 1;
	WCHAR szExePath[MAX_PATH];
	WCHAR szExecute[MAX_PATH];
	HANDLE hScM = 0;
	HANDLE hSc = 0;
	logd(L"Installing service...");
	if (GetModuleFileNameW(NULL, szExePath, MAX_PATH) < 1)
	{
		logdf(L"GetModuleFileNameW() error %d", GetLastError());
		goto eof;
	}
	wsprintfW(szExecute, L"\"%s\" --run-as-service", szExePath);
	hScM = OpenSCManagerW(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (!hScM)
	{
		logdf(L"OpenSCManagerW() error %d", GetLastError());
		goto eof;
	}
	hSc = CreateServiceW(hScM, VMAUDIOBACK_SVC_NAME, VMAUDIOBACK_SVC_NAME,
		SERVICE_ALL_ACCESS, SERVICE_WIN32_OWN_PROCESS, SERVICE_AUTO_START,
		SERVICE_ERROR_NORMAL, szExecute, NULL, NULL, NULL, NULL, NULL);
	if (!hSc)
	{
		logdf(L"CreateServiceW() error %d", GetLastError());
		goto eof;
	}
	logd(L"Service install success");
	ret = 0;
eof:
	if (hSc) { CloseServiceHandle(hSc); }
	if (hScM) { CloseServiceHandle(hScM); }
	return ret;
}

int Main_Uninstall_VMAudioBackSvc(void)
{
	int ret = 1;
	HANDLE hScM = 0;
	HANDLE hSc = 0;
	logd(L"Uninstalling service...");
	hScM = OpenSCManagerW(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (!hScM)
	{
		logdf(L"OpenSCManagerW() error %d", GetLastError());
		goto eof;
	}
	hSc = OpenServiceW(hScM, VMAUDIOBACK_SVC_NAME, DELETE);
	if (!hSc)
	{
		logdf(L"OpenServiceW() error %d", GetLastError());
		goto eof;
	}
	if (!DeleteService(hSc))
	{
		logdf(L"DeleteService() error %d", GetLastError());
		goto eof;
	}
	logd(L"Service uninstall success");
	ret = 0;
eof:
	if (hSc) { CloseServiceHandle(hSc); }
	if (hScM) { CloseServiceHandle(hScM); }
	return ret;
}

int Main_RunAs_VMAudioBackSvc(void)
{
	SERVICE_TABLE_ENTRYW arr[2];
	memset(&arr, 0, sizeof(arr));
	arr[0].lpServiceName = VMAUDIOBACK_SVC_NAME;
	arr[0].lpServiceProc = VMAudioBackSvcMain;
	if (!StartServiceCtrlDispatcherW(arr))
	{
		logdf(L"StartServiceCtrlDispatcherW() error %d", GetLastError());
		return 1;
	}
	return 0;
}

int Main_VMAudioBack(void)
{
	logd(L"Running as Windows application");
	return VMAudioBack_Loop(TRUE) ? 0 : 1;
}

static void MakeSpaceNull(WCHAR *p)
{
	for (; *p; ++p)
	{
		if (*p == ' ')
		{
			*p = 0; break;
		}
	}
}

int WINAPI wWinMain(HINSTANCE z_, HINSTANCE z__, WCHAR *pszCmdl, int z___)
{
	(void)(z_, z__, z___);
	MakeSpaceNull(pszCmdl);
	logging_init();
	logd(L"-------------------- WinMain --------------------");
	logdf(L"Command line: %s", pszCmdl);
	if (lstrcmpW(pszCmdl, L"--install-service") == 0)
	{
		return Main_Install_VMAudioBackSvc();
	}
	if (lstrcmpW(pszCmdl, L"--uninstall-service") == 0)
	{
		return Main_Uninstall_VMAudioBackSvc();
	}
	if (lstrcmpW(pszCmdl, L"--run-as-service") == 0)
	{
		return Main_RunAs_VMAudioBackSvc();
	}
	if (pszCmdl[0] == 0)
	{
		return Main_VMAudioBack();
	}
	MessageBoxError(L"Invalid parameter.");
	return 1;
}
