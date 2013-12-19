
#include <display_manager.h>
#include <font_manager.h>
#include <encode_manager.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <config.h>

typedef struct PT_PageDesc {
	int num;
	unsigned char *LcdFirstPosAtFile;
	unsigned char *LcdNextFirstPosAtFile;
	struct PT_PageDesc *next;
	struct PT_PageDesc *pre;
}T_PageDesc, *PT_PageDesc;

static struct stat bstat;
static unsigned char *textfilememstart;
static unsigned char *textfilememend;
static unsigned char *TextFirstPosAtFile;
static unsigned char *textcurrentposatfile;
static unsigned int dwFontSize;

static PT_DispOpr g_ptDispOpr;
static PT_EncodingOpr fileEncodingOpr;
static PT_PageDesc PT_CurPage;

int OpenTextFile(char *Textfilename)
{
	int fd_text;

	fd_text = open(Textfilename, O_RDONLY);
	if (fd_text < 0)
	{
		printf("opne text file failed !\n");
		return -1;
	}

	if ((fstat(fd_text, &bstat) == -1))
	{
		printf ("get text file stat failed!\n");
		return -1;
	}

	textfilememstart = (unsigned char *)mmap(NULL, bstat.st_size, PROT_READ, MAP_SHARED, fd_text, 0);
	if (textfilememstart == (unsigned char *)-1)
	{
		printf("mmap text file failed\n");
		return -1;
	}

	textfilememend = textfilemem + bstat.st_size;

	fileEncodingOpr = SelectEncodOprforFile(textfilemem);

	if (fileEncodingOpr)
	{
		TextFirstPosAtFile = textfilememstart + fileEncodingOpr->HeadLen;
		return 0;
	}
	else
	{
		return -1;
	}
}

int SetTextDetail(unsigned char *font_file, unsigned char *hzk_file, unsigned int size)
{
	PT_FontOpr ptmp;
	int ret;
	
	dwFontSize = size;
	ptmp = fileEncodingOpr->ptFontOprSupportedHead;
	while (ptmp)
	{
		if (strcmp(ptmp->name, "ascii") == 0)
		{
			iError = ptmp->FontInit(NULL, dwFontSize);
		}
		else if (strcmp(ptmp->name, "freetype") == 0)
		{
			iError = ptmp->FontInit(font_file, dwFontSize);
		}
		else if (strcmp(ptmp->name, "gbk") == 0)
		{
			iError = ptmp->FontInit(hzk_file, dwFontSize);
		}
		else
		{
			DeletFontOprForEncoding(fileEncodingOpr, ptmp);
		}
		ptmp = ptmp->next;

		if (iError < 0)
		{
			printf("Init the font detail failed\n");
			DeletFontOprForEncoding(fileEncodingOpr, ptmp);
			ret = -1;
		}
		else
		{
			ret = 0;
		}
	}

	return ret;
}

int SelectAndInitDisplayDevice(unsigned char *name)
{
	PT_DispOpr ptmp = T_DispOprSupportHead;

	while (ptmp)
	{
		if (strcmp(ptmp->name, name) == 0)
		{
			g_ptDispOpr = ptmp;
			rerurn(ptmp->DeviceInit());
		}

		else
		{
			ptmp = ptmp->next;
		}
	}

		return -1; 
}


int IncLcdY(int iY)
{
	if (iY + dwFontSize < g_ptDispOpr->iYres)
	{
		return (iY + dwFontSize);
	}
	else
	{
		return 0;
	}
}

int ReloateFontPos(PT_FontBitMap ptFontBitMap)
{
	int LcdY;
	int deltaX;
	int deltaY;

	if (ptFontBitMap->iYMax > g_ptDispOpr->yres)
	{
		return -1;	
	}
	if (ptFontBitMap->iXMax > g_ptDispOpr->xres)
	{
		LcdY = IncLcdY(ptFontBitMap->iCurOriginY);
		if (LcdY == 0)
		{
			return -1;
		}
		else
		{
			deltaX = 0 - ptFontBitMap->iCurOriginX;
			deltaY = LcdY - ptFontBitMap->iCurOriginY;

			ptFontBitMap->iCurOriginX += deltaX;
			ptFontBitMap->iCurOriginY += deltaY;
			
			ptFontBitMap->iXLeft	  += deltaX;
			ptFontBitMap->iXMax       += deltaX;

			ptFontBitMap->iYTop       += deltaY;
			ptFontBitMap->iYMax	  += deltaY;

			ptFontBitMap->iNextOriginX += deltaX;
			ptFontBitMap->iNextOriginY += deltaY;

			return 0;
		}
	}
	return 0;
}

int showonefont(PT_FontBitMap ptFontBitMap)
{

}

static int showonepage(unsigned char *buffstart)
{
	int len;
	int iError;
	unsigned char *codebufstart;
	unsigned int dwcode;

	int bHasNotClrScreen = 1;
	int bHasGetCode = 0;
	T_FontBitMap tFontBitMap;
	PT_FontOpr ptFontOpr;

	codebufstart = buffstart;
	ptFontBitMap->iCurOriginX = 0;
	ptFontBitMap->iCurOriginY = dwFontSize;

	while (1)
	{
		len = fileEncodingOpr->GetCodeFrmBuf(codebufstart, textfilememend, &dwcode);
		if (len == 0)
		{
			if (!bHasGetCode)
			{
				return -1;
			}		
			else
			{
				return 0;
			}
		}
		bHasGetCode = 1;
		codebufstart += len;
		
		if (dwcode == '\n')
		{
			TextFirstPosAtFile = codebufstart;

			tFontBitMap.iCurOriginX = 0;
			tFontBitMap.iCurOriginY = IncLcdY(tFontBitMap.iCurOriginY);
			if (tFontBitMap.iCurOriginY == 0)
			{
				return -1;
			}
			else
			{
				continue;
			}
		}
		else if (dwcode == '\r')
		{
			continue;
		}
		else if (dwcode == '\t')
		{
			dwcode == ' ';
		}

		ptFontOpr = fileEncodingOpr->ptFontOprSupportedHead;
		while (ptFontOpr)
		{
			iError = ptFontOpr->GetCodeFrmBuf(dwcode, &tFontBitMap);
			if (iError == 0)
			{	
				if (ReloateFontPos(&tFontBitMap))
				{
					return -1;
				}
				
				if (bHasNotClrScreen)
				{
					g_ptDispOpr->ClearScreen(BACKCOLOR);	
					bHasNotClrScreen = 0;
				}
				if (showonefont(&tFontBitMap))
				{
					return 0;
				}
				tFontBitMap.iCurOriginX = tFontBitMap.iNextOriginX;
				tFontBitMap.iCurOriginY = tFontBitMap.iNextOriginY;

				TextFirstPosAtFile = codebufstart;
				break;
			}
			ptFontOpr = ptFontOpr->next;
		}
	}

	return 0;
}

int shownextpage()
{
	PT_PageDesc ptPage;
	unsigned int *dwcode;
	int len;

	if (PT_CurPage)
	{
		textcurrentposatfile = PT_CurPage->LcdNextFirstPosAtFile;
	}
	else
	{
		textcurrentposatfile = TextFirstPosAtFile;
	}
	showonepage(textcurrentposatfile);	

	ptPage->LcdFirstPosAtFile 	= textcurrentposatfile;
	ptPage->LcdNextFirstPosAtFile 	= TextFirstPosAtFile;
	ptPage->next			= NULL;
	ptPage->pre			= NULL;

	PT_CurPage = ptPage;
	RecordPage(ptPage);

	return 0;
}










