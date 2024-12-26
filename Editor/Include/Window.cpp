#include <Window.h>

// AsClicker
AsClicker::~AsClicker()
{

}
//------------------------------------------------------------------------------------------------------------
AsClicker::AsClicker()
 : Inputs_Mouses{ { INPUT_MOUSE, {.mi = { 0, 0, 0, MOUSEEVENTF_LEFTDOWN, 0, 0 } } }, { INPUT_MOUSE, {.mi = { 0, 0, 0, MOUSEEVENTF_LEFTUP, 0, 0 } } } },
   Inputs_Keyboard{ { INPUT_KEYBOARD, {.ki = { VK_F5, 0, 0, 0, 0 } } }, { INPUT_KEYBOARD, {.ki = { VK_F5, 0, KEYEVENTF_KEYUP, 0, 0 } } } },
   Timer_Prep(5), Cursor_Prev_Cords {}
{
   std::this_thread::sleep_for(std::chrono::seconds(Timer_Prep) );  // Time to prepere windows
}
//------------------------------------------------------------------------------------------------------------
int AsClicker::Is_Running(const int &timer, const SCoordinate &test)
{
   const SCoordinate button_update_cord { 1262, 708 };  // Update
   const SCoordinate button_accept_cord { 1106, 706 };  // Accept
	auto perform_action = [](const SCoordinate &cords, INPUT *input_type, size_t input_count, int timer_ms)
   {
      SetCursorPos(cords.x, cords.y);  // After each set cursor need return to prev position
      SendInput(static_cast<UINT>(input_count), input_type, sizeof(INPUT) );
      std::this_thread::sleep_for(std::chrono::milliseconds(timer_ms) );
   };

   while (true)
   {
		//perform_action(test, Inputs_Keyboard, 2, 200);  // F5 then after 200 ms go to next action
      perform_action(button_update_cord, Inputs_Mouses, 2, (1) * 1000);  // Pressed at cord
      perform_action(button_accept_cord, Inputs_Mouses, 2, (timer) * 1000);  // Pressed at cord

      if ( (GetAsyncKeyState(VK_CONTROL) & 0x8000) && (GetAsyncKeyState('Q') & 0x8000) )
         return 0;
      if ( (GetAsyncKeyState(VK_CONTROL) & 0x8000) && (GetAsyncKeyState(VK_SHIFT) & 0x8000) && (GetAsyncKeyState('R') & 0x8000) )
      {
         PostQuitMessage(0);
         return 0;
      }
   }
}
//------------------------------------------------------------------------------------------------------------




// AButton
AButton::~AButton()
{
}
//------------------------------------------------------------------------------------------------------------
AButton::AButton(const int x_cord, const int y_cord, const EButton_Action button_action)
 : Button_Width(x_cord), Button_Height(y_cord), Button_Action(button_action), Button_Rect {}
{

}
//------------------------------------------------------------------------------------------------------------
void AButton::Activate() const
{
   int yy = 0;

   switch (Button_Action)
   {
   case EButton_Action::Clicker_Start:
      AsClicker().Is_Running(AsConfig::Clicker_Timer, Anime_Stars_Cord);
      break;
   case EButton_Action::Clicker_Settings:
      SetTimer(AsConfig::Hwnd, 1, 100, 0);

      break;
   case EButton_Action::Clicker_Exit:
      PostQuitMessage(0);
      break;
   default:
      break;
   }
}
//------------------------------------------------------------------------------------------------------------




