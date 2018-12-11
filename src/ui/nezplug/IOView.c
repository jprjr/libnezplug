#include <windows.h>
#include <stdio.h>

#include "IOView.h"

#define MEM_MAX 0xffff

enum{
	DEV_2A03,
	DEV_FDS,
	DEV_MMC5,
	DEV_VRC6,
	DEV_N106,
	DEV_DMG,
	DEV_HUC6230,
	DEV_AY8910,
	DEV_SN76489,
	DEV_SCC,
	DEV_OPL,
	DEV_OPLL,
	DEV_ADPCM,
	DEV_ADPCM2,
	DEV_MSX,
	DEV_MAX_,
};


extern int (*ioview_ioread_DEV_2A03   )(int a);
extern int (*ioview_ioread_DEV_FDS    )(int a);
extern int (*ioview_ioread_DEV_MMC5   )(int a);
extern int (*ioview_ioread_DEV_VRC6   )(int a);
extern int (*ioview_ioread_DEV_N106   )(int a);
extern int (*ioview_ioread_DEV_DMG    )(int a);
extern int (*ioview_ioread_DEV_HUC6230)(int a);
extern int (*ioview_ioread_DEV_AY8910 )(int a);
extern int (*ioview_ioread_DEV_SN76489)(int a);
extern int (*ioview_ioread_DEV_SCC    )(int a);
extern int (*ioview_ioread_DEV_OPL    )(int a);
extern int (*ioview_ioread_DEV_OPLL   )(int a);
extern int (*ioview_ioread_DEV_ADPCM  )(int a);
extern int (*ioview_ioread_DEV_ADPCM2 )(int a);
extern int (*ioview_ioread_DEV_MSX    )(int a);

unsigned int ioview_idc[16]={
	 IDC_MEMORY,IDC_MEMORY2,IDC_MEMORY3,IDC_MEMORY4
	,IDC_MEMORY5,IDC_MEMORY6,IDC_MEMORY7,IDC_MEMORY8
	,IDC_MEMORY9,IDC_MEMORY10,IDC_MEMORY11,IDC_MEMORY12
	,IDC_MEMORY13,IDC_MEMORY14,IDC_MEMORY15,IDC_MEMORY16
};
unsigned int speed_idc[5]={
	IDC_SPEED1,IDC_SPEED2,IDC_SPEED3,IDC_SPEED4,IDC_SPEED5
};

char devstring[DEV_MAX_][20]={
	"2A03    - REG",
	"2C33    - REG",
	"MMC5    - REG",
	"VRC6    - REG",
	"N1xx    - REG",
	"DMG     - REG",
	"HuC6230 - REG",
	"PSG     - REG",
	"DCSG    - REG",
	"SCC     - REG",
	"OPL     - REG",
	"OPLL    - REG",
	"ADPCM   - REG",
	"ADPCM   - MEMORY",
	"MSX     - 1BitDAC",
};

#define DEVICE(x,y) case x:return y != NULL ? y(address) : 0x100;

int ioview_ioread(int devicenum, int address){
	switch(devicenum){
		DEVICE(DEV_2A03   ,ioview_ioread_DEV_2A03   )
		DEVICE(DEV_FDS    ,ioview_ioread_DEV_FDS    )
		DEVICE(DEV_MMC5   ,ioview_ioread_DEV_MMC5   )
		DEVICE(DEV_VRC6   ,ioview_ioread_DEV_VRC6   )
		DEVICE(DEV_N106   ,ioview_ioread_DEV_N106   )
		DEVICE(DEV_DMG    ,ioview_ioread_DEV_DMG    )
		DEVICE(DEV_HUC6230,ioview_ioread_DEV_HUC6230)
		DEVICE(DEV_AY8910 ,ioview_ioread_DEV_AY8910 )
		DEVICE(DEV_SN76489,ioview_ioread_DEV_SN76489)
		DEVICE(DEV_SCC    ,ioview_ioread_DEV_SCC    )
		DEVICE(DEV_OPL    ,ioview_ioread_DEV_OPL    )
		DEVICE(DEV_OPLL   ,ioview_ioread_DEV_OPLL   )
		DEVICE(DEV_ADPCM  ,ioview_ioread_DEV_ADPCM  )
		DEVICE(DEV_ADPCM2 ,ioview_ioread_DEV_ADPCM2 )
		DEVICE(DEV_MSX    ,ioview_ioread_DEV_MSX    )
	}
	return 0x100;
}

LRESULT CALLBACK IOViewDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	int mempos=0;
	char strbuffer[256];
	int loop=0,loop2,membf,iosel=0;
	switch(message)
	{
	case WM_INITDIALOG :
		SendMessage(hDlg,WM_COMMAND,IDC_SPEED3,0);
		CheckRadioButton(hDlg,IDC_SPEED1,IDC_SPEED5,IDC_SPEED3);
		mempos=0;
		SetScrollPos(GetDlgItem(hDlg,IDC_MEMSCR),SB_CTL,0,TRUE);
		//コンボボックスアイテムの追加
		for(loop=0;loop<DEV_MAX_;loop++)
			SendMessage(GetDlgItem(hDlg,IDC_DEVICE), CB_ADDSTRING, loop, (LPARAM)devstring[loop]);
		SendMessage(GetDlgItem(hDlg,IDC_DEVICE), CB_SETCURSEL, 0, 0);

	case WM_INITDIALOG|0x8000 :
		SetScrollRange(GetDlgItem(hDlg,IDC_MEMSCR),SB_CTL,0,(MEM_MAX-0xff)/0x10,TRUE);
		return TRUE;

	case WM_TIMER :
		mempos=GetScrollPos(GetDlgItem(hDlg,IDC_MEMSCR),SB_CTL)<<4;
		iosel = SendMessage(GetDlgItem(hDlg,IDC_DEVICE), CB_GETCURSEL, 0, 0);
		sprintf(strbuffer,"");
		for(loop=0;loop<16;loop++){
			sprintf(strbuffer,"%04X|",mempos + loop*16);
			for(loop2=0;loop2<16;loop2++){
				membf = ioview_ioread(iosel, mempos + loop2 + loop*16);
				if(membf == 0x100)sprintf(strbuffer,"%s   ",strbuffer);
				else sprintf(strbuffer,"%s %02X",strbuffer,membf);
			}
			SetWindowText(GetDlgItem(hDlg,ioview_idc[loop]),strbuffer);
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
		if (LOWORD(wParam) == IDC_DEVICE)
		{
			if(HIWORD(wParam) == CBN_DROPDOWN)
			{
				KillTimer(hDlg,0);
			}
			else if(HIWORD(wParam) == CBN_CLOSEUP)
			{
				SetScrollPos(GetDlgItem(hDlg,IDC_MEMSCR),SB_CTL,0,TRUE);
				for(loop=0;loop<5;loop++){
					if(IsDlgButtonChecked(hDlg,speed_idc[loop]))
						SendMessage(hDlg,WM_COMMAND,speed_idc[loop],0);
				}
				SendMessage(hDlg,WM_TIMER,0,0);
			}
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

