
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

int RegisterEncodingOpr(PT_EncodingOpr ptEncodingOpr)
{
	PT_EncodingOPr ptmp;

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
