#include "Game.h"

Game::Game()
{
	_mode = -1;
	_x = LEFT, _y = TOP;
	_b = new Board(SIZE, LEFT, TOP);
	_loop = _turn = _changeTurn = true;
	_finish = false;
	_showCursor = false;
	_p1Name = _p2Name = "";
	_countP1Wins = _countP2Wins = 0;
	_loadSymbols = false;
}

Game::~Game()
{
	delete _b;
	_b = nullptr;
}

void Game::startGame()
{
	Common::clearConsole();
	while (_loop)
	{
		bool pause = false;
		_finish = false;
		printInterface();
		_x = _b->getXAt(5, 6);
		_y = _b->getYAt(5, 6);
		Common::gotoXY(_x, _y);
		moveDown();
		while (!_finish && !pause)
		{
			if (_mode == 0 || (_turn == 1 && _countRounds % 2 == 0) || (_turn == 0 && _countRounds%2==1))
			{
				switch (Common::getConsoleInput())
				{
				case 0:
					Common::playSound(4);
					break;
				case 1:
					saveData();
					Menu::exitScreen();
					return;
				case 2:
					moveUp();
					break;
				case 3:
					moveLeft();
					break;
				case 4:
					moveRight();
					break;
				case 5:
					moveDown();
					break;
				case 6:
					processCheckBoard();
					break;
				case 7:
					pause = 1;
					_loadSymbols = 1;
					Menu::helpScreen();
					_showCursor = false;
					break;
				}
			}
			else
			{
				Point p = (_mode == 1) ? _b->PVC_easy() : _b->PVC_hard();
				moveToDirection(p.getX(), p.getY());
				processCheckBoard();
			}
		}
		if (pause)
			continue;
		_countRounds++;
		askContinue();
		if (_loop)
		{
			swap(_p1Name, _p2Name);
			swap(_countP1Wins, _countP2Wins);
		}
	}
	saveData();
}

void Game::processCheckBoard()
{
	int c = _b->checkBoard(_x, _y, _turn);
	if (c == -1 || c == 1)
	{
		Common::playSound(3);
		Common::gotoXY(_x, _y);
		if (c == 1)
		{
			Common::setConsoleColor(BRIGHT_WHITE, BLUE);
			putchar(88);
			Common::gotoXY(86, 8);
			cout << _b->getCountX();
		}
		else
		{
			Common::setConsoleColor(BRIGHT_WHITE, RED);
			putchar(79);
			Common::gotoXY(86, 21);
			cout << _b->getCountO();
		}
		int result = processFinish();
		if (result == 2)
		{
			int color[] = { RED, BLUE }, pos[] = { 22,9 };
			Common::setConsoleColor(BRIGHT_WHITE, BLACK);
			Common::gotoXY(83, pos[_turn]);
			cout << "IN TURN!";
			Common::gotoXY(83, pos[!_turn]);
			cout << "        ";
			Common::gotoXY(_x, _y);
			_showCursor = true;
			Common::showCursor(_showCursor);
		}
	}
	else
	{
		if (_mode == 0)
			Common::playSound(5);
	}
}

int Game::processFinish()
{
	Common::gotoXY(0, _b->getYAt(_b->getSize() - 1, _b->getSize() - 1) + 2);
	int pWhoWin = _b->testBoard(_x, _y);
	switch (pWhoWin)
	{
	case -1:
		_finish = 1;
		_countP2Wins++;
		printWinPos();
		if (_p2Name != "Computer")
			printP2Win();
		else
			printBotWin();
		break;
	case 1:
		_finish = 1;
		_countP1Wins++;
		printWinPos();
		if (_p1Name != "Computer")
			printP1Win();
		else
			printBotWin();
		break;
	case 0:
		_finish = 1;
		printDraw();
		break;
	case 2:
		_turn = !_turn; // change turn if nothing happen
		_changeTurn = 1;
	}
	return pWhoWin;
}

void Game::moveRight()
{
	if (_x < _b->getXAt(_b->getSize() - 1, _b->getSize() - 1))
	{
		Common::playSound(_turn);
		if (_b->getCheckAtXY(_x, _y) == 0)
		{
			Common::gotoXY(_x, _y);
			putchar(32);
		}
		_x += 4;
		Common::gotoXY(_x, _y);
		printTurnSymbol();
	}
	else
	{
		Common::playSound(4);
	}
}

