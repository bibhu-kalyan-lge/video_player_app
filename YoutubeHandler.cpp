#include "YoutubeHandler.h"

#include <array>
#include <cstdio>
#include <sstream>
#include <string>

YoutubeStreams getYoutubeStreams(
    const std::string& youtubeUrl)
{
    YoutubeStreams streams;

    std::string command =
        "yt-dlp -g -f \"bestvideo+bestaudio\" \"" +
        youtubeUrl +
        "\"";

    FILE* pipe =
        _popen(command.c_str(), "r");

    if (!pipe)
    {
        return streams;
    }

    std::array<char, 4096> buffer;
    std::string result;

    while (fgets(
               buffer.data(),
               static_cast<int>(buffer.size()),
               pipe))
    {
        result += buffer.data();
    }

    _pclose(pipe);

    std::stringstream ss(result);

    std::getline(
        ss,
        streams.videoUrl);

    std::string secondLine;

    std::getline(
        ss,
        secondLine);

    streams.audioUrl = secondLine;

    return streams;
}