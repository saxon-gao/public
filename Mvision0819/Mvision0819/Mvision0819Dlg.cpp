
// Mvision0819Dlg.cpp: 实现文件
//

#include "stdafx.h"
#include "Mvision0819.h"
#include "Mvision0819Dlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

volatile BOOL m_bRun;
volatile BOOL m_bShowFlag; //打开相机
CString strMsg;
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据

	enum { IDD = IDD_ABOUTBOX };


	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMvision0819Dlg 对话框



CMvision0819Dlg::CMvision0819Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MVISION0819_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_isServerOpen = FALSE;
	m_hListenThread = NULL;
	m_SockListen = NULL;
}

void CMvision0819Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDITGETBOX, m_EditRevBox);
}

BEGIN_MESSAGE_MAP(CMvision0819Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_SERVER_DIALOG, &CMvision0819Dlg::OnBnClickedButtonServerDialog)
	ON_BN_CLICKED(IDC_BUTTONSTART, &CMvision0819Dlg::OnBnClickedButtonstart)
	ON_BN_CLICKED(IDC_BUTTONEND, &CMvision0819Dlg::OnBnClickedButtonend)
	ON_BN_CLICKED(IDC_BUTTONQUIT, &CMvision0819Dlg::OnBnClickedButtonquit)
	ON_BN_CLICKED(IDC_BUTTONSEND, &CMvision0819Dlg::OnBnClickedButtonsend)
	ON_BN_CLICKED(IDC_BUTTON_OPEN_CAM, &CMvision0819Dlg::OnBnClickedButtonOpenCam)
	ON_BN_CLICKED(IDC_BUTTON_SAVA_PIC, &CMvision0819Dlg::OnBnClickedButtonSavaPic)
	ON_BN_CLICKED(IDC_BUTTON_WALL, &CMvision0819Dlg::OnBnClickedButtonWall)
	ON_BN_CLICKED(IDC_BUTTON_GROUND, &CMvision0819Dlg::OnBnClickedButtonGround)
END_MESSAGE_MAP()


// CMvision0819Dlg 消息处理程序

BOOL CMvision0819Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	EnableWindow(IDC_BUTTONSEND, FALSE);
	EnableWindow(IDC_BUTTONEND, FALSE);
	SetDlgItemText(IDC_EDITPORT, _T("8000"));
	GetDlgItem(IDC_EDITPORT)->SetFocus();

	SetDlgItemText(IDC_EDIT5, _T("80"));
	SetDlgItemText(IDC_EDIT6, _T("0.65"));
	SetDlgItemText(IDC_EDIT7, _T("1"));
	SetDlgItemText(IDC_EDIT8, _T("30000"));
	SetDlgItemText(IDC_EDIT9, _T("80000"));

	SetDlgItemText(IDC_EDIT10, _T("133"));
	SetDlgItemText(IDC_EDIT11, _T("0.65"));
	SetDlgItemText(IDC_EDIT12, _T("1"));
	SetDlgItemText(IDC_EDIT13, _T("800000"));
	SetDlgItemText(IDC_EDIT14, _T("1200000"));

	//hThread_tcp = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadFunc_tcp, NULL, 0, &ThreadID_tcp);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMvision0819Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMvision0819Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMvision0819Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMvision0819Dlg::OnBnClickedButtonServerDialog()
{

}


void CMvision0819Dlg::OnBnClickedButtonstart()
{
	

	// TODO: 在此添加控件通知处理程序代码
	m_ServicePort = GetDlgItemInt(IDC_EDITPORT);
	if (m_ServicePort <= 1024 || m_ServicePort > 65535)
	{
		AfxMessageBox(_T("请输入合适端口"));
		SetDlgItemText(IDC_EDITPORT, _T(""));
		GetDlgItem(IDC_EDITPORT)->SetFocus();
		return;
	}
	StartServer();
	m_isServerOpen = TRUE;
	OnEnChangeEditsendbox(); //如果发送框中原有内容，则使发送按钮有效
}

