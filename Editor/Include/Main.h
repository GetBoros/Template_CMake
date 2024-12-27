#pragma once

#include <Window.h>

//------------------------------------------------------------------------------------------------------------
class AsEngine
{
public:
	~AsEngine();
	AsEngine();

	int APIENTRY Update(HINSTANCE handle_instance, int cmd_show);

private:
	int Tick(HINSTANCE handle_instance) const;
	void On_Paint();
	void On_LMB_Down();
	void On_Timer_Update();

	static LRESULT Window_Procedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	ULONG_PTR GDI_Plus_Token;
	AWindow *Window;

	static AsEngine *Self;
};
//------------------------------------------------------------------------------------------------------------





//------------------------------------------------------------------------------------------------------------
#pragma region TASKS MAIN
/*

X	- While in settings press some button to save cursor location
		- 

X	- Create button what stored all user actions, and can be played, paused, stopped, saved, loaded
X		- IO to file, or nah?


X	- When pressed Clicker_Settings start record user inputs
X		- Pressing buttons, clicking at location elsewhere.
X			- Save those actions to file
X		- When pressed Clicker_Start start those actions
X			- Resize Window and expose images with paterns
X			- 
X			- How to expose user array of actions, with images?

*/
#pragma endregion
