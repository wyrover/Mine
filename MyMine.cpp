#include "stdafx.h"
#include "Mine.h"
#include "MyMine.h"
#include "MineDefs.h"
#include "HeroDlg.h"

#define ID_TIMER_EVENT	WM_USER + 1

CMyMine::CMyMine()
{
	m_uTimer = 0;
	m_brsBG.CreateSolidBrush(COLOR_GRAY);
	InitGame();
}

CMyMine::~CMyMine()
{
}

BEGIN_MESSAGE_MAP(CMyMine, CWnd)
//{{AFX_MSG_MAP(CMyMine)
ON_WM_PAINT()
ON_WM_SHOWWINDOW()
ON_WM_TIMER()
ON_WM_LBUTTONUP()
ON_WM_RBUTTONUP()
ON_WM_LBUTTONDOWN()
ON_WM_RBUTTONDOWN()
ON_WM_MOUSEMOVE()
ON_WM_KEYDOWN()
ON_WM_INITMENU()
ON_WM_CLOSE()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CMyMine::LoadConfig()
{
	char pszTmp[128] = {0};
	
	GetPrivateProfileString("HERO", "time", "0", 
		pszTmp, 127, ".\\hero.ini");
	m_uHighTime = atoi(pszTmp);

	m_uXNum		= 30;	//X坐标上的方块个数
	m_uYNum		= 16;	//Y坐标上的方块个数
	m_uMineNum	= 99;	//地雷个数

	m_bMarkful  = TRUE;
	m_bColorful = TRUE;
}
/*载入图片资源*/
void CMyMine::LoadBitmap()
{
	if (m_bColorful) {
		m_clrDark = COLOR_DARK_GRAY;
		m_bmpMine.DeleteObject();
		m_bmpMine.LoadBitmap(IDB_MINE_COLOR);
		m_bmpNumber.DeleteObject();
		m_bmpNumber.LoadBitmap(IDB_NUM_COLOR);
		m_bmpButton.DeleteObject();
		m_bmpButton.LoadBitmap(IDB_BTN_COLOR);
	}
	else {
		m_clrDark = COLOR_BLACK;
		m_bmpMine.DeleteObject();
		m_bmpMine.LoadBitmap(IDB_MINE_GRAY);
		m_bmpNumber.DeleteObject();
		m_bmpNumber.LoadBitmap(IDB_NUM_GRAY);
		m_bmpButton.DeleteObject();
		m_bmpButton.LoadBitmap(IDB_BTN_GRAY);
	}
}
/*初始化游戏*/
void CMyMine::InitGame()
{
	LoadBitmap();
	LoadConfig();
	m_nLeaveNum = m_uMineNum;
	m_uSpendTime = 0;
	m_uBtnState = BS_NORMAL;	//设置当前方块状态
	m_uGameState = GS_WAIT;		//设置当前游戏状态
	if (m_uTimer) {
		KillTimer(ID_TIMER_EVENT);
		m_uTimer = 0;
	}
	m_pNewMine = NULL;			//清空当前选中的小方块
	m_pOldMine = NULL;			//清空上次选中的小方块
	//初始化表示雷区的二维数组
	for (UINT i = 0; i<m_uYNum; i++) {
		for (UINT j = 0; j<m_uXNum; j++) {
			m_pMines[i][j].uRow = i;
			m_pMines[i][j].uCol = j;
			m_pMines[i][j].uState = STATE_NORMAL;
			m_pMines[i][j].uAttrib = ATTRIB_EMPTY;
			m_pMines[i][j].uOldState = STATE_NORMAL;
		}
	}
}

void CMyMine::StartGame()
{	
	InitGame();
	Invalidate();
}

/*在雷区铺设地雷*/
void CMyMine::LayMines(UINT row, UINT col)
{
	srand( (unsigned)time( NULL ) );
	UINT i, j;
	for(UINT index = 0; index < m_uMineNum;) {		
		i = rand() % m_uYNum;
		j = rand() % m_uXNum;
		if (i == row && j == col) continue;
		if(m_pMines[i][j].uAttrib != ATTRIB_MINE) {
			m_pMines[i][j].uAttrib = ATTRIB_MINE;
			index++;
		}
	}	
}

