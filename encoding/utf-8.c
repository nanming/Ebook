
#include <encode_manage.h>

static int isUtf8Encoding(unsigned char *buffhead);
static int Utf8GetCodeFrmBuf(unsigned char *buffstart, unsigned char *buffend, unsigned int *dwcode);

static T_EncodingOpr Utf8EncodingOpr = {
	.name		= "utf-8",
	.Headlen	= 2,
	.isSupport	= isUtf8Encoding,
	.GetCodeFrmBuf	= Utf8GetCodeFrmBuf,
};

static int isUtf8Encoding(unsigned char *buffhead)
{
	const char aStrUtf8[] = {0xEF,0xBB,0xBF, 0};

	if (strncmp((const char *)buffhead, aStrUtf8, 3))
	{
		return 1;
	}		
	else
	{
		return 0;
	}
}

static int GetWordNum(unsigned char *addr)
{
	unsigned char tmp = 0x80;
	unsigned num = 0;
	unsigned char c = *addr;

	while(c & tmp)
	{
		num++;
		tmp >> 1;
	}
	
	if(num == 1)
	{
		return -1;
	}
	else
	{
		return num;
	}
}

static int Utf8GetCodeFrmBuf(unsigned char *buffstart, unsigned char *buffend, unsigned int *dwcode)
{
	unsigned char *bufftmp = buffstart;
	unsigned char WorNum;
	unsigned int val;
	unsigned int i;

	WorNum = GetWordNum(bufftmp);
	val = bufftmp[0];
#if 0
	if(WorNum < 0)
	{
		return -1;
	}

	if (WorNum == 0)
	{
		*dwcode = (unsigned int)bufftmp;
		return 0;
	}
	else if(WorNum == 2)
	{
		*dwcode = ((bufftmp[0] & 0x1f) << 6) | (bufftmp[1] & 0x3f);
		return 0;
	}	
	else if(WorNum == 3)
	{
		*dwcode = ((bufftmp[0] & 0x0f) << 12) | ((bufftmp[1] & 0x3f) << 6) | (bufftmp[2] & 0x3f);
		return 0;
	}
	else if (WorNum == 4)
	{
		*dwcode = ((bufftmp[0] & 0x07) << 18) | ((bufftmp[1] & 0x3f) << 12) | ((bufftmp[1] & 0x3f) << 6) | (bufftmp[2] & 0x3f);
		return 0;
	}
	else
	{
		return -1;
	}
#endif		
	if ((bufftmp + WorNum) > buffend)
	{
		return -1;
	}

	if (WorNum == 0)
	{
		*dwcode = dwsum;
		return 0;
	}
	else
	{
		val = dwsum << WorNum;
		val = dwsum >> WorNum;

		for(i = 1; i < WorNum; i++)
		{
			val = val << 6;
			val += (bufftmp[i] & 0x3f);
		}
		*dwcode = val;
		return 0;
	}
	
}

int Utf8EncodingInit(void)
{
	AddFontOprForEncoding(&Utf8EncodingOpr, getFontOpr("freetype");
	AddFontOprForEncoding(&Utf8EncodingOpr, getFontOpr("ascii");
	
	return RegisterEncodingOpr(&Utf8EncodingOpr);

}
