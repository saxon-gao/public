
// Mvision0819Dlg.h: 头文件
//

#pragma once
#include "afxwin.h"
#include "ArrayList.h"
#include "halconcpp.h" 
#include <iostream>
using namespace std;
using namespace HalconCpp;

BOOL socket_Select(SOCKET hSocket, DWORD nTimeOut, BOOL bRead);
DWORD WINAPI ListenThreadFunc(LPVOID Lparam);
DWORD WINAPI ClientThreadProc(LPVOID Lparam);
DWORD WINAPI ListenThreadFunc_tcp(LPVOID Lparam); //tcp发送线程
CString GetTime();

void ThreadFunc(LPVOID lpParam); //相机连接线程
// CMvision0819Dlg 对话框
class CMvision0819Dlg : public CDialogEx
{
// 构造
public:
	CMvision0819Dlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MVISION0819_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	BOOL StartServer(void);  //开启服务
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonServerDialog();
	afx_msg void OnBnClickedButtonstart();
public:
	BOOL EnableWindow(DWORD DlgId, BOOL bUsed);
	SOCKET m_SockListen;
	UINT m_ServicePort;
	HANDLE m_hListenThread;
	HANDLE m_hListenThread_tcp; //tcp发送
	HANDLE hThread_tcp;
	DWORD ThreadID_tcp;
	CArray<CClientItem> m_ClientArray;
	CEdit m_EditRevBox;
	void SetRevBoxText(CString strMsg);
	void RemoveClientFromArray(CClientItem in_item);
	void SendClientMsg(CString strMsg, CClientItem * WhoseItem);
private:
	BOOL equal(const CClientItem *p1, const CClientItem * p2);
	
	BOOL m_isServerOpen;
public:

	afx_msg void OnEnChangeEditsendbox();
	afx_msg void OnBnClickedButtonhide();

	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg void OnBnClickedButtonend();
	afx_msg void OnBnClickedButtonquit();
	afx_msg void OnBnClickedButtonsend();
	afx_msg void OnBnClickedButtonOpenCam();
protected:
	HANDLE hThread_cam;
	DWORD ThreadID_cam;
public:
	HObject  ho_Image_cam,ho_GrayImage_cam;
	HTuple m_HWindowID_cam;
	HTuple  hv_AcqHandle_cam;
	HTuple m_gao, m_kuan;
	HTuple m_FGHandle, m_ImageWidth, m_ImageHeight;
	HTuple  hv_RevInfo_cam, hv_RevInfoValues_cam, hv_DeviceInfo_cam, hv_DeviceInfoValues;
	afx_msg void OnBnClickedButtonSavaPic();
	void readShowValue(int mode);
	afx_msg void OnBnClickedButtonWall();
	afx_msg void OnBnClickedButtonGround();

	int num10, num13, num14;
	double  num11, num12;
};
