// EggsCatcher.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "windows.h"
#include <iostream>

using namespace std;

void gotoxy(int x, int y)
{
	COORD c = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

class Egg {

	char body;

public:

	COORD pos;

	Egg(int start_x, int start_y, char body) {
	
		pos.X = start_x;
		pos.Y = start_y;
		this->body = body;
	}

	void setBody(char c) {
		
		this->body = c;
	}

	char getBody() {
		
		return this->body;
	}

	void fallDown() {
		
		pos.Y++;
	}
	void setRandomPos(int min_x, int max_x) {

		int x = rand() % (max_x);
		if (x < min_x) x = min_x;
		this->pos.X = x;
	}
};

class Catcher {

public:

	int length;
	COORD headPos;
	int min_x, max_x;

	Catcher(int x, int y, int min_x, int max_x, int length) {
	
		if (x < min_x) x = min_x;
		if (x > max_x) x = max_x;

		this->min_x = min_x;
		this->max_x = max_x;

		headPos.X = x;
		headPos.Y = y;
		this->length = length;
	}

	void turnLeft() {
		if (headPos.X > min_x)
			headPos.X--;
	}
	void turnRight() {
		if (headPos.X + length - 1 < max_x)
			headPos.X++;
	}
};

class GameManager {

	int score = 0;
	int mapHeight;
	int mapWidth;
	bool isGameOver = false;
	bool quitGame = false;

	Catcher *catcher;
	Egg *egg;

public:

	GameManager(int h, int w) {
	
		this->mapHeight = h;
		this->mapWidth = w;
	}

	void Play() {
		
	play:

		init();
		int frames = 0;
		while (!isGameOver) {
			
			if (frames % 2 == 0) 
				egg->fallDown();
			frames++;

			checkInput();
			render();
			checkLogic();
			if (quitGame) {
				goto quit;
			}
			Sleep(10);
		}

		if (isGameOver) {
		
			gotoxy(1, mapHeight + 2);
			cout << "GAME OVER!!";

			while (true) {
				
				if (GetKeyState(VK_ESCAPE) & 0x8000) {

					goto quit;
				}
				else if (GetKeyState(VK_RETURN) & 0x8000) {
					
					goto play;
				}
			}
		}
	quit: {}
	}

	void init() {
	
		system("cls");
		CONSOLE_CURSOR_INFO lpCursor;
		lpCursor.bVisible = false;
		lpCursor.dwSize = 25;
		SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &lpCursor);
		//SetConsoleDisplayMode(GetStdHandle(STD_OUTPUT_HANDLE), CONSOLE_FULLSCREEN_MODE, 0);

		isGameOver = false;
		score = 0;
		catcher = new Catcher(mapWidth/2, mapHeight - 2, 1, mapWidth - 2, 10);
		egg = new Egg(mapWidth / 2, 1, 'o');
		egg->setRandomPos(1, mapWidth - 1);
	}

	void checkInput() {
		
		if (GetKeyState(VK_ESCAPE) & 0x8000) {

			quitGame = true;
		}

		if (!isGameOver) {
		
			if (GetKeyState(VK_LEFT) & 0x8000) {
				
				catcher->turnLeft();
			}
			else if (GetKeyState(VK_RIGHT) & 0x8000) {
				
				catcher->turnRight();
			}
		}
	}

	void checkLogic() {
		
		if (egg->pos.Y >= catcher->headPos.Y) {
			
			if (egg->pos.X >= catcher->headPos.X && egg->pos.X <= catcher->headPos.X + catcher->length - 1) {
				egg = new Egg(mapWidth / 2, 1, 'o');
				egg->setRandomPos(1, mapWidth - 1);
				score++;
			}
			else {
				isGameOver = true;
			}
		}
	}

	void drawEdge() {

		gotoxy(0, 0);
	
		for (int i = 0; i < mapHeight; i++) {

			for (int j = 0; j < mapWidth; j++) {

				if (i == 0 || i == mapHeight - 1) {

					cout << '=';
				}
				else {

					if (j == 0 || j == mapWidth - 1) {
						cout << '|';
					}
					else {
						
						if (j == egg->pos.X && i == egg->pos.Y)
							cout << egg->getBody();
						else
							cout << ' ';
					}
				}
			}
			cout << endl;
		}
	}

	void render() {

		drawEdge();
		
		for (int i = 0; i < catcher->length; i++) {
			gotoxy(catcher->headPos.X + i, catcher->headPos.Y);
			cout << '=';
		}
		gotoxy(1, mapHeight + 1);
		cout << "SCORES: " << score;
	}
};

int main()
{

	GameManager gameManager(25, 80);
	gameManager.Play();
    return 0;
}

