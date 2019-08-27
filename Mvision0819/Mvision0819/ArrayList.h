#pragma once
#include "stdafx.h"
class CMvision0819Dlg;

class CClientItem
{
public:
	CString m_strIp;
	SOCKET m_ClientSocket;
	HANDLE m_hThread;
	CMvision0819Dlg *m_pMainWnd;
	CClientItem() {
		m_ClientSocket = INVALID_SOCKET;
		m_hThread = NULL;
		m_pMainWnd = NULL;
	}
};