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
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp)
{
   size_t total_size = size * nmemb;
   userp->append((char*)contents, total_size);
   return total_size;
}
//------------------------------------------------------------------------------------------------------------
static std::wstring stringToWstring(const std::string& str) {
   if (str.empty()) return L"";

   // Определяем размер выходной строки
   int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
   if (size_needed <= 0) {
      throw std::runtime_error("Failed to convert string to wstring");
   }

   // Создаем буфер и конвертируем
   std::wstring wstr(size_needed, 0);
   MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &wstr[0], size_needed);

   // Удаляем нулевой символ в конце
   wstr.pop_back();
   return wstr;
}
//------------------------------------------------------------------------------------------------------------
static size_t Write_To_File(void* contents, size_t size, size_t nmemb, FILE* file)
{// Коллбэк для записи данных в файл
   size_t total_size = size * nmemb;
   fwrite(contents, 1, total_size, file);  // Запись данных в файл
   return total_size;
}
//------------------------------------------------------------------------------------------------------------
static void Read_Data_From_File()
{
   size_t found_pos = 0;
   const wchar_t* find_text = L"<div class=\"club-boost__image\"><img src=";
   std::wstring line{};

   std::wifstream in_file(L"output.txt", std::ios::in | std::ios::binary);
   if (in_file.is_open())
   {
      while (std::getline(in_file, line))
      {
         found_pos = line.find(find_text);
         if (found_pos != std::wstring::npos)
            return;
      }
      in_file.close();
   }
}
//------------------------------------------------------------------------------------------------------------
static void SimulateKeyPress(WORD keyCode) {
   INPUT input[2] = {}; // Массив для нажатия и отпускания клавиши

   // Настройка события для нажатия клавиши
   input[0].type = INPUT_KEYBOARD;
   input[0].ki.wVk = keyCode;   // Код клавиши
   input[0].ki.dwFlags = 0;     // Нажатие клавиши

   // Настройка события для отпускания клавиши
   input[1].type = INPUT_KEYBOARD;
   input[1].ki.wVk = keyCode;   // Код клавиши
   input[1].ki.dwFlags = KEYEVENTF_KEYUP; // Отпускание клавиши

   // Отправляем события нажатия и отпускания клавиши
   SendInput(2, input, sizeof(INPUT));
}
//------------------------------------------------------------------------------------------------------------
static void Press_Button_Combo(const char key_button_hold)
{
   // Массив для нажатия и отпускания клавиш
   INPUT input[4] = {};

   // Настройка события для нажатия клавиши CTRL
   input[0].type = INPUT_KEYBOARD;
   input[0].ki.wVk = VK_CONTROL;  // Код клавиши CTRL
   input[0].ki.dwFlags = 0;       // Нажатие клавиши

   // Настройка события для нажатия клавиши C
   input[1].type = INPUT_KEYBOARD;
   input[1].ki.wVk = key_button_hold;         // Код клавиши C
   input[1].ki.dwFlags = 0;       // Нажатие клавиши

   // Настройка события для отпускания клавиши C
   input[2].type = INPUT_KEYBOARD;
   input[2].ki.wVk = key_button_hold;         // Код клавиши C
   input[2].ki.dwFlags = KEYEVENTF_KEYUP; // Отпускание клавиши

   // Настройка события для отпускания клавиши CTRL
   input[3].type = INPUT_KEYBOARD;
   input[3].ki.wVk = VK_CONTROL;  // Код клавиши CTRL
   input[3].ki.dwFlags = KEYEVENTF_KEYUP; // Отпускание клавиши

   // Отправляем события для нажатия и отпускания клавиш
   SendInput(4, input, sizeof(INPUT));
}
//------------------------------------------------------------------------------------------------------------
static void Handle_Clipboard()
{
   WCHAR* psz_text;
   HANDLE handle_data;

   if (!OpenClipboard(0))
      return;

   handle_data = GetClipboardData(CF_UNICODETEXT);
   if (!handle_data != 0)
      return;

   if (!(psz_text = static_cast<WCHAR*>(GlobalLock(handle_data) ) ) )
      return;

   if (!wcsstr(psz_text, L"http://") != 0 && !wcsstr(psz_text, L"https://") != 0)  // try to find proto in text
      EmptyClipboard();
   else
      if (psz_text != 0)
         wcsncpy_s(test, wcslen(psz_text) + 1, psz_text, wcslen(psz_text));

   GlobalUnlock(handle_data);
   CloseClipboard();
}
//------------------------------------------------------------------------------------------------------------
static void func(const char *ip_twitch_irc)
{
   std::string oauth_token = "oauth:sn6jr44d94bdcc2ixwv7bzwk17mbob"; // Ваш токен
   std::string username = "getboros"; // Ваш никнейм на Twitch
   std::string server = "irc.chat.twitch.tv"; // IRC-сервер Twitch
   int port = 6667; // Порт для подключения

   // Инициализация Winsock
   WSADATA wsaData;
   if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
      return;

   // Создаем сокет
   SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
   if (sock == INVALID_SOCKET)
   {
      WSACleanup();
      return;
   }

   // Настройка адреса сервера
   sockaddr_in serverAddr;
   serverAddr.sin_family = AF_INET;
   serverAddr.sin_port = htons(port);
   //inet_pton(AF_INET, ip_twitch_irc, &serverAddr.sin_addr.s_addr);


   // Подключение к серверу
   if (connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
   {
      closesocket(sock);
      WSACleanup();
      return;
   }
   
   // "Подключено к Twitch IRC!" Отправляем команды авторизации
   std::string passCmd = "PASS " + oauth_token + "\r\n";
   std::string nickCmd = "NICK " + username + "\r\n";
   std::string joinCmd = "JOIN #your_channel\r\n";  // Присоединение к каналу

   send(sock, passCmd.c_str(), (int)passCmd.length(), 0); // Отправка токена
   send(sock, nickCmd.c_str(), (int)nickCmd.length(), 0); // Отправка ника
   send(sock, joinCmd.c_str(), (int)joinCmd.length(), 0); // Подключение к каналу

   // Чтение данных от сервера
   char buffer[512 * 2] {};

   while (true)
   {
      int bytesReceived = recv(sock, buffer, sizeof(buffer) - 1, 0);
      if (bytesReceived > 0)
      {
         buffer[bytesReceived] = '\0';

         // TEMP
         if (strstr(buffer, "PRIVMSG") != nullptr)  // Если сервер прислал сообщение, обрабатываем его
            AsConfig::Throw();
         // TEMP END
      }
      else if (bytesReceived == 0) {
         // Соединение закрыто сервером
         break;
      }
      else {
         break;
      }
   }



   // Закрытие сокета
   closesocket(sock);
   WSACleanup();
}
//------------------------------------------------------------------------------------------------------------
static void resolveDNSWithCurl(char *&ip_twitch_irc, const std::string& hostname)
{
   CURL* curl;
   CURLcode res;
   const std::string broadcasterId = "gp762nuuoqcoxypju8c569th9wz7q5";
   // Инициализация libcurl
   curl = curl_easy_init();
   if (!curl) {
      return;
   }

   std::string url = "https://api.twitch.tv/helix/channels?broadcaster_id=" + broadcasterId;

   // Создаем DNS-запрос
   curl_easy_setopt(curl, CURLOPT_URL, hostname.c_str());
   curl_easy_setopt(curl, CURLOPT_NOBODY, 1L); // Не нужно загружать тело ответа

   res = curl_easy_perform(curl);

   res = curl_easy_getinfo(curl, CURLINFO_PRIMARY_IP, &ip_twitch_irc);
   if ((res == CURLE_OK) && ip_twitch_irc)
      return;  //std::cout << "IP-адрес для " << hostname << ": " << ip << std::endl;

   // Очистка ресурсов
   curl_easy_cleanup(curl);
}
//------------------------------------------------------------------------------------------------------------
static size_t Test_00(void* contents, size_t size, size_t nmemb, void* userp) {
   size_t realsize = size * nmemb;
   std::string* data = (std::string*)userp;
   data->append((char*)contents, realsize);
   return realsize;
}
//------------------------------------------------------------------------------------------------------------
static void getTwitchChannelName(const std::string& oauthToken, const std::string& broadcasterId, const std::string& yourClientId) {
   CURL* curl;
   CURLcode res;
   std::string readBuffer;

   // Инициализация cURL
   curl_global_init(CURL_GLOBAL_DEFAULT);
   curl = curl_easy_init();

   if (curl) {
      // Устанавливаем URL
      std::string url = "https://api.twitch.tv/helix/channels?broadcaster_id=" + broadcasterId;
      curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

      // Создаем список заголовков
      struct curl_slist* headers = nullptr;
      std::string clientIdHeader = "Client-ID: " + yourClientId;
      headers = curl_slist_append(headers, clientIdHeader.c_str());
      std::string authHeader = "Authorization: Bearer " + oauthToken;
      headers = curl_slist_append(headers, authHeader.c_str());

      curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, Test_00);
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

      res = curl_easy_perform(curl);

      curl_slist_free_all(headers);
      curl_easy_cleanup(curl);
   }

   curl_global_cleanup();
}//------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------




