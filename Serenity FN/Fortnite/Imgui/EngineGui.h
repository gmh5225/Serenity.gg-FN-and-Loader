#include <WinUser.h>
#include <string>
#include <string.h>
#include <processthreadsapi.h>

int current = 1;

#define IM_ARRAYSIZE(_ARR)          ((int)(sizeof(_ARR)/sizeof(*_ARR)))

namespace hotkeys
{
    int aimkey;
    int hipfirekey;
}

static int keystatus = 0;
static int realkey = 0;

bool GetKey(int key)
{
    realkey = key;
    return true;
}
void ChangeKey(void* blank)
{
    keystatus = 1;
    while (true)
    {
        for (int i = 0; i < 0x87; i++)
        {
            if (GetKeyState(i) & 0x8000)
            {
                hotkeys::aimkey = i;
                keystatus = 0;
                return;
            }
        }
    }
}

static const char* keyNames[] =
{
    "unknown",
    "vk_lbutton",
    "vk_rbutton",
    "vk_cancel",
    "vk_mbutton",
    "vk_xbutton1",
    "vk_xbutton2",
    "unknown",
    "vk_back",
    "vk_tab",
    "unknown",
    "unknown",
    "vk_clear",
    "vk_return",
    "unknown",
    "unknown",
    "vk_shift",
    "vk_control",
    "vk_menu",
    "vk_pause",
    "vk_capital",
    "vk_kana",
    "unknown",
    "vk_junja",
    "vk_final",
    "vk_kanji",
    "unknown",
    "vk_escape",
    "vk_convert",
    "vk_nonconvert",
    "vk_accept",
    "vk_modechange",
    "vk_space",
    "vk_prior",
    "vk_next",
    "vk_end",
    "vk_home",
    "vk_left",
    "vk_up",
    "vk_right",
    "vk_down",
    "vk_select",
    "vk_print",
    "vk_execute",
    "vk_snapshot",
    "vk_insert",
    "vk_delete",
    "vk_help",
    "0",
    "1",
    "2",
    "3",
    "4",
    "5",
    "6",
    "7",
    "8",
    "9",
    "unknown",
    "unknown",
    "unknown",
    "unknown",
    "unknown",
    "unknown",
    "unknown",
    "a",
    "b",
    "c",
    "d",
    "e",
    "f",
    "g",
    "h",
    "i",
    "j",
    "k",
    "l",
    "m",
    "n",
    "o",
    "p",
    "q",
    "r",
    "s",
    "t",
    "u",
    "v",
    "w",
    "x",
    "y",
    "z",
    "vk_lwin",
    "vk_rwin",
    "vk_apps",
    "unknown",
    "vk_sleep",
    "vk_numpad0",
    "vk_numpad1",
    "vk_numpad2",
    "vk_numpad3",
    "vk_numpad4",
    "vk_numpad5",
    "vk_numpad6",
    "vk_numpad7",
    "vk_numpad8",
    "vk_numpad9",
    "vk_multiply",
    "vk_add",
    "vk_separator",
    "vk_subtract",
    "vk_decimal",
    "vk_divide",
    "vk_f1",
    "vk_f2",
    "vk_f3",
    "vk_f4",
    "vk_f5",
    "vk_f6",
    "vk_f7",
    "vk_f8",
    "vk_f9",
    "vk_f10",
    "vk_f11",
    "vk_f12",
    "vk_f13",
    "vk_f14",
    "vk_f15",
    "vk_f16",
    "vk_f17",
    "vk_f18",
    "vk_f19",
    "vk_f20",
    "vk_f21",
    "vk_f22",
    "vk_f23",
    "vk_f24",
    "unknown",
    "unknown",
    "unknown",
    "unknown",
    "unknown",
    "unknown",
    "unknown",
    "unknown",
    "vk_numlock",
    "vk_scroll",
    "vk_oem_nec_equal",
    "vk_oem_fj_masshou",
    "vk_oem_fj_touroku",
    "vk_oem_fj_loya",
    "vk_oem_fj_roya",
    "unknown",
    "unknown",
    "unknown",
    "unknown",
    "unknown",
    "unknown",
    "unknown",
    "unknown",
    "unknown",
    "vk_lshift",
    "vk_rshift",
    "vk_lcontrol",
    "vk_rcontrol",
    "vk_lmenu",
    "vk_rmenu"
};

static bool Items_ArrayGetter(void* data, int idx, const char** out_text)
{
    const char* const* items = (const char* const*)data;
    if (out_text)
        *out_text = items[idx];
    return true;
}
void HotkeyButton(int aimkey, void* changekey, int status)
{
    const char* preview_value = NULL;
    if (aimkey >= 0 && aimkey < IM_ARRAYSIZE(keyNames))
        Items_ArrayGetter(keyNames, aimkey, &preview_value);

    std::string aimkeys;
    if (preview_value == NULL)
        aimkeys = "Select Key";
    else
        aimkeys = preview_value;

    if (status == 1)
    {
        aimkeys = "Press the key";
    }
    if (ImGui::Button(aimkeys.c_str(), ImVec2(125, 45)))
    {
        if (status == 0)
        {
            CreateThread(0, 0, (LPTHREAD_START_ROUTINE)changekey, nullptr, 0, nullptr);
        }
    }
}

