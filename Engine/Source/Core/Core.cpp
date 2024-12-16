#include "Core.h"

// AsExamples
AsExamples::~AsExamples()
{

}
//------------------------------------------------------------------------------------------------------------
AsExamples::AsExamples()
{

}
//------------------------------------------------------------------------------------------------------------
void AsExamples::Init()
{
   // !!! Transfer for tutorial
   /*
      WS_EX_DLGMODALFRAME (0x00000001L) ������� ���� � ������, ������� �� ���������� ����. ������ ������������ ��� �������� ��������� ����.
      WS_EX_NOPARENTNOTIFY (0x00000004L) ���������, ��� ������������ ���� �� ����� �������� �����������, ����� �������� ���� ��������� ��� ������������.
      WS_EX_TOPMOST � ������ ���� ������ ������ ������ ����.
      WS_EX_ACCEPTFILES (0x00000010L) ��������� ������������� ����� �� ���������� � ����. ��� ���� ���� ���� �������� ��������� WM_DROPFILES.
      WS_EX_TRANSPARENT (0x00000020L) ������ ���� �������������� ��� ������ ����, �� ���� �������������� ���������� � ����� ������. �����: ����� ���� ��������� ��������������, ����� ������������� ������������ SetLayeredWindowAttributes.
      WS_EX_MDICHILD (0x00000040L) ���������, ��� ���� �������� �������� ��� ���� MDI (Multiple Document Interface). ������������ ��� ���������� � MDI.
      WS_EX_TOOLWINDOW � ������� ���� ��� ������ �� ������ �����.
      WS_EX_WINDOWEDGE (0x00000100L) ��������� �������� ������� (3D-������) � ����.
      WS_EX_CLIENTEDGE (0x00000200L) ��������� ���������� (����������) ������� � ���������� ������� ����. ����� ������������ ��� �������� ����� ������ ��� ������ �����.
      WS_EX_CONTEXTHELP (0x00000400L) ��������� ������ "?" � ��������� ����. ������� �� ��� ��������� ����������� ������� ��� ��������� ����������.
      WS_EX_LAYERED � ��������� ��������� ���������� ��� �������������� ����.
      WS_EX_NOACTIVATE � ���� �� �������� ����� ��� ��������.
   */

}
//------------------------------------------------------------------------------------------------------------




// AsTools
unsigned long long AsTools::How_Fast = 0LL;
//------------------------------------------------------------------------------------------------------------
AsTools::~AsTools()
{
   end_tick = __rdtsc();
   delta_tick = end_tick - start_tick;  // 194176 170824 192866
   How_Fast = delta_tick;

   if (delta_tick > 2954178)  // 2954178
      How_Fast = 0;  // Good || 55`907`530 | 42`215`898
   else if (delta_tick < 2954178)
      How_Fast = 0;  // Bad |
}
//------------------------------------------------------------------------------------------------------------
AsTools::AsTools()
   : start_tick(0LL), end_tick(0LL), delta_tick(0LL)
{
   start_tick = __rdtsc();
}
//------------------------------------------------------------------------------------------------------------




// AsConfig
EHUD_State AsConfig::HUD_State = EHUD_State::None;
bool AsConfig::Is_Playing = true;
HWND AsConfig::Hwnd = 0;
HDC AsConfig::Hdc = 0;
POINT AsConfig::Cursor_Pos = {};
//------------------------------------------------------------------------------------------------------------
void AsConfig::Throw()
{
	return throw 13;
}
//------------------------------------------------------------------------------------------------------------
LRESULT AsConfig::Window_Procedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
      HUD_State = EHUD_State::Draw;
      break;

   case WM_LBUTTONDOWN:
   {// !!! Refactoring make class or func if nothing changed

      std::this_thread::sleep_for(std::chrono::milliseconds(150));  // Time to prepare windows
      AsConfig::Is_Playing = !AsConfig::Is_Playing;

      InvalidateRect(hWnd, 0, AsConfig::Is_Background);  // Need redraw image, just call WM_PAINT set image name

      if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
         AsConfig::Is_Playing = true;

      while (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
      {
         std::this_thread::sleep_for(std::chrono::milliseconds(10));  // Not every milliseconds get and move
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
