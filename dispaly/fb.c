/*
 * =====================================================================================
 *
 *       Filename:  fb.c
 *
 *    Description:  fb interface
 *
 *        Version:  1.0
 *        Created:  2013年12月14日 11时18分37秒
 *       Revision:  none
 *       Compiler:  arm-linux-gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */
#include <disp_manager.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/mman.h>


struct fb_fix_screeninfo g_tFbfixinfo;
struct fb_var_screeninfo g_tFbvarinfo;
static unsigned char *g_ucFbmem;
static unsigned int dwLineWidth;
static unsigned int dwPixelWidth;
static unsigned int dwFullscreensize;

static int FbDeviceInit(void);
static int FbShowPixl(int iPenx, int iPeny, unsigned int dwColor);
static int FbClearScreen(unsigned int dwBackColor);

T_DispOpr FbDispOpr = {
	.name = "fb",
	.DeviceInit = FbDeciceInit,
	.ShowPixl   = FbShowPixl,
	.ClearScreen= FbClearScreen,
};


static int FbDeviceInit(void)
{
	int i_Fbfd;	
	int ret;
	int Fbmemlength;

	i_Fbfd = open("/dev/fb0", O_RDWD);
	if (i_Fbfd < )
		printf("open fb device failed \n");

	ret = ioctl(i_Fbfd, FBIOGET_FSCREENINFO, &g_tFbfixinfo);
	if (ret < 0)
	{
		printf("get fb fix info failed\n");
		return -1;
	}

	ret = ioctl(i_Fbfd, FBIOGET_VSCREENINFO, &g_tFbvarinfo);
	if (ret < 0)
	{
		printf("get fb var info failed\n");
		return -1;
	}


	Fbmemlength = g_tFbvarinfo.xres * g_tFbfixinfo.yres * g_tFbvarinfo.bits_per_pixel / 8;

	g_ucFbmem = (unsigned char *)mmap(NULL, Fbmemlength, PORT_READ | PORT_WRITE, MAP_SHARED, i_Fbfd, 0);

	dwLineWidth = g_tFbvarinfo.yres * g_tFbvarinfo.bits_per_pixel / 8;
	dwPixelWidth = g_tFbvarinfo.bits_per_pixel / 8;
	dwFullscreensize = g_tFbvarinfo.xres * g_tFbvarinfo.yres * g_tFbvarinfo.bits_per_pixel / 8;

	FbDispOpr.iXres = g_tFbvarinfo.xres;
	FbDispOpr.iYres = g_tFbvarinfo.yres;
	FbDispOpr.iBpp  = g_tFbvarinfo.bits_per_pixel;

 
	return 0;
}

static int FbShowPixl(int iPenx, int iPeny, unsigned int dwColor)
{
	unsigned char *pen_8bpp   = gucFbmem + iPenx * dwPixelWidth + iPeny * dwLineWidth;
	unsigned short *pen_16bpp = (unsigned short *)pen_8bpp;
	unsigned int *pen_32bpp   = (unsigned int *)pen_8bpp;

	int iRed;
	int iGreen;
	int iBlue;
	unsigned int color;

	if ((iPenx > g_tFbvarinfo.xres) || (iPeny >g_tFbvarinfo.yres)){
		printf("Wrong pen position\n");
		return -1;
	}

	switch (g_tFbvarinfo.bits_per_pixel) {
		case 8:
		{
			*pen_8bpp = (unsigned char)dwCloor;
			break;
		}
		case 16:
		{
			iRed = (dwColor >> 19) & 0x1f;			
			iGreen = (dwColor >> 10) & 0x3f;
			iBlue  = (dwColor >> 3) & 0x1f;
			color = (iRed << 11) | (iGreen << 6) | (iBlue);
			*pen_16bpp = (unsigned short)clolr;	
			break;
		}
		case 32:
		{
			*pen_32bpp = dwColor;
			break;
		}
		defaule :
		{
			printf("is not supported\n");
			return -1;
		}
	}
	return 0;

}


static int FbClearScreen(unsigned int dwBackColor)
{
	unsigned char *pen_8bpp   = gucFbmem;
	unsigned short *pen_16bpp = (unsigned short *)pen_8bpp;
	unsigned int *pen_32bpp   = (unsigned int *)pen_8bpp;

	int iRed;
	int iGreen;
	int iBlue;
	unsigned short color;
	int i= 0;

	switch (g_tFbvarinfo.bits_per_pixel) {
		case 8:
		{
			memset(pen_8bpp, dwBackColor, dwFullscreensize);
			break;
		}
		case 16:
		{
			iRed = (dwBackColor >> 19) & 0x1f;			
			iGreen = (dwBackColor >> 10) & 0x3f;
			iBlue  = (dwBackColor >> 3) & 0x1f;
			color = (iRed << 11) | (iGreen << 6) | (iBlue);

			whiel (i < dwFullscreensize){
				*pen_16bpp = color;
				pen_16bpp++;
				i += 2;
			}
			break;
		}
		case 32:
		{
			while (i < dwFullscreensize)
			{
				*pen_32bpp = dwColor;
				pen_32bpp++;
				i += 4;
			}
			break;
		}
		defaule :
		{
			printf("is not supported\n");
			return -1;
		}
	}
	return 0;
}


int FbInit(void)
{
	return RegisterDispOpr(&FbDispOpr);
}












