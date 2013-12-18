
#include <display_manager.h>
#include <font_manager.h>
#include <encode_manager.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>



static struct stat bstat;
static unsigned char *textfilemem;
static unsigned char *textfilememend;
static unsigned char *TextFirstPosAtFile;
static PT_EncodingOpr fileEncodingOpr;

int OpenTextFile(char *Textfilename)
{
	int fd_text;
	int ret;

	fd_text = open(Textfilename, O_RDONLY);
	if (fd_text < 0)
	{
		printf("opne text file failed !\n");
		return -1;
	}

	if ((fstat(fd_text, &bstat) == -1))
	{
		printf ("get text file stat failed!\n");
		return -1;
	}

	ret = (unsigned char *)mmap(NULL, bstat.st_size, PROT_READ, MAP_SHARED, fd_text, 0);
	if (ret == (unsigned char *)-1)
	{
		printf("mmap text file failed\n");
		return -1;
	}

	textfilememend = textfilemem + bstat.st_size;

	fileEncodingOpr = SelectEncodOprforFile(textfilemem);

	if (fileEncodingOpr)
	{
		TextFirstPosAtFile = textfilemem + fileEncodingOpr->HeadLen;
		return 0;
	}
	else
	{
		return -1;
	}
}

int SetTextDetail(unsigned char *font_file, unsigned char *hzk_file, unsigned int size)
{
	PT_FontOpr ptmp;
	int ret;
	
	ptmp = fileEncodingOpr->ptFontOprSupportedHead;
	while (ptmp)
	{
		if (strcmp(ptmp->name, "ascii") == 0)
		{
			iError = ptmp->FontInit(NULL, size);
		}
		else if (strcmp(ptmp->name, "freetype") == 0)
		{
			iError = ptmp->FontInit(font_file, size);
		}
		else if (strcmp(ptmp->name, "gbk") == 0)
		{
			iError = ptmp->FontInit(hzk_file, size);
		}
		else
		{
			DeletFontOprForEncoding(fileEncodingOpr, ptmp);
		}
		ptmp = ptmp->next;

		if (iError < 0)
		{
			printf("Init the font detail failed\n");
			DeletFontOprForEncoding(fileEncodingOpr, ptmp);
			ret = -1;
		}
		else
		{
			ret = 0;
		}
	}

	return ret;
}











