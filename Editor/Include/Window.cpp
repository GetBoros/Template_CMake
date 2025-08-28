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
void AsClicker::MoveCursorSmoothly(int startX, int startY, int endX, int endY, int steps) {
   for (int i = 0; i <= steps; ++i) {
      int x = startX + (endX - startX) * i / steps;
      int y = startY + (endY - startY) * i / steps;
      SetCursorPos(x, y);
      std::this_thread::sleep_for(std::chrono::milliseconds(5) );  // Задержка для плавности
   }
}
//------------------------------------------------------------------------------------------------------------
int AsClicker::Is_Running(const int timer, const SCoordinate &test)
{
   constexpr int delay_ms = 200;  // give site time to response next 150 ms or less? || ( 8 card 150 ) ( 3 card 100)
   constexpr int ms_at_sec = 1000;  // how many ms in one second
   constexpr int delay_arg = 4;  // how many else arg we have
   constexpr int delay_ccl = 2;  // !!! change to 3 if fail || how much wait per cycle in while loop
   constexpr int delay_ms_rest = (delay_ccl * ms_at_sec) - (delay_ms * delay_arg) - 400;
   const SCoordinate button_update_cord { 1181, 770 };  // { 1262, 773 };  // Update 1181 770
   const SCoordinate button_accept_cord { 1091, 773 };  // Accept
   const SCoordinate button_gamesa_cord { 480, 648 };  // Accept
   const SCoordinate button_fscren_cord { 1415, 875 };  // AStar fullscreen button cords while in full open
   const SCoordinate button_fscrrv_cord { 1050, 500 };  // AStar fullscreen button cords opens in full screen
   const SCoordinate button_cardsr_cord { 1339, 665 };  // last byte 1406, 767  || Size 70 | 135
   INPUT inputMouseHold = { INPUT_MOUSE, {.mi = { 0, 0, 0, MOUSEEVENTF_LEFTDOWN, 0, 0 } } };
   INPUT inputMouseRelease = { INPUT_MOUSE, {.mi = { 0, 0, 0, MOUSEEVENTF_LEFTUP, 0, 0 } } };
   SCoordinate card_offset_cord = button_cardsr_cord;
   SCoordinate ixbt { 1395, 835 };
   std::chrono::steady_clock::time_point time_now {};
   std::chrono::steady_clock::time_point time_start= std::chrono::steady_clock::now();

	auto perform_action = [](const SCoordinate &cords, INPUT *input_type, size_t input_count, int timer_ms)
   {
      SetCursorPos(cords.x, cords.y);  // Move cursor to cord location
      SendInput(static_cast<UINT>(input_count), input_type, sizeof(INPUT) );
      std::this_thread::sleep_for(std::chrono::milliseconds(timer_ms) );
   };

   auto key_combination = [](const int modifer, const int key)
      {
         return (GetAsyncKeyState(modifer) & 0x8000) && (GetAsyncKeyState(key) & 0x8000);
      };

   //perform_action(SCoordinate(120, 20), Inputs_Mouses, 2, delay_ms);  // First page cords
  
   //perform_action(test, Inputs_Keyboard, 2, 200);  // F5 then after 200 ms go to next action

   // IXBT Spam
   /*
   while (true)
   {
      for (int i = 0; i < 5; i++)
      {
         ixbt.y = 835 - i * 30;
         perform_action(ixbt, Inputs_Mouses, 2, 250);  // 1395 | 710 - 831 5
      }

      if (key_combination(VK_CONTROL, 'Q'))  // every 3 sec check if pressed end cycle
         return 0;
   }
   */
  
   //Clicker simulator
   /*
   int a = 250;
   while (true)
   {
      SendInput(2, Inputs_Mouses, sizeof(INPUT) );
      std::this_thread::sleep_for(std::chrono::milliseconds(75) );  // Задержка для плавности

      time_now = std::chrono::steady_clock::now();  // get current time

      if (std::chrono::duration_cast<std::chrono::milliseconds>(time_now - time_start).count() >= delay_ms_rest)
      {// if from start time point 3 seconds past do the scope

         //SendInput(1, &inputMouseHold, sizeof(INPUT) );
         std::this_thread::sleep_for(std::chrono::milliseconds(250) );  // Задержка для плавности || 1500
         if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
            return 0;
         if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
            if (a > 0)
               a = a - 5;
         time_start = std::chrono::steady_clock::now();  // update time_start to current time
      }
   }
   */

   // HOLD LKM while write CTRL
   /*
   while (true)
   {
      SendInput(2, Inputs_Mouses, sizeof(INPUT));
      std::this_thread::sleep_for(std::chrono::milliseconds(75));  // Задержка для плавности
      //SendInput(2, Inputs_Mouses, sizeof(INPUT));
      //std::this_thread::sleep_for(std::chrono::milliseconds(150));  // Задержка для плавности
      //SendInput(1, &inputMouseHold, sizeof(INPUT));
      //std::this_thread::sleep_for(std::chrono::milliseconds(1000) );  // Задержка для плавности
      //SendInput(2, Inputs_Mouses, sizeof(INPUT));
      //std::this_thread::sleep_for(std::chrono::milliseconds(1000));  // Задержка для плавности

      if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
         SendInput(1, &inputMouseHold, sizeof(INPUT) );
   
      if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
         return 0;

      if (GetAsyncKeyState(']') & 0x8000)
         return 0;
   
      if (GetAsyncKeyState(VK_SPACE) & 0x8000)
      {
         SendInput(1, &inputMouseHold, sizeof(INPUT));
         std::this_thread::sleep_for(std::chrono::milliseconds(150));  // Задержка для плавности
      }
   }
   */
   // Drag file
   /*
   SetCursorPos(621, 230);  // After each set cursor need return to prev position
   Sleep(delay_ms);
   SendInput(1, &inputMouseHold, sizeof(INPUT) );
   
   MoveCursorSmoothly(621, 230, 621, 500, 7);
   
   Sleep(200);
   SetCursorPos(621, 500);  // After each set cursor need return to prev position
   SendInput(1, &inputMouseRelease, sizeof(INPUT));
   
   return 0;
   */
   
   // In Game buy items
   /*
   while (true)
   {
      perform_action(button_gamesa_cord, Inputs_Mouses, 2, delay_ms);  // 518 648
      perform_action(button_gamesa_cord, Inputs_Mouses, 2, delay_ms);  // 518 648
      
      if ( (GetAsyncKeyState(VK_CONTROL) & 0x8000) && (GetAsyncKeyState('Q') & 0x8000) )
         return 0;
      if ( (GetAsyncKeyState(VK_CONTROL) & 0x8000) && (GetAsyncKeyState(VK_SHIFT) & 0x8000) && (GetAsyncKeyState('R') & 0x8000) )
      {
         PostQuitMessage(0);
         return 0;
      }
   }
   */

   // Cards capture
   /*
   while (true)
   {
      // Double click
      perform_action(button_fscrrv_cord, Inputs_Mouses, 2, delay_ms);
      perform_action(button_fscrrv_cord, Inputs_Mouses, 2, delay_ms);

      // Click on card
      perform_action(SCoordinate(1343, 769), Inputs_Mouses, 2, delay_ms);  // Press on cards
      
      // Double click
      perform_action(button_fscrrv_cord, Inputs_Mouses, 2, delay_ms);
      perform_action(button_fscrrv_cord, Inputs_Mouses, 2, delay_ms);

      if (card_offset_cord.y == 1409)
      {
         card_offset_cord.y = button_cardsr_cord.x;
         card_offset_cord.x += 1;
      }

      //card_offset_cord.y += 1;
      std::this_thread::sleep_for(std::chrono::seconds(330 / 5) );  // 5 min + 30 sec
   }
   */
   
   // Site sacrifice cards
   while (true)  // 3 sec
   {
      perform_action(button_update_cord, Inputs_Mouses, 2, delay_ms);  // Click to sacrifice card and wait delay_ms

      if (key_combination(VK_CONTROL, 'Q') )  // every 3 sec check if pressed end cycle
         return 0;

      //time_now = std::chrono::steady_clock::now();  // get current time

      //if(std::chrono::duration_cast<std::chrono::milliseconds>(time_now - time_start).count() >= delay_ms_rest)
      //{// if from start time point 3 seconds past do the scope

      //   perform_action(button_update_cord, Inputs_Mouses, 2, delay_ms);  // Click to update button page || 1181 770
      //   if (key_combination(VK_CONTROL, 'Q') )  // every 3 sec check if pressed end cycle
      //      return 0;

      //   time_start = std::chrono::steady_clock::now();  // update time_start to current time
      //}
   }

   return 0;
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
   {// rework

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
      //Capture_Screen_Rect(300, 300, 600, 600, L"screenshot.png");
      LKM_Down();
      break;
   case EWindow_State::Hold:
      LKM_Hold();
      break;
   case EWindow_State::Exit:
      break;
   case EWindow_State::Load_Resources:
      Load_Resources();
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

   Draw_Image();

   EndPaint(AsConfig::Hwnd, &ps);
}
//------------------------------------------------------------------------------------------------------------
void AWindow::Draw_Image() const
{// graphics.DrawImage(gdi_image, 0, 0);  // Draw full image in folder

   int width = 0;
   int height = 0;
   int i = 0;
   int *image_arrays_cords = 0;
   int *image_array_patern = 0;
   int image_button_count = 0;
   int menu_starting[]{ (int)EImage_Button::Play, (int)EImage_Button::Settings, (int)EImage_Button::Exit };
   int menu_settings[]{ (int)EImage_Button::Play, (int)EImage_Button::Write, (int)EImage_Button::Return };
	const int button_width = 27;
   const int window_button_count = Window_Rect.right / button_width;
   Gdiplus::Image *gdi_image = 0;
   Gdiplus::Graphics gdi_graphics(Hdc);

   // 1.0. Pattern case to what images to draw
   if (AsConfig::Button_Active != EButton_Action::Clicker_Settings)  // If click at button setting change draw pattern
      image_array_patern = menu_starting;
   else
      image_array_patern = menu_settings;

   // 2.0. Draw images as background
   gdi_image = new Gdiplus::Image(AsConfig::Clicker_Image_Folder() );  // Load Image from folder
   if (gdi_image->GetLastStatus() == Gdiplus::Ok)
   {
      // 2.1. Initialize images rects, cords and save to array
      width = gdi_image->GetWidth();
      height = gdi_image->GetHeight();
      image_button_count = width / button_width;  // how many images || buttons
      image_arrays_cords = new int[image_button_count] {};  // here store images || buttons cords

      for (i = 0; i < image_button_count; i++)
         image_arrays_cords[i] = button_width * i;  // Set each button cord | location

      // 2.2. Draw image part from file image to app image
      for (i = 0; i < window_button_count; i++)
      {
         Gdiplus::Rect source_rect(image_arrays_cords[image_array_patern[i] ], 0, button_width + 1, height);  // from this rect
         Gdiplus::Rect destin_rect(image_arrays_cords[i], 0, button_width + 1, height);  // to this rect

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
void AWindow::Load_Resources()
{

}
//------------------------------------------------------------------------------------------------------------
void AWindow::Capture_Screen_Rect(int x, int y, int width, int height, const std::wstring &filename)
{// HBITMAP -> Gdiplus::Bitmap -> Save to file

   HDC hScreenDC = 0;  // work with screen
   HDC hMemoryDC = 0;  // work with images in memory
   HBITMAP hBitmap = 0;  // need to cpy image from screen, it`s like API?
   CLSID pngClsid { };

   // 1.0. Init data
   hScreenDC = GetDC(0);  // monitor descriptor
   hMemoryDC = CreateCompatibleDC(hScreenDC);  // context to work with image in allocated memory
   hBitmap = CreateCompatibleBitmap(hScreenDC, width, height);  // create arr ** to empty bytes after save data here
   
   // 1.1. Save image to hBitmap | cpy pxls hScreenDC -> hMemoryDC -> hBitmap (if SelectObject)
   SelectObject(hMemoryDC, hBitmap);  // say hMemoryDC to draw in hBitmap || BitBlt now draw in hBitmap
   BitBlt(hMemoryDC, 0, 0, width, height, hScreenDC, x, y, SRCCOPY);  // cpy from screen_dc to memory_dc
   
   // 1.2. Encode to png and save to filename folder
   Gdiplus::Bitmap bitmap(hBitmap, 0);  // expose hBitmap to highlevel, easy work with
   pngClsid = GetEncoderClsid(L"image/png");  // change image format?
   bitmap.Save(filename.c_str(), &pngClsid, 0);  // Save to curr format png to file in file name

   // 1.3. Free memory
   DeleteObject(hBitmap);
   DeleteDC(hMemoryDC);
   ReleaseDC(nullptr, hScreenDC);
}
//------------------------------------------------------------------------------------------------------------
CLSID AWindow::GetEncoderClsid(const WCHAR* format)
{
   UINT i = 0;
   UINT encoder_size = 0;
   UINT encoder_count = 0;
   std::unique_ptr<BYTE[], std::default_delete<BYTE[]>> buffer = 0;
   Gdiplus::ImageCodecInfo *encoders = 0;

   Gdiplus::GetImageEncodersSize(&encoder_count, &encoder_size);  // codec count can use and buffer size to store them
   if (encoder_size == 0)
      return CLSID();

   buffer = std::make_unique<BYTE[]>(encoder_size);
   encoders = reinterpret_cast<Gdiplus::ImageCodecInfo *>(buffer.get() );
   Gdiplus::GetImageEncoders(encoder_count, encoder_size, encoders);  // get info about all codec we have

   for (i = 0; i < encoder_count; ++i)
   {
      if (wcscmp(encoders[i].MimeType, format) == 0)  // if MimeType == image/png compare and execute
         return encoders[i].Clsid;
   }

   return CLSID();
}
//------------------------------------------------------------------------------------------------------------
