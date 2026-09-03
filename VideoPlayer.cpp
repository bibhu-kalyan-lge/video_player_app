#include "VideoPlayer.h"
#include "Utils.h"

#include <gst/gst.h>
#include <gst/video/video.h>
#include <gst/audio/audio.h>

#include <iostream>

bool VideoPlayer::initialize(
    const std::string& filepath)
{
    gchar* uri =
        gst_filename_to_uri(
            filepath.c_str(),
            nullptr);

    if (!uri)
    {
        return false;
    }

    std::string pipeline_desc =
        "playbin uri=" + std::string(uri);

    pipeline =
        gst_parse_launch(
            pipeline_desc.c_str(),
            nullptr);

    g_free(uri);

    return pipeline != nullptr;
}

bool VideoPlayer::initializeUrl(
    const std::string& url)
{
    std::string pipeline_desc =
        "playbin uri=\"" + url + "\"";

    pipeline =
        gst_parse_launch(
            pipeline_desc.c_str(),
            nullptr);

    return pipeline != nullptr;
}

void VideoPlayer::onVideoPadAdded(
    GstElement* src,
    GstPad* newPad,
    gpointer data)
{
    GstElement* videoConvert =
        GST_ELEMENT(data);

    GstPad* sinkPad =
        gst_element_get_static_pad(
            videoConvert,
            "sink");

    if (gst_pad_is_linked(sinkPad))
    {
        gst_object_unref(sinkPad);
        return;
    }

    GstCaps* caps =
        gst_pad_get_current_caps(newPad);

    if (!caps)
    {
        caps =
            gst_pad_query_caps(
                newPad,
                nullptr);
    }

    const GstStructure* str =
        gst_caps_get_structure(
            caps,
            0);

    const gchar* name =
        gst_structure_get_name(str);

    if (g_str_has_prefix(name, "video/"))
    {
        GstPadLinkReturn ret =
            gst_pad_link(
                newPad,
                sinkPad);

        if (ret == GST_PAD_LINK_OK)
        {
            std::cout
                << "[VIDEO] Linked decodebin -> videoconvert"
                << std::endl;
        }
    }

    gst_caps_unref(caps);
    gst_object_unref(sinkPad);
}


void VideoPlayer::onAudioPadAdded(
    GstElement* src,
    GstPad* newPad,
    gpointer data)
{
    GstElement* audioConvert =
        GST_ELEMENT(data);

    GstPad* sinkPad =
        gst_element_get_static_pad(
            audioConvert,
            "sink");

    if (gst_pad_is_linked(sinkPad))
    {
        gst_object_unref(sinkPad);
        return;
    }

    GstCaps* caps =
        gst_pad_get_current_caps(newPad);

    if (!caps)
    {
        caps =
            gst_pad_query_caps(
                newPad,
                nullptr);
    }

    const GstStructure* str =
        gst_caps_get_structure(
            caps,
            0);

    const gchar* name =
        gst_structure_get_name(str);

    if (g_str_has_prefix(name, "audio/"))
    {
        GstPadLinkReturn ret =
            gst_pad_link(
                newPad,
                sinkPad);

        if (ret == GST_PAD_LINK_OK)
        {
            std::cout
                << "[AUDIO] Linked decodebin -> audioconvert"
                << std::endl;
        }
    }

    gst_caps_unref(caps);
    gst_object_unref(sinkPad);
}