BOOL CMvision0819Dlg::StartServer(void)
{
	m_hListenThread = CreateThread(NULL, 0, ListenThreadFunc, this, 0, 0);
	m_hListenThread_tcp = CreateThread(NULL, 0, ListenThreadFunc_tcp, this, 0, 0);
	if (m_hListenThread == NULL)
	{
		return FALSE;
	}
	else {
		return TRUE;
	}
}
void CMvision0819Dlg::OnEnChangeEditsendbox()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	CString strMsg;
	GetDlgItemText(IDC_EDITSENDBOX, strMsg);
	if (strMsg == _T("") || !m_isServerOpen)
	{
		EnableWindow(IDC_BUTTONSEND, FALSE);
	}
	else {
		EnableWindow(IDC_BUTTONSEND, TRUE);
	}
}
//等待客户端的连接，连接上了就把客户端信息加入链表
DWORD WINAPI ListenThreadFunc(LPVOID Lparam)
{
	//以下代码为初始化网络
	WSADATA wsaData;
	int nRet;
	if ((nRet = WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0) {
		
		AfxMessageBox(_T("WSAStartup failed"));
		exit(0);
	}
	

	CMvision0819Dlg * pServer = (CMvision0819Dlg *)Lparam;;
	if (INVALID_SOCKET == (pServer->m_SockListen = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)))
	{
		AfxMessageBox(_T("建立socket失败"));
		return 0;
	}
	sockaddr_in service;
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = htonl(INADDR_ANY);
	service.sin_port = htons(pServer->m_ServicePort);
	if (0 != bind(pServer->m_SockListen, (sockaddr *)&service, sizeof(sockaddr_in)))
	{
		AfxMessageBox(_T("绑定端口失败"));
		return 0;
	}
	if (0 != listen(pServer->m_SockListen, 5))
	{
		AfxMessageBox(_T("监听端口失败"));
		return 0;
	}

	//提示建立socket成功
	pServer->EnableWindow(IDC_BUTTONEND, TRUE);
	//	pServer->EnableWindow(IDC_BUTTONSEND,TRUE);
	pServer->EnableWindow(IDC_BUTTONSTART, FALSE);
	pServer->SetRevBoxText(_T("服务启动成功,开始监听端口"));
	//进入循环，监听端口
	while (TRUE)
	{
		if (socket_Select(pServer->m_SockListen, 100, TRUE))
		{
			sockaddr_in clientAddr;
			int iLen = sizeof(sockaddr_in);
			SOCKET accSock = accept(pServer->m_SockListen, (struct sockaddr *)&clientAddr, &iLen);
			if (accSock == INVALID_SOCKET)
			{
				continue;
			}
			//将节点加入链中
			CClientItem tItem;
			tItem.m_ClientSocket = accSock;
			tItem.m_strIp = inet_ntoa(clientAddr.sin_addr); //IP地址
			tItem.m_pMainWnd = pServer;
			int idx = pServer->m_ClientArray.Add(tItem); //idx是第x个连接的客户端
			tItem.m_hThread = CreateThread(NULL, 0, ClientThreadProc, //创建一个线程并挂起:CREATE_SUSPENDED
				&(pServer->m_ClientArray.GetAt(idx)), CREATE_SUSPENDED, NULL);
			pServer->m_ClientArray.GetAt(idx).m_hThread = tItem.m_hThread;
			//等把hThread加入了节点，才开始执行线程，如下
			ResumeThread(tItem.m_hThread);
			pServer->SetRevBoxText(tItem.m_strIp + _T("上线"));
			Sleep(100);
		}
	}
}
#define MAX_BUFF 256
DWORD WINAPI ClientThreadProc(LPVOID Lparam) { //利用异步IO模型循环读取socket内的信息，并发送给各个用户
	USES_CONVERSION;
	//CString strMsg;
	CClientItem ClientItem = *(CClientItem *)Lparam;
	while (TRUE)
	{
		if (socket_Select(ClientItem.m_ClientSocket, 100, TRUE))
		{
			char szRev[MAX_BUFF] = { 0 };
			int iRet = recv(ClientItem.m_ClientSocket, szRev, sizeof(szRev), 0);
			if (iRet > 0)
			{
				strMsg = A2T(szRev); //中文出现乱码，英文正常
//				strMsg.Format(_T("%s"),szRev); //这么写连英文都不对了
				ClientItem.m_pMainWnd->SetRevBoxText(ClientItem.m_strIp + _T(">>") + strMsg);
				ClientItem.m_pMainWnd->SendClientMsg(strMsg, &ClientItem);
			}
			else {
				strMsg = ClientItem.m_strIp + _T(" 已离开");
				ClientItem.m_pMainWnd->RemoveClientFromArray(ClientItem);
				ClientItem.m_pMainWnd->SetRevBoxText(strMsg);
				break;
			}
		}
	}
	return 0;
}



