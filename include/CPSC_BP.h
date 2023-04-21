// #include <iostream>
#include <vector>

#ifdef _WIN32
// Windows Start
#define OS_NAME "Windows"
#include <windows.h>
// Windows End
#elif __APPLE__
// macOS code
#define OS_NAME "macOS"
// macOS End
#elif __linux__
// Linux code
#define OS_NAME "Linux"
#include <linux/fb.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
// Linux End
#elif __ANDROID__
// Android code
#define OS_NAME "Android"
// Android End
#else
#error "Unknown platform"
#endif

class CPSC_BP
{
public:
#ifdef _WIN32
    // Windows Start
    bool Can_Capture_Screen = true;
#elif __APPLE__
    // macOS code
    bool Can_Capture_Screen = false;
// macOS End
#elif __linux__
    // Linux code
    bool Can_Capture_Screen = true;
// Linux End
#elif __ANDROID__
    bool Can_Capture_Screen = false;
// Android End
#else
    bool Can_Capture_Screen = false;
#endif

    bool Request_Screen_Capture_API()
    {
#ifdef _WIN32
        // Windows Start
        Can_Capture_Screen = true;
        return true;
#elif __APPLE__
        // macOS code
        return false;
// macOS End
#elif __linux__
        // Linux code
        Can_Capture_Screen = true;
        return true;
// Linux End
#elif __ANDROID__
        // Android code
        return false;
// Android End
#else
        return false;
#endif
    }

    static int Get_Total_Number_Of_Monitors_API()
    {
#ifdef _WIN32
        return GetSystemMetrics(SM_CMONITORS);
#elif __linux__
        FILE *fp;
        char *line = NULL;
        size_t len = 0;
        ssize_t read;
        int num_monitors = 0;

        fp = popen("xrandr --query | grep ' connected' | wc -l", "r");
        if (fp == NULL)
        {
            perror("popen failed");
            exit(EXIT_FAILURE);
        }

        if ((read = getline(&line, &len, fp)) != -1)
        {
            num_monitors = atoi(line);
        }

        pclose(fp);
        free(line);

        return num_monitors;
#else
        return 0;
#endif
    }
#ifdef _WIN32
    static BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
    {
        std::vector<HMONITOR> *monitors = reinterpret_cast<std::vector<HMONITOR> *>(dwData);
        monitors->push_back(hMonitor);
        return TRUE;
    }
#endif

    // Don't Forget to Free Data Using delete Keyword
    static bool Capture_Screen_API(int Monitor_To_Capture, int &width, int &height, unsigned char **RGBA_Buffer)
    {
        // Check The Total Number Of Monitors
        if (Monitor_To_Capture >= Get_Total_Number_Of_Monitors_API())
        {
            return false;
        }

#ifdef _WIN32
        std::vector<HMONITOR> monitors;
        if (!(EnumDisplayMonitors(nullptr, nullptr, MonitorEnumProc, reinterpret_cast<LPARAM>(&monitors))))
        {
            return false;
        }

        HMONITOR hMonitor = monitors[Monitor_To_Capture];

        // Get the monitor info
        MONITORINFO monitorInfo;
        monitorInfo.cbSize = sizeof(monitorInfo);
        GetMonitorInfo(hMonitor, &monitorInfo);

        // Get the screen dimensions
        width = monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left;
        height = monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top;

        // Create a device context for the entire screen
        HDC hdcScreen = GetDC(NULL);

        // Create a memory device context and bitmap to store the screen capture
        HDC hdcMemDC = CreateCompatibleDC(hdcScreen);
        HBITMAP hbmScreen = CreateCompatibleBitmap(hdcScreen, width, height);
        HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMemDC, hbmScreen);

        // Capture the screen and store it in the bitmap
        BitBlt(hdcMemDC, 0, 0, width, height, hdcScreen, 0, 0, SRCCOPY);

        (*RGBA_Buffer) = new unsigned char[width * height * 4];
        BITMAPINFOHEADER bi = {sizeof(BITMAPINFOHEADER), width, -height, 1, 32, BI_RGB, 0, 0, 0, 0, 0};
        GetDIBits(hdcMemDC, hbmScreen, 0, height, (*RGBA_Buffer), (BITMAPINFO *)&bi, DIB_RGB_COLORS);

        // Clean up
        SelectObject(hdcMemDC, hbmOld);
        DeleteDC(hdcMemDC);
        ReleaseDC(NULL, hdcScreen);

        return true;
#elif __linux__

        char fb_path[64];
        sprintf(fb_path, "/dev/fb%d", Monitor_To_Capture);

        int fb_fd = 0;
        struct fb_var_screeninfo screen_info;
        unsigned char *fb_ptr = nullptr;

        // Open the framebuffer device
        int fb_fd = open(fb_path, O_RDWR);
        if (fb_fd == -1)
        {
            return false;
        }
        if (ioctl(fb_fd, FBIOGET_VSCREENINFO, &screen_info) == -1)
        {
            close(fb_fd);
            return false;
        }

        width = screen_info.xres_virtual;
        height = screen_info.yres_virtual;

        fb_ptr = (unsigned char *)mmap(0, (width * height * 4),
                                       PROT_READ | PROT_WRITE, MAP_SHARED, fb_fd, 0);
        if (fb_ptr == MAP_FAILED)
        {
            close(fb_fd);
            return false;
        }

        (*RGBA_Buffer) = new unsigned char[(width * height * 4)];
        memcpy((*RGBA_Buffer), fb_ptr, (width * height * 4));

        // Cleanup
        munmap(fb_ptr, (width * height * 4));
        close(fb_fd);

        return true;
#else
        return false;
#endif
    }
};