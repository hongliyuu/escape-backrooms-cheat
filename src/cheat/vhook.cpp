#include "vhook.h"
#include "_sdk.h"
#include "gvalue.h"
#include "kismet.h"

vhook* vhook::get()
{
	static vhook inst;
	return &inst;
}

void vhook::hook_func(void* post_render, void* wnd_proc)
{
    HWND window = nullptr;
    while (true)
    {
        gvalue::world = SDK::UWorld::GetWorld();
        window = kismet::get_window();
        if (gvalue::world && gvalue::world->OwningGameInstance &&
            gvalue::world->OwningGameInstance->LocalPlayers.Num() > 0 &&
            gvalue::world->OwningGameInstance->LocalPlayers[0] &&
            gvalue::world->OwningGameInstance->LocalPlayers[0]->ViewportClient && window)
        {
            break;
        }
        Sleep(100);
    }

    gvalue::vtb = *(void***)gvalue::world->OwningGameInstance->LocalPlayers[0]->ViewportClient;
    if (!gvalue::vtb)
    {
        return;
    }

    DWORD protect = 0;
    if (!VirtualProtect(&gvalue::vtb[0x63], sizeof(void*), PAGE_EXECUTE_READWRITE, &protect))
    {
        return;
    }

    gvalue::def_post_render = (fn_post_render)(gvalue::vtb[0x63]);
    gvalue::vtb[0x63] = post_render;
    DWORD ignored = 0;
    VirtualProtect(&gvalue::vtb[0x63], sizeof(void*), protect, &ignored);

    gvalue::def_wnd_proc = (WNDPROC)SetWindowLongPtrA(window, GWLP_WNDPROC, (LONG_PTR)wnd_proc);
}

void vhook::unhook_func()
{
    if (HWND window = kismet::get_window(); window && gvalue::def_wnd_proc)
    {
        SetWindowLongPtrA(window, GWLP_WNDPROC, (LONG_PTR)gvalue::def_wnd_proc);
    }

    if (!gvalue::vtb || !gvalue::def_post_render)
    {
        return;
    }

    DWORD protect = 0;
    if (VirtualProtect(&gvalue::vtb[0x63], sizeof(void*), PAGE_EXECUTE_READWRITE, &protect))
    {
        gvalue::vtb[0x63] = gvalue::def_post_render;
        DWORD ignored = 0;
        VirtualProtect(&gvalue::vtb[0x63], sizeof(void*), protect, &ignored);
    }
}
