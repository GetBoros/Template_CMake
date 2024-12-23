#pragma once

#include "framework.h"
#include "Core.h"

// SMouse_Cords
struct SMouse_Cords
{
	int x;
	int y;
};
wchar_t test[100]{};
//------------------------------------------------------------------------------------------------------------
struct SCoordinate
{
	consteval SCoordinate(int x, int y) : x(x), y(y) {}

	int x, y;
};
//------------------------------------------------------------------------------------------------------------
class ConstexprClass
{
public:
	constexpr ConstexprClass(int x, int y) : x_(x), y_(y) {}

	constexpr int sum() const { return x_ + y_; }
	constexpr int multiply() const { return x_ * y_; }

private:
	int x_;
	int y_;
};
//------------------------------------------------------------------------------------------------------------
class AsClicker
{
public:
	~AsClicker();
	AsClicker();

	int Is_Running(int &timer, const SCoordinate &test);

private:

	POINT Cursor_Prev_Cords;
	INPUT Inputs_Mouses[2];
	INPUT Inputs_Keyboard[2];
	int Timer_Prep;

	// TEMP
};
//------------------------------------------------------------------------------------------------------------
class AsMain_Window
{
public:
	~AsMain_Window();
	AsMain_Window();

	int APIENTRY Main(HINSTANCE handle_instance, int cmd_show);

private:
	void Window_Create() const;  // Or Add || Better create class widget
	void On_Paint(HWND hwnd);
	void On_LMB_Down(HWND hwnd);

	void Draw_Image(HDC hdc, const wchar_t *image_path) const;

	int Tick();

	int Cmd_Show;
	int Tick_Seconds;
	HINSTANCE Handle_Instance;
	ULONG_PTR GDI_Plus_Token;

	static LRESULT Window_Procedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	static constexpr SCoordinate Anime_Stars_Cord { 1181, 705 };
	static constexpr SCoordinate Youtube_Emo_Cord { 1357, 725 };
	static constexpr SCoordinate Twitch_Rewd_Cord { 1357, 725 };
	static AsMain_Window *Self;
};
//------------------------------------------------------------------------------------------------------------





//------------------------------------------------------------------------------------------------------------
#pragma region TASKS MAIN
/*

X	- Make Exit button


X	- What if save curr user position end return to it?
		- While thread sleep return to prev custom location

X	- When red button pressed:
		- start recording actions:
			- clicked mouse position
			- button pressed
		- try to show user what action done?
		- timer to bgn cycle
		- timer to end cycle
		- combination to end all cycled action (CTRL + P) - exit from program

X	- Config
X		- Transfer to config main settings

*/
#pragma endregion
//------------------------------------------------------------------------------------------------------------
#pragma region TASKS 13.12.2024
/*

V - Show Text without background
V		- Can use Transperenci with winui
V		- Text in window show fully 100% alpha
V		- Add func to add custom text? || Not today
*/
#pragma endregion
//------------------------------------------------------------------------------------------------------------
#pragma region TASKS 14.12.2024
/*
V  - When pressed change image to record
V		- Start autoclicker? for now
*/
#pragma endregion
//------------------------------------------------------------------------------------------------------------
#pragma region TASKS 15.12.2024
/*
V - While Hold change window location
V    - save cords
V        - Only while hold lkm
V       - change location with cursor pos.
*/
#pragma endregion
//------------------------------------------------------------------------------------------------------------




//------------------------------------------------------------------------------------------------------------
#pragma region DROPED
/*
X	- Use cookies or Login
		- For login need more widgets

X	- Try to get info from site what is current card
		- https://animestars.org/clubs/25/boost/
		- use curl to get data

X	- Show image from main page
X		- Example:
X			- /uploads/cards_image/197/e/jemblema-bagrovyh-lvov-1731289425.webp
X			- https://animestars.org/uploads/cards_image/197/e/jemblema-bagrovyh-lvov-1731289425.webp
X			- 523 start from here can save to file

X      - get from read_buffer image url?:  /uploads/cards_image/197/e/jemblema-bagrovyh-lvov-1731289425.webp
X         - Start: <div class=\"club-boost__image\"><img src=\"
X         - End: " alt="Карта"></div>
X      - concat with site https://animestars.org/
X      - Save image to default folder
X      - show user those image in app?

*/
#pragma endregion
//------------------------------------------------------------------------------------------------------------
