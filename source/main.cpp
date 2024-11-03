#include <array>
#include <random>
#include <wiiuse/wpad.h>
#include "grrlib_class.h"

static constexpr u32 GRRLIB_BLACK   = 0x000000FF;
static constexpr u32 GRRLIB_RED     = 0xFF0000FF;
static constexpr u32 GRRLIB_LIME    = 0x00FF00FF;
static constexpr u32 GRRLIB_YELLOW  = 0xFFFF00FF;
static constexpr u32 GRRLIB_BLUE    = 0x0000FFFF;
static constexpr u32 GRRLIB_FUCHSIA = 0xFF00FFFF;
static constexpr u32 GRRLIB_AQUA    = 0x00FFFFFF;
static constexpr u32 GRRLIB_WHITE   = 0xFFFFFFFF;

int main(int argc, char **argv)
{
    constexpr std::array<u32, 8> ColorArray = {GRRLIB_BLACK, GRRLIB_RED, GRRLIB_LIME,
        GRRLIB_YELLOW, GRRLIB_BLUE, GRRLIB_FUCHSIA, GRRLIB_AQUA, GRRLIB_WHITE};

    std::default_random_engine Engine;
    std::uniform_int_distribution<> RandomInt(0, ColorArray.size() - 1);

    Screen::Initialize();
    Screen::SetBackgroundColor(GRRLIB_BLACK);

    WPAD_Init();
    WPAD_SetDataFormat(WPAD_CHAN_ALL, WPAD_FMT_BTNS_ACC);
    WPAD_SetVRes(WPAD_CHAN_ALL, Screen::GetWidth(), Screen::GetHeight());

    s8 ColorScreen = 0;
    bool ShowRandom = true;

    while(true)
    {
        WPAD_ScanPads(); // Scan the Wiimotes
        WPADData *PadData0 = WPAD_Data(WPAD_CHAN_0);

        if(PadData0->btns_d & WPAD_BUTTON_HOME)
        {
            break;
        }
        if(PadData0->btns_d & WPAD_BUTTON_LEFT)
        {
            ShowRandom = false;
            if(--ColorScreen < 0)
            {
                ColorScreen = 7;
            }
            Screen::SetBackgroundColor(ColorArray[ColorScreen]);
        }
        if(PadData0->btns_d & WPAD_BUTTON_RIGHT)
        {
            ShowRandom = false;
            if(++ColorScreen > 7)
            {
                ColorScreen = 0;
            }
            Screen::SetBackgroundColor(ColorArray[ColorScreen]);
        }
        if(PadData0->btns_d & WPAD_BUTTON_UP || PadData0->btns_d & WPAD_BUTTON_DOWN)
        {
            ShowRandom = true;
        }

        if(ShowRandom == true)
        {
            for(u16 x = 0; x < Screen::GetWidth(); ++x)
            {
                for(u16 y = 0; y < Screen::GetHeight(); ++y)
                {
                    Screen::SetPixel(x, y, ColorArray[RandomInt(Engine)]);
                }
            }
        }

        Screen::Render(); // Render the frame buffer to the TV
    }

    Screen::Exit();

    exit(0);
}
