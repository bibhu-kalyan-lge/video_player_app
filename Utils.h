#pragma once

#include <gst/gst.h>
#include <string>

std::string formatTime(gint64 nanoseconds);

const char* stateToString(GstState state);