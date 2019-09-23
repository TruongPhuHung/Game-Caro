#include <stdio.h>
#include <windows.h>
#include <ctype.h>
#include <stdbool.h>

void FixConsoleWindow() {
	HWND consoleWindow = GetConsoleWindow();
	LONG style = GetWindowLong(consoleWindow, GWL_STYLE);
	style = style & ~(WS_MAXIMIZEBOX) & ~(WS_THICKFRAME);
	SetWindowLong(consoleWindow, GWL_STYLE, style);
}

void GotoXY(int x, int y) {
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

#define BOARD_SIZE 12
#define LEFT 3
#define TOP 1

struct _POINT { int x, y, c; };
struct _POINT _A[BOARD_SIZE][BOARD_SIZE];
bool _TURN;// TRUE LÀ LƯỢT NGƯỜI THỨ NHẤT, FALSE LÀ LƯỢT NGƯỜI THỨ HAI
int _COMMAND;	// NHẬN GIÁ TRỊ NGƯỜI DÙNG NHẬP
int _X, _Y; // TỌA ĐỘ HIỆN TẠI CỦA CON TRỎ
int IndexX, IndexY;	// CHỈ SỐ VỊ TRÍ ĐÁNH DẤU CUỐI CÙNG

void ResetData() {
	for (int i = 0; i < BOARD_SIZE; i++)
		for (int j = 0; j < BOARD_SIZE; j++) {
			_A[i][j].x = 4 * j + LEFT + 2;
			_A[i][j].y = 2 * i + TOP + 1;
			// 0 means has no sign, -1 is true sign, 1 is false sign
			_A[i][j].c = 0;
		}
	_TURN = true; _COMMAND = -1;
	_X = _A[0][0].x, _Y = _A[0][0].y;
}

void DrawBoard(int pSize) {
	for (int i = 0; i <= pSize; i++) {
		for (int j = 0; j <= pSize; j++) {
			GotoXY(LEFT + 4 * j, TOP + 2 * i);
			printf(".");
		}
	}
}

void StartGame() {
	system("cls");
	ResetData();
	DrawBoard(BOARD_SIZE);
}

void GabageCollect() {

}

void ExitGame() {
	system("cls");
	GabageCollect();
}

int ProcessFinish(int pwhowin) {
	GotoXY(0, _A[BOARD_SIZE - 1][BOARD_SIZE - 1].y + 2);
	switch (pwhowin) {
	case -1:
		printf("Nguoi choi %d da thang va nguoi choi %d da thua\n", true, false);
		break;
	case 1:
		printf("Nguoi choi %d da thang va nguoi choi %d da thua\n", false, true);
		break;
	case 0:
		printf("Nguoi choi %d da hoa nguoi choi %d\n", false, true);
		break;
	case 2:
		_TURN = !_TURN;
	}
	GotoXY(_X, _Y);
	return pwhowin;
}

int AskContinue() {
	GotoXY(0, _A[BOARD_SIZE - 1][BOARD_SIZE - 1].y + 4);
	printf("Nhan 'y/n' de tiep tuc/dung: ");
	return toupper(getch());
}

int TestBoard() {
	if (isFullBoard() == 1)
		return 0;
	else {
		if (VictoryCondition(5) == 1)
			return (_TURN == true ? -1 : 1);
		else
			return 2;
	}
}

int CheckOnBoard(int pX, int pY) {
	for(int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			if (_A[i][j].x == pX && _A[i][j].y == pY && _A[i][j].c == 0) {
				if (_TURN == true)
					_A[i][j].c = -1;		//Nếu là lượt của TRUE thì c= -1
				else
					_A[i][j].c = 1;			//Nếu là lượt của FALSE thì c= 1
				return _A[i][j].c;
			}
		}
	}
	return 0;
}

void GetLastPosition (int LastX, int LastY) {
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			if (_A[i][j].x == LastX && _A[i][j].y == LastY) {
				IndexX = i;
				IndexY = j;
			}
		}
	}
}

int isFullBoard() {
	int NumsOfCheckedShell = 0;
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			if (_A[i][j].c != 0)
				NumsOfCheckedShell++;
		}
	}
	if (NumsOfCheckedShell == BOARD_SIZE * BOARD_SIZE)
		return 1;
	return 0;
}
/*Kiểm tra xem quân cờ đánh cuối cùng có đủ 5 quân cờ trên cùng 1 hàng hay không. 
- Lấy ra vị trí của quân cờ cuối cùng trong bảng
- Lấy ra giá trị của quân cờ đó
- Kiểm tra trên cùng 1 hàng ngang, dọc, chéo có đủ 5 quân cùng loại hay không*/

