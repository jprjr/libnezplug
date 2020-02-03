#include <windows.h>
#include <stdio.h>

#include "ChMask.h"

extern unsigned char chmask[0x80];

int a,b;

#define IDC_START IDC_2A03_SQ1
#define IDC_END   IDC_MSX_DA

LRESULT CALLBACK ChMaskDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_INITDIALOG :
		for(a=IDC_START,b=0; a<=IDC_END; a++,b++){
			CheckDlgButton(hDlg,a,chmask[b]);
		}
		return TRUE;

	case WM_COMMAND :
		if (LOWORD(wParam) == IDC_ALL_ENABLE){
			for(a=IDC_START,b=0; a<=IDC_END; a++,b++){
				chmask[b] = 1;
			}
			SendMessage(hDlg,WM_INITDIALOG,0,0);
			return TRUE;
		}
		if (LOWORD(wParam) == IDC_ALL_DISABLE){
			for(a=IDC_START,b=0; a<=IDC_END; a++,b++){
				chmask[b] = 0;
			}
			SendMessage(hDlg,WM_INITDIALOG,0,0);
			return TRUE;
		}
		if (LOWORD(wParam) == IDC_ALL_REVERSE){
			for(a=IDC_START,b=0; a<=IDC_END; a++,b++){
				chmask[b] = chmask[b] ? 0 : 1;
			}
			SendMessage(hDlg,WM_INITDIALOG,0,0);
			return TRUE;
		}
		if (LOWORD(wParam) >= IDC_START && LOWORD(wParam) <= IDC_END){
			chmask[LOWORD(wParam)-IDC_START] = chmask[LOWORD(wParam)-IDC_START] ? 0 : 1;
			SendMessage(hDlg,WM_INITDIALOG,0,0);
			return TRUE;
		}
		if (LOWORD(wParam) == IDCANCEL){
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		break;

	}
	return FALSE;
}

