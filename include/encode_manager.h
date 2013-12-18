/*
 * =====================================================================================
 *
 *       Filename:  encode_manager.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年12月16日 09时03分20秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */
#ifndef _ENCODE_MANAGER_H
#define _ENCODE_MANAGER_H

typedef struct EncodingOpr {
	char *name;
	int HeadLen;
	int (*isSupport)(unsigned char *buffhead);
	int (*GetCodeFrmBuf)(unsigned char *buffstart, unsigned char *buffend, unsigned int *dwcode);
	PT_FontOpr ptFontOprSupportedHead;
	struct EncodingOpr *next;
}T_EncodingOpr, *PT_EncodingOpr;

int AddFontOprForEncoding(PT_EncodingOpr ptEncodingOpr, PT_FontOpr, ptFontOpr);
int RegisterEncodingOpr(PT_EncodigOpr ptEncodingOpr);

int EncodingInit(void);

int Utf16leEncodingInit(void);
int Utf16BeEncodingInit(void);
int AsciiEncodingInit(void);
int Utf8EncodingInit(void);
PT_EncodingOpr SelectEncodOprforFile(unsigned char *buffhead);


