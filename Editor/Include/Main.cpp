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




// Global Variables:
AsEngine Engine;
//------------------------------------------------------------------------------------------------------------
int APIENTRY wWinMain(_In_ HINSTANCE handle_instance, _In_opt_ HINSTANCE handle_prev, _In_ LPWSTR cmd_line, _In_ int nCmdShow)
{
   return Engine.Update(handle_instance, nCmdShow);
}
//------------------------------------------------------------------------------------------------------------




// AsEngine
AsEngine *AsEngine::Self = 0;
//------------------------------------------------------------------------------------------------------------
AsEngine::~AsEngine()
{
   Gdiplus::GdiplusShutdown(GDI_Plus_Token);  // Clear memmory
}
//------------------------------------------------------------------------------------------------------------
AsEngine::AsEngine()
 : Button_Active(EButton_Action::Clicker_Exit), Is_Button_Clicked(false), Cmd_Show(0), Tick_Seconds(2), Handle_Instance(0), GDI_Plus_Token(0ULL),
   Window(0), Cursor_Stored {}
{
   Self = this;
   Gdiplus::GdiplusStartupInput gdiplusStartupInput;   // Init GDI+
   Gdiplus::GdiplusStartup(&GDI_Plus_Token, &gdiplusStartupInput, 0);
}
//------------------------------------------------------------------------------------------------------------
int AsEngine::Update(HINSTANCE handle_instance, int cmd_show)
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
int AsEngine::Tick() const
{
   HACCEL haccel_table = 0;
   MSG msg {};

   haccel_table = LoadAccelerators(Handle_Instance, MAKEINTRESOURCE(IDC_WINDOWSPROJECT1) );
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
void AsEngine::Window_Create()
{
   Window = new AWindow(50, 50);  // Starting window locations

   AsConfig::Hwnd = CreateWindowExW( /*WS_EX_LAYERED | */WS_EX_DLGMODALFRAME | WS_EX_TOPMOST | WS_EX_TOOLWINDOW | WS_EX_ACCEPTFILES | WS_EX_NOACTIVATE,  // WS_EX_DLGMODALFRAME offsets
      L"H", L"H", WS_POPUP, Window->Window_Rect.left, Window->Window_Rect.top, Window->Window_Rect.right, Window->Window_Rect.bottom, 0, 0, Handle_Instance, 0);

   if (!AsConfig::Hwnd != 0)
      return;

   ShowWindow(AsConfig::Hwnd, Cmd_Show);   // Or make add to view port func
   UpdateWindow(AsConfig::Hwnd);  // Call WM_Paint
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
   std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms) );  // Delay is click or hold

   if ( !(GetAsyncKeyState(VK_LBUTTON) & 0x8000) )
      return Window->Handle(EWindow_State::Down);  // Clicked Once
   else
      return Window->Handle(EWindow_State::Hold);  // Holding
}
//------------------------------------------------------------------------------------------------------------
void AsEngine::On_Timer_Update()
{
   int is_pressed = 0x8000;
   if (GetAsyncKeyState(VK_CONTROL) & is_pressed)
   {
      GetCursorPos(&Cursor_Stored);
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
