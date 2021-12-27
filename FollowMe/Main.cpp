#include <windows.h>
#include "FollowMe.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdline, int show) {
	FollowMe fm;
	fm.create(hInstance, 800, 600, 100, true);
	
	return 0;
}