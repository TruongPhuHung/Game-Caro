#include "View.c"
#include "Model.c"

void StartGame() {
	system("cls");
	ResetData();
	DrawBoard(BOARD_SIZE);
}

void ExitGame() {
	system("cls");
	GabageCollect();
}