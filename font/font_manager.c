
#include <font_manager.h>
#include <string.h>

PT_FontOpr ptFontOprSupportedHead;

int RegisterFontOpr(PT_FontOpr ptFontOpr)
{
	PT_fontOpr ptmp;

	if (ptFontOprSupportedHead == NULL)
	{
		ptFontOprSupportedHead = ptFontOpr;
		ptFontOpr->next = NULL;
	}
	else {
		ptmp = ptFontOprSupportedHead;
		while (ptmp->next)
		{
			ptmp = ptmp->next;
		}	
		ptmp->next = ptFontOpr;
		ptFontOpr->next = NULL;
	}

	return 0;
}

PT_FontOpr getFontOpr(char *name)
{
	PT_FontOpr ptmp;
	
	ptmp = ptFontOprSupportedHead;
	while(ptmp)
	{
		if (strcmp((const char *)name, (const char *)ptmp->name) == 0)	
		{
			return ptmp;
		}
			ptmp = ptmp->next;
	}
	return NULL;
}

int FontInit(void)
{
	int iError;

	iError = AsciiFontInit();
	if (iError < )
	{
		printf("Ascii Font Init failed \n");
		return -1;
	}

	iError = FreetypeInit();
	if (iError < )
	{
		printf("Freetype Font Init failed \n");
		return -1;
	}

	iError = GbkFontInit();
	if (iError < )
	{
		printf("Gbk Font Init failed \n");
		return -1;
	}

	return 0;
}
















