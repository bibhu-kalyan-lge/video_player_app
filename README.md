# GStreamer Video Player

A C++ multimedia application developed using the GStreamer framework that supports local media playback, media analysis, YouTube stream extraction, and YouTube audio/video playback using custom GStreamer pipelines.

---

# Features

## Local Media Playback

- Open video files using Windows File Explorer.
- Play local multimedia content using GStreamer.
- Display media metadata.
- Monitor playback progress.
- Track pipeline state transitions.
- Handle End-of-Stream (EOS) events.
- Error reporting and diagnostics.

## YouTube Playback

- Accept YouTube URLs.
- Extract media streams using yt-dlp.
- Extract independent video and audio stream URLs.
- Play YouTube content using custom GStreamer pipelines.
- Decode audio and video independently.
- Support synchronized audio and video playback.
- Handle adaptive streaming formats.

---

# Technologies Used

## Programming Language

- C++

## Multimedia Framework

- GStreamer 1.28.x

## Compiler

- MSVC x64

## Streaming Utility

- yt-dlp

---

# Project Structure

```text
video_player_app/
│
├── main.cpp
│
├── VideoPlayer.cpp
├── VideoPlayer.h
│
├── YoutubeHandler.cpp
├── YoutubeHandler.h
│
├── MediaInfo.cpp
├── MediaInfo.h
│
├── FilePicker.cpp
├── FilePicker.h
│
├── Utils.cpp
├── Utils.h
│
└── build.bat
```

---

# Component Responsibilities

## main.cpp

Responsibilities:

- Application entry point.
- User interaction.
- Playback mode selection.
- Coordination between application modules.

Menu:

```text
1. Open Local Video
2. Play YouTube Video
```

---

## VideoPlayer

Files:

```text
VideoPlayer.cpp
VideoPlayer.h
```

Responsibilities:

- Media playback.
- GStreamer pipeline creation.
- Runtime monitoring.
- State tracking.
- Error handling.
- Audio/video synchronization.

Implemented Functions:

```cpp
bool initialize(
    const std::string& filepath);

bool initializeUrl(
    const std::string& url);

bool initializeYoutube(
    const YoutubeStreams& streams);

void play();
```

Dynamic Pad Callbacks:

```cpp
void onVideoPadAdded(
    GstElement* src,
    GstPad* newPad,
    gpointer data);

void onAudioPadAdded(
    GstElement* src,
    GstPad* newPad,
    gpointer data);
```

---

## YoutubeHandler

Files:

```text
YoutubeHandler.cpp
YoutubeHandler.h
```

Responsibilities:

- YouTube URL processing.
- yt-dlp integration.
- Stream extraction.
- Media URL management.

Implemented Structure:

```cpp
struct YoutubeStreams
{
    std::string videoUrl;
    std::string audioUrl;
};
```

Implemented Function:

```cpp
YoutubeStreams getYoutubeStreams(
    const std::string& youtubeUrl);
```

Internal Command:

```bash
yt-dlp -g -f "bestvideo+bestaudio"
```

---

## MediaInfo

Files:

```text
MediaInfo.cpp
MediaInfo.h
```

Responsibilities:

- Media metadata extraction.
- Media analysis.
- Information display.

Information Reported:

```text
Container Format
Video Codec
Audio Codec
Resolution
Frame Rate
Duration
```

---

## FilePicker

Files:

```text
FilePicker.cpp
FilePicker.h
```

Responsibilities:

- Windows File Explorer integration.
- Local media file selection.

---

## Utils

Files:

```text
Utils.cpp
Utils.h
```

Responsibilities:

- Helper utilities.
- Time formatting.
- State conversion.

Implemented Functions:

```cpp
formatTime(...)
stateToString(...)
```

---

# Local Playback Architecture

```text
Local Video File
        ↓
FilePicker
        ↓
VideoPlayer
        ↓
GStreamer playbin
        ↓
Playback
```

---

# YouTube Playback Architecture

## Stream Extraction Workflow

```text
YouTube URL
        ↓
yt-dlp
        ↓
YoutubeStreams
        ↓
videoUrl
audioUrl
```

## Playback Workflow

```text
YouTube URL
        ↓
YoutubeHandler
        ↓
YoutubeStreams
        ↓
VideoPlayer
        ↓
Custom GStreamer Pipeline
        ↓
Playback
```

---

# Current YouTube Pipeline

## Video Branch