/*自动打开相关不是地雷的格子*/
void CMyMine::ExpandMines(UINT row, UINT col)
{
	UINT i, j;
	UINT minRow = (row == 0) ? 0 : row - 1;
	UINT maxRow = row + 2;
	UINT minCol = (col == 0) ? 0 : col - 1;
	UINT maxCol = col + 2;
	UINT around = GetAroundNum(row, col);

	m_pMines[row][col].uState = 15 - around;
	m_pMines[row][col].uOldState = 15 - around;
	//在指定位置画出地雷
	DrawSpecialMine(row, col);
	if (around == 0) {
		for (i = minRow; i < maxRow; i++) {
			for (j = minCol; j < maxCol; j++) {
				if (!(i == row && j == col) && 
					m_pMines[i][j].uState == STATE_NORMAL
					&& m_pMines[i][j].uAttrib != ATTRIB_MINE) {
					if (!IsInMineArea(i, j)) continue;
					ExpandMines(i, j);
				}
			}
		}
	}
}
/*获得周围地雷个数*/
UINT CMyMine::GetAroundNum(UINT row, UINT col)
{
	UINT i, j;
	UINT around = 0;
	UINT minRow = (row == 0) ? 0 : row - 1;
	UINT maxRow = row + 2;
	UINT minCol = (col == 0) ? 0 : col - 1;
	UINT maxCol = col + 2;
	
	for (i = minRow; i < maxRow; i++) {
		for (j = minCol; j < maxCol; j++) {
			if (!IsInMineArea(i, j)) continue;
			if (m_pMines[i][j].uAttrib == ATTRIB_MINE) around++;
		}
	}
	return around;
}

/*得到周围格子状态*/
UINT CMyMine::GetAroundFlags(UINT row, UINT col)
{
	UINT i, j;
	UINT flags = 0;
	UINT minRow = (row == 0) ? 0 : row - 1;
	UINT maxRow = row + 2;
	UINT minCol = (col == 0) ? 0 : col - 1;
	UINT maxCol = col + 2;
	
	for (i = minRow; i < maxRow; i++) {
		for (j = minCol; j < maxCol; j++) {
			if (!IsInMineArea(i, j)) continue;
			if (m_pMines[i][j].uState == STATE_FLAG) flags++;
		}
	}
	return flags;
}

/*地雷判断*/
BOOL CMyMine::IsMine(UINT row, UINT col)
{
	return (m_pMines[row][col].uAttrib == ATTRIB_MINE);
}

/*雷区判断*/
BOOL CMyMine::IsInMineArea(UINT row, UINT col)
{
	return (row >= 0 && row < m_uYNum && col >= 0 && col < m_uXNum);
}
/*游戏结束*/
void CMyMine::Dead(UINT row, UINT col)
{
	CRect rcBtn(m_uBtnRect[1], 15, m_uBtnRect[2], 39);
	CRect rcMineArea(MINE_AREA_LEFT, MINE_AREA_TOP, 
		MINE_AREA_LEFT + m_uXNum * MINE_WIDTH, MINE_AREA_TOP + m_uYNum * MINE_HEIGHT);
	
	UINT i, j;
	if (m_pMines[row][col].uAttrib == ATTRIB_MINE) {//打开了是地雷的格子
		for (i = 0; i < m_uYNum; i++) {
			for (j = 0; j < m_uXNum; j++) {
				m_pMines[row][col].uState = STATE_BLAST;
				m_pMines[row][col].uOldState = STATE_BLAST;
				if (m_pMines[i][j].uAttrib == ATTRIB_MINE
					&& m_pMines[i][j].uState != STATE_FLAG) {
					m_pMines[i][j].uState = STATE_MINE;
					m_pMines[i][j].uOldState = STATE_MINE;
				}
			}
		}
	}
	else {											//打开了判断错误的格子
		for (i = 0; i < m_uYNum; i++) {
			for (j = 0; j < m_uXNum; j++) {
				m_pMines[row][col].uState = STATE_ERROR;
				m_pMines[row][col].uOldState = STATE_ERROR;
				if (m_pMines[i][j].uAttrib == ATTRIB_MINE
					&& m_pMines[i][j].uState != STATE_FLAG) {
					m_pMines[i][j].uState = STATE_MINE;
					m_pMines[i][j].uOldState = STATE_MINE;
				}
			}
		}
	}
	
	InvalidateRect(rcMineArea);
	m_uBtnState = BS_DEAD;
	InvalidateRect(rcBtn);	
	m_uGameState = GS_DEAD;
	if (m_uTimer != 0) {
		KillTimer(ID_TIMER_EVENT);
		m_uTimer = 0;
	}

	AfxMessageBox("您踩到地雷了,游戏结束");

}

