#include "MediaInfo.h"

#include <cstdlib>
#include <iostream>

void printMediaInfo(
    const std::string& filepath)
{
    std::cout
        << "===================================\n"
        << "MEDIA INFORMATION\n"
        << "===================================\n";

    std::string cmd =
        "gst-discoverer-1.0 \"" +
        filepath +
        "\"";

    system(cmd.c_str());

    std::cout
        << "===================================\n\n";
}