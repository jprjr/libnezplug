#include <windows.h>
#include <stdio.h>

#include "MemView.h"

#define MEM_MAX 0xffff //←どうせ全部FFFFH

extern int (*memview_memread)(int a);
extern int MEM_IO,MEM_RAM,MEM_ROM;
unsigned int memview_idc[16]={
	 IDC_MEMORY,IDC_MEMORY2,IDC_MEMORY3,IDC_MEMORY4
	,IDC_MEMORY5,IDC_MEMORY6,IDC_MEMORY7,IDC_MEMORY8
	,IDC_MEMORY9,IDC_MEMORY10,IDC_MEMORY11,IDC_MEMORY12
	,IDC_MEMORY13,IDC_MEMORY14,IDC_MEMORY15,IDC_MEMORY16
};

LRESULT CALLBACK MemViewDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	int mempos=0;
	char strbuffer[256];
	int loop=0;
	switch(message)
	{
	case WM_INITDIALOG :
		SendMessage(hDlg,WM_COMMAND,IDC_SPEED3,0);
		CheckRadioButton(hDlg,IDC_SPEED1,IDC_SPEED5,IDC_SPEED3);
		mempos=0;
		SetScrollPos(GetDlgItem(hDlg,IDC_MEMSCR),SB_CTL,0,TRUE);
	case WM_INITDIALOG|0x8000 :
		SetScrollRange(GetDlgItem(hDlg,IDC_MEMSCR),SB_CTL,0,(MEM_MAX-0xff)/0x10,TRUE);
		if(memview_memread==NULL){
			/*MessageBox(hDlg,"Memory Viewer can't start now.\nPlay music in supported file format, Memory Viewer can start."
				,"Memory Viewer Error",0);
			//EndDialog(hDlg, LOWORD(wParam));
			//DestroyWindow(hDlg);
			*/
			return TRUE;
		}
		return TRUE;

	case WM_TIMER :
		mempos=GetScrollPos(GetDlgItem(hDlg,IDC_MEMSCR),SB_CTL)<<4;
		if(memview_memread==NULL){
			for(loop=0;loop<16;loop++){
				sprintf(strbuffer,"%04X|                                                  ",mempos + loop*16);
				SetWindowText(GetDlgItem(hDlg,memview_idc[loop]),strbuffer);
			}
//			SetScrollRange(GetDlgItem(hDlg,IDC_MEMSCR),SB_CTL,-1,-1,TRUE);
//			SetScrollPos(GetDlgItem(hDlg,IDC_MEMSCR),SB_CTL,-1,TRUE);
			return TRUE;
		}

		sprintf(strbuffer,"");
		for(loop=0;loop<16;loop++){
			sprintf(strbuffer,"%04X| %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X   \r\n"
				,mempos + loop*16
				,memview_memread!=NULL ? memview_memread(mempos + 0x0 + loop*16) : 0
				,memview_memread!=NULL ? memview_memread(mempos + 0x1 + loop*16) : 0
				,memview_memread!=NULL ? memview_memread(mempos + 0x2 + loop*16) : 0
				,memview_memread!=NULL ? memview_memread(mempos + 0x3 + loop*16) : 0
				,memview_memread!=NULL ? memview_memread(mempos + 0x4 + loop*16) : 0
				,memview_memread!=NULL ? memview_memread(mempos + 0x5 + loop*16) : 0
				,memview_memread!=NULL ? memview_memread(mempos + 0x6 + loop*16) : 0
				,memview_memread!=NULL ? memview_memread(mempos + 0x7 + loop*16) : 0
				,memview_memread!=NULL ? memview_memread(mempos + 0x8 + loop*16) : 0
				,memview_memread!=NULL ? memview_memread(mempos + 0x9 + loop*16) : 0
				,memview_memread!=NULL ? memview_memread(mempos + 0xa + loop*16) : 0
				,memview_memread!=NULL ? memview_memread(mempos + 0xb + loop*16) : 0
				,memview_memread!=NULL ? memview_memread(mempos + 0xc + loop*16) : 0
				,memview_memread!=NULL ? memview_memread(mempos + 0xd + loop*16) : 0
				,memview_memread!=NULL ? memview_memread(mempos + 0xe + loop*16) : 0
				,memview_memread!=NULL ? memview_memread(mempos + 0xf + loop*16) : 0
			);
			SetWindowText(GetDlgItem(hDlg,memview_idc[loop]),strbuffer);
			sprintf(strbuffer,"");
		}

		return TRUE;

	case WM_COMMAND :
		if (LOWORD(wParam) == IDCANCEL){
			KillTimer(hDlg,0);
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		if (LOWORD(wParam) == IDC_SPEED1){
			KillTimer(hDlg,0);
			SendMessage(hDlg,WM_TIMER,0,0);
			return TRUE;
		}
		if (LOWORD(wParam) == IDC_SPEED2){
			SetTimer(hDlg,0,200,0);
			return TRUE;
		}
		if (LOWORD(wParam) == IDC_SPEED3){
			SetTimer(hDlg,0,100,0);
			return TRUE;
		}
		if (LOWORD(wParam) == IDC_SPEED4){
			SetTimer(hDlg,0,50,0);
			return TRUE;
		}
		if (LOWORD(wParam) == IDC_SPEED5){
			SetTimer(hDlg,0,0,0);
			return TRUE;
		}
		if (LOWORD(wParam) == IDC_ADR_IO){
			SendMessage(hDlg,WM_VSCROLL,LOWORD(wParam|0x8000),0);
			return TRUE;
		}
		if (LOWORD(wParam) == IDC_ADR_RAM){
			SendMessage(hDlg,WM_VSCROLL,LOWORD(wParam|0x8000),0);
			return TRUE;
		}
		if (LOWORD(wParam) == IDC_ADR_ROM){
			SendMessage(hDlg,WM_VSCROLL,LOWORD(wParam|0x8000),0);
			return TRUE;
		}
		break;

    case WM_VSCROLL:
		{
			int nScrollCode = (int) LOWORD(wParam);
			int nPos = (int)(short int) HIWORD(wParam);
			mempos=GetScrollPos(GetDlgItem(hDlg,IDC_MEMSCR),SB_CTL)<<4;

			switch(nScrollCode){
				case SB_LINEDOWN:		//1行下へスクロール
					nPos = (mempos>>4) + 1;
					break;
				case SB_LINEUP:			//1行上へスクロール
					nPos = (mempos>>4) - 1;
					break;
				case SB_PAGEDOWN:		//1ページ下へスクロール
					nPos = (mempos>>4) + 16;
					break;
				case SB_PAGEUP:			//1ページ上へスクロール
					nPos = (mempos>>4) - 16;
					break;
				case SB_BOTTOM:			//一番下までスクロール
					nPos = (MEM_MAX>>4)-0xf;
					break;
				case SB_TOP:			//一番上までスクロール
					nPos = 0;
					break;
				case SB_THUMBPOSITION:	//絶対位置へスクロール
				case SB_THUMBTRACK:
					break;
				case IDC_ADR_IO|0x8000:		//アドレス指定ボタン（IO）
					nPos = MEM_IO>>4;
					break;
				case IDC_ADR_RAM|0x8000:		//アドレス指定ボタン（RAM）
					nPos = MEM_RAM>>4;
					break;
				case IDC_ADR_ROM|0x8000:		//アドレス指定ボタン（ROM）
					nPos = MEM_ROM>>4;
					break;
				case SB_ENDSCROLL:		//スクロール終了
				default:
					return TRUE;
			}
			if(nPos < 0) nPos=0;
			if(nPos > (MEM_MAX>>4)-0xf) nPos=(MEM_MAX>>4)-0xf;
			SetScrollPos(GetDlgItem(hDlg,IDC_MEMSCR),SB_CTL,nPos,TRUE);
			mempos = nPos<<4;
			SendMessage(hDlg,WM_TIMER,0,0);
		}	
		return TRUE;
	}
	return FALSE;
}

