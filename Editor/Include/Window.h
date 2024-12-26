#include "framework.h"
#include "Core.h"

//------------------------------------------------------------------------------------------------------------
enum class EWindow_State : byte
{
	None,
	Draw,
	Down,
	Hold,
	Exit

};
//------------------------------------------------------------------------------------------------------------
struct SCoordinate
{
	consteval SCoordinate(int x, int y) : x(x), y(y) {}

	int x, y;
};
//------------------------------------------------------------------------------------------------------------
class AsClicker
{
public:
	~AsClicker();
	AsClicker();

	int Is_Running(const int &timer, const SCoordinate &test);

private:
	int Timer_Prep;
	POINT Cursor_Prev_Cords;
	INPUT Inputs_Mouses[2];
	INPUT Inputs_Keyboard[2];
};
//------------------------------------------------------------------------------------------------------------
class AButton
{
public:
	~AButton();
	AButton(const int x_cord, const int y_cord, const EButton_Action button_action, const RECT &window_rect);

	void Handler() const;
	void Activate() const;
	
	EButton_Action Button_Action;
	int Button_Width;
	int Button_Height;
	RECT Button_Rect;
	
private:
	static constexpr SCoordinate Twitch_Rewd_Cord{ 1357, 725 };
	static constexpr SCoordinate Youtube_Emo_Cord{ 1357, 725 };  // 607 636 667 694
	static constexpr SCoordinate Anime_Stars_Cord{ 1111, 711 };
};
//------------------------------------------------------------------------------------------------------------
class AWindow
{
public:
	~AWindow();
	AWindow(const int x_cord, const int y_cord);  // Window start Location

	void Init();  // For now just create Clicker app
	void Handle(const EWindow_State window_state);  // Handle window state

	int On_Button_Clicked();  // Used AsConfig::Cursor_Pos to check where clicked mouse
	void Update_Button_Active();

	RECT Window_Rect;

private:
	void Draw_Frame();
	void Draw_Image() const;
	void Draw_Active();
	void LKM_Down();
	void LKM_Hold();

	HDC Hdc;
	EButton_Action Button_Active;
	std::vector<AButton *> *Buttons_Vector;
};
//------------------------------------------------------------------------------------------------------------
