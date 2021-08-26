// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include <afxwin.h>
#include <afxdllx.h>

static AFX_EXTENSION_MODULE GTS_MOTION_DLL = { NULL, NULL };

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH: {
    TRACE0("MCommCtrlor.DLL 正在初始化!\n");
        // 扩展 DLL 一次性初始化
    if (!AfxInitExtensionModule(GTS_MOTION_DLL, hModule))
            return 0;
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
    TRACE0("MCommCtrlor.DLL 正在终止!\n");
    // 在调用析构函数之前终止该库
    AfxTermExtensionModule(GTS_MOTION_DLL);
    break;
    }
    return TRUE;
}

