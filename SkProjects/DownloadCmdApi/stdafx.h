// stdafx.h : �i�b�����Y�ɤ��]�t�зǪ��t�� Include �ɡA
// �άO�g�`�ϥΫo�ܤ��ܧ�
// �M�ױM�� Include �ɮ�
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ���T�w�q������ CString �غc�禡

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // �q Windows ���Y�ư����`�ϥΪ�����
#endif

#include <afx.h>
#include <afxwin.h>         // MFC �֤߻P�зǤ���
#include <afxext.h>         // MFC �X�R�\��
#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC �䴩�� Internet Explorer 4 �q�α��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // MFC �䴩�� Windows �q�α��
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <iostream>



// TODO: �b���Ѧұz���{���һݭn����L���Y
typedef signed   char             S08;
typedef unsigned char             U08;
typedef signed   short int        S16;
typedef unsigned short int        U16;
typedef signed   long int         S32;
typedef unsigned long int         U32;
typedef float                     F32;
typedef double                    D64;