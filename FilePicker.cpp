#include "FilePicker.h"

#include <windows.h>
#include <commdlg.h>

std::string selectVideoFile()
{
    OPENFILENAME ofn;
    char filename[MAX_PATH] = "";

    ZeroMemory(&ofn, sizeof(ofn));

    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = nullptr;

    ofn.lpstrFilter =
        "Video Files\0*.mp4;*.mkv;*.avi;*.mov\0"
        "All Files\0*.*\0";

    ofn.lpstrFile = filename;
    ofn.nMaxFile = MAX_PATH;

    ofn.Flags =
        OFN_FILEMUSTEXIST |
        OFN_PATHMUSTEXIST;

    ofn.lpstrTitle = "Select a Video File";

    if (GetOpenFileName(&ofn))
    {
        return std::string(filename);
    }

    return "";
}