/*获得胜利*/
BOOL CMyMine::Victory()
{
	UINT i, j;
	CRect rcBtn(m_uBtnRect[1], 15, m_uBtnRect[2], 39);
	
	for (i = 0; i < m_uYNum; i++) {
		for (j = 0; j < m_uXNum; j++) {
			if (m_pMines[i][j].uState == STATE_NORMAL) return FALSE;
			if (m_pMines[i][j].uState == STATE_DICEY) return FALSE;
		}
	}
	
	m_uBtnState = BS_VICTORY;
	m_uGameState = GS_VICTORY;
	Invalidate();
	if (m_uTimer != 0) {
		KillTimer(ID_TIMER_EVENT);
		m_uTimer = 0;
	}

	AfxMessageBox("恭喜您胜利了,游戏结束");

	if(m_uSpendTime < m_uHighTime)
	{
		CHeroDlg dlg;

		dlg.m_time = m_uSpendTime;

		dlg.SetWriteFlg(TRUE);

		dlg.DoModal();
	}

	return TRUE;
}
/*打开指定行列周围格子*/
void CMyMine::OpenAround(UINT row, UINT col)
{
	if (GetAroundFlags(row, col) != GetAroundNum(row, col)) return;
	UINT i, j;
	UINT around = 0;
	UINT minRow = (row == 0) ? 0 : row - 1;
	UINT maxRow = row + 2;
	UINT minCol = (col == 0) ? 0 : col - 1;
	UINT maxCol = col + 2;
	
	for (i = minRow; i < maxRow; i++) {
		for (j = minCol; j < maxCol; j++) {
			if (!IsInMineArea(i, j)) continue;
			if (m_pMines[i][j].uState == STATE_NORMAL) {
				ExpandMines(i, j);
				around = GetAroundNum(i, j);
				m_pMines[i][j].uState = 15 - around;
				m_pMines[i][j].uOldState = 15 - around;
			}
		}
	}
	// 检查是否胜利
	if (Victory()) {
		for (i = 0; i < m_uYNum; i++) {
			for (j = 0; j < m_uXNum; j++) {
				if (m_pMines[i][j].uAttrib == ATTRIB_MINE) {
					m_pMines[i][j].uState = STATE_FLAG;
					m_pMines[i][j].uOldState = STATE_FLAG;
				}
			}
		}
		m_nLeaveNum = 0;
		Invalidate();
	}
}

BOOL CMyMine::ErrorAroundFlag(UINT row, UINT col)
{
	if (GetAroundFlags(row, col) != GetAroundNum(row, col)) return FALSE;
	UINT i, j;
	UINT minRow = (row == 0) ? 0 : row - 1;
	UINT maxRow = row + 2;
	UINT minCol = (col == 0) ? 0 : col - 1;
	UINT maxCol = col + 2;
	
	for (i = minRow; i < maxRow; i++) {
		for (j = minCol; j < maxCol; j++) {
			if (!IsInMineArea(i, j)) continue;
			if (m_pMines[i][j].uState == STATE_FLAG) {
				if (m_pMines[i][j].uAttrib != ATTRIB_MINE) {
					Dead(i, j);
					return TRUE;
				}
			}
		}
	}
	
	return FALSE;
}

void CMyMine::OnLRBtnDown(UINT row, UINT col)
{
	UINT i, j;
	UINT minRow = (row == 0) ? 0 : row - 1;
	UINT maxRow = row + 2;
	UINT minCol = (col == 0) ? 0 : col - 1;
	UINT maxCol = col + 2;
	
	for (i = minRow; i < maxRow; i++) {
		for (j = minCol; j < maxCol; j++) {
			if (!IsInMineArea(i, j)) continue;
			//			if (i == row && j == col) continue;
			if (m_pMines[i][j].uState == STATE_NORMAL) {
				m_pMines[i][j].uState = STATE_EMPTY;
			}
			else if (m_pMines[i][j].uState == STATE_DICEY) {
				m_pMines[i][j].uState = STATE_DICEY_DOWN;
			}
		}
	}
}

void CMyMine::OnLRBtnUp(UINT row, UINT col)
{
	UINT i, j;
	UINT minRow = (row == 0) ? 0 : row - 1;
	UINT maxRow = row + 2;
	UINT minCol = (col == 0) ? 0 : col - 1;
	UINT maxCol = col + 2;
	
	for (i = minRow; i < maxRow; i++) {
		for (j = minCol; j < maxCol; j++) {
			if (!IsInMineArea(i, j)) continue;
			//			if (i == row && j == col) continue;
			if (m_pMines[i][j].uOldState == STATE_NORMAL) {
				m_pMines[i][j].uState = STATE_NORMAL;
			}
			else if (m_pMines[i][j].uOldState == STATE_DICEY) {
				m_pMines[i][j].uState = STATE_DICEY;
			}
		}
	}
	//	Invalidate();
}

