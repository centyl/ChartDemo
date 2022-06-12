#pragma once

#include <Windows.h>

namespace Application::Window {

    enum class WindowStyle : unsigned long long {
        Overlapped = WS_OVERLAPPED,
        Popup = WS_POPUP,
        Child = WS_CHILD,
        Minimize = WS_MINIMIZE,
        Visible = WS_VISIBLE,
        Disabled = WS_DISABLED,
        ClipSiblings = WS_CLIPSIBLINGS,
        ClipChildren = WS_CLIPCHILDREN,
        Maximize = WS_MAXIMIZE,
        Border = WS_BORDER,
        DlgFrame = WS_DLGFRAME,
        VerticalScroll = WS_VSCROLL,
        HorizontalScroll = WS_HSCROLL,
        SysMenu = WS_SYSMENU,
        ThickFrame = WS_THICKFRAME,
        Group = WS_GROUP,
        TabStop = WS_TABSTOP,

        Caption = Border | DlgFrame,
        MinimizeBox = Group,
        MaximizeBox = TabStop,
        Tiled = Overlapped,
        Iconic = Minimize,
        SizeBox = ThickFrame,

        OverlappedWindow = Overlapped | Caption | SysMenu | ThickFrame | MinimizeBox | MaximizeBox,

        TiledWindow = OverlappedWindow,

        PopupWindow = Popup | Border | SysMenu,
        ChildWindow = Child
    };

    WindowStyle& operator|(WindowStyle lhs, const WindowStyle& rhs);
}