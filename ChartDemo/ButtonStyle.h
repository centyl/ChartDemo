#pragma once

#include <Windows.h>

namespace Application::Window {

    enum class ButtonStyle {
        PushButton = BS_PUSHBUTTON,
        DefaultPushButton = BS_DEFPUSHBUTTON,
        Checkbox = BS_CHECKBOX,
        AutoCheckbox = BS_AUTOCHECKBOX,
        RadioButton = BS_RADIOBUTTON,
        ThreeState = BS_3STATE,
        AutoThreeState = BS_AUTO3STATE,
        GroupBox = BS_GROUPBOX,
        UserButton = BS_USERBUTTON,
        AutoRadioButton = BS_AUTORADIOBUTTON,
        PushBox = BS_PUSHBOX,
        OwnerDraw = BS_OWNERDRAW,
        TypeMask = BS_TYPEMASK,
        LeftText = BS_LEFTTEXT,
        Text = BS_TEXT,
        Icon = BS_ICON,
        Bitmap = BS_BITMAP,
        Left = BS_LEFT,
        Right = BS_RIGHT,
        Center = BS_CENTER,
        Top = BS_TOP,
        Bottom = BS_BOTTOM,
        VerticalCenter = BS_VCENTER,
        PushLike = BS_PUSHLIKE,
        Multiline = BS_MULTILINE,
        Notify = BS_NOTIFY,
        Flat = BS_FLAT,
        RightButton = LeftText
    };

    ButtonStyle& operator|(ButtonStyle lhs, const ButtonStyle& rhs);
}

