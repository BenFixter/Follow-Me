#pragma once
#include "EasyGraphics.h"

using namespace std;

class FollowMe :
	public EasyGraphics
{
public:
	FollowMe();
	~FollowMe();
	
	//static const int REDRAW_TIMER_WHENCLICK = 5;
	//static const int REDRAW_TIMER_1 = 1;
	//static const int REDRAW_TIMER_2 = 2;
	//static const int REDRAW_TIMER_3 = 3;
	//static const int REDRAW_TIMER_4 = 4;

	int x = 0;
	int y = 0;
	int startX = 0;
	int startY = 0;
	int endX = 0;
	int endY = 0;
	int gridSize = 4;
	int level = 0;
	int steps = 0;
	bool cheat = false;
	bool completed = false;

	string levelText;
	string sideContentText;
	string footerText;

	//File variable
	string name;
	int id;

	enum GameState { PREPARE, START, END }; //https://beginnersbook.com/2017/09/cpp-enumeration/
	GameState gameState;
	
	vector<int> tmp;
	vector<vector<int>> path;
	vector<vector<int> > arr{ {0,0,0,0,0,0},
							  {0,0,0,0,0,0},
							  {0,0,0,0,0,0},
							  {0,0,0,0,0,0},
							  {0,0,0,0,0,0},
							  {0,0,0,0,0,0} };
	//Functions
	virtual void onDraw();
	virtual void onKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual void displayGrid();
	virtual void drawDog();
	bool isInVector(int x, int y);
	void genPath();
	void drawCheatLine();
	void resetGrid();
	void newLevel();
	void writeName();
	void readFromFile();
	void writeToFile();

	void onTimer(UINT nIDEvent);
	void killAllTimers();
};