void CMyMine::SizeWindow()
{
	UINT uWidth = DEFAULT_FRAME_X + m_uXNum * MINE_WIDTH +
		LINE_WIDTH_0 * 3 + SIDE_WIDTH_0 + SIDE_WIDTH_1; 
	UINT uHeight = DEFAULT_FRAME_Y + m_uYNum * MINE_HEIGHT + 
		LINE_WIDTH_0 * 3 + SIDE_WIDTH_0 * 2 + SIDE_WIDTH_1 + SHELL_S_H;	
	SetWindowPos(&wndTopMost, 0, 0, uWidth, uHeight, 
		SWP_NOZORDER | SWP_NOMOVE | SWP_NOCOPYBITS);	// SWP_NOCOPYBITS does not do function？？？
	
	GetClientRect(&m_rcClient);
	// Button Position
	m_uBtnRect[0] = m_rcClient.right / 2 - 12;			// the left side of the start(state) button without 3d border
	m_uBtnRect[1] = m_rcClient.right / 2 - 13;			// the left side of the start(state) button with 3d border
	m_uBtnRect[2] = m_rcClient.right / 2 + 12;			// the right side of the start(state) button 
	
	// Number Position
	m_uNumRect[0] = m_rcClient.right - 55;				// the left side of the spend time rect	with 3d border
	m_uNumRect[1] = m_rcClient.right - 15;				// the right side of the spend time rect
	m_uNumRect[2] = m_rcClient.right - 54;				// the left side of the spend time rect without 3d border
	//	m_uNumRect[3] = m_rcClient.right - 41;
	//	m_uNumRect[4] = m_rcClient.right - 28;	
	
	// Shell Position
	m_uShellRcX[0] = m_rcClient.right;					//
	m_uShellRcX[1] = m_rcClient.right - 14;				// the width of the small(big) shell
	
	m_uShellRcY[0] = m_rcClient.bottom;					// 
	m_uShellRcY[1] = m_rcClient.bottom - SHELL_L_START_Y - 5;		// the height of the big shell
}

void CMyMine::DrawButton(CPaintDC &dc)
{
	CDC cdc;	
	cdc.CreateCompatibleDC(&dc);
	cdc.SelectObject(m_bmpButton);	
	dc.StretchBlt(m_uBtnRect[0], 16, 24, 24, &cdc, 0, 24 * m_uBtnState, 24, 24, SRCCOPY);
	
	dc.Draw3dRect(m_uBtnRect[1], 15, 26, 26, m_clrDark, m_clrDark);
}

void CMyMine::DrawNumber(CPaintDC &dc)
{
	CDC cdc;	
	cdc.CreateCompatibleDC(&dc);
	cdc.SelectObject(m_bmpNumber);
	
	dc.Draw3dRect(16, 15, 41, 25, m_clrDark, COLOR_WHITE);
	dc.Draw3dRect(m_uNumRect[0], 15, 41, 25, m_clrDark, COLOR_WHITE);
	int num;
	// draw remaining mine numbers
	num = (m_nLeaveNum < 0) ? 11 : m_nLeaveNum / 100;
	dc.StretchBlt(17, 16, 13, 23, &cdc, 0, 276 - 23 * (num+1), 13, 23, SRCCOPY);
	num = (m_nLeaveNum < 0) ? -(m_nLeaveNum - num * 100) / 10 : (m_nLeaveNum - num * 100) / 10;
	dc.StretchBlt(30, 16, 13, 23, &cdc, 0, 276 - 23 * (num+1), 13, 23, SRCCOPY);
	num = (m_nLeaveNum < 0) ? -m_nLeaveNum % 10 : m_nLeaveNum % 10;
	dc.StretchBlt(43, 16, 13, 23, &cdc, 0, 276 - 23 * (num+1), 13, 23, SRCCOPY);
	// draw take seconds
	num = m_uSpendTime / 100;
	dc.StretchBlt(m_uNumRect[0], 16, 13, 23, &cdc, 0, 276 - 23 * (num+1), 13, 23, SRCCOPY);
	num = (m_uSpendTime - num * 100) / 10;
	dc.StretchBlt(m_uNumRect[0] + 13, 16, 13, 23, &cdc, 0, 276 - 23 * (num+1), 13, 23, SRCCOPY);
	num = m_uSpendTime % 10;
	dc.StretchBlt(m_uNumRect[0] + 26, 16, 13, 23, &cdc, 0, 276 - 23 * (num+1), 13, 23, SRCCOPY);
}

