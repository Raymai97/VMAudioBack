#undef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#include "VMAudioBackSvc.h"
#include "VMAudioBack.h"
#include "util.h"

static SERVICE_STATUS_HANDLE s_SvcStatusHandle;
static SERVICE_STATUS s_SvcStatus = { SERVICE_WIN32_OWN_PROCESS };

static void ReportSvcStatus(DWORD dwCurrentState, DWORD dwWin32ExitCode, DWORD dwWaitHint)
{
    static DWORD dwCheckPoint;
    s_SvcStatus.dwCurrentState = dwCurrentState;
    s_SvcStatus.dwWin32ExitCode = dwWin32ExitCode;
    s_SvcStatus.dwWaitHint = dwWaitHint;

    if (dwCurrentState == SERVICE_START_PENDING)
    {
        s_SvcStatus.dwControlsAccepted = 0;
    }
    else
    {
        s_SvcStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;
    }

    if ((dwCurrentState == SERVICE_RUNNING) || (dwCurrentState == SERVICE_STOPPED))
    {
        s_SvcStatus.dwCheckPoint = 0;
    }
    else
    {
        s_SvcStatus.dwCheckPoint = dwCheckPoint++;
    }

    SetServiceStatus(s_SvcStatusHandle, &s_SvcStatus);
}

void WINAPI VMAudioBackSvcCtrl(DWORD dwCtrl)
{
    switch (dwCtrl)
    {
    case SERVICE_CONTROL_STOP:
        logd(L"Received SERVICE_CONTROL_STOP...");
        ReportSvcStatus(SERVICE_STOPPED, 0, 0);
        break;
    }
}

void WINAPI VMAudioBackSvcMain(DWORD nArg, WCHAR **pszArgs)
{
    (void)(nArg, pszArgs);
	logd(L"Running as Windows service");
	s_SvcStatusHandle = RegisterServiceCtrlHandlerW(VMAUDIOBACK_SVC_NAME, VMAudioBackSvcCtrl);
	if (!s_SvcStatusHandle)
	{
		logdf(L"RegisterServiceCtrlHandlerW() error %d", GetLastError());
		goto eof;
	}
    ReportSvcStatus(SERVICE_RUNNING, 0, 0);
    VMAudioBack_Loop(FALSE);
eof:
	/* s_hSvcCtrl no need to be closed */
    return;
}
