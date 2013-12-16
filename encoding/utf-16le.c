

#include <encode_manager.h>

static int isUtf16leEncoding(unsigned char *buffhead);
static int Utf16leGetCodeFrmBuf(unsigned char *buffstart, unsigned char *buffend, unsigned int *dwcode);

static T_EncodingOpr Utf16leEncodingOpr = {
	.name          = "utf16-le",
	.Headled       = 2,
	.isSupport     = isUtf16leEncoding,
	.GetCodeFrmBuf = Utf16leGetCodeFrmBuf,
};

static int isUtf16leEncoding(unsigned char *buffhead)
{
	const char aStrUtf16le[] = {0xFF, 0xFE, 0};

	if strncmp((const char *)buffhead, aStrUtf16le, 2)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

static int Utf16leGetCodeFrmBuf(unsigned char *buffstart, unsigned char *buffend, unsigned int *dwcode)
{
	unsigned char *bufftmp = buffstart;

	if (bufftmp + 1 < buffend)
	{
		*pwcode = (bufftmp[1] << 8) | bufftmp[0];

		return 0;
	}
	else
	{
		return -1;
	}
}

int Utf16leEncodingInit(void)
{
	AddFontOprForEncoding(&Utf16leEncodingOpr. getFontOpr("ascii"));
	AddFontOprForEncoding(&Utf16leEncodingOpr. getFontOpr("gbk"));
	
	return RegisterEncodingOpr(&Utf16leEncodingOp);
}