DWORD WINAPI ListenThreadFunc_tcp(LPVOID Lparam)
{
	USES_CONVERSION;


	CMvision0819Dlg * pServer = (CMvision0819Dlg *)Lparam;;
	while (TRUE)
	{
		//if (socket_Select(ClientItem.m_ClientSocket, 100, TRUE))
		{

			if (strMsg == "1")
			{
				pServer->OnBnClickedButtonSavaPic();
				//pServer->OnBnClickedButton2();//地面
			
				strMsg = "0";
			}
			else if (strMsg == "2")
			{
				pServer->OnBnClickedButtonSavaPic();
				//pServer->OnBnClickedButton3();//墙壁
				
				strMsg = "0";
			}
			else if (strMsg == "3")
			{
				pServer->OnBnClickedButtonSavaPic();
				//pServer->OnBnClickedButton4();//
				
				strMsg = "0";
			}

			Sleep(100);
		}
	}

}


void CMvision0819Dlg::SendClientMsg(CString strMsg, CClientItem * pWhoseItem)
{
	USES_CONVERSION;
	char szBuf[256] = { 0 };
	strcpy_s(szBuf, 256, T2A(strMsg));
	for (int i = 0; i < m_ClientArray.GetCount(); i++)
	{
		if (!pWhoseItem || !equal(pWhoseItem, &(m_ClientArray.GetAt(i))))
		{
			send(m_ClientArray.GetAt(i).m_ClientSocket, szBuf, 256, 0);
		}
	}
}

