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
   const SCoordinate button_update_cord { 1262, 773 };  // Update
   const SCoordinate button_accept_cord { 1091, 773 };  // Accept
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
AButton::AButton(const int x_cord, const int y_cord, const EButton_Action button_action, const RECT &window_rect)
 : Button_Width(x_cord), Button_Height(y_cord), Button_Action(button_action), Button_Rect {}
{
   Button_Rect.left = window_rect.left + AsConfig::Window_Offset / 2 + Button_Width * (int)button_action;
   Button_Rect.top = window_rect.top + AsConfig::Window_Offset / 2;
   Button_Rect.right = Button_Rect.left + Button_Width;
   Button_Rect.bottom = Button_Rect.top + Button_Height;
}
//------------------------------------------------------------------------------------------------------------
void AButton::Handler(const EButton_Action button_action) const
{
   switch (button_action)
   {
   case EButton_Action::Clicker_Start:
      AsClicker().Is_Running(AsConfig::Clicker_Timer, Anime_Stars_Cord);
      break;
   case EButton_Action::Clicker_Settings:
      AsConfig::Button_Active = EButton_Action::Clicker_Settings;
      //SetTimer(AsConfig::Hwnd, 1, 100, 0);  // ID(1), ms(100)
      break;
   case EButton_Action::Clicker_Exit:
      PostQuitMessage(0);
      break;
   case EButton_Action::Clicker_Down:
      LKM_Down();
      break;
   case EButton_Action::Clicker_Update_Rect:
      Update_Rect();
      break;
   default:
      AsConfig::Throw();
      break;
   }
}
//------------------------------------------------------------------------------------------------------------
void AButton::LKM_Down() const
{
   if (PtInRect(&Button_Rect, AsConfig::Cursor_Pos) )  // First button
      Handler(Button_Action);
}
//------------------------------------------------------------------------------------------------------------
void AButton::Update_Rect() const
{
   RECT rect {};
   GetWindowRect(AsConfig::Hwnd, &rect);  // Get current window rect transform

   Button_Rect.left = rect.left + AsConfig::Window_Offset / 2 + Button_Width * (int)Button_Action;
   Button_Rect.top = rect.top + AsConfig::Window_Offset / 2;
   Button_Rect.right = Button_Rect.left + Button_Width;
   Button_Rect.bottom = Button_Rect.top + Button_Height;
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
AWindow::AWindow(const int x_cord, const int y_cord, HINSTANCE handle_instance)
 : Window_Rect{ x_cord, y_cord, AsConfig::Window_Offset + 1, AsConfig::Window_Offset + 1 }, Buttons_Vector(0),
   Hdc(0)
{
   const int button_width = 27;
   const int button_height = 20;
   const wchar_t *w_name = AsConfig::Window_Name;

	Buttons_Vector = new std::vector<AButton *>();
	Buttons_Vector->reserve(3);
  
   for (int i = 0; i <= (int)EButton_Action::Clicker_Exit; i++)  // Create custom buttons and resize AWindow
   {// Create buttons needed for clicker

      Buttons_Vector->push_back(new AButton(button_width, button_height, (EButton_Action)i, Window_Rect) );
      Window_Rect.right += button_width;
      if (Window_Rect.bottom < button_height)
         Window_Rect.bottom = button_height + AsConfig::Window_Offset + 1;
   }

   AsConfig::Hwnd = CreateWindowExW(WS_EX_DLGMODALFRAME | WS_EX_TOPMOST | WS_EX_TOOLWINDOW | WS_EX_ACCEPTFILES | WS_EX_NOACTIVATE, w_name, w_name,
      WS_POPUP, Window_Rect.left, Window_Rect.top, Window_Rect.right, Window_Rect.bottom, 0, 0, handle_instance, 0);
}
//------------------------------------------------------------------------------------------------------------
void AWindow::Init()
{

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
void AWindow::Draw_Frame()
{
   PAINTSTRUCT ps{};
   Hdc = BeginPaint(AsConfig::Hwnd, &ps);

   // Draw Buttons 0, 1, 4
   Draw_Image();

   EndPaint(AsConfig::Hwnd, &ps);
}
//------------------------------------------------------------------------------------------------------------
void AWindow::Draw_Image() const
{// graphics.DrawImage(gdi_image, 0, 0);  // Draw full image in folder

	const int button_width = 27;
   int width = 0;
   int height = 0;
   int i = 0;
   int *image_arrays_cords = 0;
   int *image_array_patern = 0;
   int menu_starting[]{ (int)EImage_Button::Play, (int)EImage_Button::Settings, (int)EImage_Button::Exit };
   int menu_settings[]{ (int)EImage_Button::Play, (int)EImage_Button::Write, (int)EImage_Button::Return };
   Gdiplus::Image *gdi_image = 0;
   Gdiplus::Graphics gdi_graphics(Hdc);

   if (AsConfig::Button_Active != EButton_Action::Clicker_Settings)
      image_array_patern = menu_starting;
   else
      image_array_patern = menu_settings;

   gdi_image = new Gdiplus::Image(AsConfig::Clicker_Image_Folder() );
   if (gdi_image->GetLastStatus() == Gdiplus::Ok)
   {
      width = gdi_image->GetWidth();
      height = gdi_image->GetHeight();
      int image_button_count = width / button_width;
      image_arrays_cords = new int[image_button_count] {};
      const int window_button_count = Window_Rect.right / 27;

      for (i = 0; i < image_button_count; i++)
         image_arrays_cords[i] = button_width * i;

      for (i = 0; i < window_button_count; i++)
      {
         Gdiplus::Rect source_rect(image_arrays_cords[image_array_patern[i] ], 0, button_width + 1, height);  // from to put
         Gdiplus::Rect destin_rect(image_arrays_cords[i], 0, button_width + 1, height);  // where to put

         gdi_graphics.DrawImage(gdi_image, destin_rect, source_rect.X, source_rect.Y, source_rect.Width, source_rect.Height, Gdiplus::UnitPixel);
      }

   }
   delete gdi_image;
}
//------------------------------------------------------------------------------------------------------------
void AWindow::Draw_Active()
{
 //  const int width = Buttons_Vector->at(0)->Button_Width;
 //  const int height = Buttons_Vector->at(0)->Button_Height;  // while only one raw always 19
	//const int index = static_cast<int>(Button_Active);
 //  HPEN pen_green = CreatePen(PS_SOLID, 1, RGB(0, 255, 0) );  // Создаем зеленую ручку для рисования линий
 //  HGDIOBJ pen_prev = SelectObject(Hdc, pen_green);
 //  RECT rect { width * index, 0, width * (index + 1), height - 1 };

 //  MoveToEx(Hdc, rect.left, rect.top, 0);
 //  LineTo(Hdc, rect.right, rect.top);
 //  LineTo(Hdc, rect.right, rect.bottom);
 //  LineTo(Hdc, rect.left, rect.bottom);
 //  LineTo(Hdc, rect.left, rect.top);

 //  SelectObject(Hdc, pen_prev);
 //  DeleteObject(pen_green);
}
//------------------------------------------------------------------------------------------------------------
void AWindow::LKM_Down()
{
   GetCursorPos(&AsConfig::Cursor_Pos);  // Check cords & rect to handle
   
   for (int i = 0; i < Buttons_Vector->size(); i++)
      Buttons_Vector->at(i)->Handler(EButton_Action::Clicker_Down);
}
//------------------------------------------------------------------------------------------------------------
void AWindow::LKM_Hold()
{
   while (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
   {// If hold LMB then move window

      std::this_thread::sleep_for(std::chrono::milliseconds(10) );  // Delay to move window
      GetCursorPos(&AsConfig::Cursor_Pos);
      MoveWindow(AsConfig::Hwnd, AsConfig::Cursor_Pos.x, AsConfig::Cursor_Pos.y, Window_Rect.right, Window_Rect.bottom, false);
   }

   for (int i = 0; i < Buttons_Vector->size(); i++)
      Buttons_Vector->at(i)->Handler(EButton_Action::Clicker_Update_Rect);
}
//------------------------------------------------------------------------------------------------------------