static int keystatuss = 0;
static int realkeys = 0;

bool GetKeys(int key)
{
    realkeys = key;
    return true;
}
void ChangeKeys(void* blank)
{
    keystatuss = 1;
    while (true)
    {
        for (int i = 0; i < 0x87; i++)
        {
            if (GetKeyState(i) & 0x8000)
            {
                hotkeys::hipfirekey = i;
                keystatuss = 0;
                return;
            }
        }
    }
}

static const char* keyNamess[] =
{
    "unknown",
    "vk_lbutton",
    "vk_rbutton",
    "vk_cancel",
    "vk_mbutton",
    "vk_xbutton1",
    "vk_xbutton2",
    "unknown",
    "vk_back",
    "vk_tab",
    "unknown",
    "unknown",
    "vk_clear",
    "vk_return",
    "unknown",
    "unknown",
    "vk_shift",
    "vk_control",
    "vk_menu",
    "vk_pause",
    "vk_capital",
    "vk_kana",
    "unknown",
    "vk_junja",
    "vk_final",
    "vk_kanji",
    "unknown",
    "vk_escape",
    "vk_convert",
    "vk_nonconvert",
    "vk_accept",
    "vk_modechange",
    "vk_space",
    "vk_prior",
    "vk_next",
    "vk_end",
    "vk_home",
    "vk_left",
    "vk_up",
    "vk_right",
    "vk_down",
    "vk_select",
    "vk_print",
    "vk_execute",
    "vk_snapshot",
    "vk_insert",
    "vk_delete",
    "vk_help",
    "0",
    "1",
    "2",
    "3",
    "4",
    "5",
    "6",
    "7",
    "8",
    "9",
    "unknown",
    "unknown",
    "unknown",
    "unknown",
    "unknown",
    "unknown",
    "unknown",
    "a",
    "b",
    "c",
    "d",
    "e",
    "f",
    "g",
    "h",
    "i",
    "j",
    "k",
    "l",
    "m",
    "n",
    "o",
    "p",
    "q",
    "r",
    "s",
    "t",
    "u",
    "v",
    "w",
    "x",
    "y",
    "z",
    "vk_lwin",
    "vk_rwin",
    "vk_apps",
    "unknown",
    "vk_sleep",
    "vk_numpad0",
    "vk_numpad1",
    "vk_numpad2",
    "vk_numpad3",
    "vk_numpad4",
    "vk_numpad5",
    "vk_numpad6",
    "vk_numpad7",
    "vk_numpad8",
    "vk_numpad9",
    "vk_multiply",
    "vk_add",
    "vk_separator",
    "vk_subtract",
    "vk_decimal",
    "vk_divide",
    "vk_f1",
    "vk_f2",
    "vk_f3",
    "vk_f4",
    "vk_f5",
    "vk_f6",
    "vk_f7",
    "vk_f8",
    "vk_f9",
    "vk_f10",
    "vk_f11",
    "vk_f12",
    "vk_f13",
    "vk_f14",
    "vk_f15",
    "vk_f16",
    "vk_f17",
    "vk_f18",
    "vk_f19",
    "vk_f20",
    "vk_f21",
    "vk_f22",
    "vk_f23",
    "vk_f24",
    "unknown",
    "unknown",
    "unknown",
    "unknown",
    "unknown",
    "unknown",
    "unknown",
    "unknown",
    "vk_numlock",
    "vk_scroll",
    "vk_oem_nec_equal",
    "vk_oem_fj_masshou",
    "vk_oem_fj_touroku",
    "vk_oem_fj_loya",
    "vk_oem_fj_roya",
    "unknown",
    "unknown",
    "unknown",
    "unknown",
    "unknown",
    "unknown",
    "unknown",
    "unknown",
    "unknown",
    "vk_lshift",
    "vk_rshift",
    "vk_lcontrol",
    "vk_rcontrol",
    "vk_lmenu",
    "vk_rmenu"
};

static bool Items_ArrayGetters(void* data, int idx, const char** out_text)
{
    const char* const* items = (const char* const*)data;
    if (out_text)
        *out_text = items[idx];
    return true;
}
void HotkeyButtons(int hipfirekey, void* changekeys, int status)
{
    const char* preview_value = NULL;
    if (hipfirekey >= 0 && hipfirekey < IM_ARRAYSIZE(keyNamess))
        Items_ArrayGetters(keyNamess, hipfirekey, &preview_value);

    std::string hipfirekeys;
    if (preview_value == NULL)
        hipfirekeys = "Select Key";
    else
        hipfirekeys = preview_value;

    if (status == 1)
    {
        hipfirekeys = "Press the key";
    }
    if (ImGui::Button(hipfirekeys.c_str(), ImVec2(125, 45)))
    {
        if (status == 0)
        {
            CreateThread(0, 0, (LPTHREAD_START_ROUTINE)changekeys, nullptr, 0, nullptr);
        }
    }
}
