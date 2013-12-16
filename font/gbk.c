

#include <font_manager.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

static struct stat hzkstat;

static unsigned char *Hzk_mem;
static unsigned char *Hzk_mem_end;

static int GbkFontInit(unsigned char *pcFontfile, unsigned int dwfontsize);
static int GbkGetFontBitMap(unsigned int dwcode, PT_FontBitMap ptFontBitMap);  

static T_FontOpr GbkFontOpr = {
	.name		= "gbk",
	.FontInit	= GbkFontInit,
	.GetFontBitMap	= GbkGetFontBitMap,
};


static int GbkFontInit(unsigned char *pcFontfile, unsigned int dwfontsize)
{
	int Hzk_fd;
	
	Hzk_fd = open(pcFontfile, O_RDONLY);
	if (Hzk_fd < 0)
	{
		printf("open Hzk file failed \n");
		return -1;
	}
	
	if (fstat(Hzk_fd, &hzkstat) == -1)
	{
		printf("get Hzk file stat failed\n");
		return -1;
	}


	Hzk_mem = (unsigned char *)mmap(NULL, hzkstat.st_size, PORT_READ, MAP_SHARED, Hzk_fd, 0); 
	if (Hzk_mem = (unsigned char *)-1)
	{
		printf("mmap hzk file failed\n");
		return -1;
	}
	
	Hzk_mem_end = Hzk_mem + hzkstat.st_size;

	return 0;
}

static int GbkGetFontBitMap(unsigned int dwcode, PT_FontBitMap ptFontBitMap)
{
	int iArea;
	int iWhere;

	int iPenY = ptFontBitMap->iCurOriginX;
	int iPenX = ptFontBitMap->iCurOriginY;

	if (dwcode & 0xffff0000)
	{
		printf("do not support this code :0x%x\n", dwcode);
		return -1;
	}

	iArea = (int)(dwcode & 0xff) - 0xA1;
	iWhere = (int)((dwcode >> 8) & 0xff) - 0xA1;

	if ((iArea < 0) || (iWhere < 0))
	{
		printf("%s,%d\n", __FUNCTION__, __LINE__);
		return -1;
	}

	ptFontBitMap->iXLeft		= iPenX;
	ptFontBitMap->iYTop		= iPenY - 16;
	ptFontBitMap->iXMax		= iPenX + 16;
	ptFontBitMap->iYMax		= iPenY;
	ptFontBitMap->iBpp		= 1;
	ptFontBitMap->iPitch		= 2;
	ptFontBitMap->pucBuffer 	= Hzk_mem + (iArea * 94 + iWhere) * 32;

	if (ptFontBitMap->pucBuffer > Hzk_mem_end)
	{
		printf("ptFontBitMap->pucBuffer error\n");
		return -1;
	}

	ptFontBitMap->iNextOriginX	= iPenX + 16;
	ptFontBitMap->iNextOriginY	= iPenY;

	return 0;
}

int GbkFontInit(void)
{
	return RegisterFontOpr(&GbkFontOpr);
}