bool VideoPlayer::initializeYoutube(
    const YoutubeStreams& streams)
{
    if (streams.videoUrl.empty() ||
        streams.audioUrl.empty())
    {
        return false;
    }

    pipeline =
        gst_pipeline_new(
            "youtube-pipeline");

    if (!pipeline)
    {
        return false;
    }

    GstElement* videoSrc =
        gst_element_factory_make(
            "souphttpsrc",
            "video-source");

    GstElement* videoDecode =
        gst_element_factory_make(
            "decodebin",
            "video-decode");

    GstElement* videoConvert =
        gst_element_factory_make(
            "videoconvert",
            "video-convert");

    GstElement* videoSink =
        gst_element_factory_make(
            "autovideosink",
            "video-sink");

    GstElement* audioSrc =
        gst_element_factory_make(
            "souphttpsrc",
            "audio-source");

    GstElement* audioDecode =
        gst_element_factory_make(
            "decodebin",
            "audio-decode");

    GstElement* audioConvert =
        gst_element_factory_make(
            "audioconvert",
            "audio-convert");

    GstElement* audioSink =
        gst_element_factory_make(
            "autoaudiosink",
            "audio-sink");

    if (!videoSrc ||
        !videoDecode ||
        !videoConvert ||
        !videoSink ||
        !audioSrc ||
        !audioDecode ||
        !audioConvert ||
        !audioSink)
    {
        return false;
    }

    g_object_set(
        videoSrc,
        "location",
        streams.videoUrl.c_str(),
        nullptr);

    g_object_set(
        audioSrc,
        "location",
        streams.audioUrl.c_str(),
        nullptr);

    gst_bin_add_many(
        GST_BIN(pipeline),

        videoSrc,
        videoDecode,
        videoConvert,
        videoSink,

        audioSrc,
        audioDecode,
        audioConvert,
        audioSink,

        nullptr);

    if (!gst_element_link(
            videoSrc,
            videoDecode))
    {
        std::cout
            << "Failed video source link"
            << std::endl;
    }

    if (!gst_element_link(
            audioSrc,
            audioDecode))
    {
        std::cout
            << "Failed audio source link"
            << std::endl;
    }

    if (!gst_element_link(
            videoConvert,
            videoSink))
    {
        std::cout
            << "Failed video sink link"
            << std::endl;
    }

    if (!gst_element_link(
            audioConvert,
            audioSink))
    {
        std::cout
            << "Failed audio sink link"
            << std::endl;
    }

    g_signal_connect(
        videoDecode,
        "pad-added",
        G_CALLBACK(
            VideoPlayer::onVideoPadAdded),
        videoConvert);

    g_signal_connect(
        audioDecode,
        "pad-added",
        G_CALLBACK(
            VideoPlayer::onAudioPadAdded),
        audioConvert);

    std::cout
        << "[YOUTUBE] Video Source Added"
        << std::endl;

    std::cout
        << "[YOUTUBE] Audio Source Added"
        << std::endl;

    return true;
}

void VideoPlayer::play()
{
    std::cout
        << "===================================\n"
        << "      GStreamer Video Player\n"
        << "===================================\n\n";

    gst_element_set_state(
        pipeline,
        GST_STATE_PLAYING);

    GstBus* bus =
        gst_element_get_bus(
            pipeline);

    bool running = true;

    gint64 last_second = -1;

    while (running)
    {
        gint64 position;
        gint64 duration;

        if (gst_element_query_position(
                pipeline,
                GST_FORMAT_TIME,
                &position) &&
            gst_element_query_duration(
                pipeline,
                GST_FORMAT_TIME,
                &duration))
        {
            gint64 current_second =
                position / GST_SECOND;

            if (current_second != last_second)
            {
                last_second =
                    current_second;

                GstState current;
                GstState pending;

                gst_element_get_state(
                    pipeline,
                    &current,
                    &pending,
                    0);

                std::cout
                    << "[PLAYBACK] "
                    << formatTime(position)
                    << " / "
                    << formatTime(duration)
                    << " | STATE="
                    << stateToString(current)
                    << std::endl;
            }
        }

        GstMessage* msg =
            gst_bus_timed_pop_filtered(
                bus,
                100 * GST_MSECOND,
                static_cast<GstMessageType>(
                    GST_MESSAGE_ERROR |
                    GST_MESSAGE_EOS |
                    GST_MESSAGE_STATE_CHANGED));

        if (!msg)
        {
            continue;
        }

        switch (GST_MESSAGE_TYPE(msg))
        {
            case GST_MESSAGE_STATE_CHANGED:
            {
                if (GST_MESSAGE_SRC(msg) ==
                    GST_OBJECT(pipeline))
                {
                    GstState old_state;
                    GstState new_state;
                    GstState pending;

                    gst_message_parse_state_changed(
                        msg,
                        &old_state,
                        &new_state,
                        &pending);

                    std::cout
                        << "[STATE] "
                        << stateToString(old_state)
                        << " -> "
                        << stateToString(new_state)
                        << std::endl;
                }

                break;
            }

            case GST_MESSAGE_EOS:
            {
                std::cout
                    << "\n===================================\n"
                    << "END OF STREAM\n"
                    << "Playback completed successfully.\n"
                    << "===================================\n";

                running = false;
                break;
            }

            case GST_MESSAGE_ERROR:
            {
                GError* err;
                gchar* debug;

                gst_message_parse_error(
                    msg,
                    &err,
                    &debug);

                std::cout
                    << "\n===================================\n"
                    << "ERROR DETECTED\n"
                    << "===================================\n";

                std::cout
                    << "Reason: "
                    << err->message
                    << std::endl;

                if (debug)
                {
                    std::cout
                        << "\nDebug Info:\n"
                        << debug
                        << std::endl;
                }

                std::cout
                    << "===================================\n";

                g_error_free(err);
                g_free(debug);

                running = false;
                break;
            }

            default:
                break;
        }

        gst_message_unref(msg);
    }

    gst_object_unref(bus);

    gst_element_set_state(
        pipeline,
        GST_STATE_NULL);

    gst_object_unref(pipeline);

    pipeline = nullptr;
}