// ACurl
ACurl::~ACurl()
{
}
//------------------------------------------------------------------------------------------------------------
ACurl::ACurl()
{
   CURL* curl = 0;
   CURLcode res{};
   const char site_url[] = "https://animestars.org/";
   const char site_url_club_page[] = "https://animestars.org/clubs/25/boost/";
   std::string read_buffer{};  // conver to wstring?
   std::wstring read_buffer_w{};
   FILE* file = 0;

   errno_t err = fopen_s(&file, "output.txt", "wb");   // Открытие файла для записи
   if (err != 0 || !file != 0)
      return;
   curl = curl_easy_init();
   if (!curl != 0)
      return;
   curl_easy_setopt(curl, CURLOPT_URL, site_url_club_page);  // Установка URL
   curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, Write_To_File);  // Настройка коллбэка для записи полученных данных
   curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);  // Передача файла для записи данных
   //curl_easy_setopt(curl, CURLOPT_WRITEDATA, &read_buffer);  // Передача указателя на строку в WriteCallback
   res = curl_easy_perform(curl);  // Выполнение запроса
   if (res != CURLE_OK)
      AsConfig::Throw();
   //   std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
   //else
   //   std::cout << "Received Data:\n" << read_buffer << std::endl;  // Вывод полученных данных

   fclose(file);  // Закрытие файла
   curl_easy_cleanup(curl);  // Очистка ресурсов

   Read_Data_From_File();
   //read_buffer_w = stringToWstring(read_buffer);
   //auto test = read_buffer_w.find(L"<div class=\"club-boost__image\"><img src=\"");
}
//------------------------------------------------------------------------------------------------------------




