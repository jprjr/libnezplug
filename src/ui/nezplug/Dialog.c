#include <windows.h>

#include "ui/nezplug/Dialog.h"
#include "ui/nezplug/FileInfo.h"
#include "ui/nezplug/MemView.h"
#include "ui/nezplug/IOView.h"
#include "ui/nezplug/ChMask.h"
#include "ui/nezplug/Dump.h"

void NEZFileInfoDlg(HINSTANCE hDllInstance, HWND hwndParent)
{
	instance = hDllInstance;
	ShowWindow(
		CreateDialog(hDllInstance,(LPCTSTR)IDD_SONGINFO,hwndParent,(DLGPROC)DialogProc),SW_SHOW);
//	DialogBox(hDllInstance,(LPCTSTR)IDD_SONGINFO,hwndParent,(DLGPROC)DialogProc);
}

void NEZMemViewDlg(HINSTANCE hDllInstance, HWND hwndParent)
{
	ShowWindow(
		CreateDialog(hDllInstance,(LPCTSTR)IDD_MEMVIEW,hwndParent,(DLGPROC)MemViewDialogProc),SW_SHOW);
}

void NEZChMaskDlg(HINSTANCE hDllInstance, HWND hwndParent)
{
	ShowWindow(
		CreateDialog(hDllInstance,(LPCTSTR)IDD_CHMASK,hwndParent,(DLGPROC)ChMaskDialogProc),SW_SHOW);
}

void NEZIOViewDlg(HINSTANCE hDllInstance, HWND hwndParent)
{
	ShowWindow(
		CreateDialog(hDllInstance,(LPCTSTR)IDD_IOVIEW,hwndParent,(DLGPROC)IOViewDialogProc),SW_SHOW);
}

void NEZDumpDlg(HINSTANCE hDllInstance, HWND hwndParent)
{
	ShowWindow(
		CreateDialog(hDllInstance,(LPCTSTR)IDD_DUMP,hwndParent,(DLGPROC)DumpDialogProc),SW_SHOW);
}
