#include<iostream>
#include<easyx.h>
#include<conio.h>
using namespace std;

#define COL 9
#define ROW 10

#define INTERVAL 43
#define GRID_SIZE 67

//

//棋子类型
enum Pieces
{
	null=-1,
	車,馬,象,士,将,砲,卒,俥,马,相,仕,帥,炮,兵,
	BEGIN,END,
	
};
enum Pieces redChess[] = { 車,馬,象,士,将,砲,卒 };
enum Pieces blackChess[] = { 俥,马,相,仕,帥,炮,兵 };
const char* ChessName[] = { "車","馬","象","士","将","砲","卒","俥","马","相","仕","帥","炮","兵" };

struct Chess {
	enum Pieces id;//chess name
	DWORD type;//red or black
	DWORD chesstype;
	short x;
	short y;
	bool isOverRiver;
	bool isChoose;
};
Chess map[ROW][COL];
Chess withdrawChess;
struct State {
	int beginX;
	int beginY;
	int endX;
	int endY;
	int state;
}state = { -1,-1,-1,-1,BEGIN };

void ChessMove();
void init() {
	int tmp = 0;
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			map[i][j].id = null;
			map[i][j].chesstype = WHITE;
			if (i <= 4) {
				map[i][j].type = BLACK;
				if (i==0&&j <= 4) {
					tmp = j;
					map[i][j].id = blackChess[tmp];
					map[i][j].chesstype = BLACK;
				}
				if (i == 0 && j > 4) {
					tmp = 4 - (j - 4);
					map[i][j].id = blackChess[tmp];
					map[i][j].chesstype = BLACK;
				}
				if (i == 2 && (j == 1 || j == 7))
				{
					map[i][j].id = blackChess[5];
					map[i][j].chesstype = BLACK;
				}
			    if(i==3&&j%2==0)
				{
					map[i][j].id = blackChess[6];
					map[i][j].chesstype = BLACK;
				}
			
			}
			else {
				map[i][j].type = RED;
				if (i == 9 && j <= 4) {
					tmp = j;
					map[i][j].id = redChess[tmp];
					map[i][j].chesstype = RED;
				}
				if (i == 9 && j > 4) {
					tmp = 4 - (j - 4);
					map[i][j].id = redChess[tmp];
					map[i][j].chesstype = RED;
				}
				if (i == 7 && (j == 1 || j == 7))
				{
					map[i][j].id = redChess[5];
					map[i][j].chesstype = RED;
				}
				if (i == 6 && j % 2 == 0)
				{
					map[i][j].id = redChess[6];
					map[i][j].chesstype = RED;
				}

			}
			map[i][j].isOverRiver = false;
			map[i][j].x = INTERVAL + j * GRID_SIZE;
			map[i][j].y = INTERVAL + i * GRID_SIZE;
			map[i][j].isChoose = false;
			
		}
		
	}
}
void show() {
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			printf("%3d", map[i][j].id);
			
		}
		cout << endl;
	}
}
void draw() {
	
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			if (map[i][j].id == null)continue;
			settextcolor(map[i][j].chesstype);
			setlinecolor(map[i][j].chesstype);
			int increamemt = 0;
			if (map[i][j].isChoose)increamemt += 7;
			fillcircle(map[i][j].x, map[i][j].y-increamemt, 30);
			fillcircle(map[i][j].x, map[i][j].y- increamemt, 28);
			outtextxy(map[i][j].x-15, map[i][j].y-15- increamemt, ChessName[map[i][j].id]);
		}
		
	}
}
void MouseEvent() {
	
	ExMessage msg;
	if (peekmessage(&msg,EX_MOUSE)) {
		//cout << 1;
		if (msg.message == WM_LBUTTONDOWN)
		{
			
			int col=(msg.x - INTERVAL) / GRID_SIZE;
			int row=(msg.y-INTERVAL)/ GRID_SIZE;
			if (msg.x > map[row][col].x + 30 && msg.y < map[row][col].y + 30) {
				col++;
			}
			if (msg.x < map[row][col].x + 30 && msg.y > map[row][col].y + 30) {
				row++;
			}
			if (msg.x > map[row][col].x + 30 && msg.y > map[row][col].y + 30) {
				row++;
				col++;
			}
			if (state.state == BEGIN) {
				state.beginX = col;
				state.beginY = row;
				state.state = END;
				
			}
			else {
				state.endX = col;
				state.endY = row;
				state.state = BEGIN;
				
			}
			//cout << 2;
			ChessMove();
		}
	}

}
bool CanWithDraw = false;
bool handleMove_Car() {
	if (state.beginX == state.endX||state.beginY == state.endY) {
		//遍历中间是否有其他棋子
		int mix = min(state.beginX, state.endX);
		int miy = min(state.beginY, state.endY);
		int mx = max(state.beginX, state.endX);
		int my = max(state.beginY, state.endY);
		
		if(state.beginY == state.endY)
		{
			for (int i = mix; i <= mx; i++) {
				if (i!=state.beginX&&map[state.beginY][i].id != null)
				{
					if (i == state.endX && map[state.beginY][i].chesstype != map[state.beginY][state.beginX].chesstype)continue;
					return false;
				}
			}
		}
		else
		{
			for (int i = miy; i <= my; i++) {
				if (i != state.beginY && map[i][state.beginX].id != null)
				{
					if (i == state.endY && map[i][state.beginX].chesstype != map[state.beginY][state.beginX].chesstype)continue;
					return false;
				}
			}
		}
		
		return true;
	}
	return false;
}
int direction_horse[8][4] = { {1,-2,0,-1},{-1,-2,0,-1},
	{1,2,0,1},{-1,2,0,1},
{-2,-1,-1,0},{-2,1,-1,0},
	{2,-1,1,0},{2,1,1,0} };