// AWindow
AWindow::~AWindow()
{
   for (auto &button: *Buttons_Vector)
		delete button;
	delete Buttons_Vector;
}
//------------------------------------------------------------------------------------------------------------
AWindow::AWindow(const int x_cord, const int y_cord)
 : Window_State(EWindow_State::None), Window_Rect{ x_cord, y_cord, AsConfig::Window_Offset + 1, AsConfig::Window_Offset + 1 }, Buttons_Vector(0),
   Hdc(0), Button_Active(EButton_Action::Clicker_Exit)
{
	Buttons_Vector = new std::vector<AButton *>();
	Buttons_Vector->reserve(3);  // !!! Change to constexpr
   Init();
}
//------------------------------------------------------------------------------------------------------------
void AWindow::Init()
{
   for (int i = 0; i <= (int)EButton_Action::Clicker_Exit; i++)  // Create custom buttons and resize AWindow
     Add_Button(new AButton(27, 20, (EButton_Action)i) );

}
//------------------------------------------------------------------------------------------------------------
void AWindow::Handle(const EWindow_State window_state)
{
   switch (window_state)
   {
   case EWindow_State::None:
      break;
   case EWindow_State::Draw:
      Draw_Frame();
      break;
   case EWindow_State::Down:
      LKM_Down();
      break;
   case EWindow_State::Hold:
      LKM_Hold();
      break;
   case EWindow_State::Exit:
      break;
   default:
      break;
   }
}
//------------------------------------------------------------------------------------------------------------
[[nodiscard]] int AWindow::On_Button_Clicked()
{
   for (int i = 0; i < Buttons_Vector->size(); i++)
      if (PtInRect(&Buttons_Vector->at(i)->Button_Rect, AsConfig::Cursor_Pos) )  // First button
         return AsConfig::Active_Button = i;
   return 0;
}
//------------------------------------------------------------------------------------------------------------
void AWindow::Add_Button(AButton *button)
{
	RECT &button_rect = button->Button_Rect;
   const int i = static_cast<int>(Buttons_Vector->size() );
   
   Buttons_Vector->push_back(button);  // Broke logic button with diff positions

	// 1.1. Default button cords save to button_rect
   button_rect.left = Window_Rect.left + AsConfig::Window_Offset / 2 + button->Button_Width * i;
   button_rect.top = Window_Rect.top + AsConfig::Window_Offset / 2;
   button_rect.right = button_rect.left + button->Button_Width;
   button_rect.bottom = button_rect.top + button->Button_Height;

   // 1.2. Resize window after add buttons
   Window_Rect.right += button->Button_Width;
   if (Window_Rect.bottom < button->Button_Height)
      Window_Rect.bottom = button->Button_Height + AsConfig::Window_Offset + 1;
}
//------------------------------------------------------------------------------------------------------------
void AWindow::Update_Button_Active()
{
	Buttons_Vector->at(AsConfig::Active_Button)->Activate();
}
//------------------------------------------------------------------------------------------------------------
void AWindow::Draw_Frame()
{
   PAINTSTRUCT ps{};
   Hdc = BeginPaint(AsConfig::Hwnd, &ps);

   Draw_Image();
   //if (Button_Active != EButton_Action::Clicker_Exit)
   //   Draw_Active_Button(hdc);
   EndPaint(AsConfig::Hwnd, &ps);

   //if (Is_Button_Clicked)
   //   Window->Update_Button_Active();
}
//------------------------------------------------------------------------------------------------------------
void AWindow::Draw_Image() const
{
   const int button_exit = 108;
	const int button_width = 27;
	const int button_height = 20;
	const int button_third = 54;
   Gdiplus::Image *gdi_image;
   Gdiplus::Graphics graphics(Hdc);
   
   gdi_image = new Gdiplus::Image(AsConfig::Clicker_Image_Folder() );
   if (gdi_image->GetLastStatus() == Gdiplus::Ok)
   {
      graphics.DrawImage(gdi_image, 0, 0);
      
      Gdiplus::Rect source_rect(button_exit, 0, button_width + 1, button_height);
      Gdiplus::Rect destin_rect(button_third, 0, button_width + 1, button_height);

      graphics.DrawImage(gdi_image, destin_rect, source_rect.X, source_rect.Y, source_rect.Width, source_rect.Height, Gdiplus::UnitPixel);
   }
   delete gdi_image;
}
//------------------------------------------------------------------------------------------------------------
void AWindow::LKM_Down()
{
   GetCursorPos(&AsConfig::Cursor_Pos);  // Check cords & rect to handle
   
   Button_Active = (EButton_Action)On_Button_Clicked();  // Get active button
   //Is_Button_Clicked = true;
}
//------------------------------------------------------------------------------------------------------------
void AWindow::LKM_Hold()
{
   int i = 0;
   int button_vector_size = static_cast<int>(Buttons_Vector->size() );
   AButton *button = 0;
   RECT rect {};

   // 1.2. Move Window
   while (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
   {// If hold LMB then move window

      std::this_thread::sleep_for(std::chrono::milliseconds(10) );  // Delay to move window
      GetCursorPos(&AsConfig::Cursor_Pos);
      MoveWindow(AsConfig::Hwnd, AsConfig::Cursor_Pos.x, AsConfig::Cursor_Pos.y, Window_Rect.right, Window_Rect.bottom, false);
   }

   GetWindowRect(AsConfig::Hwnd, &rect);  // Get current window rect transform

   for (i; i < button_vector_size; i++)
   {
      button = Buttons_Vector->at(i);
      button->Button_Rect.left = rect.left + AsConfig::Window_Offset / 2 + button->Button_Width * i;
      button->Button_Rect.top = rect.top + AsConfig::Window_Offset / 2;
      button->Button_Rect.right = button->Button_Rect.left + button->Button_Width;
      button->Button_Rect.bottom = button->Button_Rect.top + button->Button_Height;
   }
}
//------------------------------------------------------------------------------------------------------------
