// MineDlg.h : header file
//

#if !defined(AFX_MINEDLG_H__0EE906D3_ACB0_418A_85F5_7B2125CF0ADC__INCLUDED_)
#define AFX_MINEDLG_H__0EE906D3_ACB0_418A_85F5_7B2125CF0ADC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMineDlg dialog

#include "MyMine.h"

class CMineDlg : public CDialog
{
// Construction
public:
	void InitMenu();
	CMineDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CMineDlg)
	enum { IDD = IDD_MINE_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMineDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	BOOL m_bStart;

	CMyMine mine;

	void PlayBackMusic(BOOL bCheck);

	// Generated message map functions
	//{{AFX_MSG(CMineDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnAbout();
	afx_msg void OnExitGame();
	afx_msg void OnHelp();
	afx_msg void OnPlayMusic();
	afx_msg void OnStartGame();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MINEDLG_H__0EE906D3_ACB0_418A_85F5_7B2125CF0ADC__INCLUDED_)