// ATest
void ATest::Draw_Image(HDC hdc, bool is_play)
{
   Gdiplus::Image *gdi_image;

   //gdi_image = new Gdiplus::Image(L"Pictures/1.png");
   if (is_play)
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

   delete gdi_image;
}
//------------------------------------------------------------------------------------------------------------
void ATest::Add_Second_Window(HWND hWnd)
{// !!! Refactor

   Handle_Second_Window = CreateWindowExW(
      WS_EX_DLGMODALFRAME | WS_EX_TOPMOST | WS_EX_TOOLWINDOW | WS_EX_ACCEPTFILES | WS_EX_NOACTIVATE,
      L"Test", L"Test", WS_POPUP, 150, 150, 300, 300, hWnd, 0, GetModuleHandle(NULL), 0);

   if (!Handle_Second_Window != 0)
      return;

   ShowWindow(Handle_Second_Window, SW_SHOW); // Показываем второе окно
   UpdateWindow(Handle_Second_Window);
}
//------------------------------------------------------------------------------------------------------------
void ATest::Draw_Text_Custom(HDC hdc, const wchar_t *text, COLORREF text_color, COLORREF text_bk)
{
   SetTextColor(hdc, text_color);  // Устанавливаем красный цвет текста
   SetBkColor(hdc, text_bk);  // Устанавливаем белый цвет фона текста
   TextOutW(hdc, 0, 0, text, (int)wcslen(text) );
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




// AWindow
HWND AWindow::Hwnd_Window_Second = 0;
//------------------------------------------------------------------------------------------------------------
AWindow::~AWindow()
{
}
//------------------------------------------------------------------------------------------------------------
AWindow::AWindow()
 : Title_Name_Extended {}, Window_Name_Extended {}
{
   wcsncpy_s(Title_Name_Extended, wcslen(Title_Name_Extended) - 1, L"Hello", wcslen(L"Hello") );
   wcsncpy_s(Window_Name_Extended, wcslen(Window_Name_Extended) - 1, L"Hello", wcslen(L"Hello") );
}
//------------------------------------------------------------------------------------------------------------
int AWindow::Create(HINSTANCE handle_instance, int cmd_show, bool is_timer) const
{
   HACCEL hAccelTable = 0;
   const RECT &rect = AsConfig::Window_Main_Rect;
   MSG msg {};
   WNDCLASSEXW wcex
   {
   .cbSize = sizeof(WNDCLASSEX),
   .style = CS_HREDRAW | CS_VREDRAW,
   .lpfnWndProc = &AWindow::Window_Procedure,
   .hInstance = handle_instance,
   .hIcon = LoadIcon(handle_instance, MAKEINTRESOURCE(IDI_WINDOWSPROJECT1) ),
   .hCursor = LoadCursor(0, IDC_ARROW),
   .hbrBackground = (HBRUSH)(COLOR_WINDOW + 1),
   .lpszClassName = Window_Name_Extended,
   .hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL) )
   };

   RegisterClassExW(&wcex);
   AsConfig::Hwnd = CreateWindowExW( /*WS_EX_LAYERED | */WS_EX_DLGMODALFRAME | WS_EX_TOPMOST | WS_EX_TOOLWINDOW | WS_EX_ACCEPTFILES | WS_EX_NOACTIVATE,
      Window_Name_Extended, Title_Name_Extended, WS_POPUP, rect.left, rect.top, rect.right, rect.bottom, 0, 0, handle_instance, 0 );

   if (!AsConfig::Hwnd != 0)
      return 1;

   if (is_timer)
   {
      SetTimer(AsConfig::Hwnd, AsConfig::ID_Timer, AsConfig::FPS, 0);  // ID = 1, Frame Per Seconds 1000 / 24, CullBack Func = 0
      SetLayeredWindowAttributes(AsConfig::Hwnd, AsConfig::transparentColor, AsConfig::Alpha_Visibility, LWA_COLORKEY | LWA_ALPHA);  // Set window transparencies
   }

   ShowWindow(AsConfig::Hwnd, cmd_show);
   UpdateWindow(AsConfig::Hwnd);  // Call WM_Paint

   hAccelTable = LoadAccelerators(handle_instance, MAKEINTRESOURCE(IDC_WINDOWSPROJECT1) );
   while (GetMessage(&msg, 0, 0, 0) )  // Main message loop:
   {
      if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg) )
      {
         TranslateMessage(&msg);
         DispatchMessage(&msg);
      }
   }

   return (int) msg.wParam;
}
//------------------------------------------------------------------------------------------------------------
LRESULT AWindow::Window_Procedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
    {// Change to custom class

       PAINTSTRUCT ps {};
       ATest test;

       AsConfig::Hdc = BeginPaint(hWnd, &ps);

       test.Draw_Image(AsConfig::Hdc, AsConfig::Is_Playing);
       if (!AsConfig::Is_Playing != false)
       {
          AsClicker().Is_Running(1);
          test.Draw_Image(AsConfig::Hdc, !AsConfig::Is_Playing);
       }

       //test.Draw_Text_Custom(hdc, L"Some Text", RGB(255, 0, 0), RGB(255, 255, 255) );  // use enum? || RED

       EndPaint(hWnd, &ps);
    }
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