BOOL CMvision0819Dlg::equal(const CClientItem * p1, const CClientItem * p2)
{
	if (p1->m_ClientSocket == p2->m_ClientSocket && p1->m_hThread == p2->m_hThread && p1->m_strIp == p2->m_strIp)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//客户端下线，从链表移除该节点
void CMvision0819Dlg::RemoveClientFromArray(CClientItem in_item) {
	for (int idx = 0; idx < m_ClientArray.GetCount(); idx++)
	{
		if (in_item.m_ClientSocket == m_ClientArray[idx].m_ClientSocket &&
			in_item.m_hThread == m_ClientArray[idx].m_hThread &&
			in_item.m_strIp == m_ClientArray[idx].m_strIp &&
			in_item.m_pMainWnd == m_ClientArray[idx].m_pMainWnd)
		{
			m_ClientArray.RemoveAt(idx);
		}
	}
	return;
}


//设置文本框文本
void CMvision0819Dlg::SetRevBoxText(CString strMsg) {
	m_EditRevBox.SetSel(-1, -1);
	m_EditRevBox.ReplaceSel(GetTime() + _T("\r\n  ") + strMsg + _T("\r\n"));
}
BOOL CMvision0819Dlg::EnableWindow(DWORD DlgId, BOOL bUsed)
{
	return GetDlgItem(DlgId)->EnableWindow(bUsed);

}

BOOL socket_Select(SOCKET hSocket, DWORD nTimeOut, BOOL bRead) {
	FD_SET fdset;
	timeval tv;
	FD_ZERO(&fdset);
	FD_SET(hSocket, &fdset);
	nTimeOut = nTimeOut > 1000 ? 1000 : nTimeOut;
	tv.tv_sec = 0;
	tv.tv_usec = nTimeOut;
	int iRet = 0;
	if (bRead)
	{
		iRet = select(0, &fdset, NULL, NULL, &tv);
	}
	else
	{
		iRet = select(0, NULL, &fdset, NULL, &tv);
	}
	if (iRet <= 0)
	{
		return FALSE;
	}
	else if (FD_ISSET(hSocket, &fdset))
	{
		return TRUE;
	}
	return FALSE;
}

CString GetTime() {
	SYSTEMTIME time;
	CString strTime;
	GetLocalTime(&time);
	strTime.Format(_T("%d%02d%02d %02d:%02d"), time.wYear, time.wMonth, time.wDay,
		time.wHour, time.wMinute);
	return strTime;
}




void CMvision0819Dlg::OnBnClickedButtonend()
{
	// TODO: 在此添加控件通知处理程序代码
	int AllClient = m_ClientArray.GetCount();
	for (int idx = 0; idx < AllClient; idx++)
	{
		closesocket(m_ClientArray.GetAt(idx).m_ClientSocket);
	}
	m_ClientArray.RemoveAll();
	closesocket(m_SockListen);
	EnableWindow(IDC_BUTTONSEND, FALSE);
	EnableWindow(IDC_BUTTONSTART, TRUE);
	EnableWindow(IDC_BUTTONEND, FALSE);
	SetRevBoxText(_T("停止监听端口"));
	m_isServerOpen = FALSE;
}


void CMvision0819Dlg::OnBnClickedButtonquit()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_isServerOpen)
	{
		OnBnClickedButtonend();
	}
	SendMessage(WM_CLOSE);
	// TODO: 在此添加控件通知处理程序代码
}


void CMvision0819Dlg::OnBnClickedButtonsend()
{
	// TODO: 在此添加控件通知处理程序代码
	// TODO: 在此添加控件通知处理程序代码
	CString strMsg;
	GetDlgItemText(IDC_EDITSENDBOX, strMsg);
	SendClientMsg(strMsg, NULL);
	SetRevBoxText(_T("服务器>>") + strMsg);
	SetDlgItemText(IDC_EDITSENDBOX, _T(""));
}



void CMvision0819Dlg::OnBnClickedButtonOpenCam()
{
	// TODO: 在此添加控件通知处理程序代码
	// TODO: 在此添加控件通知处理程序代码
	CWnd * pWnd = AfxGetApp()->GetMainWnd();
	if (m_bShowFlag) {
		m_bRun = TRUE;
	}
	else {
		hThread_cam = CreateThread(NULL,
			0,
			(LPTHREAD_START_ROUTINE)ThreadFunc, //
			this, //传入主窗口指针
			0,
			&ThreadID_cam);
	}
}


