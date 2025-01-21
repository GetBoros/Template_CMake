#include "Engine.h"

// AsEngine
AsEngine *AsEngine::Self = 0;
//------------------------------------------------------------------------------------------------------------
AsEngine::~AsEngine()
{
   Gdiplus::GdiplusShutdown(GDI_Plus_Token);  // Clear memmory
}
//------------------------------------------------------------------------------------------------------------
AsEngine::AsEngine()
 : GDI_Plus_Token(0ULL), Window(0)
{
   Self = this;
   Gdiplus::GdiplusStartupInput gdiplusStartupInput;   // Init GDI+
   Gdiplus::GdiplusStartup(&GDI_Plus_Token, &gdiplusStartupInput, 0);
}
//------------------------------------------------------------------------------------------------------------
int AsEngine::Update(HINSTANCE handle_instance, int cmd_show)
{
   WNDCLASSEXW wcex
   {
   .cbSize = sizeof(WNDCLASSEX),
   .style = CS_HREDRAW | CS_VREDRAW,
   .lpfnWndProc = Window_Procedure,
   .hInstance = handle_instance,
   .hIcon = LoadIcon(handle_instance, MAKEINTRESOURCE(IDI_WINDOWSPROJECT1) ),
   .hCursor = LoadCursor(0, IDC_ARROW),
   .hbrBackground = (HBRUSH)(COLOR_WINDOW + 1),
   .lpszClassName = AsConfig::Window_Name,
   .hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL) )
   };
  
   RegisterClassExW(&wcex);
   Window = new AWindow(50, 50, handle_instance);  // Starting window locations
   Window->Handle(EWindow_State::Load_Resources);
   
   if (!AsConfig::Hwnd != 0)
      return 0;

   ShowWindow(AsConfig::Hwnd, cmd_show);   // Or make add to view port func
   UpdateWindow(AsConfig::Hwnd);  // Call WM_Paint

   return Tick(handle_instance);
}
//------------------------------------------------------------------------------------------------------------
int AsEngine::Tick(HINSTANCE handle_instance) const
{
   HACCEL haccel_table = 0;
   MSG msg {};

   haccel_table = LoadAccelerators(handle_instance, MAKEINTRESOURCE(IDC_WINDOWSPROJECT1) );
   while (GetMessage(&msg, 0, 0, 0) )  // Main message loop:
   {
      if (!TranslateAccelerator(msg.hwnd, haccel_table, &msg) )
      {
         TranslateMessage(&msg);
         DispatchMessage(&msg);
      }
   }
   return (int)msg.wParam;
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::On_Paint()
{
   Window->Handle(EWindow_State::Draw);
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::On_LMB_Down()
{
   const int delay_ms = 150;

   InvalidateRect(AsConfig::Hwnd, 0, AsConfig::Is_Draw_At_BG);
   std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms) );  // if hold LMB longer then delay_ms

   if ( !(GetAsyncKeyState(VK_LBUTTON) & 0x8000) )
      return Window->Handle(EWindow_State::Down);  // Clicked Once
   else
      return Window->Handle(EWindow_State::Hold);  // Holding
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::On_Key_Down()
{
   int yy = 0;
   yy++;
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::On_Timer_Update()
{
   int is_pressed = 0x8000;
   if (GetAsyncKeyState(VK_CONTROL) & is_pressed)
   {
      GetCursorPos(&AsConfig::Cursor_Pos);
      KillTimer(AsConfig::Hwnd, 1);
   }
}
//------------------------------------------------------------------------------------------------------------
LRESULT AsEngine::Window_Procedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
   switch (message)
   {
   case WM_COMMAND:
      AsConfig::Throw();
      break;

   case WM_TIMER:
      Self->On_Timer_Update();
      break;

   case WM_PAINT:
      Self->On_Paint();
      break;

   case WM_LBUTTONDOWN:
      Self->On_LMB_Down();
      break;

   case WM_KEYDOWN:
		Self->On_Key_Down();
      break;

   case WM_MOUSEWHEEL:
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
