#pragma once

#include <Windows.h>

namespace Application::Window {

    enum class ExtendedWindowStyle {
        DlgModalFrame = WS_EX_ACCEPTFILES,
        NoParentNotify = WS_EX_NOPARENTNOTIFY,
        TopMost = WS_EX_TOPMOST,
        AcceptFiles = WS_EX_ACCEPTFILES,
        Transparent = WS_EX_TRANSPARENT,
        MdiChild = WS_EX_MDICHILD,
        ToolWindow = WS_EX_TOOLWINDOW,
        WindowEdge = WS_EX_WINDOWEDGE,
        ClientEdge = WS_EX_CLIENTEDGE,
        ContextHelp = WS_EX_CONTEXTHELP,

        Right = WS_EX_RIGHT,
        Left = WS_EX_LEFT,
        RtlReading = WS_EX_RTLREADING,
        LtrReading = WS_EX_LTRREADING,
        LeftScrollbar = WS_EX_LEFTSCROLLBAR,
        RightScrollbar = WS_EX_RIGHTSCROLLBAR,

        ControlParent = WS_EX_CONTROLPARENT,
        StaticEdge = WS_EX_STATICEDGE,
        AppWindow = WS_EX_APPWINDOW,

        OverlappedWindow = WindowEdge | ClientEdge,
        PaletteWindow = WindowEdge | ToolWindow | TopMost,

        Layered = WS_EX_LAYERED,
        NoInheritLayout = WS_EX_NOINHERITLAYOUT,

        NoRedirectionBitmap = WS_EX_NOREDIRECTIONBITMAP,

        LayoutRtl = WS_EX_LAYOUTRTL,
        Composited = WS_EX_COMPOSITED,

        NoActivate = WS_EX_NOACTIVATE
    };

}