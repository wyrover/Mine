// HeroDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Mine.h"
#include "HeroDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHeroDlg dialog


CHeroDlg::CHeroDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHeroDlg::IDD, pParent)
{
	m_bWriteflg = FALSE;
}


void CHeroDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHeroDlg)
	DDX_Text(pDX, IDC_NAME_EDIT, m_name);
	DDX_Text(pDX, IDC_TIME_EDIT, m_time);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHeroDlg, CDialog)
	//{{AFX_MSG_MAP(CHeroDlg)
	ON_BN_CLICKED(IDOK_BTN, OnBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHeroDlg message handlers

void CHeroDlg::OnOK() 
{

}

void CHeroDlg::SetWriteFlg(BOOL bflg)
{
	m_bWriteflg = bflg;
}

int CHeroDlg::DoModal() 
{
	char pszTmp[128] = {0};

	//读取配置文件
	GetPrivateProfileString("HERO", "name", "", 
		pszTmp, 127, ".\\hero.ini");
	m_name = CString(pszTmp);

	if(!m_bWriteflg)
	{
		GetPrivateProfileString("HERO", "time", "0", 
			pszTmp, 127, ".\\hero.ini");
		m_time = atoi(pszTmp);
		
	}

	return CDialog::DoModal();
}

void CHeroDlg::OnBtn() 
{
	UpdateData(TRUE);
	if(m_bWriteflg)
	{
		CString tmp;
		tmp.Format("%d", m_time);
		WritePrivateProfileString("HERO", "name", m_name, ".\\hero.ini");
		WritePrivateProfileString("HERO", "time", tmp, ".\\hero.ini");
	}
	m_bWriteflg = FALSE;

	CDialog::OnOK();
}

BOOL CHeroDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if(m_bWriteflg)
	{
		SetDlgItemText(IDOK_BTN, "记录");
	}
	
	return TRUE; 
}
