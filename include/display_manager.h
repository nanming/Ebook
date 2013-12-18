/*
 * =====================================================================================
 *
 *       Filename:  display_manager.h
 *
 *    Description:  display_manager head file
 *
 *        Version:  1.0
 *        Created:  2013年12月14日 11时21分28秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef _DISPLAY_MANAGER_H
#define _DISPLAY_MANAGER_H


typedef struct DispOpr{
	char *name;
	int iXres;
	int iYres;
	int iBpp;
	int (*DeviceInit)(void);
	int (*ShowPixl)(int iPenx, int iPeny, unsigned int dwColor);
	int (*ClearScreen)(unsigned int dwBackColor);
	struct DispOpr *next;
} T_DispOpr, *PT_DispOpr;

int SelectAndInitDisplayDevice(unsigned char *name);
