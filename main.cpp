#include <windows.h>
#include <iostream>
#include <stdlib.h>

#define _CRT_SECURE_NO_WARNINGS

bool isRussianLayout() {
    HWND foreground = GetForegroundWindow();
    DWORD threadId = GetWindowThreadProcessId(foreground, NULL);
    HKL layout = GetKeyboardLayout(threadId);

    // 0x419 - русская раскладка, 0x409 - английская
    return ((DWORD)layout & 0xFFFF) == 0x419;
}

bool blocked = false;


POINT saveCursorPos() {
    POINT pos;
    GetCursorPos(&pos);
    return pos;
}

void restoreCursorPos(POINT pos) {
    SetCursorPos(pos.x, pos.y);
}

void mouseMove(int x, int y) {
    SetCursorPos(x, y);
}

void mouseMoveRelative(int dx, int dy) {
    mouse_event(MOUSEEVENTF_MOVE, dx, dy, 0, 0);
}

void mouseClick() {
    mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
    Sleep(10);
    mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
}

void mouseClickAt(int x, int y) {
    SetCursorPos(x, y);
    Sleep(10);
    mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
    Sleep(10);
    mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
}

void mouseRightClickAt(int x, int y) {
    SetCursorPos(x, y);
    Sleep(10);
    mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
    Sleep(10);
    mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
}

void mouseLeftDownAt(int x, int y) {
    SetCursorPos(x, y);
    Sleep(10);
    mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
}

void mouseLeftUpAt(int x, int y) {
    SetCursorPos(x, y);
    Sleep(10);
    mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
}

void mouseDoubleClickAt(int x, int y) {
    SetCursorPos(x, y);
    Sleep(10);
    mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
    mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
    Sleep(100);
    mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
    mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
}

void mouseScroll(int scrollAmount) {
    mouse_event(MOUSEEVENTF_WHEEL, 0, 0, scrollAmount, 0);
}

void keyPress(int keyCode) {
    keybd_event(keyCode, 0, 0, 0);
    Sleep(10);
    keybd_event(keyCode, 0, KEYEVENTF_KEYUP, 0);
}

void keyDown(int keyCode) {
    keybd_event(keyCode, 0, 0, 0);
}

void keyUp(int keyCode) {
    keybd_event(keyCode, 0, KEYEVENTF_KEYUP, 0);
}

void keyHold(int keyCode, int holdTimeMs) {
    keybd_event(keyCode, 0, 0, 0);
    Sleep(holdTimeMs);
    keybd_event(keyCode, 0, KEYEVENTF_KEYUP, 0);
}

void typeText(const char* text) {
    for (int i = 0; text[i] != '\0'; i++) {
        SHORT vk = VkKeyScanA(text[i]);
        if (vk != -1) {
            keybd_event(LOBYTE(vk), 0, 0, 0);
            keybd_event(LOBYTE(vk), 0, KEYEVENTF_KEYUP, 0);
            Sleep(10);
        }
    }
}

void switchToEnglishLayoutIfNeeded() {
    if (isRussianLayout()) {
        std::cout << "Переключаем с русской на английскую раскладку...\n";
        keybd_event(VK_MENU, 0, 0, 0);      // Alt down
        keybd_event(VK_SHIFT, 0, 0, 0);     // Shift down
        Sleep(50);
        keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);  // Shift up
        keybd_event(VK_MENU, 0, KEYEVENTF_KEYUP, 0);   // Alt up
        Sleep(100);
    }
    else {
        std::cout << "Уже английская раскладка, не переключаем\n";
    }
}

HWND findWindow(const char* windowName) {
    return FindWindowA(NULL, windowName);
}

void minimizeWindow(const char* windowName) {
    HWND window = findWindow(windowName);
    if (window) {
        ShowWindow(window, SW_MINIMIZE);
    }
}

void maximizeWindow(const char* windowName) {
    HWND window = findWindow(windowName);
    if (window) {
        ShowWindow(window, SW_MAXIMIZE);
    }
}

void restoreWindow(const char* windowName) {
    HWND window = findWindow(windowName);
    if (window) {
        ShowWindow(window, SW_RESTORE);
    }
}

void closeWindow(const char* windowName) {
    HWND window = findWindow(windowName);
    if (window) {
        PostMessage(window, WM_CLOSE, 0, 0);
    }
}

void hideWindow(const char* windowName) {
    HWND window = findWindow(windowName);
    if (window) {
        ShowWindow(window, SW_HIDE);
    }
}

