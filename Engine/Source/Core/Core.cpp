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
      WS_EX_DLGMODALFRAME (0x00000001L) Создает окно с рамкой, похожей на диалоговое окно. Обычно используется для создания модальных окон.
      WS_EX_NOPARENTNOTIFY (0x00000004L) Указывает, что родительское окно не будет получать уведомления, когда дочерние окна создаются или уничтожаются.
      WS_EX_TOPMOST — делает окно всегда поверх других окон.
      WS_EX_ACCEPTFILES (0x00000010L) Позволяет перетаскивать файлы из Проводника в окно. При этом ваше окно получает сообщение WM_DROPFILES.
      WS_EX_TRANSPARENT (0x00000020L) Делает окно полупрозрачным для кликов мыши, то есть взаимодействие происходит с окном позади. Важно: Чтобы окно выглядело полупрозрачным, нужно дополнительно использовать SetLayeredWindowAttributes.
      WS_EX_MDICHILD (0x00000040L) Указывает, что окно является дочерним для окна MDI (Multiple Document Interface). Используется для приложений с MDI.
      WS_EX_TOOLWINDOW — создает окно без кнопки на панели задач.
      WS_EX_WINDOWEDGE (0x00000100L) Добавляет выпуклую границу (3D-эффект) к окну.
      WS_EX_CLIENTEDGE (0x00000200L) Добавляет вдавленную (внутреннюю) границу к клиентской области окна. Часто используется для создания ввода текста или других полей.
      WS_EX_CONTEXTHELP (0x00000400L) Добавляет кнопку "?" в заголовок окна. Нажатие на нее позволяет запрашивать справку для элементов интерфейса.
      WS_EX_LAYERED — позволяет создавать прозрачные или полупрозрачные окна.
      WS_EX_NOACTIVATE — окно не получает фокус при создании.
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
