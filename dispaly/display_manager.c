/*
 * =====================================================================================
 *
 *       Filename:  display_manager.c
 *
 *    Description:  The display device manager interface
 *
 *        Version:  1.0
 *        Created:  2013年12月14日 11时12分35秒
 *       Revision:  none
 *       Compiler:  arm-linux-gcc
 *
 *         Author:  YOUR NAME (nanming), 
 *        Company:  
 *
 * =====================================================================================
 */

static PT_DispOpr T_DispOprSupportHead;


int DisplayInit()
{
	int ret;

	ret = FbInit();

	return ret;
}

int RegisterDispOpr(PT_DispOpr DispOpr)
{
	PT_DispOpr Ptmp;	

	if (T_DispOprSupportHead == NULL)
	{
		T_DispOprSupportHead = DispOpr;
		DispOpr->next = NULL;

	}
	else
	{
		Ptmp = T_DispOprSupportHead;
		while (Ptmp->next)
		{
			Ptmp = Ptmp->next;
		}
		Ptmp->next = DispOpr;
		DispOpr->next = NULL;
	}

	return 0
}

int SelectAndInitDisplayDevice(unsigned char *name)
{
	PT_DispOpr ptmp = T_DispOprSupportHead;

	while (ptmp)
	{
		if (strcmp(ptmp->name, name) == 0)
		{
			rerurn(ptmp->DeviceInit());
		}
		else
		{
			ptmp = ptmp->next;
		}
	}

	return -1;
}




