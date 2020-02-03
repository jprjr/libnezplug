#include <windows.h>

#include "FileInfo.h"
#include "MemView.h"
#include "IOView.h"
#include "ChMask.h"
#include "Dump.h"

extern struct {
	char* title;
	char* artist;
	char* copyright;
	char detail[1024];
}songinfodata;


LRESULT CALLBACK DialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_INITDIALOG :
		SetWindowText(GetDlgItem(hDlg,IDC_TITLE),songinfodata.title);
		SetWindowText(GetDlgItem(hDlg,IDC_ARTIST),songinfodata.artist);
		SetWindowText(GetDlgItem(hDlg,IDC_COPYRIGHT),songinfodata.copyright);
		SetWindowText(GetDlgItem(hDlg,IDC_DETAIL),songinfodata.detail);
		return TRUE;
	case WM_COMMAND :
		if (LOWORD(wParam) == IDCANCEL){
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		//メモリービュアーボタンを押したとき
		if (LOWORD(wParam) == IDC_MEMVIEW){
			ShowWindow(
				CreateDialog(instance,(LPCTSTR)IDD_MEMVIEW,hDlg,(DLGPROC)MemViewDialogProc),SW_SHOW);
			return TRUE;
		}
		//チャンネルマスクボタンを押したとき
		if (LOWORD(wParam) == IDC_CHMASK){
			ShowWindow(
				CreateDialog(instance,(LPCTSTR)IDD_CHMASK,hDlg,(DLGPROC)ChMaskDialogProc),SW_SHOW);
			return TRUE;
		}
		//デバイスレジスタビュアーボタンを押したとき
		if (LOWORD(wParam) == IDC_DEVVIEW){
			ShowWindow(
				CreateDialog(instance,(LPCTSTR)IDD_IOVIEW,hDlg,(DLGPROC)IOViewDialogProc),SW_SHOW);
			return TRUE;
		}
		//ダンプボタンを押したとき
		if (LOWORD(wParam) == IDC_DUMP){
			ShowWindow(
				CreateDialog(instance,(LPCTSTR)IDD_DUMP,hDlg,(DLGPROC)DumpDialogProc),SW_SHOW);
			return TRUE;
		}
		break;
	}
	return FALSE;
}

