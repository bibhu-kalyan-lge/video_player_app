#pragma once

#include <gst/gst.h>
#include <string>

#include "YoutubeHandler.h"

class VideoPlayer
{
public:
    bool initialize(
        const std::string& filepath);

    bool initializeUrl(
        const std::string& url);

    bool initializeYoutube(
        const YoutubeStreams& streams);

    void play();

private:
    GstElement* pipeline = nullptr;

    static void onVideoPadAdded(
        GstElement* src,
        GstPad* newPad,
        gpointer data);

    static void onAudioPadAdded(
        GstElement* src,
        GstPad* newPad,
        gpointer data);
};