/*void CMyMine::DrawShell(CPaintDC &dc)
{
	// draw side
	dc.FillSolidRect(0, 0, m_uShellRcX[0], LINE_WIDTH_0, COLOR_WHITE);
	dc.FillSolidRect(0, 0, LINE_WIDTH_0, m_uShellRcY[0], COLOR_WHITE);
	
	// draw small shell
	dc.Draw3dRect(SHELL_S_START_X, SHELL_S_START_Y, 
		m_uShellRcX[1], SHELL_S_H, m_clrDark, COLOR_WHITE);
	dc.Draw3dRect(SHELL_S_START_X + 1, SHELL_S_START_Y + 1, 
		m_uShellRcX[1] - 2, SHELL_S_H - 2, m_clrDark, COLOR_WHITE);
	
	// draw large shell
	dc.Draw3dRect(SHELL_L_START_X, SHELL_L_START_Y,
		m_uShellRcX[1], m_uShellRcY[1], m_clrDark, COLOR_WHITE);
	dc.Draw3dRect(SHELL_L_START_X + 1, SHELL_L_START_Y + 1,
		m_uShellRcX[1] - 2, m_uShellRcY[1] - 2, m_clrDark, COLOR_WHITE);
	dc.Draw3dRect(SHELL_L_START_X + 2, SHELL_L_START_Y + 2, 
		m_uShellRcX[1] - 4, m_uShellRcY[1] - 4, m_clrDark, COLOR_WHITE);

}*/

void CMyMine::DrawMineArea(CPaintDC &dc)
{
	CDC cdc;
	cdc.CreateCompatibleDC(&dc);
	cdc.SelectObject(m_bmpMine);
	
	for (UINT i = 0; i<m_uYNum; i++) {
		for (UINT j = 0; j<m_uXNum; j++) {
			dc.StretchBlt(MINEAREA_FRAME_X + 16 * j, MINEAREA_FRAME_Y + 16 * i, 
				16, 16, &cdc, 0, 16 * m_pMines[i][j].uState, 16, 16, SRCCOPY);
		}
	}
}

void CMyMine::DrawDownNum(MINEWND* mine, UINT num)
{
	mine->uState = 15 - num;
	mine->uOldState = 15 - num;
	CRect rcMine(mine->uCol * 16, mine->uRow * 16, (mine->uCol+1) *16, (mine->uRow+1) * 16);
	InvalidateRect(rcMine);
}

void CMyMine::DrawSpecialMine(UINT row, UINT col)
{
	CRect rcMine(col * 16, row * 16, (col+1) * 16, (row+1) * 16);
	InvalidateRect(rcMine);
}

MINEWND* CMyMine::GetMine(long x, long y)
{
	if (x < MINEAREA_FRAME_X || y < MINEAREA_FRAME_Y) {
		return NULL;
	}
	UINT uCol = (UINT)(x-MINEAREA_FRAME_X) / 16;
	UINT uRow = (UINT)(y-MINEAREA_FRAME_Y) / 16;
	return &m_pMines[uRow][uCol];
}

// message functions which are created by class wizard
// this function has been studied on 2005/1/6
void CMyMine::OnPaint() 
{
	CDC cdc;						// the memory device context, used to prepare the display context for the window
	CPaintDC dc(this);				// the display device context, here copy the display context from "cdc"  
									// which has been prepared in the memory.
	CBitmap bitmap;

	if (!dc.IsPrinting()) {
		// create a memory device context "cdc" which is compatible with "dc" 
		if (cdc.CreateCompatibleDC(&dc)) {	
			// create a bitmap compatible with "dc" which used to copy to "cdc"
			if (bitmap.CreateCompatibleBitmap(&dc, m_rcClient.right, m_rcClient.bottom)) {
				// prepare the display context in "cdc"
				cdc.SelectObject(&bitmap);
				cdc.FillRect(&m_rcClient, &m_brsBG);
				
				DrawButton((CPaintDC&) cdc);
				DrawNumber((CPaintDC&) cdc);
				DrawMineArea((CPaintDC&) cdc);
				
				// copy the context from "cdc" to "dc"
				dc.BitBlt(m_rcClient.left, m_rcClient.top, m_rcClient.right, m_rcClient.bottom, &cdc, 0, 0, SRCCOPY);
				bitmap.DeleteObject();
			}
		}
	}
}

void CMyMine::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	SizeWindow();
	CWnd::OnShowWindow(bShow, nStatus);
}

