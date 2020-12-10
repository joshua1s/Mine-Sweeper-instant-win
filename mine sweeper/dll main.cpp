#include <Windows.h>
#include <thread>
#include <iostream>

typedef int(__stdcall* tWinFn)(int);
tWinFn fnWin = reinterpret_cast<tWinFn>(0x100347C);

DWORD WINAPI OnDllAttach(LPVOID base)
{
    AllocConsole(); FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);

    std::cout << "Press right shift to win.\nPress delete to uninject.\n";

    while (!GetAsyncKeyState(VK_DELETE))
    {
        if (GetAsyncKeyState(VK_RSHIFT))
            fnWin(1);

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    fclose(f);
    FreeConsole();
    FreeLibraryAndExitThread(static_cast<HMODULE>(base), 0);
}


BOOL WINAPI DllMain(HINSTANCE hinstDll, DWORD fdwReason, LPVOID lpvReserved)
{
    if (fdwReason == DLL_PROCESS_ATTACH)
        CreateThread(nullptr, 0, OnDllAttach, hinstDll, 0, nullptr);

    return TRUE;
}
