#ifndef __MYMINE_H__
#define __MYMINE_H__

typedef struct 
{
	UINT uRow;
	UINT uCol;
	UINT uState;
	UINT uAttrib;
	UINT uOldState;
} MINEWND;	// С����ṹ��

class CMyMine: public CWnd
{
public:
	
	CMyMine();
	virtual ~CMyMine();

	void StartGame();

protected:
	//{{AFX_MSG(CMyMine)
	afx_msg void OnPaint();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	// init and save functions
	void InitGame();
	void LoadConfig();
	void LoadBitmap();
	void SizeWindow();
	void LayMines(UINT row, UINT col);
	// draw functions
//	void DrawShell(CPaintDC &dc);
	void DrawButton(CPaintDC &dc);
	void DrawNumber(CPaintDC &dc);
	void DrawMineArea(CPaintDC &dc);
	void DrawDownNum(MINEWND* mine, UINT num);
	void DrawSpecialMine(UINT row, UINT col);
	// change menu check state funtions
	void SetCheckedSound();
	void SetCheckedColor();
	void SetCheckedMark();
	void SetCheckedLevel();
	void SetCheckedCheat();
	// other functions
	void ExpandMines(UINT row, UINT col);
	BOOL IsInMineArea(UINT row, UINT col);
	BOOL IsMine(UINT row, UINT col);
	UINT GetAroundNum(UINT row, UINT col);
	UINT GetAroundFlags(UINT row, UINT col);
	void Dead(UINT row, UINT col);
	BOOL Victory();

	void OnLRBtnDown(UINT row, UINT col);
	void OnLRBtnUp(UINT row, UINT col);
	void OpenAround(UINT row, UINT col);
	BOOL ErrorAroundFlag(UINT row, UINT col);
	
	MINEWND* GetMine(long x, long y);
private:
	UINT		m_uXNum;				// X����С�������
	UINT		m_uYNum;				// Y����С�������
	UINT		m_uMineNum;				// �ܵ��׸���
	int 		m_nLeaveNum;			// ʣ����׸���
	UINT		m_uSpendTime;			// �ӵ�һ����Ч�����Ŀǰ�ܹ����ѵ�ʱ��
	UINT		m_uGameState;			// ��Ϸ״̬()
	UINT		m_uTimer;				// ʱ����Ʊ���
	UINT		m_uNewState;			// ��ǰѡ�е�С�����״̬
	
	UINT		m_uHighTime;			// ��ǰ��Ϸʱ���¼

	BOOL		m_bLRBtnDown;			// �Ƿ�Ϊ���Ҽ�ͬʱ����
	BOOL		m_bClickBtn;			// ������µ�ʱ������Ƿ�λ�ڰ�ť������
	BOOL		m_bMarkful;				// �Ƿ�����ʾ���
	BOOL		m_bColorful;			// �Ƿ��ɫ��ʾ

	CBitmap		m_bmpMine;				// ��������ͼ��
	CBitmap		m_bmpNumber;			// ���ֱ���ͼ��
	CBitmap		m_bmpButton;			// ��ť����ͼ��
	CBrush		m_brsBG;				// ������ˢ����
	COLORREF	m_clrDark;				// ����ť����ɫ��
	RECT		m_rcClient;				// �ͻ�����
	UINT		m_uBtnRect[3];			// ��ť��������������
	UINT		m_uBtnState;			// ��ť״̬
	UINT		m_uNumRect[3];			// ���ֿ�������������(����ʱ����׸���)
	UINT		m_uShellRcX[2];			// �ڿ��Լ��߽������X����
	UINT		m_uShellRcY[2];			// �ڿ��Լ��߽������Y����
	
	MINEWND		m_pMines[24][30];		// ��ʾ�����ڵ�����С����Ķ�ά����
	MINEWND*	m_pNewMine;				// ��ǰѡ�е�С����
	MINEWND*	m_pOldMine;				// �ϴ�ѡ�е�С����
};

#endif