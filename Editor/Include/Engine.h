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
	void On_Key_Down();
	void On_Timer_Update();

	static LRESULT Window_Procedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	ULONG_PTR GDI_Plus_Token;
	AWindow* Window;

	static AsEngine *Self;
};
//------------------------------------------------------------------------------------------------------------
