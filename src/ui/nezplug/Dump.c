#include <windows.h>
#include <stdio.h>

#include "Dump.h"

#define MENU_MAX 10
static const char *hexstr="0123456789ABCDEF";

enum{
	MEM_FC,
	MEM_GB,
	MEM_PCE,
	MEM_MSX,
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
	DEV_MAX_,
};


extern int (*dump_MEM_FC     )(int a, unsigned char *mem);
extern int (*dump_MEM_GB     )(int a, unsigned char *mem);
extern int (*dump_MEM_PCE    )(int a, unsigned char *mem);
extern int (*dump_MEM_MSX    )(int a, unsigned char *mem);
extern int (*dump_DEV_2A03   )(int a, unsigned char *mem);
extern int (*dump_DEV_FDS    )(int a, unsigned char *mem);
extern int (*dump_DEV_MMC5   )(int a, unsigned char *mem);
extern int (*dump_DEV_VRC6   )(int a, unsigned char *mem);
extern int (*dump_DEV_N106   )(int a, unsigned char *mem);
extern int (*dump_DEV_DMG    )(int a, unsigned char *mem);
extern int (*dump_DEV_HUC6230)(int a, unsigned char *mem);
extern int (*dump_DEV_AY8910 )(int a, unsigned char *mem);
extern int (*dump_DEV_SN76489)(int a, unsigned char *mem);
extern int (*dump_DEV_SCC    )(int a, unsigned char *mem);
extern int (*dump_DEV_OPL    )(int a, unsigned char *mem);
extern int (*dump_DEV_OPLL   )(int a, unsigned char *mem);
extern int (*dump_DEV_ADPCM  )(int a, unsigned char *mem);


char dumpdevstring[DEV_MAX_][20]={
	"MEM_FC/NES",
	"MEM_GB",
	"MEM_PCE",
	"MEM_MSX,SMS,GG,CV",
	"DEV_2A03",
	"DEV_2C33",
	"DEV_MMC5",
	"DEV_VRC6",
	"DEV_N1xx",
	"DEV_DMG",
	"DEV_HuC6230",
	"DEV_PSG",
	"DEV_DCSG",
	"DEV_SCC",
	"DEV_OPL",
	"DEV_OPLL",
	"DEV_ADPCM",
};

char menustring[DEV_MAX_][MENU_MAX][20]={
/*---MEM_FC-----------------*/
	"Memory             ",
	"                   ",
	"                   ",
	"                   ",
	"                   ",
	"                   ",
	"                   ",
	"                   ",
	"                   ",
	"                   ",
/*---MEM_GB-----------------*/
	"Memory             ",
	"                   ",
	"                   ",
	"                   ",
	"                   ",
	"                   ",
	"                   ",
	"                   ",
	"                   ",
	"                   ",
/*---MEM_PCE----------------*/
	"Memory             ",
	"                   ",
	"                   ",
	"                   ",
	"                   ",
	"                   ",
	"                   ",
	"                   ",
	"                   ",
	"                   ",
/*---MEM_MSX----------------*/
	"Memory             ",
	"                   ",
	"                   ",
	"                   ",
	"                   ",
	"                   ",
	"                   ",
	"                   ",
	"                   ",
	"                   ",
/*---DEV_2A03---------------*/
	"Register           ",
	"DPCM Data          ",
	"                   ",
	"                   ",
	"                   ",
	"                   ",
	"                   ",
	"                   ",
	"                   ",
	"*DPCM Data to FlMML",
/*---DEV_FDS----------------*/
	"Register           ",
	"Wave Data          ",
	"Sweep Envelope Data",
	"                   ",
	"                   ",
	"                   ",
	"                   ",
	"*Wave Data to MCK  ",
	"*Wave Data to FlMML",
	"*Wave Data to GBwav",
/*---DEV_MMC5---------------*/
	"Register           ",
	"                   ",
	"                   ",
	"                   ",
	"                   ",
	"                   ",
	"                   ",
	"                   ",
	"                   ",
	"                   ",
/*---DEV_VRC6---------------*/
	"Register           ",
	"                   ",
	"                   ",
	"                   ",
	"                   ",
	"                   ",
	"                   ",
	"                   ",
	"                   ",
	"                   ",
/*---DEV_N106---------------*/
	"Register           ",
	"Wave Data - CH1    ",
	"Wave Data - CH2    ",
	"Wave Data - CH3    ",
	"Wave Data - CH4    ",
	"Wave Data - CH5    ",
	"Wave Data - CH6    ",
	"Wave Data - CH7    ",
	"Wave Data - CH8    ",
	"                   ",
/*---DEV_DMG----------------*/
	"Register           ",
	"Wave Data          ",
	"                   ",
	"                   ",
	"                   ",
	"                   ",
	"                   ",
	"                   ",
	"                   ",
	"                   ",
/*---DEV_HuC6230------------*/
	"Register 1         ",
	"Register 2         ",
	"Wave Data - CH1    ",
	"Wave Data - CH2    ",
	"Wave Data - CH3    ",
	"Wave Data - CH4    ",
	"Wave Data - CH5    ",
	"Wave Data - CH6    ",
	"                   ",
	"                   ",
/*---DEV_AY8910-------------*/
	"Register           ",
	"                   ",
	"                   ",
	"                   ",
	"                   ",
	"                   ",
	"                   ",
	"                   ",
	"                   ",
	"                   ",
/*---DEV_SN76489------------*/
	"Register           ",
	"                   ",
	"                   ",
	"                   ",
	"                   ",
	"                   ",
	"                   ",
	"                   ",
	"                   ",
	"                   ",
/*---DEV_SCC----------------*/
	"Register           ",
	"Wave Data - CH1    ",
	"Wave Data - CH2    ",
	"Wave Data - CH3    ",
	"Wave Data - CH4    ",
	"Wave Data - CH5    ",
	"                   ",
	"                   ",
	"                   ",
	"                   ",
/*---DEV_OPL----------------*/
	"Register           ",
	"                   ",
	"                   ",
	"                   ",
	"                   ",
	"                   ",
	"                   ",
	"                   ",
	"                   ",
	"                   ",
/*---DEV_OPLL---------------*/
	"Register           ",
	"                   ",
	"                   ",
	"                   ",
	"                   ",
	"                   ",
	"                   ",
	"                   ",
	"                   ",
	"                   ",
/*---DEV_ADPCM--------------*/
	"Register 1         ",
	"Register 2 (PCE)   ",
	"Memory             ",
	"                   ",
	"                   ",
	"                   ",
	"                   ",
	"                   ",
	"                   ",
	"                   ",
/*--------------------------
	"Register           ",
	"                   ",
	"                   ",
	"                   ",
	"                   ",
	"                   ",
	"                   ",
	"                   ",
	"                   ",
	"                   ",
/*--------------------------*/
};

