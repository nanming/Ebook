
#include <encode_manager.h>


PT_EncodingOpr PT_EncodingOprSuppotedHead;

int AddFontOprForEncoding(PT_EncodingOpr ptEncodingOpr, PT_FontOpr ptFontOpr)
{
	PT_FontOpr PT_FontOprCpy;

	if (!ptEncodingOpr || !ptFontOpr)
	{
		return -1;
	}
	else
	{
		PT_FontOprCpy = malloc(sizeof (T_FontOpr));
		if (!PT_FontOprCpy)
		{
			return -1;
		}
		else
		{
			memcpy(PT_FontOprCpy, ptFontOpr, sizeof(T_FontOpr));
			PT_FontOprCpy->next = ptEncodingOpr->ptFontOprSupportedHead;
			ptEncodingOpr->ptFontOprSupportedHead = Pt_FontOprCpy;

			return 0;
		}
	}
}

int DeletFontOprForEncoding(PT_EncodingOpr ptEncodingOpr, PT_FontOpr ptFontOpr)
{
	PT_FontOpr ptmp = PT_EncodingOpr->PT_EncodingOprSuppotedHead;
	PT_FontOpr ptmppre;

	if (!ptEncodingOpr || ! ptFontOpr)
	{
		return -1;
	}
	else
	{
		if (ptmp->name == ptFontOpr->name)
		{
			PT_EncodingOpr->PT_EncodingOprSuppotedHead = ptmp->next;
			return 0;
		}
		ptmppre = ptmp;
		ptmp = ptmp->next;
		while (ptmp)
		{
			if (strcmp(ptmp->name, ptFontOpr->name) == 0)
			{
				ptmppre->next = ptemp->next;
				free(ptmp);
				return 0;
			}
			ptmppre = ptmp;
			ptmp = ptmp->next;
		}
		return -1;
	}
}

int RegisterEncodingOpr(PT_EncodingOpr ptEncodingOpr)
{
	PT_EncodingOpr ptmp;

	if (PT_EncodingOprSuppotedHead == NULL)
	{
		PT_EncodingOprSuppotedHead = ptEncodingOpr;
		ptEncodingOpr->next = NULL;

		return 0;
	}
	else
	{
		ptmp = PT_EncodingOprSuppotedHead;
		while(ptmp->next)
		{
			ptmp = ptmp->next;
		}
		ptmp->next = ptEncodingOpr;
		ptEncodingOpr->next = NULL;

		return 0;
	}


}

void showsupportendcoding(void)
{
	PT_EncodingOpr ptmp = PT_EncodingOprSuppotedHead;
	int i = 0;

	while (ptmp)
	{
		printf("%d: %s\n", i++, ptmp->name);
		ptmp = ptmp->next;
	}
}

int EncodingInit(void)
{
	int iError;

	iError = AsciiEncodingInit();
	if (iError < 0)
	{
		printf("Ascii Encoding Init failed !\n");
		return -1;
	}

	iError = Utf16leEncodingInit();
	if (iError < 0)
	{
		printf("Utf-16le Encoding Init failed !\n");
		return -1;
	}

	iError = Utf16beEncodingInit();
	if (iError < 0)
	{
		printf("Utf-16be Encoding Init failed !\n");
		return -1;
	}

	iError = Utf8EncodingInit();
	if (iError < 0)
	{
		printf("Utf-8 Encoding Init failed !\n");
		return -1;
	}

}

PT_EncodingOpr SelectEncodOprforFile(unsigned char *buffhead)
{
	PT_EncodingOpr ptmp;

	ptmp = PT_EncodingOprSuppotedHead; 	
	while(ptmp)
	{
		if (ptmp->isSupport(buffhead) == 1)
		{
			return ptmp;
		}
		else
		{
			ptmp = ptmp->next;
		}
	}
	return NULL;
}






