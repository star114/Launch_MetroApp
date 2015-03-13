// RunMetroAppWithUserAppId.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <Windows.h>
#include <ShObjIdl.h>

HWND g_SelectedProcessWindowHandle = NULL;

VOID PhEnumChildWindows(
	_In_opt_ HWND hWnd,
	_In_ ULONG Limit,
	_In_ WNDENUMPROC Callback,
	_In_ LPARAM lParam
	)
{
	HWND childWindow = NULL;
	ULONG i = 0;

	while (i < Limit && (childWindow = FindWindowEx(hWnd, childWindow, NULL, NULL)))
	{
		if (!Callback(childWindow, lParam))
			return;

		i++;
	}
}

static BOOL CALLBACK EnumProcessWindowsProc(
	_In_ HWND hwnd,
	_In_ LPARAM lParam
	)
{
	ULONG processId;
	HWND parentWindow;

	if (!IsWindowVisible(hwnd))
		return TRUE;

	GetWindowThreadProcessId(hwnd, &processId);

	if (
		processId == (ULONG)lParam &&
		!((parentWindow = GetParent(hwnd)) && IsWindowVisible(parentWindow)) && // skip windows with a visible parent
		GetWindowTextLength(hwnd) != 0
		)
	{
		g_SelectedProcessWindowHandle = hwnd;
		return FALSE;
	}

	return TRUE;
}

int _tmain(int argc, _TCHAR* argv[])
{
	if (argc != 2)
		return 1;
	LPWSTR appId = argv[1];

	wprintf(L"Try to activate :%s\r\n", appId);
	CoInitialize(NULL);
	IApplicationActivationManager* paam = NULL;
	HRESULT hr = E_FAIL;
	__try
	{
		hr = CoCreateInstance(CLSID_ApplicationActivationManager, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&paam));
		wprintf(L"CoCreateInstance ActivationManager returns 0x%x\r\n", (DWORD) hr);
		if (FAILED(hr)) return 0;

		DWORD pid = 0;
		hr = paam->ActivateApplication(appId, nullptr, AO_NONE, &pid);
		wprintf(L"ActivateApplication Method returns 0x%x\n", hr);
		if (FAILED(hr))	return 0;

		wprintf(L"Activated  %s with pid %d\r\n", appId, pid);

		wprintf(L"switch to foreground window...\r\n");
		wprintf(L"hwnd 0x%x\r\n", (DWORD)g_SelectedProcessWindowHandle);
		PhEnumChildWindows(NULL, 0x800, EnumProcessWindowsProc, (ULONG)pid);
		if (IsWindow(g_SelectedProcessWindowHandle))
		{
			WINDOWPLACEMENT placement = { sizeof(placement) };

			GetWindowPlacement(g_SelectedProcessWindowHandle, &placement);

			if (placement.showCmd == SW_MINIMIZE)
				ShowWindowAsync(g_SelectedProcessWindowHandle, SW_RESTORE);
			else
				SetForegroundWindow(g_SelectedProcessWindowHandle);
		}
		wprintf(L"Succeed to Activation...\r\n");
	}
	__finally
	{
		if (paam) paam->Release();
	}

	CoUninitialize();

	return 0;
}