void CMyMine::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == ID_TIMER_EVENT) {
		m_uSpendTime++;
		CRect rcNum(m_uNumRect[0], 15, m_uNumRect[1], 39);
		InvalidateRect(rcNum);
		
		if (m_uSpendTime >= DEFAULT_RECORD) {
			KillTimer(ID_TIMER_EVENT);
			m_uTimer = 0;
		}
	}
	
	CWnd::OnTimer(nIDEvent);
}

void CMyMine::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CRect rcBtn(m_uBtnRect[1], 15, m_uBtnRect[2], 39);
	CRect rcMineArea(MINE_AREA_LEFT, MINE_AREA_TOP, 
		MINE_AREA_LEFT + m_uXNum * MINE_WIDTH, MINE_AREA_TOP + m_uYNum * MINE_HEIGHT);
	
	SetCapture();			// capture the mouse cursor
	m_bClickBtn = FALSE;
	m_bLRBtnDown = FALSE;
	if (rcBtn.PtInRect(point)) {					// click in the button area
		m_bClickBtn = TRUE;
		m_uBtnState = BS_DOWN;
		InvalidateRect(rcBtn);
	}
	else if (rcMineArea.PtInRect(point)) {			// click in the mine area
		// change mine state by gamestate
		switch(m_uGameState) {						
		case GS_WAIT: 
		case GS_RUN:
			m_pNewMine = GetMine(point.x, point.y);
			if (!m_pNewMine) return;
			if (m_pNewMine->uState == STATE_NORMAL) {
				m_pNewMine->uState = STATE_EMPTY;
			}
			if (m_pNewMine->uState == STATE_DICEY) {
				m_pNewMine->uState = STATE_DICEY_DOWN;
			}
			m_pOldMine = m_pNewMine;
			break;
		case GS_DEAD: 
		case GS_VICTORY:
			return;
		}
		m_uBtnState = BS_CLICK;
		InvalidateRect(rcBtn);
		// both of the left button and the right button are pushing down 
		if (nFlags == (MK_LBUTTON | MK_RBUTTON)) {
			m_bLRBtnDown = TRUE;
			OnLRBtnDown(m_pOldMine->uRow, m_pOldMine->uCol);
		}
		InvalidateRect(rcMineArea);
	}
	else {											// click in other area
		if (m_uGameState == GS_WAIT || m_uGameState == GS_RUN) {
			m_uBtnState = BS_CLICK;
			InvalidateRect(rcBtn);
		}
	}

	CWnd::OnLButtonDown(nFlags, point);
}