void showWindow(const char* windowName) {
    HWND window = findWindow(windowName);
    if (window) {
        ShowWindow(window, SW_SHOW);
    }
}

void activateWindow(const char* windowName) {
    HWND window = findWindow(windowName);
    if (window) {
        SetForegroundWindow(window);
        SetActiveWindow(window);
    }
}

void sendTextInstant(const char* text) {
    for (int i = 0; text[i] != '\0'; i++) {
        char c = text[i];
        BYTE vkCode = 0;
        bool needShift = false;

        if (c >= 'a' && c <= 'z') {
            vkCode = 0x41 + (c - 'a');
        }
        else if (c >= 'A' && c <= 'Z') {
            vkCode = 0x41 + (c - 'A');
            needShift = true;
        }
        else if (c >= '0' && c <= '9') {
            vkCode = 0x30 + (c - '0');
        }
        else if (c == ' ') {
            vkCode = VK_SPACE;
        }
        else if (c == ':') {
            vkCode = VK_OEM_1;
            needShift = true;
        }
        else if (c == '/') {
            vkCode = VK_OEM_2;
        }
        else if (c == '!') {
            vkCode = 0x31;
            needShift = true;
        }
        else {
            continue;
        }

        if (needShift) {
            keybd_event(VK_SHIFT, 0, 0, 0);
        }

        keybd_event(vkCode, 0, 0, 0);
        keybd_event(vkCode, 0, KEYEVENTF_KEYUP, 0);

        if (needShift) {
            keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);
        }
    }
}

void openChromeWithYouTube() {
    POINT originalPos;
    GetCursorPos(&originalPos);


    ShellExecuteA(0, "open", "chrome.exe", "https://www.youtube.com", 0, SW_SHOWMAXIMIZED);
    Sleep(4000);


    keyDown(VK_CONTROL);
    keyPress(0x54); // T
    keyUp(VK_CONTROL);

    SetCursorPos(originalPos.x, originalPos.y);
}
void openBrowserWithYouTube() {

    POINT originalPos;
    GetCursorPos(&originalPos);


    ShellExecuteA(0, "open", "https://www.youtube.com", 0, 0, SW_SHOWMAXIMIZED);
    Sleep(3000);


    keyDown(VK_CONTROL);
    keyPress(0x54);
    keyUp(VK_CONTROL);
    Sleep(1000);


    SetCursorPos(originalPos.x, originalPos.y);
}

void sendTextWithDelay(const char* text, int delayMs) {
    for (int i = 0; text[i] != '\0'; i++) {
        char c = text[i];
        BYTE vkCode = 0;
        bool needShift = false;

        if (c >= 'a' && c <= 'z') {
            vkCode = 0x41 + (c - 'a');
        }
        else if (c >= 'A' && c <= 'Z') {
            vkCode = 0x41 + (c - 'A');
            needShift = true;
        }
        else if (c == ' ') {
            vkCode = VK_SPACE;
        }
        else if (c == '!') {
            vkCode = 0x31;
            needShift = true;
        }
        else {
            continue;
        }

        if (needShift) {
            keybd_event(VK_SHIFT, 0, 0, 0);
            Sleep(10);
        }

        keybd_event(vkCode, 0, 0, 0);
        Sleep(delayMs);
        keybd_event(vkCode, 0, KEYEVENTF_KEYUP, 0);
        Sleep(10);

        if (needShift) {
            keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);
            Sleep(10);
        }

        Sleep(delayMs);
    }
}

