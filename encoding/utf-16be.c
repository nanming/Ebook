

#include <encode_manager.h>

static int isUtf16beEncoding(unsigned char *buffhead);
static int Utf16beGetCodeFrmBuf(unsigned char *buffstart, unsigned char *buffend, unsigned int *dwcode);

static T_EncodingOpr Utf16beEncodingOpr = {
	.name          = "utf16-be",
	.Headlen       = 2,
	.isSupport     = isUtf16beEncoding,
	.GetCodeFrmBuf = Utf16beGetCodeFrmBuf,
};

static int isUtf16beEncoding(unsigned char *buffhead)
{
	const char aStrUtf16be[] = {0xFE, 0xFF, 0};

	if (strncmp((const char *)buffhead, aStrUtf16be, 2))
	{
		return 	1;
	}
	else
	{
		return 0;
	}

}

static int Utf16beGetCodeFrmBuf(unsigned char *buffstart, unsigned char *buffend, unsigned int *dwcode)
{
	unsigned char *bufftmp = buffstart;

	if ((bufftmp + 1) < buffend)
	{
		*dwcode = (bufftmp[0] << 8) | bufftmp[1];

		return 0;
	}
	else
	{
		return -1;
	}
}

int Utf16beEncodingInit(void)
{
	AddFontOprForEncoding(&Utf16beEncodingOpr, getFontOpr("ascii");
	AddFontOprForEncoding(&Utf16beEncodingOpr, getFontOpr("gbk");

	return RegisterEncodingOpr(&Utf16beEncodingOpr);
}
