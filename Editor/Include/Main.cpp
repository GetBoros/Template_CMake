#include "Main.h"

constexpr bool DEBUG = false;

// Global func
static consteval bool Is_App()
{
   if constexpr (DEBUG)
      return true;
   else
      return false;
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
   const int timer = 1;  // !!! Move elsewhere every 1 second

   switch (Button_Action)
   {
   case EButton_Action::Clicker_Start:
      AsClicker().Is_Running(timer, Youtube_Emo_Cord);
      break;
   case EButton_Action::Clicker_Settings:
      //AsConfig::Throw();  // !!! Clicker Setting, have no idea what to do
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
 : Window_Rect{ x_cord, y_cord, AsConfig::Window_Offset + 1, AsConfig::Window_Offset + 1 }, Buttons_Vector(0)
{
	Buttons_Vector = new std::vector<AButton *>();
	Buttons_Vector->reserve(3);  // !!! Change to constexpr
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
	auto perform_action = [](const SCoordinate &cords, INPUT *input_type, size_t input_count, int timer_ms)
   {
      SetCursorPos(cords.x, cords.y);  // After each set cursor need return to prev position
      SendInput(static_cast<UINT>(input_count), input_type, sizeof(INPUT) );
      std::this_thread::sleep_for(std::chrono::milliseconds(timer_ms) );
   };

   while (true)
   {// !!! Refactoring

		perform_action(test, Inputs_Keyboard, 2, 200);  // F5 then after 200 ms go to next action
      perform_action(test, Inputs_Mouses, 2, (timer) * 1000);  // Pressed at cord

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




// Global Variables:
AsMain_Window Main_Window;
//------------------------------------------------------------------------------------------------------------
int APIENTRY wWinMain(_In_ HINSTANCE handle_instance, _In_opt_ HINSTANCE handle_prev, _In_ LPWSTR cmd_line, _In_ int nCmdShow)
{
   return Main_Window.Main(handle_instance, nCmdShow);
}
//------------------------------------------------------------------------------------------------------------




// AsMain_Window
AsMain_Window *AsMain_Window::Self = 0;
//------------------------------------------------------------------------------------------------------------
AsMain_Window::~AsMain_Window()
{
   Gdiplus::GdiplusShutdown(GDI_Plus_Token);  // Clear memmory
}
//------------------------------------------------------------------------------------------------------------
AsMain_Window::AsMain_Window()
 : Button_Active(EButton_Action::Clicker_Exit), Is_Button_Clicked(false), Cmd_Show(0), Tick_Seconds(2), Handle_Instance(0), GDI_Plus_Token(0ULL)
{
   Self = this;
   Gdiplus::GdiplusStartupInput gdiplusStartupInput;   // Init GDI+
   Gdiplus::GdiplusStartup(&GDI_Plus_Token, &gdiplusStartupInput, 0);
}
//------------------------------------------------------------------------------------------------------------
int AsMain_Window::Main(HINSTANCE handle_instance, int cmd_show)
{
   Cmd_Show = cmd_show;
   Handle_Instance = handle_instance;

   WNDCLASSEXW wcex
   {
   .cbSize = sizeof(WNDCLASSEX),
   .style = CS_HREDRAW | CS_VREDRAW,
   .lpfnWndProc = Window_Procedure,
   .hInstance = Handle_Instance,
   .hIcon = LoadIcon(Handle_Instance, MAKEINTRESOURCE(IDI_WINDOWSPROJECT1) ),
   .hCursor = LoadCursor(0, IDC_ARROW),
   .hbrBackground = (HBRUSH)(COLOR_WINDOW + 1),
   .lpszClassName = L"H",
   .hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL) )
   };
   RegisterClassExW(&wcex);
   Window_Create();
   
   return Tick();
}
//------------------------------------------------------------------------------------------------------------
void AsMain_Window::Window_Create()
{
   Window = new AWindow(50, 50);

   Window->Add_Button(new AButton(27, 20, EButton_Action::Clicker_Start) );  // Or create button in window?
   Window->Add_Button(new AButton(27, 20, EButton_Action::Clicker_Settings) );  // Or create button in window?
   Window->Add_Button(new AButton(27, 20, EButton_Action::Clicker_Exit) );  // Or create button in window?

   AsConfig::Hwnd = CreateWindowExW( /*WS_EX_LAYERED | */WS_EX_DLGMODALFRAME | WS_EX_TOPMOST | WS_EX_TOOLWINDOW | WS_EX_ACCEPTFILES | WS_EX_NOACTIVATE,  // WS_EX_DLGMODALFRAME offsets
      L"H", L"H", WS_POPUP, Window->Window_Rect.left, Window->Window_Rect.top, Window->Window_Rect.right, Window->Window_Rect.bottom, 0, 0, Handle_Instance, 0);

   if (!AsConfig::Hwnd != 0)
      return;

   ShowWindow(AsConfig::Hwnd, Cmd_Show);   // Or make add to view port func
   UpdateWindow(AsConfig::Hwnd);  // Call WM_Paint
}
//------------------------------------------------------------------------------------------------------------
void AsMain_Window::On_Paint(HWND hwnd)
{
   PAINTSTRUCT ps {};
   HDC hdc = BeginPaint(hwnd, &ps);

   Draw_Image(hdc, L"Pictures/Main_Image.png");

   if (Button_Active != EButton_Action::Clicker_Exit)
      Draw_Active_Button(hdc);

   EndPaint(AsConfig::Hwnd, &ps);

   if (Is_Button_Clicked)
      Window->Update_Button_Active();
}
//------------------------------------------------------------------------------------------------------------
void AsMain_Window::On_LMB_Down(HWND hwnd)
{
   const int delay_ms = 150;
   int i = 0;
   int button_vector_size = 0;
   AButton *button = 0;
   RECT rect {};

   // 1.0. Call WM_PAINT and delay
   InvalidateRect(hwnd, 0, AsConfig::Is_Draw_At_BG);  // Need redraw image, just call WM_PAINT set image name
   std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms) );  // Delay 15 ms || If not hold | clicked once, then redraw image or draw new image
   GetCursorPos(&AsConfig::Cursor_Pos);  // Check cords & rect to handle

   // 1.1. Click Handle
   if ( !(GetAsyncKeyState(VK_LBUTTON) & 0x8000) )  // If not hold LMB then check where clicked
   {
      Button_Active = (EButton_Action)Window->On_Button_Clicked();  // Get active button
		Is_Button_Clicked = true;
      return;
   }

	// 1.2. Move Window
   while (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
   {// If hold LMB then move window

      std::this_thread::sleep_for(std::chrono::milliseconds(10) );  // Not every milliseconds get and move
      GetCursorPos(&AsConfig::Cursor_Pos);
      MoveWindow(hwnd, AsConfig::Cursor_Pos.x, AsConfig::Cursor_Pos.y, Window->Window_Rect.right, Window->Window_Rect.bottom, false);
   }

   // 1.3. Re-Save cords for buttons in array
   GetWindowRect(hwnd, &rect);
   button_vector_size = static_cast<int>(Window->Buttons_Vector->size() );

   for (i; i < button_vector_size; i++)
	{
		button = Window->Buttons_Vector->at(i);
      button->Button_Rect.left = rect.left + AsConfig::Window_Offset / 2 + button->Button_Width * i;
      button->Button_Rect.top = rect.top + AsConfig::Window_Offset / 2;
      button->Button_Rect.right = button->Button_Rect.left + button->Button_Width * (i == 0 ? 1 : i);
      button->Button_Rect.bottom = button->Button_Rect.top + button->Button_Height;
   }
}
//------------------------------------------------------------------------------------------------------------
void AsMain_Window::Draw_Image(HDC hdc, const wchar_t *image_path) const
{
   const int button_exit = 108;
	const int button_width = 27;
	const int button_height = 20;
	const int button_third = 54;
   Gdiplus::Image *gdi_image;
   Gdiplus::Graphics graphics(hdc);
   
   gdi_image = new Gdiplus::Image(image_path);
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
void AsMain_Window::Draw_Active_Button(HDC hdc)
{
   const int width = Window->Buttons_Vector->at(0)->Button_Width;
   const int height = Window->Buttons_Vector->at(0)->Button_Height;  // while only one raw always 19
	const int index = static_cast<int>(Button_Active);
   HPEN pen_green = CreatePen(PS_SOLID, 1, RGB(0, 255, 0) );  // Создаем зеленую ручку для рисования линий
   HGDIOBJ pen_prev = SelectObject(hdc, pen_green);
   RECT rect { width * index, 0, width * (index + 1), height - 1 };

   MoveToEx(hdc, rect.left, rect.top, 0);
   LineTo(hdc, rect.right, rect.top);
   LineTo(hdc, rect.right, rect.bottom);
   LineTo(hdc, rect.left, rect.bottom);
   LineTo(hdc, rect.left, rect.top);

   SelectObject(hdc, pen_prev);
   DeleteObject(pen_green);
}
//------------------------------------------------------------------------------------------------------------
int AsMain_Window::Tick()
{
   HACCEL hAccelTable = 0;
   MSG msg {};

   hAccelTable = LoadAccelerators(Handle_Instance, MAKEINTRESOURCE(IDC_WINDOWSPROJECT1) );
   while (GetMessage(&msg, 0, 0, 0) )  // Main message loop:
   {
      if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg) )
      {
         TranslateMessage(&msg);
         DispatchMessage(&msg);
      }
   }

   return (int)msg.wParam;
}
//------------------------------------------------------------------------------------------------------------
LRESULT AsMain_Window::Window_Procedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
   switch (message)
   {
   case WM_COMMAND:
      AsConfig::Throw();
      break;

   case WM_TIMER:
      SetWindowPos(hWnd, HWND_TOPMOST, 200, 200, 0, 0, SWP_NOSIZE);
      InvalidateRect(hWnd, 0, AsConfig::Is_Draw_At_BG);  // Call WM_PAINT, draw all with background color
      break;

   case WM_PAINT:
      Self->On_Paint(hWnd);
      break;

   case WM_LBUTTONDOWN:
      Self->On_LMB_Down(hWnd);
      break;

   case WM_KEYDOWN:
      break;

   case WM_MOUSEWHEEL:
      Self->Tick_Seconds = GET_WHEEL_DELTA_WPARAM(wParam) > 0 ? ++Self->Tick_Seconds : --Self->Tick_Seconds;  // !!! Bad example
      InvalidateRect(hWnd, 0, AsConfig::Is_Draw_At_BG);
      break;

   case WM_DESTROY:
      PostQuitMessage(0);
      break;

   default:
      return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}
//------------------------------------------------------------------------------------------------------------
