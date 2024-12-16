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
int AsClicker::Is_Running(int timer)
{
   auto perform_action = [](const SCoordinate &cords, INPUT *input_type, size_t input_count, int timer_ms)
   {
      SetCursorPos(cords.x, cords.y);  // After each set cursor need return to prev position
      SendInput(static_cast<UINT>(input_count), input_type, sizeof(INPUT) );
      std::this_thread::sleep_for(std::chrono::milliseconds(timer_ms) );
   };

   while (true)
   {// !!! Refactoring

      perform_action(cord_send_cd, Inputs_Keyboard, 2, 200);  // F5
      perform_action(cord_send_cd, Inputs_Mouses, 2, (timer) * 1000);  // Pressed at cord

      if ( (GetAsyncKeyState(VK_CONTROL) & 0x8000) && (GetAsyncKeyState('Q') & 0x8000) )
         return 0;
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
 : Cmd_Show(0), Handle_Instance(0), GDI_Plus_Token(0ULL)
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
   .hIcon = LoadIcon(Handle_Instance, MAKEINTRESOURCE(IDI_WINDOWSPROJECT1)),
   .hCursor = LoadCursor(0, IDC_ARROW),
   .hbrBackground = (HBRUSH)(COLOR_WINDOW + 1),
   .lpszClassName = L"Hello",
   .hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL))
   };

   RegisterClassExW(&wcex);
   Window_Create();
   return Tick();
}
//------------------------------------------------------------------------------------------------------------
void AsMain_Window::On_Paint(HWND hwnd)
{
   PAINTSTRUCT ps{};
   HDC hdc = BeginPaint(AsConfig::Hwnd, &ps);
   Gdiplus::Image *gdi_image;

   if (AsConfig::Is_Playing)
      gdi_image = new Gdiplus::Image(L"Pictures/Record_Button.png");
   else
      gdi_image = new Gdiplus::Image(L"Pictures/Record_Button_Stop.png");

   if (gdi_image->GetLastStatus() == Gdiplus::Ok)
   {
      if (false)
      {
         Gdiplus::Bitmap resized_bitmap(AsConfig::Window_Width, AsConfig::Window_Height, PixelFormat32bppARGB);
         Gdiplus::Graphics resized_graphics(&resized_bitmap);

         resized_graphics.SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBicubic);  // interpolation quality
         resized_graphics.DrawImage(gdi_image, 0, 0, AsConfig::Window_Width, AsConfig::Window_Height);  // draw resized image

         Gdiplus::Graphics graphics(hdc);
         graphics.DrawImage(&resized_bitmap, 0, 0);  // show resized image in window
      }
      else
      {
         Gdiplus::Graphics graphics(hdc);
         graphics.DrawImage(gdi_image, 0, 0);  // show resized image in window
      }
   }

   if (!AsConfig::Is_Playing != false)
      AsClicker().Is_Running(1);

   delete gdi_image;
   EndPaint(AsConfig::Hwnd, &ps);
}
//------------------------------------------------------------------------------------------------------------
void AsMain_Window::Window_Create() const
{
   const RECT &rect = AsConfig::Window_Main_Rect;

   // Create Window
   AsConfig::Hwnd = CreateWindowExW( /*WS_EX_LAYERED | */WS_EX_DLGMODALFRAME | WS_EX_TOPMOST | WS_EX_TOOLWINDOW | WS_EX_ACCEPTFILES | WS_EX_NOACTIVATE,
      L"Hello", L"Hello", WS_POPUP, rect.left, rect.top, rect.right, rect.bottom, 0, 0, Handle_Instance, 0);

   if (!AsConfig::Hwnd != 0)
      return;

   // Or make add to view port func
   ShowWindow(AsConfig::Hwnd, Cmd_Show);
   UpdateWindow(AsConfig::Hwnd);  // Call WM_Paint
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
      InvalidateRect(hWnd, 0, AsConfig::Is_Background);  // Call WM_PAINT, draw all with background color
      break;

   case WM_PAINT:
      Self->On_Paint(hWnd);
      break;

   case WM_LBUTTONDOWN:
   {// !!! Refactoring make class or func if nothing changed

      std::this_thread::sleep_for(std::chrono::milliseconds(150) );  // Time to prepare windows
      AsConfig::Is_Playing = !AsConfig::Is_Playing;

      InvalidateRect(hWnd, 0, AsConfig::Is_Background);  // Need redraw image, just call WM_PAINT set image name

      if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
         AsConfig::Is_Playing = true;

      while (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
      {
         std::this_thread::sleep_for(std::chrono::milliseconds(10) );  // Not every milliseconds get and move
         GetCursorPos(&AsConfig::Cursor_Pos);
         MoveWindow(hWnd, AsConfig::Cursor_Pos.x, AsConfig::Cursor_Pos.y, AsConfig::Window_Main_Rect.right, AsConfig::Window_Main_Rect.bottom, false);
      }
   }
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
