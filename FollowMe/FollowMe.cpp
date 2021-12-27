#include "FollowMe.h"
#include <time.h>
#include <fstream>

FollowMe::FollowMe()
{
	setImmediateDrawMode(false);
	srand((int)time(NULL));
	//velocity[0] = 250;
	//velocity[1] = 80;
	gameState = PREPARE; //Setting gameState to PREPARE. Used to check the state of the game.

}

FollowMe::~FollowMe()
{

}
//Draws the Window;
void FollowMe::onDraw()
{
	int width = getWidth();
	int height = getHeight();
	int rightSideX = width / 2 + 200;
	drawBitmap(L"assets\\parkbackground.bmp", 0, 0, width, height);
	setFont(24, L"Tahoma");
	setTextColour(WHITE);
	drawText(L"Follow Me If You Can...", 5, 10);

	setPenColour(WHITE, 3);
	drawRectangle(rightSideX, 50, width, 3, true);

	setPenColour(RED, 3);
	setFont(20, L"Tahoma");

	if (completed)
	{
		drawBitmap(L"assets\\complete.bmp", rightSideX, 100, 60, 60);
	}

	switch (gameState)
	{
	case FollowMe::PREPARE:
	{
		drawText("High Scores: ", rightSideX, 10);

		sideContentText = "Leaderboards....";
		drawText(sideContentText.c_str(), rightSideX, 50);

		footerText = "Press Any Key To Start!";
		drawText(footerText.c_str(), 10, 500);

		gridSize = 6;

		FollowMe::readFromFile();
		level = 0;
		FollowMe::displayGrid();
		break;
	}

	case FollowMe::START:
	{
		levelText = to_string(level);
		drawText("Level: ", rightSideX, 10);
		drawText(levelText.c_str(), width / 2 + 300, 10);

		sideContentText = "Use arrow keys to move";
		drawText(sideContentText.c_str(), rightSideX, 50);

		footerText = "";
		drawText(footerText.c_str(), 10, 500);

		FollowMe::displayGrid();

		if (cheat) {
			FollowMe::drawCheatLine();
		}

		drawBitmap(L"assets\\star.bmp", (50 + (60 * endX)), (60 + (60 * endY)), 60, 60);
		FollowMe::drawDog();

		break;
	}
	case FollowMe::END:
	{
		drawText("Level: ", rightSideX, 10);
		drawText(levelText.c_str(), width / 2 + 300, 10);

		sideContentText = "Use arrow keys to move";
		drawText(sideContentText.c_str(), rightSideX, 50);


		footerText = "Level Failed :(";
		drawText(footerText.c_str(), 10, 500);

		FollowMe::displayGrid();
		FollowMe::drawCheatLine();

		drawBitmap(L"assets\\star.bmp", (50 + (60 * endX)), (60 + (60 * endY)), 60, 60);
		drawBitmap(L"assets\\dogface.bmp", (50 + (60 * startX)), (60 + (60 * startY)), 60, 60);
		drawBitmap(L"assets\\cross.bmp", rightSideX, 100, 60, 60);

		FollowMe::writeName();
		break;
	}
	default:
		break;
	}

	EasyGraphics::onDraw();
}

//When a key is pressed. I mainly used this for testing my code.
void FollowMe::onKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//The keys that can be used depending on the gameState
	switch (gameState)
	{
	case FollowMe::PREPARE:
		gameState = START;
		gridSize = 4;
		FollowMe::newLevel();
		break;
	case FollowMe::START:
		switch (nChar)
		{
		case VK_UP:
		{
			cheat ^= true;
			onDraw();
			//killAllTimers();
			//setTimer(REDRAW_TIMER_4, 30);
			break;
		}
		case VK_DOWN:
		{
			footerText = "Level Complete : ) Press Any Key To Move To The Next Level";
			drawText(footerText.c_str(), 0, 0);
			completed = true;
			onDraw();
			//killAllTimers();
			//setTimer(REDRAW_TIMER_2, 30);
			break;
		}
		case VK_LEFT:
		{
			gameState = END;
			//killAllTimers();
			//setTimer(REDRAW_TIMER_1, 30);
			onDraw();
			break;
		}
		case VK_RIGHT:
		{
			if (completed)
			{
				completed = false;
				FollowMe::newLevel();
				onDraw();
			}
			//killAllTimers();
			//setTimer(REDRAW_TIMER_3, 30);
			break;
		}
		default:
			break;
		}
		break;
	case FollowMe::END:
	{
		//if the string contains characters it will remove a character when VK_BACK is pressed.
		if (nChar == VK_BACK)
		{
			if (name.size() > 0)
				name.erase(name.size() - 1, 1);
			onDraw();

		}
		//If a letter from the alphabet is pressed and the size is less than 15 it will add it to name.
		else if (nChar >= 'A' && nChar <= 'Z')
		{
			if (name.size() < 15)
				name += nChar;
			onDraw();
		}
		if (nChar == VK_RETURN)
		{
			//call functions to write and reset the grid aswell as reseting the level and 
			//setting the gameState to prepare ready for the next gane.
			FollowMe::writeToFile();
			FollowMe::resetGrid();
			gameState = PREPARE;
			onDraw();
		}
		break;
	}
	default:
		break;
	}
}

//Loops through the arr vector and draws one of three bitmaps depending on the value at the index.
void FollowMe::displayGrid()
{
	for (int i = 0; i < gridSize; i++)
	{
		for (int j = 0; j < gridSize; j++)
		{
			int x_pos = 50 + (60 * i);
			int y_pos = 60 + (60 * j);

			if (arr[i][j] == 0)
			{
				drawBitmap(L"assets\\tile1.bmp", x_pos, y_pos, 60, 60);
			}
			if (arr[i][j] == 1)
			{
				drawBitmap(L"assets\\tile2.bmp", x_pos, y_pos, 60, 60);
			}
			if (arr[i][j] == 3)
			{
				drawBitmap(L"assets\\star.bmp", x_pos, y_pos, 60, 60);
			}
		}
	}
}