void Game::moveLeft()
{
	if (_x > _b->getXAt(0, 0))
	{
		Common::playSound(_turn);
		if (_b->getCheckAtXY(_x, _y) == 0)
		{
			Common::gotoXY(_x, _y);
			putchar(32);
		}
		_x -= 4;
		Common::gotoXY(_x, _y);
		printTurnSymbol();
	}
	else
	{
		Common::playSound(4);
	}
}

void Game::moveDown()
{
	if (_y < _b->getYAt(_b->getSize() - 1, _b->getSize() - 1))
	{
		Common::playSound(_turn);
		if (_b->getCheckAtXY(_x, _y) == 0)
		{
			Common::gotoXY(_x, _y);
			putchar(32);
		}
		_y += 2;
		Common::gotoXY(_x, _y);
		printTurnSymbol();
	}
	else
	{
		Common::playSound(4);
	}
}

void Game::moveUp()
{
	if (_y > _b->getYAt(0, 0))
	{
		Common::playSound(_turn);
		if (_b->getCheckAtXY(_x, _y) == 0)
		{
			Common::gotoXY(_x, _y);
			putchar(32);
		}
		_y -= 2;
		Common::gotoXY(_x, _y);
		printTurnSymbol();
	}
	else
	{
		Common::playSound(4);
	}
}

void Game::printTurnSymbol()
{
	if (_b->getCheckAtXY(_x,_y))
	{
		if (_showCursor == false)
		{
			_showCursor = true;
			Common::showCursor(_showCursor);
		}
	}
	else
	{
		if (_showCursor == true)
		{
			_showCursor = false;
			Common::showCursor(_showCursor);
		}
		if (_changeTurn == 1)
		{
			if (_turn == 1)
			{
				Common::setConsoleColor(BRIGHT_WHITE, LIGHT_BLUE);
			}
			else
			{
				Common::setConsoleColor(BRIGHT_WHITE, LIGHT_RED);
			}
			_changeTurn = 0;
		}
		if (_turn == 1)
			putchar(120);
		else
			putchar(111);
		Common::gotoXY(_x, _y);
	}
}         
	

void Game::printInterface()
{
	Common::setConsoleColor(BRIGHT_WHITE, BLACK);
	_b->drawBoard();
	if (_loadSymbols)
	{
		printLoadedSymbols();
		_loadSymbols = 0;
	}
	Common::setConsoleColor(BRIGHT_WHITE, BLACK);
	Menu::printRectangle(69, 1, 33, 10);
	Menu::printRectangle(69, 14, 33, 10);
	Common::setConsoleColor(BRIGHT_WHITE, BLUE);
	Common::gotoXY(82, 3);
	cout << "Player 1:";
	Common::gotoXY(86 - _p1Name.length() / 2, 4);
	cout << _p1Name;
	Common::gotoXY(80, 5);
	cout << "Winning score:";
	string score1 = to_string(_countP1Wins) + "/" + to_string(_countRounds);
	Common::gotoXY(86 - score1.find('/'), 6);
	cout << score1;
	Common::gotoXY(84,7);
	cout << "Moves:";
	Common::gotoXY(86, 8);
	cout << _b->getCountX();
	if (_turn)
	{
		Common::gotoXY(83, 9);
		cout << "IN TURN!";
	}
	Common::setConsoleColor(BRIGHT_WHITE, RED);
	Common::gotoXY(82, 16);
	cout << "Player 2:";
	Common::gotoXY(86 - _p2Name.length() / 2, 17);
	cout << _p2Name;
	Common::gotoXY(80, 18);
	cout << "Winning score:";
	string score2 = to_string(_countP2Wins) + "/" + to_string(_countRounds);
	Common::gotoXY(86 - score2.find('/'), 19);
	cout << score2;
	Common::gotoXY(84, 20);
	cout << "Moves:";
	Common::gotoXY(86, 21);
	cout << _b->getCountO();
	Common::setConsoleColor(BRIGHT_WHITE, BLACK);
	if (_turn)
		Common::gotoXY(83, 9);
	else
		Common::gotoXY(83, 22);
	cout << "IN TURN!";
	Menu::printRectangle(69, 27, 14, 2);
	Menu::printRectangle(88, 27, 14, 2);
	Common::setConsoleColor(BRIGHT_WHITE, GREEN);
	Common::gotoXY(73, 28);
	cout << "H : Help";
	Common::setConsoleColor(BRIGHT_WHITE, YELLOW);
	Common::gotoXY(91, 28);
	cout << "Esc : Exit";
}

