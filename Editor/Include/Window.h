#include "framework.h"
#include "Core.h"

//------------------------------------------------------------------------------------------------------------
enum class EButton_Action : byte
{
	Clicker_Start,
	Clicker_Settings,
	Cliker_Record,
	Clicker_Exit
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
	AButton(const int x_cord, const int y_cord, const EButton_Action button_action);

	void Activate() const;

	EButton_Action Button_Action;
	int Button_Width;
	int Button_Height;
	RECT Button_Rect;

	static constexpr SCoordinate Youtube_Emo_Cord{ 1357, 725 };  // 607 636 667 694
	static constexpr SCoordinate Anime_Stars_Cord{ 1181, 705 };
};
//------------------------------------------------------------------------------------------------------------
class AWindow
{
public:
	~AWindow();
	AWindow(const int x_cord, const int y_cord);

	int On_Button_Clicked();  // Used AsConfig::Cursor_Pos to check where clicked mouse
	void Add_Button(AButton* button);
	void Update_Button_Active();

	RECT Window_Rect;

	std::vector<AButton*>* Buttons_Vector;
};
//------------------------------------------------------------------------------------------------------------
