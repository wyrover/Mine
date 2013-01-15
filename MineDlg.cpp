// MineDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Mine.h"
#include "MineDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "HeroDlg.h"
#include "HelpDlg.h"
#include <mmsystem.h>

#define IDC_MINEAREA 1009

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMineDlg dialog

CMineDlg::CMineDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMineDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMineDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMineDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMineDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMineDlg, CDialog)
	//{{AFX_MSG_MAP(CMineDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(IDR_ABOUT, OnAbout)
	ON_COMMAND(IDR_EXIT_GAME, OnExitGame)
	ON_COMMAND(IDR_HELP, OnHelp)
	ON_COMMAND(IDR_PLAY_MUSIC, OnPlayMusic)
	ON_COMMAND(IDR_START_GAME, OnStartGame)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMineDlg message handlers

BOOL CMineDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	m_bStart = FALSE;

	InitMenu();

	CRect rect(0, 0, 200, 200);
    mine.CreateEx( WS_EX_CLIENTEDGE, _T("MyMineClass"), NULL, WS_VISIBLE | WS_BORDER | WS_CHILD,
        CRect( 0, 0, 500, 600 ), this, IDC_MINEAREA );
	
	GetDlgItem( IDC_MINEAREA )->SetFocus();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMineDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMineDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMineDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CMineDlg::OnOK() 
{
	CDialog::OnOK();
}

void CMineDlg::OnCancel() 
{
	CDialog::OnCancel();
}

void CMineDlg::OnAbout() 
{
	CAboutDlg dlg;
	dlg.DoModal();
}

void CMineDlg::OnExitGame() 
{
	CDialog::OnCancel();	
}

void CMineDlg::OnHelp() 
{
	CHelpDlg dlg;
	dlg.DoModal();
}

void CMineDlg::OnPlayMusic() 
{
	CWnd*   pMain   =   AfxGetMainWnd();   
	CMenu*   pMenu   =   pMain->GetMenu();
	//判断播放音乐菜单当前状态
	BOOL bCheck = (BOOL)pMenu->GetMenuState(IDR_PLAY_MUSIC, MF_CHECKED);
	
	if(m_bStart)
	{
		if(bCheck)
		{
			pMenu->CheckMenuItem(IDR_PLAY_MUSIC, 
				MF_BYCOMMAND | MF_UNCHECKED);
		}
		else
		{
			pMenu->CheckMenuItem(IDR_PLAY_MUSIC, 
				MF_BYCOMMAND | MF_CHECKED);
		}
		
		PlayBackMusic(!bCheck);			//调用播放背景音乐功能函数
	}
}

void CMineDlg::OnStartGame() 
{
	mine.StartGame();					//调用游戏开始接口函数
}

void CMineDlg::InitMenu()
{
	CWnd*   pMain   =   AfxGetMainWnd();   
	CMenu*   pMenu   =   pMain->GetMenu();
	pMenu->CheckMenuItem(IDR_PLAY_MUSIC,MF_BYCOMMAND| MF_UNCHECKED);
}

void CMineDlg::PlayBackMusic(BOOL bCheck)
{
	if(bCheck)
	{								//播放指定音乐文件
		sndPlaySound("music.wav",SND_ASYNC); 
	}
	else
	{								//停止播放
		sndPlaySound(NULL,SND_PURGE); 
	}
}