unsigned int dump_menu[MENU_MAX]={
	 IDC_DUMP1 ,
	 IDC_DUMP2 ,
	 IDC_DUMP3 ,
	 IDC_DUMP4 ,
	 IDC_DUMP5 ,
	 IDC_DUMP6 ,
	 IDC_DUMP7 ,
	 IDC_DUMP8 ,
	 IDC_DUMP9 ,
	 IDC_DUMP10,
};

int a,b;
unsigned char txt[524288];
unsigned char mem[65536];
unsigned char ffile[400];
unsigned char dfile[400];

#define DEVICE(x,y) case x: return y != NULL ? y(dumpnum,(unsigned char *)mem) : -2;break;
/* return値
   0以上:ダンプ正常終了（バイナリで出力された。）
   -1   :ダンプ正常終了（テキストで出力された。）
   -2   :変化なし
*/
int _dump(int devicenum, int dumpnum){
	switch(devicenum){
		DEVICE(MEM_FC     ,dump_MEM_FC     )
		DEVICE(MEM_GB     ,dump_MEM_GB     )
		DEVICE(MEM_PCE    ,dump_MEM_PCE    )
		DEVICE(MEM_MSX    ,dump_MEM_MSX    )
		DEVICE(DEV_2A03   ,dump_DEV_2A03   )
		DEVICE(DEV_FDS    ,dump_DEV_FDS    )
		DEVICE(DEV_MMC5   ,dump_DEV_MMC5   )
		DEVICE(DEV_VRC6   ,dump_DEV_VRC6   )
		DEVICE(DEV_N106   ,dump_DEV_N106   )
		DEVICE(DEV_DMG    ,dump_DEV_DMG    )
		DEVICE(DEV_HUC6230,dump_DEV_HUC6230)
		DEVICE(DEV_AY8910 ,dump_DEV_AY8910 )
		DEVICE(DEV_SN76489,dump_DEV_SN76489)
		DEVICE(DEV_SCC    ,dump_DEV_SCC    )
		DEVICE(DEV_OPL    ,dump_DEV_OPL    )
		DEVICE(DEV_OPLL   ,dump_DEV_OPLL   )
		DEVICE(DEV_ADPCM  ,dump_DEV_ADPCM  )
	}
	return -2;
}

