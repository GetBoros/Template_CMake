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
AButton::AButton()
 : Width(0), Height(0), Button_Rect {}
{
}
//------------------------------------------------------------------------------------------------------------
void AButton::Set_Size(int width, int height)
{
	Width = width;
	Height = height;
}
//------------------------------------------------------------------------------------------------------------




// AWindow
AWindow::~AWindow()
{
}
//------------------------------------------------------------------------------------------------------------
AWindow::AWindow()
 : X(0), Y(0), Width(0), Height(0), Button(0), Window_Temp(EWindow_Temp::None), Window_Offset(6)
{
   Width = Window_Offset + 1;
   Height = Window_Offset + 1;
}
//------------------------------------------------------------------------------------------------------------
void AWindow::Handle()
{
   const int timer = 1;  // !!! Move elsewhere every 1 second

   switch (Window_Temp)
   {
   case EWindow_Temp::None:
      break;
   case EWindow_Temp::Clicker:
      AsClicker().Is_Running(timer, Anime_Stars_Cord);
      break;
   case EWindow_Temp::Exit:
      PostQuitMessage(0);
      break;
   default:
      break;
   }
}
//------------------------------------------------------------------------------------------------------------
void AWindow::Button_Click_Handle()
{
   if (PtInRect(&Button->Button_Rect, AsConfig::Cursor_Pos) )  // First button
      Window_Temp = EWindow_Temp::Clicker;
}
//------------------------------------------------------------------------------------------------------------
void AWindow::Set_Location(int x, int y)
{
	X = x;
	Y = y;
}
//------------------------------------------------------------------------------------------------------------
void AWindow::Set_Size(int width, int height)
{
	Width = width;
	Height = height;
}
//------------------------------------------------------------------------------------------------------------
void AWindow::Add_Button(AButton *button)
{
   Button = button;  // !!! Temp add to array all added buttons
	
   // Give button his x, y cords
   button->Button_Rect.left = X + Window_Offset / 2;
   button->Button_Rect.top = Y + Window_Offset / 2;
   button->Button_Rect.right = button->Button_Rect.left + button->Width;
   button->Button_Rect.bottom = button->Button_Rect.top + button->Height;

   // Resize window after add buttons
   Width += button->Width;
   if (Height < button->Height)
      Height = button->Height + Window_Offset + 1;
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
      perform_action(test, Inputs_Mouses, 2, (timer) * /*150*/1000);  // Pressed at cord

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
 : Cmd_Show(0), Tick_Seconds(2), Handle_Instance(0), GDI_Plus_Token(0ULL)
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
	Button = new AButton();
   Window = new AWindow();
   Button->Set_Size(27, 20);  // Must be before to add to window
   Window->Set_Location(50, 50);  // Must be before add buttons
   Window->Add_Button(Button);

   AsConfig::Hwnd = CreateWindowExW( /*WS_EX_LAYERED | */WS_EX_DLGMODALFRAME | WS_EX_TOPMOST | WS_EX_TOOLWINDOW | WS_EX_ACCEPTFILES | WS_EX_NOACTIVATE,
      L"H", L"H", WS_POPUP, Window->X, Window->Y, Window->Width, Window->Height, 0, 0, Handle_Instance, 0);

   if (!AsConfig::Hwnd != 0)
      return;

   ShowWindow(AsConfig::Hwnd, Cmd_Show);   // Or make add to view port func
   UpdateWindow(AsConfig::Hwnd);  // Call WM_Paint
}
//------------------------------------------------------------------------------------------------------------
void AsMain_Window::On_Paint(HWND hwnd)
{
   PAINTSTRUCT ps {};
   HDC hdc = BeginPaint(AsConfig::Hwnd, &ps);

   Draw_Image(hdc, L"Pictures/Main_Image.png");

   EndPaint(AsConfig::Hwnd, &ps);
}
//------------------------------------------------------------------------------------------------------------
void AsMain_Window::On_LMB_Down(HWND hwnd)
{
   const int delay_ms = 150;

   // 1.0. Call WM_PAINT and delay
   InvalidateRect(hwnd, 0, AsConfig::Is_Draw_At_BG);  // Need redraw image, just call WM_PAINT set image name
   std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms) );  // Delay 15 ms || If not hold | clicked once, then redraw image or draw new image
   
   // 1.1. Click Handle
   if ( !(GetAsyncKeyState(VK_LBUTTON) & 0x8000) )
	{// If not hold LMB then check where clicked

      GetCursorPos(&AsConfig::Cursor_Pos);  // Check cords & rect to handle
      Window->Button_Click_Handle();  // Check where button was clicked
      Window->Handle();  // Handle all actions responsed on button clicked
   }
   
   while (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
   {// If hold LMB then move window

      std::this_thread::sleep_for(std::chrono::milliseconds(10) );  // Not every milliseconds get and move
      GetCursorPos(&AsConfig::Cursor_Pos);
      MoveWindow(hwnd, AsConfig::Cursor_Pos.x, AsConfig::Cursor_Pos.y, Window->Width, Window->Height, false);
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