```text
souphttpsrc(videoUrl)
        ↓
decodebin
        ↓
videoconvert
        ↓
autovideosink
```

## Audio Branch

```text
souphttpsrc(audioUrl)
        ↓
decodebin
        ↓
audioconvert
        ↓
autoaudiosink
```

Complete Pipeline:

```text
                    GstPipeline
                         │
      ┌──────────────────┴──────────────────┐
      │                                     │
Video Branch                         Audio Branch
      │                                     │
souphttpsrc(videoUrl)          souphttpsrc(audioUrl)
      │                                     │
decodebin                         decodebin
      │                                     │
videoconvert                     audioconvert
      │                                     │
autovideosink                    autoaudiosink
```

---

# Dynamic Pad Handling

The pipeline uses decodebin to automatically detect media formats and connect them dynamically.

Video Callback:

```cpp
void onVideoPadAdded(...)
```

Audio Callback:

```cpp
void onAudioPadAdded(...)
```

Workflow:

```text
decodebin
      ↓
Dynamic Pad Created
      ↓
Pad Type Detection
      ↓
Video Branch / Audio Branch
      ↓
Pipeline Linking
```

---

# Playback Monitoring

Application monitors:

```text
Pipeline States
Playback Duration
Current Position
EOS Events
Errors
```

Observed States:

```text
GST_STATE_NULL
GST_STATE_READY
GST_STATE_PAUSED
GST_STATE_PLAYING
```

Example Output:

```text
[STATE] NULL -> READY
[STATE] READY -> PAUSED
[STATE] PAUSED -> PLAYING

[PLAYBACK] 00:10 / 05:25
```

---

# Error Handling

Implemented handling for:

```text
Pipeline Errors
Network Errors
Decode Errors
Playback Errors
End-of-Stream Events
```

Example:

```cpp
GST_MESSAGE_ERROR
GST_MESSAGE_EOS
GST_MESSAGE_STATE_CHANGED
```

---

# GStreamer Components Used

Core Components:

```text
playbin
decodebin
GstPipeline
GstBus
GstMessage
```

Streaming Components:

```text
souphttpsrc
decodebin
queue
videoconvert
audioconvert
autovideosink
autoaudiosink
```

---

# GStreamer Concepts Learned

```text
✓ GstElement
✓ GstPipeline
✓ GstBus
✓ GstMessage
✓ GstState
✓ playbin
✓ decodebin
✓ Dynamic Pads
✓ Pad Linking
✓ Network Streaming
✓ Adaptive Streaming
✓ YouTube Stream Extraction
✓ Audio / Video Separation
✓ Multi-Source Pipelines
✓ Audio/Video Synchronization
✓ Runtime Pipeline Monitoring
✓ Error Handling
✓ EOS Handling
```

---

# Build Instructions

Open:

```text
Visual Studio Developer Command Prompt (x64)
```

Build:

```cmd
build.bat
```

Run:

```cmd
main.exe
```

---

# Application Status

```text
Local Video Playback                     ✅
File Picker Integration                  ✅
Media Metadata Analysis                  ✅
Playback Monitoring                      ✅
State Tracking                           ✅
EOS Handling                             ✅
Error Handling                           ✅

YouTube URL Support                      ✅
yt-dlp Integration                       ✅
YouTube Stream Extraction                ✅

Video Stream Extraction                  ✅
Audio Stream Extraction                  ✅

YouTube Video Playback                   ✅
YouTube Audio Playback                   ✅

Dynamic Pad Handling                     ✅
Multi-Source Pipeline                    ✅
Independent Media Decoding               ✅
Synchronized Audio/Video Playback        ✅
```

---

# Future Enhancements

```text
□ queue-based buffering optimization
□ queue2 support
□ FPS monitoring using fpsdisplaysink
□ Playback controls (Pause / Resume)
□ Seeking support
□ Volume control
□ Subtitle support
□ Playlist support
□ DASH pipeline exploration
□ HLS pipeline exploration
□ Pipeline visualization using DOT graphs
□ Qt-based GUI
□ Custom video rendering window
□ Performance optimization
```

---

# Learning Outcome

This project provided practical experience with:

- GStreamer application development.
- Multimedia playback architecture.
- Network streaming workflows.
- YouTube adaptive streaming.
- Media stream extraction using yt-dlp.
- Dynamic pad handling.
- Multi-source pipeline design.
- Audio/video synchronization.
- Runtime pipeline monitoring.
- Modern multimedia framework development in C++.