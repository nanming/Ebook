
#include <font_manager.h>
#include <ft2build.h>

#include FT_FREETYPE_H
#include FT_GLYPH_H

static int FreetypeFontInit(char *fontfile, unsigned int dwfontsize);
static int FreetypeGetBitMap(unsigned int dwcode, PT_FontBitMap ptFontBitMap);

T_FontOpr FreetypeFontOpr = {
	.name		= "freetype",
	.FontInit	= FreetypeFontInit,
	.GetCodeBitMap	= FreetypeGetBitMap,
};

static FT_Library g_tLibrary;
static FT_Face g_tFace;
static FT_GlyphSlot g_tSlot;

static int FreetypeFontInit(char *fontfile, unsigned int dwfontsize)
{
	int iError;

	iError = FT_Init_FreeType(&g_tLibrary);

	if (iError)
	{
		printf("FreeType Init failed\n");
		return -1;
	}
	
	iError = FT_New_Face(g_tLibrary, fontfile, 0, &g_tFace);
	if (iError)
	{
		printf("FreeType New Face Init failed\n");
		return -1;
	}

	g_tSlot = g_tFace->glypy;

	iError = FT_Set_Pixel_Sizes(g_tFace, dwfontsize, 0);
	if (iError)
	{
		printf("FreeType Set Pixel Size failed\n");
		return -1;
	}

	return 0; 
}


static int FreetypeGetBitMap(unsigned int dwcode, PT_FontBitMap ptFontBitMap)
{
	int iError;
	int iPenX = ptFontBitMap->iCurOriginX;
	int iPenY = ptFontBitMap->iCurOriginY;

	iError = FT_Load_Char(g_tFace, dwcode, FT_LOAD_RENDER | FT_LOAD_MONOCHROME);
	if (iError)
	{
		printf("FT_Load_Char error for code : %d\n", dwcode);
	}

	ptFontBitMap->iXLeft		= iPenX + g_tSlot->bitmap_left;
	ptFontBitMap->iYTop		= iPenY - g_tSlot->bitmap_top;
	ptFontBitMap->iXMax		= ptFontBitMap->iXLeft + g_tSlot->bitmap_top.width;
	ptFontBitMap->iYMax		= ptFontBitMap->iYTop + g_tSlot->bitmap_top.rows;
	ptFontBitMap->iBpp		= 1;
	ptFontBitMap->iPitch		= g_tSlot->bitmap.pitch;
	ptFontBitMap->pucBuffer 	= g_tSlot->bitmap.buffer;

	ptFontBitMap->iNextOriginX 	= iPenX + g_tSlot->advance.x / 64;
	ptFontBitMap->iNextOriginY	= iPenY;

	return 0;
}

int FreeTypeInit(void)
{
	return RegisterFontOpr(&FreetypeFontOpr);
}
