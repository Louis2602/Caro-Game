#pragma once
#include <iostream>
#include <Random>
#include <conio.h>
#include <fstream>
#include "Menu.h"
#include "Board.h"
#include "Point.h"

#define SIZE 14
#define LEFT 4
#define TOP 1

using namespace std;
class Game
{
private:
	Board* _b;       //Bảng cờ carô
	string _p1Name;   //Tên người chơi 1
	string _p2Name;   //Tên người chơi 2
	int _countP1Wins; //Số lần người chơi 1 thắng
	int _countP2Wins; //Số lần người chơi 2 thắng
	int _countRounds; //Tổng số ván cờ
	bool _finish;     //1: Ván cờ đã kết thúc, 0: ván cờ chưa kết thúc 
	bool _turn;       //1: lượt người chơi 1, 0: lượt người chơi 2
	int _x, _y;       //Vị trí x, y trên màn hình console
	bool _changeTurn; //Cờ thay đổi khi đổi lượt
	bool _loop;       //1: Chơi tiếp, 0: dừng lại
	bool _showCursor; //Cờ để ẩn, hiện con trỏ
	int _mode;        //0: chế đồ người-người, 1: chế độ người-máy(dễ), 2: chế độ người-máy(khó)
	bool _loadSymbols;//1: load lại các kí hiệu trên bàn cơ khi chưa kết thúc ván cờ
public:
	Game();                             //Phương thức tạo lập
	~Game();							 //Phương thức hủy
	void startGame();					 //Bắt đầu trò chơi
	int processFinish();			     //Xử lí kết thúc
	void processCheckBoard();            //Kiểm tra thắng, thua
	void moveRight();					 //Di chuyển phải
	void moveLeft();					 //Di chuyển trái
	void moveUp();						 //Di chuyển lên
	void moveDown();					 //Di chuyển xuống
	void printTurnSymbol();				 //In kí hiệu gợi ý lượt
	void printInterface();				 //Giao diện ván cờ
	void setUpGame(int, string = "");    //Thiết lập game
	void saveData();					 //Lưu dữ liệu ván cờ
	void moveToDirection(int, int);      //Di chuyển đến vị trí(máy)
	void printWinPos();					 //In ra vị trí thắng
	void printLoadedSymbols();			 //Load các kí hiệu X,O
	void askContinue();				     //Hỏi tiếp tục ván cờ
	void printP1Win();					 //In ra người chơi 1 thắng
	void printP2Win();					 //In ra người chơi 2 thắng
	void printBotWin();					 //In ra bot thắng
	void printDraw();					 //In ra kết quả hòa
};