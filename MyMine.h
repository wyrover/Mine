#ifndef __MYMINE_H__
#define __MYMINE_H__

typedef struct 
{
	UINT uRow;
	UINT uCol;
	UINT uState;
	UINT uAttrib;
	UINT uOldState;
} MINEWND;	// 小方块结构体

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
	UINT		m_uXNum;				// X方向小方块个数
	UINT		m_uYNum;				// Y方向小方块个数
	UINT		m_uMineNum;				// 总的雷个数
	int 		m_nLeaveNum;			// 剩余的雷个数
	UINT		m_uSpendTime;			// 从第一次有效点击到目前总共花费的时间
	UINT		m_uGameState;			// 游戏状态()
	UINT		m_uTimer;				// 时间控制变量
	UINT		m_uNewState;			// 当前选中的小方块的状态
	
	UINT		m_uHighTime;			// 当前游戏时间记录

	BOOL		m_bLRBtnDown;			// 是否为左右键同时按下
	BOOL		m_bClickBtn;			// 左键按下的时候鼠标是否位于按钮区域内
	BOOL		m_bMarkful;				// 是否能显示标记
	BOOL		m_bColorful;			// 是否彩色显示

	CBitmap		m_bmpMine;				// 雷区背景图像
	CBitmap		m_bmpNumber;			// 数字背景图像
	CBitmap		m_bmpButton;			// 按钮背景图像
	CBrush		m_brsBG;				// 背景画刷对象
	COLORREF	m_clrDark;				// 各按钮的深色调
	RECT		m_rcClient;				// 客户区域
	UINT		m_uBtnRect[3];			// 按钮框区域坐标数组
	UINT		m_uBtnState;			// 按钮状态
	UINT		m_uNumRect[3];			// 数字框区域坐标数组(包括时间和雷个数)
	UINT		m_uShellRcX[2];			// 内框以及边界的坐标X方向
	UINT		m_uShellRcY[2];			// 内框以及边界的坐标Y方向
	
	MINEWND		m_pMines[24][30];		// 表示雷区内的所有小方块的二维数组
	MINEWND*	m_pNewMine;				// 当前选中的小方块
	MINEWND*	m_pOldMine;				// 上次选中的小方块
};

#endif