int MainCross() {
	int NumsOfCheckShellOnTheLine = 0;
	for (int i = 0; i < 5; i++) {
		if (_A[IndexX + i][IndexY + i].c == _A[IndexX][IndexY].c)
			NumsOfCheckShellOnTheLine++;
		else break;
	}
	for (int i = 1; i < 5; i++) {
		if (_A[IndexX - i][IndexY - i].c == _A[IndexX][IndexY].c)
			NumsOfCheckShellOnTheLine++;
		else break;
	}
	return NumsOfCheckShellOnTheLine;
}

int SubCross() {
	int NumsOfCheckShellOnTheLine = 0;
	for (int i = 0; i < 5; i++) {
		if (_A[IndexX + i][IndexY - i].c == _A[IndexX][IndexY].c)
			NumsOfCheckShellOnTheLine++;
		else break;
	}
	for (int i = 1; i < 5; i++) {
		if (_A[IndexX - i][IndexY + i].c == _A[IndexX][IndexY].c)
			NumsOfCheckShellOnTheLine++;
		else break;
	}
	return NumsOfCheckShellOnTheLine;
}

int Horizon() {
	int NumsOfCheckShellOnTheLine = 0;
	for (int i = 0; i < 5; i++) {
		if (_A[IndexX + i][IndexY].c == _A[IndexX][IndexY].c)
			NumsOfCheckShellOnTheLine++;
		else break;
	}
	for (int i = 1; i < 5; i++) {
		if (_A[IndexX - i][IndexY].c == _A[IndexX][IndexY].c)
			NumsOfCheckShellOnTheLine++;
		else break;
	}
	return NumsOfCheckShellOnTheLine;
}

int Vertical() {
	int NumsOfCheckShellOnTheLine = 0;
	for (int i = 0; i < 5; i++) {
		if (_A[IndexX][IndexY + i].c == _A[IndexX][IndexY].c)
			NumsOfCheckShellOnTheLine++;
		else break;
	}
	for (int i = 1; i < 5; i++) {
		if (_A[IndexX][IndexY - i].c == _A[IndexX][IndexY].c)
			NumsOfCheckShellOnTheLine++;
		else break;
	}
	return NumsOfCheckShellOnTheLine;
}

int VictoryCondition(int NumCheckShell) {
	if(MainCross() == NumCheckShell || SubCross() == NumCheckShell || Horizon() == NumCheckShell || Vertical() == NumCheckShell)
		return 1;
	return 0;
}

//**********************************
// NHÓM HÀM PHÍM ĐIỀU KHIỂN

void MoveRight() {
	if (_X < _A[BOARD_SIZE - 1][BOARD_SIZE - 1].x) {
		_X += 4;
		GotoXY(_X, _Y);
	}
}

void MoveLeft() {
	if (_X > _A[0][0].x) {
		_X -= 4;
		GotoXY(_X, _Y);
	}
}

void MoveDown() {
	if (_Y < _A[BOARD_SIZE - 1][BOARD_SIZE - 1].y) {
		_Y += 2;
		GotoXY(_X, _Y);
	}
}

void MoveUp() {
	if (_Y > _A[0][0].y) {
		_Y -= 2;
		GotoXY(_X, _Y);
	}
}

void GameControl() {
	bool validEnter = true;
	while (1) {
		_COMMAND = toupper(getch());
		if (_COMMAND == 27) {
			ExitGame();
			return;
		}
		else {
			if ((_COMMAND == 'A') || (_COMMAND == 'J')) MoveLeft();
			else if (_COMMAND == 'W') MoveUp();
			else if (_COMMAND == 'S') MoveDown();
			else if (_COMMAND == 'D') MoveRight();
			else if (_COMMAND == 13) {
				switch (CheckOnBoard(_X, _Y)) {
				case -1:
					printf("X"); break;
				case 1:
					printf("O"); break;
				case 0: validEnter = false; // KHÔNG CHO PHÉP ĐÁNH VÀO Ô ĐÃ ĐÁNH
				}
				GetLastPosition(_X, _Y);
				if (validEnter == true) {
					switch (ProcessFinish(TestBoard())) {
					case -1: case 1: case 0:
						if (AskContinue() != 'Y') {
							ExitGame();
							return 0;
						}
						else StartGame();
					}
				}
				validEnter = true;
			}
		}
	}
}

void MainMenu() {

}

void PropertiesWindow() {

}



void main() {
	FixConsoleWindow();
	StartGame();
	GameControl();
}