void ThreadFunc(LPVOID pParam)
{
	CMvision0819Dlg * pMainWindow;
	pMainWindow = (CMvision0819Dlg *)pParam; //强制转化为主窗口指针
	HTuple HWindowID_cam;
	HTuple  hv_DeviceInfoValues, hv_Length;
	CRect Rect;
	CWnd * pWnd = pMainWindow->GetDlgItem(IDC_STATIC_CAM);
	HWindowID_cam = (Hlong)pWnd->m_hWnd;
	pWnd->GetWindowRect(&Rect);
	OpenWindow(0, 0, Rect.Width(), Rect.Height(), HWindowID_cam, "visible", "", &(pMainWindow->m_HWindowID_cam));
	//显示相机捕捉的图像





	InfoFramegrabber("GigEVision2", "revision", &(pMainWindow->hv_RevInfo_cam), &(pMainWindow->hv_RevInfoValues_cam));
	//
	//check your installed cameras for device:
	InfoFramegrabber("GigEVision2", "device", &(pMainWindow->hv_DeviceInfo_cam), &(hv_DeviceInfoValues));
	//
	//open camera with default settings and first device:
	TupleLength(hv_DeviceInfoValues, &hv_Length);
	if ((hv_DeviceInfoValues[0]) == NULL)
	{
		AfxMessageBox(_T("建立socket失败"));
		return;
	}
	else {

	OpenFramegrabber("GigEVision2", 1, 1, 0, 0, 0, 0, "default", -1, "default", -1,
		"false", "default", HTuple(hv_DeviceInfoValues[0]), 0, -1, &(pMainWindow->hv_AcqHandle_cam));
	//

	GrabImageStart(pMainWindow->hv_AcqHandle_cam, -1);
	ClearWindow(pMainWindow->m_HWindowID_cam);
	GrabImage(&(pMainWindow->ho_Image_cam), pMainWindow->hv_AcqHandle_cam);
	GetImagePointer1((pMainWindow->ho_Image_cam), NULL, NULL, &(pMainWindow->m_ImageWidth), &(pMainWindow->m_ImageHeight));
	SetPart(pMainWindow->m_HWindowID_cam, 0, 0, pMainWindow->m_ImageHeight - 1, pMainWindow->m_ImageWidth - 1);
	m_bShowFlag = TRUE;//设置运行状态
	m_bRun = TRUE;
	while (m_bShowFlag) {
		if (m_bRun) {
			GrabImageAsync(&(pMainWindow->ho_Image_cam), pMainWindow->hv_AcqHandle_cam, -1);
			DispObj(pMainWindow->ho_Image_cam, pMainWindow->m_HWindowID_cam);
			//	(pMainWindow->ho_Image_1) <<( pMainWindow->capture);

			Sleep(50);
		}
	}
	ClearWindow(pMainWindow->m_HWindowID_cam);
	CloseFramegrabber(pMainWindow->hv_AcqHandle_cam);
	CloseWindow(pMainWindow->m_HWindowID_cam);


	}
	ExitThread(0);
}


void CMvision0819Dlg::OnBnClickedButtonSavaPic()
{
	// TODO: 在此添加控件通知处理程序代码

	HTuple   hv_i;


	for (hv_i = 1; hv_i <= 2; hv_i += 1)
	{

		WriteImage(ho_Image_cam, "tiff", 0, "text_tem2.jpg");

	}
}



