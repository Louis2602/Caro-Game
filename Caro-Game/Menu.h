#pragma once
#include <iostream>
#include <string>
#include <unordered_map>
#include <filesystem>
#include "Common.h"
#include "Game.h"
using namespace std;

class Menu
{
private:
	static bool exitGame;				//Cờ thoát game
	static int current_option;			//Lựa chọn hiện tại
	const static string options[8];		//Tên các lựa chọn
public:
	static void mainScreen();								//Màn hình chính
	static void mainMenu();									//Menu
	static void playMenu();									//Menu play
	static void loadScreen();								//Màn hình load file
	static void helpScreen();								//Màn hình hướng dẫn
	static void exitScreen();								//Màn hình thoát
	static void playPvP();									//Chế độ người-người
	static void playPvC1();									//Chế độ người-máy(dễ)
	static void playPvC2();									//Chế độ người-máy(khó)
	static void printLogo();								//In ra logo
	static void goBack() {}									//Trở về menu
	static void printOptionsBoard();						//In ra các bảng lựa chọn
	static void printAnimation();							//In hiệu ứng mở đầu
	static void printRectangle(int, int, int, int);			//Vẽ hình chữ nhật
	static void changeOption(bool, bool);					//Thay đổi lựa chọn
	static void changeFile(int, vector<string>&, int&);		//Thay đổi file
};