void Game::moveToDirection(int x, int y)
{
	while (_x < x)
	{
		Sleep(300);
		moveRight();
	}
	while (_x > x)
	{
		Sleep(300);
		moveLeft();
	}
	while (_y < y)
	{
		Sleep(300);
		moveDown();
	}
	while (_y > y)
	{
		Sleep(300);
		moveUp();
	}
}

void Game::setUpGame(int mode, string fileName)
{
	if (mode == 3)
	{
		string fullPath = "load\\" + fileName + ".txt";
		ifstream inFile(fullPath);
		getline(inFile, _p1Name);
		getline(inFile, _p2Name);
		inFile >> _mode;
		inFile >> _countRounds;
		inFile >> _countP1Wins;
		inFile >> _countP2Wins;
		inFile >> _finish;
		if (!_finish)
		{
			_loadSymbols = true;
			_b->loadBoard(inFile);
		}
		else
		{
			_b->resetData();
		}
		inFile.close();
	}
	else
	{
		Common::setConsoleColor(BRIGHT_WHITE, GRAY);
		Common::clearConsole();
		Menu::printLogo();
		_mode = mode;
		Common::setConsoleColor(BRIGHT_WHITE, LIGHT_BLUE);
		Common::gotoXY(35, 18);
		cout << "Enter player 1's name:  ";
		Common::showCursor(true);
		getline(cin, _p1Name);
		if (_mode == 0)
		{
			Common::setConsoleColor(BRIGHT_WHITE, LIGHT_RED);
			Common::gotoXY(35, 21);
			cout << "Enter player 2's name:  ";
			getline(cin, _p2Name);
			Common::showCursor(false);
		}
		else
		{
			_p2Name = "Computer";
		}
		Common::showCursor(false);
		_b->resetData();
	}
}

void Game::printWinPos()
{
	Common::playSound(9);
	Point* win = _b->getWinPos();
	int pWhoWin = _b->testBoard(_x, _y);
	int symbol = (_turn) ? 88 : 79;
	int symbol2 = (_turn) ? 120 : 111;
	int loop = 15;
	while (loop)
	{
		Common::setConsoleColor(BRIGHT_WHITE, loop);
		for (int i = 0; i < 5; i++)
		{
			Common::gotoXY(win[i].getX(), win[i].getY());
			putchar(symbol);
		}
		Sleep(100);
		for (int i = 0; i < 5; i++)
		{
			Common::gotoXY(win[i].getX(), win[i].getY());
			putchar(symbol2);
		}
		Sleep(300);
		loop--;
	}
	delete[] win;
}

void Game::saveData()
{
	string path;
	if (_countRounds % 2 == 1 && (!_finish || _loop))
	{
		path = "load\\" + _p2Name + "-" + _p1Name + ".txt";
	}
	else
	{
		path = "load\\" + _p1Name + "-" + _p2Name + ".txt";
	}
	ofstream outFile(path);
	outFile << _p1Name << endl;
	outFile << _p2Name << endl;
	outFile << _mode << endl;
	outFile << _countRounds << endl;
	outFile << _countP1Wins << endl;
	outFile << _countP2Wins << endl;
	outFile << _finish << endl;
	if (!_finish)
	{
		_b->saveBoard(outFile);
	}
	outFile.close();
}

void Game::printLoadedSymbols()
{
	for (int i = 0; i < _b->getSize(); i++)
	{
		for (int j = 0; j < _b->getSize(); j++)
		{
			_x = _b->getXAt(i, j);
			_y = _b->getYAt(i, j);
			Common::gotoXY(_x, _y);
			int c = _b->getCheckAtXY(_x, _y);
			if (c == 1)
			{
				Common::setConsoleColor(BRIGHT_WHITE, BLUE);
				putchar(88);
			}
			else if(c == -1)
			{
				Common::setConsoleColor(BRIGHT_WHITE, RED);
				putchar(79);
			}
		}
	}
	if (_b->getCountX() > _b->getCountO())
	{
		Common::setConsoleColor(BRIGHT_WHITE, RED);
		_turn = 0;
	}
	else
	{
		Common::setConsoleColor(BRIGHT_WHITE, BLUE);
		_turn = 1;
	}
}

