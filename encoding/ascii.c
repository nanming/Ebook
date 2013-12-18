
#include <encode_manager.h>

static int isAsciiEncoding(unsigned char *buffhead);
static int AsciiGetCodeFrmBuf(unsigned char *buffstart, unsigned char *buffend, unsigned int *dwcode);

static T_EncodingOpr AsciiEncodingOpr = {
	.name = "asicii",
	.HeadLen = 0,
	.isSupport = isAsciiEncoding,
	.GetCodeFrmBuf = AsciiGetCodeFrmBuf,
};

static int isAsiciiEncoding(unsigned char *buffhead)
{
	const char aStrUtf8[] = {0xEF, 0xBB, 0xBF, 0};
	const char aStrUtf16le[] = {0xFF, 0xFE, 0};
	const char aStrUtf16be[] = {0xFE, 0xFF, 0};

	if (strncmp((const char *)buffhead, aStrUtf8, 3))
	{
		return 0;
	}
	else if (strncmp((const char *)buffhead, aStrUtf16le, 2))
	{
		return 0;
	}
	else if (strncmp((const char *)buffhead, aStrUtf16be, 2))
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

static int AsciiGetCodeFrmBuf(unsigned char *buffstart, unsigned char *buffend, unsigned int *dwcode)
{
	unsigned char *bufftmp = buffstart;
	unsigned char c = *bufftmp;

	if ((bufftmp < buffend) && (c < (unsigned char)0x80))
	{
		*dwcode = (unsigned int)c;
		return 1;
	}
	if ((bufftmp < buffend) && ( c > (unsigned char)0x80))
	{
		*dwcode = (bufftmp[1] << 8) | bufftmp[0];
		return 2;
	}
	if (bufftmp < buffend)
	{
		*dwcode = (unsigned int)c;
		return 1;
	}
	else
	{
		return 0;
	}
}


int AsciiEncodingInit(void)
{
	AddFontOprForEncoding(&AsciiEncodingOpr, getFontOpr("freetype"));
	AddFontOprForEncoding(&AsciiEncodingOpr, getFontOpr("ascii"));
	AddFontOprForEncoding(&AsciiEncodingOpr, getFontOpr("gbk"));

	return RegisterEncodingOpr(&AsciiEncodInit);
}



