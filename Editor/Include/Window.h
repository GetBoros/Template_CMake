#include "framework.h"
#include "Core.h"

//------------------------------------------------------------------------------------------------------------
enum class EImage_Button : byte
{
	Play,
	Settings,
	Write,
	Return,
	Exit,
	Last
};
//------------------------------------------------------------------------------------------------------------
enum class EWindow_State : byte
{
	None,
	Draw,
	Down,
	Hold,
	Exit,
	Load_Resources
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

	void Handler(const EButton_Action button_action) const;
	
private:
	void LKM_Down() const;
	void Update_Rect() const;

	const EButton_Action Button_Action;
	const int Button_Width;
	const int Button_Height;
	mutable RECT Button_Rect;

	static constexpr SCoordinate Twitch_Rewd_Cord{ 1357, 725 };
	static constexpr SCoordinate Youtube_Emo_Cord{ 1357, 725 };  // 607 636 667 694
	static constexpr SCoordinate Anime_Stars_Cord{ 1111, 711 };
};
//------------------------------------------------------------------------------------------------------------
class AWindow
{
public:
	~AWindow();
	AWindow(const int x_cord, const int y_cord, HINSTANCE handle_instance);  // Window start Location

	void Init();  // For now just create Clicker app
	void Handle(const EWindow_State window_state);  // Handle window state

	RECT Window_Rect;

private:
	void Draw_Frame();
	void Draw_Image() const;  // !!! Refactor || too many uselles(repeatable actions) make Frame_DC class someday
	void Draw_Active();  // !!! Or must be in button?
	void LKM_Down();
	void LKM_Hold();
	void Load_Resources();
	void Capture_Screen_Rect(int x, int y, int width, int height, const std::wstring &filename);
	CLSID GetEncoderClsid(const WCHAR *format);

	HDC Hdc;
	std::vector<AButton *> *Buttons_Vector;
};
//------------------------------------------------------------------------------------------------------------
