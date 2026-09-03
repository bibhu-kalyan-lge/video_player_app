#pragma once

#include <string>

struct YoutubeStreams
{
    std::string videoUrl;
    std::string audioUrl;
};

YoutubeStreams getYoutubeStreams(
    const std::string& youtubeUrl);