#pragma once
#pragma comment(lib, "gdiplus.lib")

#include "MathLibrary.h"  // .dll
#include <Windows.h>
#include <string>
#include <fstream>  // Read write from files
#include <gdiplus.h>  // Work with image
#include <thread>  // Use Threads
#include <array>


#define WIN32_LEAN_AND_MEAN  // Exclude rarely-used stuff from Windows headers

// AsExamples
class AsExamples
{
public:
	~AsExamples();
	AsExamples();

	void Init();
};
//------------------------------------------------------------------------------------------------------------




// AsTools
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




// ENUMS
enum class EHUD_State : byte
{
	None,
	Draw
};



// AsConfig
class AsConfig
{
public:
	~AsConfig();
	static void Throw();

	static constexpr BYTE Alpha_Visibility = 255;  // 255 - 100% / 2 - 50% / 4 - 25%
	static constexpr bool Is_Draw_At_BG = true;  // if true draw background || if false clear background

	static constexpr int Window_Background_Transparency = 255;  // If 255 - 100%
	static constexpr int Window_Offset = 3 * 2;  // -||-

	//static constexpr COLORREF transparentColor = RGB(background_transparency, background_transparency, background_transparency);
	static constexpr COLORREF transparentColor{ (Window_Background_Transparency, Window_Background_Transparency, Window_Background_Transparency) };  // !!! TEMP
	
	static HWND Hwnd;
	static POINT Cursor_Pos;
};
//------------------------------------------------------------------------------------------------------------

#pragma region UNUSED_INCLUDES
//#pragma comment(lib, "ws2_32.lib")  // Winsock

//#include <stdlib.h>
//#include <malloc.h>
//#include <memory.h>
//#include <tchar.h>
#pragma endregion
