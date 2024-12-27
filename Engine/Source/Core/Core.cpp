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
EButton_Action AsConfig::Button_Active = EButton_Action::Clicker_Exit;
int AsConfig::Active_Button = 0;
int AsConfig::Clicker_Timer = 4;
HWND AsConfig::Hwnd = 0;
POINT AsConfig::Cursor_Pos = {};
//------------------------------------------------------------------------------------------------------------
AsConfig::~AsConfig()
{
}
//------------------------------------------------------------------------------------------------------------
void AsConfig::Throw()
{
	return throw 13;
}
//------------------------------------------------------------------------------------------------------------
const wchar_t *AsConfig::Clicker_Image_Folder()
{
   return L"Pictures/Clicker_Image.png";
}
//------------------------------------------------------------------------------------------------------------
