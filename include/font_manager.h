#ifndef _FONT_MANAGER_H
#define _FONT_MANAGER_H

typedef struct FontBitMap {
	int iXLeft;
	int iYTop;
	int iXMax;
	int iYMax;
	int iBpp;
	int iPitch;
	int iCurOriginX;
	int iCurOriginY;
	int iNextOriginX;
	int iNextOriginY;
	unsigned char *pucBuffer;	
}T_FontBitMap, *PT_FontBitMap;

typedef struct FontOpr {
	char *name;
	int (*FontInit)(char *fontfile, unsigned int dwfontsize);
	int (*GetCodeBitMap)(unsigned int dwcode, PT_FontBitMap ptFontBitMap);
	struct FontOpr *next;
}T_FontOpr, *PT_FontOpr;

int RegisterFontOpr(PT_FontOpr ptFontOpr);
PT_FontOpr getFontOpr(char *name);
int Font_Init(void);