// AHUD
AHUD::~AHUD()
{
   Gdiplus::GdiplusShutdown(GDI_Plus_Token);  // Clear memmory
}
//------------------------------------------------------------------------------------------------------------
AHUD::AHUD(HINSTANCE handle_instance, int cmd_show, bool is_timer)
 : Cmd_Show(cmd_show), Timer(is_timer), Handle_Instance(handle_instance), GDI_Plus_Token(0ULL)
{
   WNDCLASSEXW wcex
   {
   .cbSize = sizeof(WNDCLASSEX),
   .style = CS_HREDRAW | CS_VREDRAW,
   .lpfnWndProc = &AsConfig::Window_Procedure,
   .hInstance = Handle_Instance,
   .hIcon = LoadIcon(Handle_Instance, MAKEINTRESOURCE(IDI_WINDOWSPROJECT1) ),
   .hCursor = LoadCursor(0, IDC_ARROW),
   .hbrBackground = (HBRUSH)(COLOR_WINDOW + 1),
   .lpszClassName = L"Hello",
   .hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL))
   };
   RegisterClassExW(&wcex);

   Gdiplus::GdiplusStartupInput gdiplusStartupInput;   // Init GDI+
   Gdiplus::GdiplusStartup(&GDI_Plus_Token, &gdiplusStartupInput, 0);
}
//------------------------------------------------------------------------------------------------------------
int AHUD::Init()
{
   Window_Create();

   return Tick();
}
//------------------------------------------------------------------------------------------------------------
void AHUD::Draw_Image()
{
   ATest test;
   PAINTSTRUCT ps{};
   AsConfig::Hdc = BeginPaint(AsConfig::Hwnd, &ps);

   test.Draw_Image(AsConfig::Hdc, AsConfig::Is_Playing);
   if (!AsConfig::Is_Playing != false)
   {
      AsClicker().Is_Running(1);
      test.Draw_Image(AsConfig::Hdc, !AsConfig::Is_Playing);
   }

   //test.Draw_Text_Custom(hdc, L"Some Text", RGB(255, 0, 0), RGB(255, 255, 255) );  // use enum? || RED

   EndPaint(AsConfig::Hwnd, &ps);
}
//------------------------------------------------------------------------------------------------------------
void AHUD::Update()
{
   switch (AsConfig::HUD_State)
   {
   case EHUD_State::None:
      break;
   case EHUD_State::Draw:
      Draw_Image();
      break;
   default:
      break;
   }

   AsConfig::HUD_State = EHUD_State::None;
}
//------------------------------------------------------------------------------------------------------------
void AHUD::Window_Create() const
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

   return;
}
//------------------------------------------------------------------------------------------------------------
int AHUD::Tick()
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




//------------------------------------------------------------------------------------------------------------
int APIENTRY wWinMain(_In_ HINSTANCE handle_instance, _In_opt_ HINSTANCE handle_prev, _In_ LPWSTR cmd_line, _In_ int cmd)
{
   return AHUD(handle_instance, cmd, false).Init();

   if (!Is_App() != false)
      return AWindow().Create(handle_instance, cmd, false);
   else
      return AsClicker().Is_Running(1);

   //char *ip_twitch_irc = 0;
   //curl_global_init(CURL_GLOBAL_DEFAULT);

   //std::string hostname = "irc.chat.twitch.tv"; // DNS-имя сервера Twitch IRC
   //resolveDNSWithCurl(*&ip_twitch_irc, hostname);

   //curl_global_cleanup();
   //func(ip_twitch_irc);
   //return 0;
}
//------------------------------------------------------------------------------------------------------------