void setmem(HWND hDlg, int ret){
	int a,b;
	unsigned char *ptxt = txt;
	if(!IsDlgButtonChecked(hDlg,IDC_DUMPFORMAT3)){
		//テキスト出力の場合
		if(ret>=0){//バイナリ出力された場合
			if(IsDlgButtonChecked(hDlg,IDC_DUMPFORMAT1)){
				//HEX保存
				memset(txt,0,sizeof(txt));
				for(a=0;a<ret;a++){
					*ptxt = hexstr[mem[a]>>4];
					ptxt++;
					*ptxt = hexstr[mem[a]&0xf];
					ptxt++;
					if((a&0xf)==0xf){
						*ptxt = '¥r';
						ptxt++;
						*ptxt = '¥n';
					}else{
						*ptxt = ' ';
					}
					ptxt++;
				}
				SetWindowText(GetDlgItem(hDlg,IDC_DUMPTEXT),txt);
			}else if(IsDlgButtonChecked(hDlg,IDC_DUMPFORMAT2)){
				//HEX保存(アドレス付き)
				memset(txt,0,sizeof(txt));
				for(a=0;a<ret;a++){
					if((a&0xf)==0x0){
						for(b=0;b<4;b++){
							*ptxt = hexstr[(a>>(12-b*4))&0xf];
							ptxt++;
						}
						*ptxt = ':';
						ptxt++;
						*ptxt = ' ';
						ptxt++;
					}
					*ptxt = hexstr[mem[a]>>4];
					ptxt++;
					*ptxt = hexstr[mem[a]&0xf];
					ptxt++;
					if((a&0xf)==0xf){
						*ptxt = '¥r';
						ptxt++;
						*ptxt = '¥n';
					}else{
						*ptxt = ' ';
					}
					ptxt++;
				}
				SetWindowText(GetDlgItem(hDlg,IDC_DUMPTEXT),txt);
			}
		}else if(ret==-1){
			memcpy(txt,mem,sizeof(mem));
			SetWindowText(GetDlgItem(hDlg,IDC_DUMPTEXT),txt);
		}
	}else{
		//ファイル出力の場合
		if(ret>-2){
			OPENFILENAME ofn;
			memset(&ofn,0,sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hDlg;
			ofn.lpstrFilter = "All files(*.*)¥0*.*¥0¥0";
			ofn.lpstrFile = ffile;
			ofn.nMaxFile = sizeof(ffile);
			ofn.lpstrFileTitle = dfile;
			ofn.nMaxFileTitle = sizeof(dfile);
			ofn.Flags = OFN_HIDEREADONLY;
			//ofn.lpstrTitle = "Save dump file";
			if(GetSaveFileName(&ofn)){
				FILE *f = fopen(ffile,"wb");
				if(f!=NULL){
					if(ret>= 0) fwrite(mem,1,ret,f);
					if(ret==-1) fprintf(f,"%s",mem);
					fclose(f);
				}
			}
		}
	}
}	


#define DMPBUTTON(x,y) if(LOWORD(wParam) == x){setmem(hDlg,_dump(iosel,y));return TRUE;}

LRESULT CALLBACK DumpDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	int loop=0,iosel=0;
	switch(message)
	{
	case WM_INITDIALOG :
		for(loop=0;loop<DEV_MAX_;loop++)
			SendMessage(GetDlgItem(hDlg,IDC_DMPDEVICE), CB_ADDSTRING, loop, (LPARAM)dumpdevstring[loop]);
		SendMessage(GetDlgItem(hDlg,IDC_DMPDEVICE), CB_SETCURSEL, 0, 0);
		for(loop=0;loop<MENU_MAX;loop++){
			SetWindowText(GetDlgItem(hDlg,dump_menu[loop]),menustring[iosel][loop]);
		}

		CheckRadioButton(hDlg,IDC_DUMPFORMAT1,IDC_DUMPFORMAT2,IDC_DUMPFORMAT1);
		sprintf(dfile,"dump.bin");
		return TRUE;

	case WM_COMMAND :
		iosel = SendMessage(GetDlgItem(hDlg,IDC_DMPDEVICE), CB_GETCURSEL, 0, 0);
		if (LOWORD(wParam) == IDCANCEL){
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		if (LOWORD(wParam) == IDC_DMPDEVICE)
		{
			if(HIWORD(wParam) == CBN_DROPDOWN)
			{
			}
			else if(HIWORD(wParam) == CBN_SELCHANGE )
			{
				for(loop=0;loop<MENU_MAX;loop++){
					SetWindowText(GetDlgItem(hDlg,dump_menu[loop]),menustring[iosel][loop]);
				}
			}
			return TRUE;
		}

		DMPBUTTON(IDC_DUMP1 ,1 )
		DMPBUTTON(IDC_DUMP2 ,2 )
		DMPBUTTON(IDC_DUMP3 ,3 )
		DMPBUTTON(IDC_DUMP4 ,4 )
		DMPBUTTON(IDC_DUMP5 ,5 )
		DMPBUTTON(IDC_DUMP6 ,6 )
		DMPBUTTON(IDC_DUMP7 ,7 )
		DMPBUTTON(IDC_DUMP8 ,8 )
		DMPBUTTON(IDC_DUMP9 ,9 )
		DMPBUTTON(IDC_DUMP10,10)

		if (LOWORD(wParam) == IDC_COPY)
		{
			if(OpenClipboard(NULL)){
				HGLOBAL hMem = GlobalAlloc(GMEM_FIXED, strlen(txt)+1);
				LPTSTR pMem = (LPTSTR)hMem;
				lstrcpy(pMem, (LPCTSTR)txt);
				EmptyClipboard();
				SetClipboardData(CF_TEXT, hMem);
				CloseClipboard();

			}
		}

		
		break;

	}
	return FALSE;
}

