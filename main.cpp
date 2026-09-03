#include <gst/gst.h>
#include <iostream>
#include <string>

#include "FilePicker.h"
#include "MediaInfo.h"
#include "VideoPlayer.h"
#include "YoutubeHandler.h"

int main(int argc, char* argv[])
{
    gst_init(&argc, &argv);

    std::cout
        << "=================================\n"
        << "GStreamer Video Player\n"
        << "=================================\n\n";

    std::cout
        << "1. Open Local Video\n"
        << "2. Play YouTube Video\n\n";

    int choice = 0;

    std::cout
        << "Select Option: ";

    std::cin >> choice;
    std::cin.ignore();

    VideoPlayer player;

    if (choice == 1)
    {
        std::string filepath =
            selectVideoFile();

        if (filepath.empty())
        {
            std::cout
                << "No file selected."
                << std::endl;

            return 0;
        }

        printMediaInfo(filepath);

        if (player.initialize(filepath))
        {
            player.play();
        }
        else
        {
            std::cout
                << "Failed to initialize player."
                << std::endl;
        }
    }
    else if (choice == 2)
    {
        std::string youtubeUrl;

        std::cout
            << "\nEnter YouTube URL:\n";

        std::getline(
            std::cin,
            youtubeUrl);

        std::cout
            << "\nExtracting stream URLs..."
            << std::endl;

        YoutubeStreams streams =
            getYoutubeStreams(
                youtubeUrl);

        if (streams.videoUrl.empty())
        {
            std::cout
                << "Failed to extract video stream."
                << std::endl;

            return -1;
        }

        std::cout
            << "\nSelected Stream URL:\n"
            << streams.videoUrl
            << "\n"
            << std::endl;

        if (player.initializeYoutube(streams))
        {
            player.play();
        }
        else
        {
            std::cout
                << "Failed to initialize YouTube stream."
                << std::endl;
        }
    }
    else
    {
        std::cout
            << "Invalid option selected."
            << std::endl;
    }

    return 0;
}