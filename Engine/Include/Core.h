#pragma once
#pragma comment(lib, "gdiplus.lib")

#include "MathLibrary.h"  // .dll
#include <Windows.h>
#include <string>
#include <fstream>  // Read write from files
#include <gdiplus.h>  // Work with image
#include <curl/curl.h>  // !!! Parsing Site
#include <thread>  // Use Threads

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

	static void Throw();

	static EHUD_State HUD_State;
	static bool Is_Playing;


	static constexpr BYTE Alpha_Visibility = 255;  // 255 - 100% / 2 - 50% / 4 - 25%
	static constexpr bool Is_Background = true;  // if true draw background

	static constexpr int Window_Background_Transparency = 255;  // If 255 - 100%
	static constexpr int ID_Timer = 1;  // Timer Name, can be a lot
	static constexpr int FPS = 1000;  // If need 24 frames per second - 1000 / 24
	static constexpr int Max_Char_Length = 100;
	static constexpr int Window_Height = 200;  // used to resize custom image
	static constexpr int Window_Width = 150;  // -||-
	static constexpr int Scale = 9;
	static constexpr int Image_Size = 52;

	//static constexpr COLORREF transparentColor = RGB(background_transparency, background_transparency, background_transparency);
	static constexpr COLORREF transparentColor{ (Window_Background_Transparency, Window_Background_Transparency, Window_Background_Transparency) };  // !!! TEMP
	static constexpr RECT Window_Main_Rect{ Image_Size, Image_Size, Image_Size + Scale, Image_Size + Scale };

	static HWND Hwnd;
	static HDC Hdc;
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
