// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#include <afx.h>
#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT


//增加 STL
#include <vector>
#include <string>
#include <map>
#include <queue>

// 增加基本库引用
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <iostream>
using namespace std;

#define EQUATOR_LENGTH 40074335.7091 //米
#define ASSERT_FALSE_AND_RETURN			{assert(false);return;}
#define ASSERT_FALSE_AND_RETURN_FALSE 	{assert(false);return false;}
#define ASSERT_FALSE_AND_RETURN_ERROR 	{assert(false);return -1;}
#define ASSERT_FALSE_AND_RETURN_ZERO 	{assert(false);return 0;}
#define ASSERT_FALSE_AND_RETURN_NULL 	{assert(false);return NULL;}
#define ASSERT_FALSE_AND_CONTINUE		{assert(false);continue;}
#define ASSERT_FALSE_AND_RETURN_SPACE 	{assert(false);return "";}
#define MAKEVALIDATE(minNum,maxNum)		{if(minNum > maxNum){double tmp=minNum;minNum=maxNum;maxNum=tmp;}}
#define PI 3.14159266
// #define _USE_LINUX_

#include <sstream> 
inline string _szf_format_string(string sIn)
{
	int nFind = sIn.find(".");
	if (nFind != string::npos)
	{
		while (sIn.substr(sIn.length() - 1) == "0")
		{
			sIn = sIn.substr(0, sIn.length() - 1);
		}
		if (sIn.substr(sIn.length() - 1) == ".")
			sIn = sIn.substr(0, sIn.length() - 1);
	}
	return sIn;
}
inline string _szf_format_double(double dIn)
{
	ostringstream  oIn;
	char *M_show;
	char buf[40];
	sprintf(buf, "%.15lg", dIn);
	M_show = (char *)buf;
	oIn << M_show;//40
	string sIn = oIn.str();
	return sIn;
}