void FollowMe::drawDog()
{
	setPenColour(BLACK, 3);

	int dx = (50 + (60 * startX));
	int dy = (60 + (60 * startY));
	drawBitmap(L"assets\\dogface.bmp", dx, dy, 60, 60);
	//drawRectangle(velocity[0] + move, velocity[1], 50, 50, true);
}

//Checks to make sure that x and y are not out of bounds.
bool FollowMe::isInVector(int x, int y)
{
	if (x >= 0 && x < gridSize && y >= 0 && y < gridSize)
	{
		return true;
	}
	return false;
}

//Generates a path using a random Int between 0 and 3 to determine the direction.
void FollowMe::genPath()
{
	path.clear();
	steps = 0;
	while (steps != 3 + level) {
		if (tmp.size() > 0)
			tmp.clear();

		switch (rand() % 3)
		{
		case 0: //top
		{

			if (isInVector(x, y - 1) && arr[x][y - 1] != 3)
			{
				arr[x][y - 1] = 1;
				y = y - 1;
				tmp.push_back(x);
				tmp.push_back(y);
				path.push_back(tmp);
				steps++;
				break;
			}
		}
		case 1: //bottom
		{
			if (isInVector(x, y + 1) && arr[x][y + 1] != 3)
			{

				arr[x][y + 1] = 1;
				y = y + 1;
				tmp.push_back(x);
				tmp.push_back(y);
				path.push_back(tmp);
				steps++;
				break;
			}
		}
		case 2: //left
		{
			if (isInVector(x - 1, y) && arr[x - 1][y] != 3)
			{
				arr[x - 1][y] = 1;
				x = x - 1;
				tmp.push_back(x);
				tmp.push_back(y);
				path.push_back(tmp);
				steps++;
				break;
			}
		}
		case 3: //right
		{
			if (isInVector(x + 1, y) && arr[x + 1][y] != 3)
			{
				arr[x + 1][y] = 1;
				x = x + 1;
				tmp.push_back(x);
				tmp.push_back(y);
				path.push_back(tmp);
				steps++;
				break;
			}
		}
		default:
			break;
		}
	}
	endX = x;
	endY = y;
}

//Loops through path vector
void FollowMe::drawCheatLine()
{
	for (int i = 0; i < path.size(); i++)
	{
		if (i == path.size() - 1) break;
		drawLine(80 + (60 * path[i][0]), 90 + (60 * path[i][0 + 1]), 80 + (60 * path[i + 1][0]), 90 + (60 * path[i + 1][0 + 1]));
	}
}

//Clears the path vector and sets the arr vector's values to 0.
void FollowMe::resetGrid()
{
	path.clear();

	for (int i = 0; i < gridSize; i++)
	{
		for (int j = 0; j < gridSize; j++)
		{
			arr[i][j] = 0;
		}

	}

}

//resets the grid increments level check what level to see if gridSize needs increasing. 
//Generate new random xand y ints.Sets the xand y to startXand startY for the dogFace.
//Then pushes xand y to tmp.After both values are pushed to path ie{ {startX,startY},{x,y},{x,y} }
void FollowMe::newLevel()
{
	FollowMe::resetGrid();

	level++;

	if (level > 4)
		gridSize = 5;
	if (level > 9)
		gridSize = 6;

	x = rand() % gridSize;
	y = rand() % gridSize;
	startX = x;
	startY = y;

	tmp.push_back(startX);
	tmp.push_back(startY);
	path.push_back(tmp);	


	FollowMe::genPath();


	onDraw();

}

void FollowMe::writeName()
{
	setBackColour(WHITE);
	setPenColour(BLACK, 3);

	drawRectangle(100, 200, 600, 100, true);

	setTextColour(BLACK);
	setFont(16, L"Tahoma");
	drawText("New High Score:  Enter Your Name", 245, 200);
	setTextColour(BLACK);
	setFont(20, L"Arial");
	drawText(name.c_str(), 250, 235);
}

void FollowMe::readFromFile()
{
	ifstream file("leaderboard.txt");

	if (file.fail())
		return;

	while (!file.eof() )
	{
		file >> name >> level;
		sideContentText = name + to_string(level);
	}
}

void FollowMe::writeToFile()
{
	ofstream file("leaderboard.txt"); 

	if (file.fail())
		return;

	file << name << "	" << level << endl;

}

void FollowMe::onTimer(UINT nIDEvent)
{

	//if (nIDEvent == REDRAW_TIMER_1) //left
	//{
	//	velocity[0] -= 3;
	//}
	//else if (nIDEvent == REDRAW_TIMER_2) //down
	//{
	//	velocity[1] += 3;
	//}
	//else if (nIDEvent == REDRAW_TIMER_3) //right
	//{
	//	velocity[0] += 3;
	//}
	//else if (nIDEvent == REDRAW_TIMER_4) //up
	//{
	//	velocity[1] -= 3;
	//}
}

void FollowMe::killAllTimers()
{
	//	if (REDRAW_TIMER_1)
	//		killTimer(REDRAW_TIMER_1);
	//
	//	if (REDRAW_TIMER_2)
	//		killTimer(REDRAW_TIMER_2);
	//
	//	if (REDRAW_TIMER_3)
	//		killTimer(REDRAW_TIMER_3);
	//
	//	if (REDRAW_TIMER_4)
	//		killTimer(REDRAW_TIMER_4);
}