void CMvision0819Dlg::OnBnClickedButtonWall()
{
	// TODO: 在此添加控件通知处理程序代码
	HObject  ho_Image, ho_circle, ho_RegionFillUp;

	// Local control variables
	HTuple  hv_RevInfo, hv_RevInfoValues, hv_DeviceInfo;
	HTuple  hv_DeviceInfoValues, hv_AcqHandle, hv_Width, hv_Height;
	HTuple  hv_WindowID, hv_column, hv_row, hv_radius, hv_i;
	HTuple hv_WindowHandle2;
	HObject  ho_GrayImage, ho_Regions1;
	HObject  ho_DarkRegion, ho_RegionClosing, ho_RegionOpening;
	HObject  ho_ConnectedRegions, ho_SelectedRegions;

	// Local control variables

	HTuple  hv_Area, hv_Row, hv_Column, hv_Row1, hv_Column1;
	HTuple  hv_Radius, hv_data_Z, hv_data_x;
	//ReadImage(&ho_GrayImage_cam, "./text001.jpg.tif");
	ReadImage(&ho_GrayImage_cam, "./text_tem2.jpg.tif");

	//ReadImage(&ho_GrayImage_cam, "G:/临时区/fafe1406353208/xads_PCServer/xads_PCServer/text001.jpg.tif");
	GetImageSize(ho_GrayImage_cam, &hv_Width, &hv_Height);
	hv_column = hv_Width / 2;
	hv_row = hv_Height / 2;
	Rgb1ToGray(ho_GrayImage_cam, &ho_GrayImage);

	Threshold(ho_GrayImage, &ho_Regions1, 0, 240);

	SelectObj(ho_Regions1, &ho_DarkRegion, 1);
	ClosingCircle(ho_DarkRegion, &ho_RegionClosing, 50);

	Connection(ho_RegionClosing, &ho_ConnectedRegions);
	SelectShape(ho_ConnectedRegions, &ho_SelectedRegions, ((HTuple("row").Append("area")).Append("column")),
		"and", ((HTuple(800).Append(6000)).Append(950)), ((HTuple(1350).Append(30000)).Append(1350)));

	//SelectShape(ho_ConnectedRegions, &ho_SelectedRegions, ((HTuple("width").Append("area")).Append("height")),
	//	"and", ((HTuple(1000).Append(250000)).Append(1000)), ((HTuple(1500).Append(650000)).Append(1500)));
	//SelectShape(ho_ConnectedRegions, &ho_SelectedRegions, (HTuple("circularity").Append("area")), "and", (HTuple(0.65).Append(1500)), (HTuple(1).Append(3000)));
	//AreaCenter(ho_SelectedRegions, &hv_Area, &hv_Row, &hv_Column);
	FillUp(ho_SelectedRegions, &ho_RegionFillUp);
	Connection(ho_RegionFillUp, &ho_RegionFillUp);
	SelectShape(ho_RegionFillUp, &ho_RegionFillUp, (HTuple("circularity").Append("area")), "and", (HTuple(0.5).Append(22000)), (HTuple(1).Append(50000)));
	SmallestCircle(ho_RegionFillUp, &hv_Row1, &hv_Column1, &hv_Radius);
	if (0 != (hv_Radius > 0))
	{



		hv_data_Z = (hv_row - hv_Row1) / 15.46;
		hv_data_x = -(-(hv_column - hv_Column1)) / 15.46;

		double d_Z = hv_data_Z[0].D();
		double d_X = hv_data_x[0].D();

		GetDlgItem(IDC_STATIC_PIC)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_PIC)->ShowWindow(TRUE);

		CString  PC_Z, PC_X;
		PC_Z.Format(_T("%.2lf"), d_Z);//double转Cstring
		PC_X.Format(_T("%.2lf"), d_X);//double转Cstring
		SetDlgItemText(IDC_EDIT1, PC_Z);
		SetDlgItemText(IDC_EDIT2, PC_X);

		CString strMsg = PC_X + "," + PC_Z + "," + "1";

		SendClientMsg(strMsg, NULL);
		SetRevBoxText(_T("服务器>>") + strMsg);
		SetDlgItemText(IDC_EDITSENDBOX, _T(""));

		HWND hwnd2;
		hwnd2 = GetDlgItem(IDC_STATIC_PIC)->m_hWnd;
		LONG WWindowID2 = (LONG)hwnd2;


		CRect rect2;
		GetDlgItem(IDC_STATIC_PIC)->GetWindowRect(&rect2);
		m_kuan = rect2.Width();
		m_gao = rect2.Height();



		OpenWindow(0, 0, m_kuan, m_gao, WWindowID2, "visible", "", &hv_WindowHandle2);
		ClearWindow(hv_WindowHandle2);
		HDevWindowStack::Push(hv_WindowHandle2);
		if (HDevWindowStack::IsOpen())
			DispObj(ho_SelectedRegions, HDevWindowStack::GetActive());



	}
	else {
		return;
	}
}


