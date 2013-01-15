#ifndef _MINEDEF
#define _MINEDEF
// regedit defines
#define DEFAULT_RECORD		999
// frame defines
#define	DEFAULT_FRAME_X		6
#define DEFAULT_FRAME_Y		52/*63*/

#define LINE_WIDTH_0		3					// side width of the client area
#define LINE_WIDTH_1		2

#define SIDE_WIDTH_0		6
#define SIDE_WIDTH_1		5

#define SHELL_S_H			37
#define SHELL_S_START_X		9
#define SHELL_S_START_Y		9
#define SHELL_L_START_X		9
#define SHELL_L_START_Y		52

#define MINEAREA_FRAME_X	12
#define MINEAREA_FRAME_Y	55
// mine rect defines
#define MINE_WIDTH			16
#define MINE_HEIGHT			16
#define MINE_AREA_LEFT		12
#define MINE_AREA_TOP		55		

#define STATE_NORMAL		0
#define STATE_FLAG			1
#define STATE_DICEY			2
#define STATE_BLAST			3
#define STATE_ERROR			4
#define STATE_MINE			5
#define STATE_DICEY_DOWN	6
#define STATE_NUM8			7
#define STATE_NUM7			8
#define STATE_NUM6			9
#define STATE_NUM5			10
#define STATE_NUM4			11
#define STATE_NUM3			12
#define STATE_NUM2			13
#define STATE_NUM1			14
#define STATE_EMPTY			15

#define ATTRIB_EMPTY		0
#define ATTRIB_MINE			1
// color defines
#define COLOR_BLACK			RGB(0, 0, 0)
#define COLOR_DARK_GRAY		RGB(128, 128, 128)
#define COLOR_GRAY			RGB(192, 192, 192)
#define COLOR_WHITE			RGB(255, 255, 255)
// game state defines
#define GS_WAIT				0
#define GS_RUN				1
#define GS_DEAD				2
#define GS_VICTORY			3
// game level defines
#define GL_PRIMARY			0
#define GL_SECONDRY			1
#define GL_ADVANCE			2
#define GL_CUSTOM			3
// button rect state defines
#define BS_DOWN				0
#define BS_VICTORY			1
#define BS_DEAD				2
#define BS_CLICK			3
#define BS_NORMAL			4
// dead code defines
#define DC_STRIKE_MINE		0
#define DC_ERROR_MINE		1

#endif