bool handleMove_horse() {
	
	for (int i = 0; i < 8; i++) {
		int y = state.beginY + direction_horse[i][0];
		int x = state.beginX + direction_horse[i][1];
		int stumble_y= state.beginY + direction_horse[i][2];
		int stumble_x= state.beginX+ direction_horse[i][3];
		if (state.endY == y &&
			state.endX == x &&
			map[stumble_y][stumble_x].id == null
			&&map[state.endY][state.endX].chesstype!=map[state.beginY][state.beginX].chesstype)
		{
			return true;
		}
	}
	return false;
}
int direction_elephant[4][4] = {
	{-2,-2,-1,-1},{2,-2,1,-1},
	{-2,2,-1,1},{2,2,1,1}
};
bool handleMove_elephant() {
	for (int i = 0; i < 4; i++) {
		int y = state.beginY + direction_elephant[i][0];
		int x = state.beginX + direction_elephant[i][1];
		int stumble_y = state.beginY + direction_elephant[i][2];
		int stumble_x = state.beginX + direction_elephant[i][3];
		if (state.endY == y &&
			state.endX == x &&
			map[stumble_y][stumble_x].id == null&&
			map[state.endY][state.endX].chesstype != map[state.beginY][state.beginX].chesstype&&
			map[state.endY][state.endX].type== map[state.beginY][state.beginX].type
			)
		{
			return true;
		}
	}
	return false;
}
bool handleMove_cannon() {
	if (state.beginX == state.endX || state.beginY == state.endY) {
		//遍历中间是否有其他棋子
		int mix = min(state.beginX, state.endX);
		int miy = min(state.beginY, state.endY);
		int mx = max(state.beginX, state.endX);
		int my = max(state.beginY, state.endY);
		int count = 0;
		if (state.beginY == state.endY)
		{
			
			for (int i = mix; i <= mx; i++) {
				if (i != state.beginX && map[state.beginY][i].id != null)
				{

					if (i == state.endX)
					{	
						continue;
					}

					count++;
				}
			}
			
		}
		else
		{
			for (int i = miy; i <= my; i++) {
				if (i != state.beginY && map[i][state.beginX].id != null)
				{
					if (i == state.endY)continue;
					
					count++;
				}
			}
		}

		if (count <= 1)return true;
	}
	return false;
}
bool handleMove_soldier() {
	int judge;//-1 represent redChess,1 represent blackChess
	if (map[state.beginY][state.beginX].chesstype == RED)judge = -1;
	else
		judge = 1;
	if (map[state.beginY][state.beginX].isOverRiver) {
		if (state.beginX == state.endX && state.endY - state.beginY == judge)
		{
			return true;
		}
		if (state.beginY == state.endY && abs(state.beginX - state.endX) == 1)
		{
			return true;
		}
	}
	else {
		if (state.beginX == state.endX && state.endY - state.beginY == judge)
		{
			return true;
		}
	}
	return false;
}
int direction_guard[4][2] = {
	{1,1},{1,-1},{-1,1},{-1,-1}
};
bool handleMove_guard() {
	int centery;
	int centerx=4;
	if (map[state.beginY][state.beginX].chesstype == RED) {
		centery = 8;
	}
	else {
		centery = 1;
	}
	if (abs(centerx - state.endX) <= 1 && abs(centery - state.endY) <= 1) {
		for (int i = 0; i < 4; i++) {
			int y = state.beginY + direction_guard[i][0];
			int x = state.beginX + direction_guard[i][1];
			if (state.endX == x && state.endY == y)return true;
		}
	}
	return false;
}
bool handleMove_general() {
	int centery;
	int centerx = 4;
	if (map[state.beginY][state.beginX].chesstype == RED) {
		centery = 8;
	}
	else {
		centery = 1;
	}
	if (abs(centerx - state.endX) <= 1 && abs(centery - state.endY) <= 1) {
		int absy = abs(state.endY - state.beginY);
		int absx = abs(state.endX - state.beginX);
		if ((absy == 1 && absx == 0) || (absy == 0 && absx == 1))return true;
	}
	return false;
}
void ChessMove() {
	//cout << 1;
	if (state.state == BEGIN &&
		state.endX != -1 && state.endY != -1 &&
		map[state.beginY][state.beginX].id != null &&
		!(state.beginX == state.endX && state.beginY == state.endY)
		&& map[state.beginY][state.beginX].chesstype!= map[state.endY][state.endX].chesstype
		)
	{

		bool CanMove = false;
		switch (map[state.beginY][state.beginX].id) {
		case 車:
		case 俥:
		{
			if (handleMove_Car())CanMove = true;
			break;
		}
		case 馬:
		case 马:
		{
			CanMove = handleMove_horse();
			break;
		}
		case 象:
		case 相:
		{
			CanMove = handleMove_elephant();
			break;
		}
		case 砲:
		case 炮:
		{
			CanMove = handleMove_cannon();
			break;
		}
		case 卒:
		case 兵:
		{
			CanMove = handleMove_soldier();
			break;
		}
		case 士:
		case 仕:
		{
			CanMove = handleMove_guard();
			break;
		}
		case 将:
		case 帥:
		{
			CanMove = handleMove_general();
			break;
		}
		default:
			break;
		}
		
		if(CanMove)
		{
			withdrawChess.id = map[state.endY][state.endX].id;
			//withdrawChess.type = map[state.endY][state.endX].type;
			withdrawChess.isOverRiver = map[state.endY][state.endX].isOverRiver;
			withdrawChess.chesstype = map[state.endY][state.endX].chesstype;

			map[state.endY][state.endX].id = map[state.beginY][state.beginX].id;
			//map[state.endY][state.endX].type = map[state.beginY][state.beginX].type;

			map[state.endY][state.endX].isOverRiver = map[state.beginY][state.beginX].isOverRiver;
			map[state.endY][state.endX].chesstype = map[state.beginY][state.beginX].chesstype;

			map[state.beginY][state.beginX].id = null;
			map[state.beginY][state.beginX].chesstype = WHITE;

			CanWithDraw = true;
			if (map[state.endY][state.endX].type != map[state.beginY][state.beginX].type)map[state.endY][state.endX].isOverRiver = true;
		}
		
	}
	if(state.state == BEGIN)map[state.beginY][state.beginX].isChoose = false;

	if (state.state == END && map[state.beginY][state.beginX].id != null) {
		map[state.beginY][state.beginX].isChoose = true;
	}
	if(state.state==BEGIN&&(state.beginX == state.endX && state.beginY == state.endY))map[state.beginY][state.beginX].isChoose = false;
}
void withdraw() {
	if (!CanWithDraw)return;
	map[state.beginY][state.beginX].id = map[state.endY][state.endX].id;
	map[state.endY][state.endX].id = withdrawChess.id;
	map[state.endY][state.endX].type = withdrawChess.type;
	map[state.endY][state.endX].isOverRiver = withdrawChess.isOverRiver;

	CanWithDraw = false;
}
void KeyBoardEvent() {
	if (_kbhit())
	{
		switch (_getch())
		{
		case 'z':
			withdraw();
			break;
		case 'x':
			show();
			break;
		case 'a':
			
			break;
		case 'd':
			
			break;
		default:
			break;
		}
	}
}
void pushButton(int x,int y, LPCTSTR text) {
	int h = textheight(text);
	int w = textwidth(text);
	fillrectangle(x, y,x+120,y+40);
	int ty = (40 - h) / 2 + y;
	int tx = (120 - w) / 2 + x;
	outtextxy(tx, ty, text);
}
void startInterface() {
	pushButton(253, 279, "开始游戏");
	pushButton(253, 279+50, "说明");
	pushButton(253, 279+100, "退出游戏");

}
IMAGE img_board;
bool startMouseChooseEvent() {
	ExMessage msg;
	if (peekmessage(&msg, EX_MOUSE)) {
		//cout << 1;
		if (msg.message == WM_LBUTTONDOWN)
		{
			int x = msg.x;
			int y = msg.y;
			if (x > 253 && x < 373) {
				if (y > 279 && y < 319) {//start game
					init();
					BeginBatchDraw();
					while (1)
					{
						cleardevice();
						putimage(-36, 0, &img_board);
						draw();
						MouseEvent();
						KeyBoardEvent();

						//Sleep(30);
						FlushBatchDraw();

					}
					EndBatchDraw();
				}
				if (y > 329 && y < 369) {//introduction of game operation

				}
				if (y > 379 && y < 419) {//quit game
					return true;
				}
			}
		}
	}
	return false;
}
int main() {
	
	initgraph(626, 698);
	
	loadimage(&img_board, _T("./chess.png"));
	putimage(-36, 0, &img_board);
	settextstyle(30, 0, "楷体");
	setlinestyle(PS_SOLID, 2);
	setfillcolor(RGB(252, 215, 162));
	
	setbkmode(TRANSPARENT);
	startInterface();
	while (1) {
		if (startMouseChooseEvent())break;
	}
	
	//show();
	
	return 0;
}