void CMvision0819Dlg::OnBnClickedButtonGround()
{
	// TODO: 在此添加控件通知处理程序代码
	// TODO: 在此添加控件通知处理程序代码
	HObject  ho_Image, ho_circle;

	// Local control variables
	HTuple  hv_RevInfo, hv_RevInfoValues, hv_DeviceInfo;
	HTuple  hv_DeviceInfoValues, hv_AcqHandle, hv_Width, hv_Height;
	HTuple  hv_WindowID, hv_column, hv_row, hv_radius, hv_i;
	HTuple hv_WindowHandle1;

	HObject   ho_GrayImage, ho_Regions, ho_DarkRegion;
	HObject  ho_RegionClosing, ho_RegionOpening, ho_ConnectedRegions;
	HObject  ho_SelectedRegions, ho_RegionFillUp;

	// Local control variables
	//HTuple  hv_Width, hv_Height, hv_column, hv_row;
	HTuple   hv_Row1, hv_Column1, hv_Radius, hv_data_Y;
	HTuple  hv_data_X;

	ReadImage(&ho_GrayImage_cam, "./text_tem2.jpg.tif");
	GetImageSize(ho_GrayImage_cam, &hv_Width, &hv_Height);
	hv_column = hv_Width / 2;
	hv_row = hv_Height / 2;

	num10 = GetDlgItemInt(IDC_EDIT10);
	num11 = GetDlgItemInt(IDC_EDIT11);
	num12 = GetDlgItemInt(IDC_EDIT12);
	num13 = GetDlgItemInt(IDC_EDIT13);
	num14 = GetDlgItemInt(IDC_EDIT14);


	Rgb1ToGray(ho_GrayImage_cam, &ho_GrayImage);

	Threshold(ho_GrayImage, &ho_Regions, 0, num10);
	SelectObj(ho_Regions, &ho_DarkRegion, 1);
	ClosingCircle(ho_DarkRegion, &ho_RegionClosing, 4);
	OpeningCircle(ho_RegionClosing, &ho_RegionOpening, 1.5);

	Connection(ho_RegionOpening, &ho_ConnectedRegions);
	//SelectShape(ho_ConnectedRegions, &ho_SelectedRegions, (HTuple("circularity").Append("area")), "and", (HTuple(0.65).Append(800000)), (HTuple(1).Append(1200000)));
	SelectShape(ho_ConnectedRegions, &ho_SelectedRegions, (HTuple("circularity").Append("area")), "and", (HTuple(num11).Append(num13)), (HTuple(num12).Append(num14)));
	FillUp(ho_SelectedRegions, &ho_RegionFillUp);
	SmallestCircle(ho_RegionFillUp, &hv_Row1, &hv_Column1, &hv_Radius);

	if (0 != (hv_Radius > 0))
	{




		hv_data_Y = -(hv_row - hv_Row1) / 15.46;
		hv_data_X = -(-(hv_column - hv_Column1)) / 15.46;

		double d_Y = hv_data_Y[0].D();
		double d_X = hv_data_X[0].D();


		CString  PC_Y, PC_X;
		PC_Y.Format(_T("%.2lf"), d_Y);//double转Cstring
		PC_X.Format(_T("%.2lf"), d_X);//double转Cstring
		SetDlgItemText(IDC_EDIT3, PC_Y);
		SetDlgItemText(IDC_EDIT4, PC_X);

		CString strMsg = PC_X + "," + PC_Y + "," + "1";
		GetDlgItem(IDC_STATIC_PIC)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_PIC)->ShowWindow(TRUE);

		SendClientMsg(strMsg, NULL);
		SetRevBoxText(_T("服务器>>") + strMsg);
		SetDlgItemText(IDC_EDITSENDBOX, _T(""));

		//DisplayImage(ho_SelectedRegions, true);

		HWND hwnd2;
		hwnd2 = GetDlgItem(IDC_STATIC_PIC)->m_hWnd;
		LONG WWindowID2 = (LONG)hwnd2;


		CRect rect2;
		GetDlgItem(IDC_STATIC_PIC)->GetWindowRect(&rect2);
		m_kuan = rect2.Width();
		m_gao = rect2.Height();





		OpenWindow(0, 0, m_kuan, m_gao, WWindowID2, "visible", "", &hv_WindowHandle1);
		HDevWindowStack::Push(hv_WindowHandle1);

		if (HDevWindowStack::IsOpen())
			DispObj(ho_RegionFillUp, HDevWindowStack::GetActive());





	}
	else {
		return;
	}

}