/*int main() {
    std::cout << "Запускаем блокнот...\n";

    // Запускаем блокнот
    system("start notepad.exe");
    Sleep(1000); // Увеличил задержку чтобы блокнот успел открыться

    // Находим и активируем блокнот
    HWND notepad = FindWindowA("Notepad", NULL);
    if (notepad) {
        ShowWindow(notepad, SW_MAXIMIZE);
        SetForegroundWindow(notepad);
        SetActiveWindow(notepad);

        // Ждем немного чтобы окно активировалось
        Sleep(500);

        // Теперь проверяем и переключаем раскладку
        switchToEnglishLayoutIfNeeded();

        // Курсор и клик
        SetCursorPos(500, 300);
        mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
        mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);

        // Мгновенная печать
        sendTextInstant("HELLO WORLD ");

        // Печать с задержкой
        sendTextWithDelay("IT WORKS!", 50);
    }
    else {
        std::cout << "Блокнот не найден!\n";
    }

    return 0;
}*/
int main() {
    ShowWindow(GetConsoleWindow(), SW_HIDE);

    NOTIFYICONDATA nid = {};
    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.hWnd = GetConsoleWindow();
    nid.uID = 1;
    nid.uFlags = NIF_ICON | NIF_TIP;
    nid.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    lstrcpyW(nid.szTip, L"Outline Opener");
    Shell_NotifyIcon(NIM_ADD, &nid);
    while (1) {
        if (GetAsyncKeyState(VK_NUMPAD1) & 0x8000) {
            POINT originalPos = saveCursorPos();

            ShellExecuteA(0, "open", "C:\\Program Files (x86)\\Outline\\Outline.exe", 0, 0, SW_SHOW);
            mouseMove(957, 483);
            Sleep(3000);
            mouseClick();
            minimizeWindow("Outline");

            restoreCursorPos(originalPos);
            Sleep(300);
        }
        if (GetAsyncKeyState(VK_NUMPAD2) & 0x8000) {
            openBrowserWithYouTube();
            Sleep(300);
        }
        if (GetAsyncKeyState(VK_NUMPAD3) & 0x8000) {
            keyDown(VK_LWIN);
            keyDown(VK_SHIFT);
            keyPress(0x53);
            keyUp(VK_SHIFT);
            keyUp(VK_LWIN);
        }
        if (GetAsyncKeyState(VK_NUMPAD4) & 0x8000) {
            ShellExecuteA(0, "open", "explorer", 0, 0, SW_SHOW);
        }
        if (GetAsyncKeyState(VK_NUMPAD5) & 0x8000) {
            ShellExecuteA(0, "open", "\"C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\Common7\\IDE\\devenv.exe\"", 0, 0, SW_SHOW);
        }
        if (GetAsyncKeyState(VK_NUMPAD6) & 0x8000) {
            ShellExecuteA(0, "open", "C:\\Program Files (x86)\\Steam\\steam.exe", "", 0, SW_SHOW);
        }
        if (GetAsyncKeyState(VK_NUMPAD8) & 0x8000) {
            // Открыть переводчик
            ShellExecuteA(0, "open", "https://translate.google.com", 0, 0, SW_SHOW);
        }
        if (GetAsyncKeyState(VK_NUMPAD7) & 0x8000) {
            ShellExecuteA(0, "open", "calc.exe", 0, 0, SW_SHOW);
        }
        if (GetAsyncKeyState(VK_NUMPAD9) & 0x8000) {
            system("start notepad.exe");
            Sleep(1000);

            HWND notepad = FindWindowA("Notepad", NULL);
            if (notepad) {
                ShowWindow(notepad, SW_MAXIMIZE);
                SetForegroundWindow(notepad);
                SetActiveWindow(notepad);
                SYSTEMTIME time;
                GetLocalTime(&time);
                char date[20];
                sprintf_s(date, sizeof(date), "%02d/%02d/%02d", time.wDay, time.wMonth, time.wYear % 100);

                Sleep(500);

                switchToEnglishLayoutIfNeeded();

                SetCursorPos(500, 300);
                mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
                mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);

                sendTextInstant("Note: ");
                keyPress(VK_RETURN);
                keyPress(VK_RETURN);
                sendTextInstant("Date: ");
                typeText(date);
                Sleep(200);
                POINT originalPos = saveCursorPos();
                mouseMove(59, 51);
                Sleep(2000);
                mouseClick();
                restoreCursorPos(originalPos);
                Sleep(300);
            }
            else {
                std::cout << "Блокнот не найден!\n";
            }
        }
        if (GetAsyncKeyState(VK_ADD) & 0x8000) {
            for (int i = 0; i < 5; i++) {
                keyPress(VK_VOLUME_UP);
                Sleep(50);
            }
        }
        if (GetAsyncKeyState(VK_SUBTRACT) & 0x8000) {
            for (int i = 0; i < 5; i++) {
                keyPress(VK_VOLUME_DOWN);
                Sleep(50);
            }
        }
        if (GetAsyncKeyState(VK_MULTIPLY) & 0x8000) {
            POINT originalPos;
            GetCursorPos(&originalPos);

            SetCursorPos(0, 0);

            bool unlocked = false;
            while (!unlocked) {
                SetCursorPos(3000, 320);
                if (GetAsyncKeyState(VK_CONTROL) && GetAsyncKeyState(VK_NUMPAD0)) {
                    unlocked = true;
                }
                Sleep(100);
            }

            BlockInput(FALSE);
            SetCursorPos(originalPos.x, originalPos.y);
        }
        if (GetAsyncKeyState(VK_NUMPAD0) & 0x8000) {
            break;
        }
        Sleep(50);
    }
    return 0;
}
