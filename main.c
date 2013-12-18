/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  Ebook main function
 *
 *        Version:  1.0
 *        Created:  2013年12月14日 09时59分24秒
 *       Revision:  none
 *       Compiler:  arm-linux-gcc
 *
 *         Author:  YOUR NAME (nanming), 
 *        Company:  
 *
 * =====================================================================================
 */

#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main (int argc, char **argv)
{
	int iError;
	char blist;
        unsigned int dwFontsize;
	char acFreetypeFile[128];
	char acHzkFile[128];
	char acDisplayDevice[128];
	char Opr;

	while ((iError = getopt(argc, argv, "ls:f:h:d:")) != -1)
	{
		switch(iError)
		{
			case 'l':
				blist = 1;
				break;
			case 's':
				dwFontsize = strtoul(optarg, NULL, 0);
				break;
			case 'f':
				strncpy(acFreetypeFile, optarg, 128);
				acFreetypeFile[127] = '\0';
				break;
			case 'h':
				strncpy(acHzkFile, optarg, 128);
				acHzkFile[127] = '\0';
				break;
			case 'd':
				strncpy(acDisplayDevice, optarg, 128);
				acDisplayDevice[127] = '\0';
				break;
			default:
				printf("Usage: %s [-s size] [-f font_file] [-h hzk_file] [-d display] <text_files> \n", argv[0]);	
				printf("Usage: %s -l\n", argv[0]);
				return -1;
				break;
		}		
	}

	iError = DisplayInit();
	if (iError < 0)
	{
		printf("DisplayInit error..\n");
		return -1;
	}

	iError = FontInit();
	if (iError < 0)
	{
		printf("FontInit error..\n");
		return -1;
	}
	iError = EncodingInit();
	if (iError < 0)
	{
		printf("EncodingInit error..\n");
		return -1;
	}
	iError = OpenTextFile(text_files);
	if (iError < 0)
	{
		printf("Open file failed !\n");
		return -1;
	}
	iError = SetTextDetail(font_file, hzk_file, size);
	if (iError < 0)
	{
		printf("Set Text Detail failed !\n");
		return -1;
	}
	iError = SelectDisplayDevice(display);
	if (iError < 0)
	{
		printf("Select Display Device failed!\n");
		return -1;
	}

	if (blist)
	{
		printf("Support display devices\n");
		showsupportdisplay();

		printf("Support font types\n");
		showsupportfont();

		printf("Support Encoding types\n");
		showsupportendcoding();
	}
	iError = shownextpage();
	if (iErros < 0)
	{
		printf(" show the first page error \n");
		return -1;
	}

	while(1)
	{
		printf("enter the show process\n");
		do {
			Opr = getchar();
		} while ((Opr != 'n') && (Opr != 'u') && (Opr != 'q'));

		if (Opr == 'n')
		{
			shownextpage();
		}
		if (Opr == 'u')
		{
			showprepage();
		}
		if (Opr == 'q')
		{
			return 0;
		}
	}
	return 0;
}



