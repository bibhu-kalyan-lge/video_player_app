#include "Utils.h"

#include <iomanip>
#include <sstream>

std::string formatTime(gint64 nanoseconds)
{
    gint64 total_seconds =
        nanoseconds / GST_SECOND;

    int hours =
        static_cast<int>(total_seconds / 3600);

    int minutes =
        static_cast<int>((total_seconds % 3600) / 60);

    int seconds =
        static_cast<int>(total_seconds % 60);

    std::ostringstream oss;

    if (hours > 0)
    {
        oss << std::setw(2)
            << std::setfill('0')
            << hours
            << ":";
    }

    oss << std::setw(2)
        << std::setfill('0')
        << minutes
        << ":";

    oss << std::setw(2)
        << std::setfill('0')
        << seconds;

    return oss.str();
}

const char* stateToString(GstState state)
{
    switch (state)
    {
        case GST_STATE_NULL:
            return "NULL";

        case GST_STATE_READY:
            return "READY";

        case GST_STATE_PAUSED:
            return "PAUSED";

        case GST_STATE_PLAYING:
            return "PLAYING";

        default:
            return "UNKNOWN";
    }
}