void CMyMine::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CRect rcBtn(m_uBtnRect[1], 15, m_uBtnRect[2], 39);
	CRect rcMineArea(MINE_AREA_LEFT, MINE_AREA_TOP, 
		MINE_AREA_LEFT + m_uXNum * MINE_WIDTH, MINE_AREA_TOP + m_uYNum * MINE_HEIGHT);
	
	if (rcBtn.PtInRect(point)) {					// click in the button area
		if (m_bClickBtn) {
			Invalidate();
			InitGame();
		}
		else {
			if (m_uGameState == GS_WAIT || m_uGameState == GS_RUN) {
				m_uBtnState = BS_NORMAL;
				InvalidateRect(rcBtn);
			}
		}
	}
	else if (rcMineArea.PtInRect(point)) {			// click in the mine area
		CString value;
		// different process with different gamestate
		UINT around = 0;
		switch(m_uGameState) {
		case GS_WAIT: case GS_RUN:
			// first get the MINEWND which if pushing down
			m_pOldMine = GetMine(point.x, point.y);
			if (!m_pOldMine) {
				ReleaseCapture();
				return;
			}
			// do normal process
			// judge whether the lr button are both pushed down
			if (m_bLRBtnDown) {
				m_bLRBtnDown = FALSE;
				OnLRBtnUp(m_pOldMine->uRow, m_pOldMine->uCol);
				if (m_uGameState == GS_WAIT) {
					m_uBtnState = BS_NORMAL;
					Invalidate();
					ReleaseCapture();
					return;
				}
				// if the around flags number equal to the around mines number, expand.
				if (m_pOldMine->uState != STATE_FLAG) {
					OpenAround(m_pOldMine->uRow, m_pOldMine->uCol);
				}
				// check whether the MINEWND around the special MINEWND is a mine, if it is then dead.
				if (ErrorAroundFlag(m_pOldMine->uRow, m_pOldMine->uCol)) {
//					Dead(m_pOldMine->uRow, m_pOldMine->uCol);
					ReleaseCapture();
					return;
				}
			}
			else {
				WritePrivateProfileSection("ERROR", "", "E:\\log.txt");
				// start the game, init the mines area
				if (m_uGameState == GS_WAIT) {
					if (m_uTimer) {
						KillTimer(ID_TIMER_EVENT);
						m_uTimer = 0;
					}
					// the following five lines refresh the remining mine num rect immediately 
					// when click in the mine area at the first time
					m_uSpendTime = 1;
					Invalidate();
					m_uTimer = SetTimer(ID_TIMER_EVENT, 1000, NULL);
					LayMines(m_pOldMine->uRow, m_pOldMine->uCol);		// lay all the mines down 
					m_uGameState = GS_RUN;
				}
				if (m_pOldMine->uOldState == STATE_NORMAL) {
					// out log
					value.Format("%d", m_pOldMine);
					WritePrivateProfileString("ERROR", "ADD", value, "E:\\log.txt");
					value.Format("%d", m_pOldMine->uRow);
					WritePrivateProfileString("ERROR", "ROW", value, "E:\\log.txt");
					value.Format("%d", m_pOldMine->uCol);
					WritePrivateProfileString("ERROR", "COL", value, "E:\\log.txt");
					// end
					// first judge if the special MINEWND is a mine
					if (IsMine(m_pOldMine->uRow, m_pOldMine->uCol)) {
						Dead(m_pOldMine->uRow, m_pOldMine->uCol);
						ReleaseCapture();
						return;
					}
					// the special MINEWND is not a mine 
					around = GetAroundNum(m_pOldMine->uRow, m_pOldMine->uCol);
					// out log
					value.Format("%d", around);
					WritePrivateProfileString("ERROR", "AROUND", value, "E:\\log.txt");
					// end
					if (around == 0) ExpandMines(m_pOldMine->uRow, m_pOldMine->uCol);
					else DrawDownNum(m_pOldMine, around);
				}
				else if (m_pOldMine->uOldState == STATE_DICEY) {
					m_pOldMine->uState = STATE_DICEY;
				}
				if (Victory()) {
					Invalidate();
					ReleaseCapture();
					return;
				}
			}
			break;
		case GS_VICTORY: case GS_DEAD:
			ReleaseCapture();		// release the cursor
			return;
		}
		m_uBtnState = BS_NORMAL;
		Invalidate();
	}
	else {											// click in other area
		if (m_uGameState == GS_WAIT || m_uGameState == GS_RUN) {
			m_uBtnState = BS_NORMAL;
			InvalidateRect(rcBtn);
		}
	}
	
	ReleaseCapture();		// release the cursor
	CWnd::OnLButtonUp(nFlags, point);
}

void CMyMine::OnRButtonDown(UINT nFlags, CPoint point) 
{
	CRect rcBtn(m_uBtnRect[1], 15, m_uBtnRect[2], 39);
	CRect rcMineArea(MINE_AREA_LEFT, MINE_AREA_TOP, 
		MINE_AREA_LEFT + m_uXNum * MINE_WIDTH, MINE_AREA_TOP + m_uYNum * MINE_HEIGHT);
	
	m_bLRBtnDown = FALSE;
	if (rcMineArea.PtInRect(point)) {				// point in mine area
		if (m_uGameState == GS_WAIT || m_uGameState == GS_RUN) {
			m_pNewMine = GetMine(point.x, point.y);
			if (!m_pNewMine) return;
			// both of the left button and the right button are pushing down 
			if (nFlags == (MK_LBUTTON | MK_RBUTTON)) {
				m_bLRBtnDown = TRUE;
				OnLRBtnDown(m_pNewMine->uRow, m_pNewMine->uCol);
			}
			else {
				switch(m_pNewMine->uState) {
				case STATE_NORMAL:
					m_pNewMine->uState = STATE_FLAG;
					m_pNewMine->uOldState = STATE_FLAG;
					m_nLeaveNum--;
					break;
				case STATE_FLAG:
					m_pNewMine->uState = STATE_DICEY;
					m_pNewMine->uOldState = STATE_DICEY;
					m_nLeaveNum++;
					break;
				case STATE_DICEY:
					m_pNewMine->uState = STATE_NORMAL;
					m_pNewMine->uOldState = STATE_NORMAL;
					break;
				}
			}
			Invalidate();
			// check if victory
//			Victory();
		}
	}
	
	CWnd::OnRButtonDown(nFlags, point);
}

