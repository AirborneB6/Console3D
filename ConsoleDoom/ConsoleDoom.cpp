// ConsoleDoom.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "pch.h"
#include <iostream>

const float PI = 3.14159f;

const int SCREEN_WIDTH  = 224;
const int SCREEN_HEIGHT = 140;


const float MAX_VIEW_DISTANCE = 15;
const float FOV = PI / 4;


const std::string ascii = "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\\|()1{}[]?-_+~<>i!lI;:,\"^`\'. ";


float playerX = 2;
float playerY = 2;
float playerA = 0;

int playerHealth = 100;

const int mapHeight = 16;
const int mapWidth = 16;


const char& getDistanceCharacter(const float& distance)
{
	float result = 0;
	result = (distance / MAX_VIEW_DISTANCE) * ascii.length();

	return ascii[(int)result];
}

int main()
{
	std::wstring map;

	map += L"################";
	map += L"#......#.#.....#";
	map += L"#..............#";
	map += L"#......#.#.....#";
	map += L"#..............#";
	map += L"#......#.#.....#";
	map += L"#..............#";
	map += L"#......#.#.....#";
	map += L"####...........#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#...############";
	map += L"#..............#";
	map += L"#...........#..#";
	map += L"#..............#";
	map += L"################";


	wchar_t* screen = new wchar_t[SCREEN_WIDTH * SCREEN_HEIGHT];

	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;

	SMALL_RECT windowRect = { 0 , 0 , SCREEN_WIDTH , SCREEN_HEIGHT };

	SetConsoleWindowInfo(hConsole, true, &windowRect);
	SetConsoleTitle(L"ayy lmao");

	bool gameRunning = true;

	auto tp1 = std::chrono::system_clock::now();
	auto tp2 = std::chrono::system_clock::now();

	while (gameRunning)
	{
		//GET TIME ELAPSED
		tp2 = std::chrono::system_clock::now();
		std::chrono::duration<float> elapsed = tp2 - tp1;
		tp1 = tp2;
		float elapsedTime = elapsed.count();


		//handle input
		if (GetAsyncKeyState((unsigned short)'A') & 0x8000)
		{
			playerA -= 2.0f * elapsedTime;
		}
		if (GetAsyncKeyState((unsigned short)'D') & 0x8000)
		{
			playerA += 2.0f * elapsedTime;
		}
		if (GetAsyncKeyState((unsigned short)'W') & 0x8000)
		{
			float amt = 3.5f;

			float unitX = sinf(playerA);
			float unitY = cosf(playerA);
			playerX += elapsedTime * amt * unitX;
			playerY += elapsedTime * amt * unitY;
		}
		if (GetAsyncKeyState((unsigned short)'S') & 0x8000)
		{
			float amt = 3.5f;

			float unitX = sinf(playerA);
			float unitY = cosf(playerA);
			playerX -= elapsedTime * amt * unitX;
			playerY -= elapsedTime * amt * unitY;
		}

		//SET SCREEN BUFFER
		for (int x = 0; x < SCREEN_WIDTH; x++)
		{
			float rayAngle = (playerA - FOV / 2) + ((float)x / (float)SCREEN_WIDTH)*FOV;

			float distanceToWall = 0;
			bool hitWall = false;

			//unit vector for player space
			float eyeX = sinf(rayAngle);
			float eyeY = cosf(rayAngle);


			while (hitWall == false && distanceToWall < MAX_VIEW_DISTANCE)
			{
				distanceToWall += 0.1f;

				float testX = (int)(playerX + eyeX * distanceToWall);
				float testY = (int)(playerY + eyeY * distanceToWall);

				if (testX < 0 || testX > mapWidth || testY < 0 || testY > mapHeight)
				{
					hitWall = true;
					distanceToWall = MAX_VIEW_DISTANCE;
				}
				else
				{
					if (map[testY * mapWidth + testX] == '#')
					{
						hitWall = true;
					}
				}
			}

			int ceilingH = (int)((float)SCREEN_HEIGHT / 2 - SCREEN_HEIGHT / ((float)distanceToWall));
			int floorH = SCREEN_HEIGHT - ceilingH;

			for (int y = 0; y < SCREEN_HEIGHT; y++)
			{
				if (y < ceilingH)
				{
					screen[y*SCREEN_WIDTH + x] = ' ';
				}
				else if (y < floorH)
				{
					screen[y*SCREEN_WIDTH + x] = getDistanceCharacter(distanceToWall);
				}
				else
				{
					screen[y*SCREEN_WIDTH + x] = '.';
				}
			}
		}

		//DRAW
		screen[SCREEN_WIDTH*SCREEN_HEIGHT - 1] = '\0';
		WriteConsoleOutputCharacter(hConsole, screen, SCREEN_WIDTH * SCREEN_HEIGHT, { 0 , 0 }, &dwBytesWritten);
	}
	

	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
