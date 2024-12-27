#pragma once
#pragma comment(lib, "gdiplus.lib")

#include "MathLibrary.h"  // .dll
#include <Windows.h>
#include <string>
#include <fstream>  // Read write from files
#include <gdiplus.h>  // Work with image
#include <thread>  // Use Threads
#include <vector>

#define WIN32_LEAN_AND_MEAN  // Exclude rarely-used stuff from Windows headers

//------------------------------------------------------------------------------------------------------------
enum class EHUD_State : byte
{
	None,
	Draw
};
//------------------------------------------------------------------------------------------------------------
enum class EButton_Action : byte
{
	Clicker_Start,
	Clicker_Settings,
	Clicker_Exit,
	Clicker_Down,
	Clicker_Update_Rect
};
//------------------------------------------------------------------------------------------------------------
class AsConfig
{
public:
	~AsConfig();
	static void Throw();
	static const wchar_t *Clicker_Image_Folder();

	static EButton_Action Button_Active;
	static int Clicker_Timer;  // Delay betwen clicks
	static int Active_Button;  // Current pressed button index
	static HWND Hwnd;
	static POINT Cursor_Pos;  // Last clicked mouse cords stored here
	
	static constexpr bool Is_Draw_At_BG = true;  // if true draw background || if false clear background
	static constexpr int Window_Offset = 3 * 2;  // -||-
	static constexpr wchar_t Window_Name[] = L"A";
};
//------------------------------------------------------------------------------------------------------------
class AsExamples
{
public:
	~AsExamples();
	AsExamples();

	void Init();
};
//------------------------------------------------------------------------------------------------------------
class AsTools
{
public:
	~AsTools();
	AsTools();

private:
	unsigned long long start_tick, end_tick, delta_tick;

	static unsigned long long How_Fast;
};
//------------------------------------------------------------------------------------------------------------

#pragma region UNUSED_INCLUDES
//#pragma comment(lib, "ws2_32.lib")  // Winsock

//#include <stdlib.h>
//#include <malloc.h>
//#include <memory.h>
//#include <tchar.h>
#pragma endregion