void CMyMine::OnRButtonUp(UINT nFlags, CPoint point) 
{
	CRect rcBtn(m_uBtnRect[1], 15, m_uBtnRect[2], 39);
	CRect rcMineArea(MINE_AREA_LEFT, MINE_AREA_TOP, 
		MINE_AREA_LEFT + m_uXNum * MINE_WIDTH, MINE_AREA_TOP + m_uYNum * MINE_HEIGHT);
	
	m_pOldMine = GetMine(point.x, point.y);
	if (!m_pOldMine) return;
	// judge whether the lr button are both pushed down
	if (m_bLRBtnDown) {
		m_bLRBtnDown = FALSE;
		OnLRBtnUp(m_pOldMine->uRow, m_pOldMine->uCol);
		if (m_uGameState == GS_WAIT) {
			m_uBtnState = BS_NORMAL;
			Invalidate();
			return;
		}
		// if the around flags number equal to the around mines number, expand.
		if (m_pOldMine->uState != STATE_FLAG) {
			OpenAround(m_pOldMine->uRow, m_pOldMine->uCol);
		}
		// check whether the MINEWND around the special MINEWND is a mine, if it is then dead.
		if (ErrorAroundFlag(m_pOldMine->uRow, m_pOldMine->uCol)) {
//			Dead(m_pOldMine->uRow, m_pOldMine->uCol);
//			ReleaseCapture();
			return;
		}
	}
	else {
		Victory();
	}
				
	CWnd::OnRButtonUp(nFlags, point);
}

void CMyMine::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (nFlags == MK_LBUTTON || nFlags == (MK_LBUTTON | MK_RBUTTON)) {
		CRect rcBtn(m_uBtnRect[1], 15, m_uBtnRect[2], 39);
		CRect rcMineArea(MINE_AREA_LEFT, MINE_AREA_TOP, 
			MINE_AREA_LEFT + m_uXNum * MINE_WIDTH, MINE_AREA_TOP + m_uYNum * MINE_HEIGHT);
		
		if (rcBtn.PtInRect(point)) {				// point in button area
			switch(m_uGameState) {
			case GS_WAIT: case GS_RUN:
				m_uBtnState = (m_bClickBtn) ? BS_DOWN : BS_CLICK;
				break;
			case GS_DEAD: case GS_VICTORY:
				if (m_bClickBtn) m_uBtnState = BS_DOWN;
				break;
			}
			InvalidateRect(rcBtn);
		}
		else if (rcMineArea.PtInRect(point)) {		// point in mine area
			switch(m_uGameState) {
			case GS_WAIT: case GS_RUN:
				m_pNewMine = GetMine(point.x, point.y);
				if (!m_pNewMine || !m_pOldMine) return;
				if (m_pNewMine->uCol != m_pOldMine->uCol || m_pNewMine->uRow != m_pOldMine->uRow) {
					// change the new mine rect state
					switch(m_pNewMine->uState) {
					case STATE_NORMAL:
						m_pNewMine->uState = STATE_EMPTY;
						break;
					case STATE_DICEY:
						m_pNewMine->uState = STATE_DICEY_DOWN;
						break;
					}
					// resume the old mine rect state
					switch(m_pOldMine->uOldState) {
					case STATE_NORMAL:
						m_pOldMine->uState = STATE_NORMAL;
						break;
					case STATE_DICEY:
						m_pOldMine->uState = STATE_DICEY;
						break;
					}
					// judge whether the lr button are pushed down
					if (m_bLRBtnDown) {
						OnLRBtnUp(m_pOldMine->uRow, m_pOldMine->uCol);
						OnLRBtnDown(m_pNewMine->uRow, m_pNewMine->uCol);
					}
					m_pOldMine = m_pNewMine;
				}
				InvalidateRect(rcMineArea);
				break;
			case GS_VICTORY: case GS_DEAD:
				return;
			}
		}
		else {										// point in other area
			switch(m_uGameState) {
			case GS_WAIT: case GS_RUN:
				m_uBtnState = (m_bClickBtn) ? BS_NORMAL : BS_CLICK;
				if (m_pNewMine) {
					if (m_pNewMine->uOldState == STATE_NORMAL) {
						m_pNewMine->uState = STATE_NORMAL;
					}
					else if (m_pNewMine->uOldState == STATE_DICEY) {
						m_pNewMine->uState = STATE_DICEY;
					}
				}
				break;
			case GS_DEAD: 
				m_uBtnState = BS_DEAD;
				break;
			case GS_VICTORY:
				m_uBtnState = BS_VICTORY;
				break;
			}
			Invalidate();
		}
	}
	
	CWnd::OnMouseMove(nFlags, point);
}

