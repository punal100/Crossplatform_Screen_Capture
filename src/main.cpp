#include <iostream>
#include "CPSC_BP.h"
// #include <A/A.h>

int main()
{
    std::cout << "Foo main" << std::endl;

    char Total_Monitors[64];
    sprintf(Total_Monitors, "Total Monitors: %d", CPSC_BP::Get_Total_Number_Of_Monitors_API());
    std::cout << Total_Monitors << std::endl;

    int Width;
    int Height;
    unsigned char *RGBA_Buffer = nullptr;
    bool IsSuccessful = CPSC_BP::Capture_Screen_API(0, Width, Height, &RGBA_Buffer);

    if (RGBA_Buffer != nullptr)
    {
        delete RGBA_Buffer;
        RGBA_Buffer = nullptr;
    }

    return 0;
}