void Game::askContinue()
{
	Common::gotoXY(38, 21);
	Common::setConsoleColor(BRIGHT_WHITE, GREEN);
	cout << "Do you want to play another round?";
	string str[2] = { "Yes", "No" };
	int left[] = { 38,42,48,61,65,70 }, word[] = { 32,32,175,174 }, color[] = { BLACK, GREEN }, top = 24;
	bool choice = 1;
	auto print1 = [&]()
	{
		int i = 0;
		while (i < 2)
		{
			Common::playSound(2);
			Common::setConsoleColor(BRIGHT_WHITE, color[i]);
			Common::gotoXY(left[choice * 3], top);        putchar(word[i * 2]);
			Common::gotoXY(left[choice * 3 + 1], top);    cout << str[choice];
			Common::gotoXY(left[choice * 3 + 2], top);    putchar(word[i * 2 + 1]);
			if (!i++)
				choice = !choice;
		}
	};
	print1();
	while (true)
	{
		int key = Common::getConsoleInput();
		if ((key == 3 && choice == 1) || (key == 4 && choice == 0))
		{
			print1();
		}
		else if (key == 6)
		{
			if (!choice)
			{
				_b->resetData();
				_loop = 1;
				_changeTurn = 1;
			}
			else
			{
				_loop = 0;
			}
			return;
		}
		else
		{
			Common::playSound(4);
		}
	}
}
void Game::printP1Win()
{
	Common::playSound(6);
	Common::setConsoleColor(BRIGHT_WHITE, BLACK);
	Common::clearConsole();
	unsigned char P1_1[] = {
		219, 219, 219, 219, 219, 219, 187, 176, 219, 219, 187, 176, 176, 176, 176, 176, 176, 219, 219, 219, 219, 219, 187, 176, 219, 219, 187, 176, 176, 176, 219, 219, 187, 219, 219, 219, 219, 219, 219, 219, 187, 219, 219, 219, 219, 219, 219, 187, 176, 32, 32, 32, 176, 176, 219, 219, 219, 187, 176, 176,
		219, 219, 201, 205, 205, 219, 219, 187, 219, 219, 186, 176, 176, 176, 176, 176, 219, 219, 201, 205, 205, 219, 219, 187, 200, 219, 219, 187, 176, 219, 219, 201, 188, 219, 219, 201, 205, 205, 205, 205, 188, 219, 219, 201, 205, 205, 219, 219, 187, 32, 32, 32, 176, 219, 219, 219, 219, 186, 176, 176,
		219, 219, 219, 219, 219, 219, 201, 188, 219, 219, 186, 176, 176, 176, 176, 176, 219, 219, 219, 219, 219, 219, 219, 186, 176, 200, 219, 219, 219, 219, 201, 188, 176, 219, 219, 219, 219, 219, 187, 176, 176, 219, 219, 219, 219, 219, 219, 201, 188, 32, 32, 32, 219, 219, 201, 219, 219, 186, 176, 176,
		219, 219, 201, 205, 205, 205, 188, 176, 219, 219, 186, 176, 176, 176, 176, 176, 219, 219, 201, 205, 205, 219, 219, 186, 176, 176, 200, 219, 219, 201, 188, 176, 176, 219, 219, 201, 205, 205, 188, 176, 176, 219, 219, 201, 205, 205, 219, 219, 187, 32, 32, 32, 200, 205, 188, 219, 219, 186, 176, 176,
		219, 219, 186, 176, 176, 176, 176, 176, 219, 219, 219, 219, 219, 219, 219, 187, 219, 219, 186, 176, 176, 219, 219, 186, 176, 176, 176, 219, 219, 186, 176, 176, 176, 219, 219, 219, 219, 219, 219, 219, 187, 219, 219, 186, 176, 176, 219, 219, 186, 32, 32, 32, 219, 219, 219, 219, 219, 219, 219, 187,
		200, 205, 188, 176, 176, 176, 176, 176, 200, 205, 205, 205, 205, 205, 205, 188, 200, 205, 188, 176, 176, 200, 205, 188, 176, 176, 176, 200, 205, 188, 176, 176, 176, 200, 205, 205, 205, 205, 205, 205, 188, 200, 205, 188, 176, 176, 200, 205, 188, 32, 32, 32, 200, 205, 205, 205, 205, 205, 205, 188,
	};
	unsigned char P1_2[] = {
		32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 176, 219, 219, 187, 176, 176, 176, 176, 176, 176, 176, 219, 219, 187, 219, 219, 187, 219, 219, 219, 187, 176, 176, 219, 219, 187, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
		32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 176, 219, 219, 186, 176, 176, 219, 219, 187, 176, 176, 219, 219, 186, 219, 219, 186, 219, 219, 219, 219, 187, 176, 219, 219, 186, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
		32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 176, 200, 219, 219, 187, 219, 219, 219, 219, 187, 219, 219, 201, 188, 219, 219, 186, 219, 219, 201, 219, 219, 187, 219, 219, 186, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
		32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 176, 176, 219, 219, 219, 219, 201, 205, 219, 219, 219, 219, 186, 176, 219, 219, 186, 219, 219, 186, 200, 219, 219, 219, 219, 186, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
		32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 176, 176, 200, 219, 219, 201, 188, 176, 200, 219, 219, 201, 188, 176, 219, 219, 186, 219, 219, 186, 176, 200, 219, 219, 219, 186, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
		32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 176, 176, 176, 200, 205, 188, 176, 176, 176, 200, 205, 188, 176, 176, 200, 205, 188, 200, 205, 188, 176, 176, 200, 205, 205, 188, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
	};
	int top = 6, num_char = 60, count = 0;
	int color[] = { BRIGHT_WHITE, BLACK, BRIGHT_WHITE, BLUE, BRIGHT_WHITE, GREEN, BRIGHT_WHITE, PURPLE };
	while (count++ < 7)
	{
		Common::setConsoleColor(BRIGHT_WHITE, color[count]);
		for (int i = 0; i < 6; i++)
		{
			Common::gotoXY(24, i + top);
			for (int j = 0; j < num_char; j++)
			{
				putchar(P1_1[i * num_char + j]);
			}
			Common::gotoXY(24, i + top + 7);
			for (int j = 0; j < num_char; j++)
			{
				putchar(P1_2[i * num_char + j]);
			}
			Sleep(100);
		}
		Sleep(300);
	}
}
void Game::printP2Win()
{
	Common::playSound(6);
	Common::setConsoleColor(BRIGHT_WHITE, BLACK);
	Common::clearConsole();
	unsigned char P2_1[] = {
		219, 219, 219, 219, 219, 219, 187, 176, 219, 219, 187, 176, 176, 176, 176, 176, 176, 219, 219, 219, 219, 219, 187, 176, 219, 219, 187, 176, 176, 176, 219, 219, 187, 219, 219, 219, 219, 219, 219, 219, 187, 219, 219, 219, 219, 219, 219, 187, 176, 32, 32, 219, 219, 219, 219, 219, 219, 187, 176,
		219, 219, 201, 205, 205, 219, 219, 187, 219, 219, 186, 176, 176, 176, 176, 176, 219, 219, 201, 205, 205, 219, 219, 187, 200, 219, 219, 187, 176, 219, 219, 201, 188, 219, 219, 201, 205, 205, 205, 205, 188, 219, 219, 201, 205, 205, 219, 219, 187, 32, 32, 200, 205, 205, 205, 205, 219, 219, 187,
		219, 219, 219, 219, 219, 219, 201, 188, 219, 219, 186, 176, 176, 176, 176, 176, 219, 219, 219, 219, 219, 219, 219, 186, 176, 200, 219, 219, 219, 219, 201, 188, 176, 219, 219, 219, 219, 219, 187, 176, 176, 219, 219, 219, 219, 219, 219, 201, 188, 32, 32, 176, 176, 219, 219, 219, 201, 205, 188,
		219, 219, 201, 205, 205, 205, 188, 176, 219, 219, 186, 176, 176, 176, 176, 176, 219, 219, 201, 205, 205, 219, 219, 186, 176, 176, 200, 219, 219, 201, 188, 176, 176, 219, 219, 201, 205, 205, 188, 176, 176, 219, 219, 201, 205, 205, 219, 219, 187, 32, 32, 219, 219, 201, 205, 205, 188, 176, 176,
		219, 219, 186, 176, 176, 176, 176, 176, 219, 219, 219, 219, 219, 219, 219, 187, 219, 219, 186, 176, 176, 219, 219, 186, 176, 176, 176, 219, 219, 186, 176, 176, 176, 219, 219, 219, 219, 219, 219, 219, 187, 219, 219, 186, 176, 176, 219, 219, 186, 32, 32, 219, 219, 219, 219, 219, 219, 219, 187,
		200, 205, 188, 176, 176, 176, 176, 176, 200, 205, 205, 205, 205, 205, 205, 188, 200, 205, 188, 176, 176, 200, 205, 188, 176, 176, 176, 200, 205, 188, 176, 176, 176, 200, 205, 205, 205, 205, 205, 205, 188, 200, 205, 188, 176, 176, 200, 205, 188, 32, 32, 200, 205, 205, 205, 205, 205, 205, 188,
	};
	unsigned char P2_2[]{
		32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 176, 219, 219, 187, 176, 176, 176, 176, 176, 176, 176, 219, 219, 187, 219, 219, 187, 219, 219, 219, 187, 176, 176, 219, 219, 187, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
		32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 176, 219, 219, 186, 176, 176, 219, 219, 187, 176, 176, 219, 219, 186, 219, 219, 186, 219, 219, 219, 219, 187, 176, 219, 219, 186, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
		32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 176, 200, 219, 219, 187, 219, 219, 219, 219, 187, 219, 219, 201, 188, 219, 219, 186, 219, 219, 201, 219, 219, 187, 219, 219, 186, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
		32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 176, 176, 219, 219, 219, 219, 201, 205, 219, 219, 219, 219, 186, 176, 219, 219, 186, 219, 219, 186, 200, 219, 219, 219, 219, 186, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
		32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 176, 176, 200, 219, 219, 201, 188, 176, 200, 219, 219, 201, 188, 176, 219, 219, 186, 219, 219, 186, 176, 200, 219, 219, 219, 186, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
		32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 176, 176, 176, 200, 205, 188, 176, 176, 176, 200, 205, 188, 176, 176, 200, 205, 188, 200, 205, 188, 176, 176, 200, 205, 205, 188, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
	};
	int top = 6, num_char = 59, count  = 0;
	int color[] = { BRIGHT_WHITE, BLACK, BRIGHT_WHITE, BLUE, BRIGHT_WHITE, GREEN, BRIGHT_WHITE, PURPLE};
	while (count++ < 7)
	{
		Common::setConsoleColor(BRIGHT_WHITE, color[count]);
		for (int i = 0; i < 6; i++)
		{
			Common::gotoXY(24, i + top);
			for (int j = 0; j < num_char; j++)
			{
				putchar(P2_1[i * num_char + j]);
			}
			Common::gotoXY(24, i + top + 7);
			for (int j = 0; j < num_char; j++)
			{
				putchar(P2_2[i * num_char + j]);
			}
			Sleep(100);
		}
		Sleep(300);
	}
}
void Game::printBotWin()
{
	Common::playSound(7);
	Common::setConsoleColor(BRIGHT_WHITE, BLACK);
	Common::clearConsole();
	unsigned char Bot_1[] = {
		176, 219, 219, 219, 219, 219, 187, 176, 176, 219, 219, 219, 219, 219, 187, 176, 219, 219, 219, 187, 176, 176, 176, 219, 219, 219, 187, 219, 219, 219, 219, 219, 219, 187, 176, 219, 219, 187, 176, 176, 176, 219, 219, 187, 219, 219, 219, 219, 219, 219, 219, 219, 187, 219, 219, 219, 219, 219, 219, 219, 187, 219, 219, 219, 219, 219, 219, 187, 176,
		219, 219, 201, 205, 205, 219, 219, 187, 219, 219, 201, 205, 205, 219, 219, 187, 219, 219, 219, 219, 187, 176, 219, 219, 219, 219, 186, 219, 219, 201, 205, 205, 219, 219, 187, 219, 219, 186, 176, 176, 176, 219, 219, 186, 200, 205, 205, 219, 219, 201, 205, 205, 188, 219, 219, 201, 205, 205, 205, 205, 188, 219, 219, 201, 205, 205, 219, 219, 187,
		219, 219, 186, 176, 176, 200, 205, 188, 219, 219, 186, 176, 176, 219, 219, 186, 219, 219, 201, 219, 219, 219, 219, 201, 219, 219, 186, 219, 219, 219, 219, 219, 219, 201, 188, 219, 219, 186, 176, 176, 176, 219, 219, 186, 176, 176, 176, 219, 219, 186, 176, 176, 176, 219, 219, 219, 219, 219, 187, 176, 176, 219, 219, 219, 219, 219, 219, 201, 188,
		219, 219, 186, 176, 176, 219, 219, 187, 219, 219, 186, 176, 176, 219, 219, 186, 219, 219, 186, 200, 219, 219, 201, 188, 219, 219, 186, 219, 219, 201, 205, 205, 205, 188, 176, 219, 219, 186, 176, 176, 176, 219, 219, 186, 176, 176, 176, 219, 219, 186, 176, 176, 176, 219, 219, 201, 205, 205, 188, 176, 176, 219, 219, 201, 205, 205, 219, 219, 187,
		200, 219, 219, 219, 219, 219, 201, 188, 200, 219, 219, 219, 219, 219, 201, 188, 219, 219, 186, 176, 200, 205, 188, 176, 219, 219, 186, 219, 219, 186, 176, 176, 176, 176, 176, 200, 219, 219, 219, 219, 219, 219, 201, 188, 176, 176, 176, 219, 219, 186, 176, 176, 176, 219, 219, 219, 219, 219, 219, 219, 187, 219, 219, 186, 176, 176, 219, 219, 186,
		176, 200, 205, 205, 205, 205, 188, 176, 176, 200, 205, 205, 205, 205, 188, 176, 200, 205, 188, 176, 176, 176, 176, 176, 200, 205, 188, 200, 205, 188, 176, 176, 176, 176, 176, 176, 200, 205, 205, 205, 205, 205, 188, 176, 176, 176, 176, 200, 205, 188, 176, 176, 176, 200, 205, 205, 205, 205, 205, 205, 188, 200, 205, 188, 176, 176, 200, 205, 188,

	};
	unsigned char Bot_2[] = {
		32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 176, 219, 219, 187, 176, 176, 176, 176, 176, 176, 176, 219, 219, 187, 219, 219, 187, 219, 219, 219, 187, 176, 176, 219, 219, 187, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
		32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 176, 219, 219, 186, 176, 176, 219, 219, 187, 176, 176, 219, 219, 186, 219, 219, 186, 219, 219, 219, 219, 187, 176, 219, 219, 186, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
		32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 176, 200, 219, 219, 187, 219, 219, 219, 219, 187, 219, 219, 201, 188, 219, 219, 186, 219, 219, 201, 219, 219, 187, 219, 219, 186, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
		32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 176, 176, 219, 219, 219, 219, 201, 205, 219, 219, 219, 219, 186, 176, 219, 219, 186, 219, 219, 186, 200, 219, 219, 219, 219, 186, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
		32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 176, 176, 200, 219, 219, 201, 188, 176, 200, 219, 219, 201, 188, 176, 219, 219, 186, 219, 219, 186, 176, 200, 219, 219, 219, 186, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
		32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 176, 176, 176, 200, 205, 188, 176, 176, 176, 200, 205, 188, 176, 176, 200, 205, 188, 200, 205, 188, 176, 176, 200, 205, 205, 188, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32
	};
	int top = 6, num_char = 69, count  = 0;
	int color[] = { BRIGHT_WHITE, BLACK, BRIGHT_WHITE, BLUE, BRIGHT_WHITE, GREEN, BRIGHT_WHITE, PURPLE };
	while (count++ < 7)
	{
		Common::setConsoleColor(BRIGHT_WHITE, color[count]);
		for (int i = 0; i < 6; i++)
		{
			Common::gotoXY(20, i + top);
			for (int j = 0; j < num_char; j++)
			{
				putchar(Bot_1[i * num_char + j]);
			}
			Common::gotoXY(20, i + top + 7);
			for (int j = 0; j < num_char; j++)
			{
				putchar(Bot_2[i * num_char + j]);
			}
			Sleep(100);
		}
		Sleep(900);
	}
}
void Game::printDraw()
{
	Common::playSound(6);
	Common::setConsoleColor(BRIGHT_WHITE, BLACK);
	Common::clearConsole();
	unsigned char Draw_1[] = {
		219, 219, 219, 219, 219, 219, 187, 176, 32, 32, 219, 219, 219, 219, 219, 219, 187, 176, 219, 219, 187, 176, 176, 176, 176, 176, 176, 219, 219, 219, 219, 219, 187, 176, 219, 219, 187, 176, 176, 176, 219, 219, 187, 219, 219, 219, 219, 219, 219, 219, 187, 219, 219, 219, 219, 219, 219, 187, 176,
		200, 205, 205, 205, 205, 219, 219, 187, 32, 32, 219, 219, 201, 205, 205, 219, 219, 187, 219, 219, 186, 176, 176, 176, 176, 176, 219, 219, 201, 205, 205, 219, 219, 187, 200, 219, 219, 187, 176, 219, 219, 201, 188, 219, 219, 201, 205, 205, 205, 205, 188, 219, 219, 201, 205, 205, 219, 219, 187,
		176, 176, 219, 219, 219, 201, 205, 188, 32, 32, 219, 219, 219, 219, 219, 219, 201, 188, 219, 219, 186, 176, 176, 176, 176, 176, 219, 219, 219, 219, 219, 219, 219, 186, 176, 200, 219, 219, 219, 219, 201, 188, 176, 219, 219, 219, 219, 219, 187, 176, 176, 219, 219, 219, 219, 219, 219, 201, 188,
		219, 219, 201, 205, 205, 188, 176, 176, 32, 32, 219, 219, 201, 205, 205, 205, 188, 176, 219, 219, 186, 176, 176, 176, 176, 176, 219, 219, 201, 205, 205, 219, 219, 186, 176, 176, 200, 219, 219, 201, 188, 176, 176, 219, 219, 201, 205, 205, 188, 176, 176, 219, 219, 201, 205, 205, 219, 219, 187,
		219, 219, 219, 219, 219, 219, 219, 187, 32, 32, 219, 219, 186, 176, 176, 176, 176, 176, 219, 219, 219, 219, 219, 219, 219, 187, 219, 219, 186, 176, 176, 219, 219, 186, 176, 176, 176, 219, 219, 186, 176, 176, 176, 219, 219, 219, 219, 219, 219, 219, 187, 219, 219, 186, 176, 176, 219, 219, 186,
		200, 205, 205, 205, 205, 205, 205, 188, 32, 32, 200, 205, 188, 176, 176, 176, 176, 176, 200, 205, 205, 205, 205, 205, 205, 188, 200, 205, 188, 176, 176, 200, 205, 188, 176, 176, 176, 200, 205, 188, 176, 176, 176, 200, 205, 205, 205, 205, 205, 205, 188, 200, 205, 188, 176, 176, 200, 205, 188,
	};
	unsigned char Draw_2[] = {
		32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 219, 219, 219, 219, 219, 219, 187, 176, 219, 219, 219, 219, 219, 219, 187, 176, 176, 219, 219, 219, 219, 219, 187, 176, 176, 219, 219, 187, 176, 176, 176, 176, 176, 176, 176, 219, 219, 187, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
		32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 219, 219, 201, 205, 205, 219, 219, 187, 219, 219, 201, 205, 205, 219, 219, 187, 219, 219, 201, 205, 205, 219, 219, 187, 176, 219, 219, 186, 176, 176, 219, 219, 187, 176, 176, 219, 219, 186, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
		32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 219, 219, 186, 176, 176, 219, 219, 186, 219, 219, 219, 219, 219, 219, 201, 188, 219, 219, 219, 219, 219, 219, 219, 186, 176, 200, 219, 219, 187, 219, 219, 219, 219, 187, 219, 219, 201, 188, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
		32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 219, 219, 186, 176, 176, 219, 219, 186, 219, 219, 201, 205, 205, 219, 219, 187, 219, 219, 201, 205, 205, 219, 219, 186, 176, 176, 219, 219, 219, 219, 201, 205, 219, 219, 219, 219, 186, 176, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
		32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 219, 219, 219, 219, 219, 219, 201, 188, 219, 219, 186, 176, 176, 219, 219, 186, 219, 219, 186, 176, 176, 219, 219, 186, 176, 176, 200, 219, 219, 201, 188, 176, 200, 219, 219, 201, 188, 176, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32,
		32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 200, 205, 205, 205, 205, 205, 188, 176, 200, 205, 188, 176, 176, 200, 205, 188, 200, 205, 188, 176, 176, 200, 205, 188, 176, 176, 176, 200, 205, 188, 176, 176, 176, 200, 205, 188, 176, 176, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32
	};
	int top = 6, num_char = 59, count = 0;
	int color[] = { BRIGHT_WHITE, BLACK, BRIGHT_WHITE, BLUE, BRIGHT_WHITE, GREEN, BRIGHT_WHITE, PURPLE };
	while (count++ < 7)
	{
		Common::setConsoleColor(BRIGHT_WHITE, color[count]);
		for (int i = 0; i < 6; i++)
		{
			Common::gotoXY(24, i + top);
			for (int j = 0; j < num_char; j++)
			{
				putchar(Draw_1[i * num_char + j]);
			}
			Common::gotoXY(24, i + top + 7);
			for (int j = 0; j < num_char; j++)
			{
				putchar(Draw_2[i * num_char + j]);
			}
			Sleep(100);
		}
		Sleep(300);
	}
}
