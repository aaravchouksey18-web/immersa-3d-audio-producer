# 3D Audio Producer

A desktop audio workstation that turns **2D mono audio samples** into **3D
spatial audio**, placed by you in a visual 3D soundstage. Position a listener
and one or more sound producers in space, then listen through stereo
headphones or a surround-sound system while OpenAL Soft does the binaural /
multichannel panning.

> **Lineage:** a fork of the *binaural-audio-editor* project. The original
> wxWidgets + OpenSceneGraph UI stack has been replaced with **Dear ImGui** +
> **raylib**; audio decoding moved from libsndfile to **dr_wav / dr_flac**
> (see `CHANGELOG.md` for the full history).
>
> Note: project files from 3D Audio Producer v3.0.0 or earlier will not load
> in the current version.

## Features

- Place and move **sound producers** and the **listener** in 3D (camera
  zoom / pan / rotate, keyboard movement)
- **Timeline editor** with playback markers (start / pause / resume / end)
  and per-timeline playback of a single selected object
- **Sound bank** for assigning audio files to producers — the app keeps its
  own copy of the audio data as signed 32-bit samples to avoid corrupting
  your source files
- **Effect zones** — Echo, Standard Reverb, and EAX Reverb, creatable and
  editable in GUI dialogs
- **HRTF** change & test dialogs
- **External orientation device** — control the listener's orientation with
  an Adafruit BNO055 IMU over serial (`docs/external-orientation-imu.md`)
- **Project management** — projects save and load audio + timeline data from
  a folder the app extracts automatically, so sharing a project is as simple
  as zipping that folder
- Experimental **5.1 / 6.1 / 7.1 surround output** via OpenAL Soft's config

## UI themes

The UI ships with the built-in default raygui look. Additional themes
(candy, lavanda, cyber, ...) are **not bundled** — install them from the
[raygui style gallery](https://raylibtech.itch.io/rguistyler) into
`data/styles/<name>/` if you want the alternate looks.

## Required libraries

- [OpenAL Soft](https://github.com/kcat/openal-soft)
- [raylib](https://github.com/raysan5/raylib) **4.2**
- Boost (Math Quaternion headers + ASIO serial) —
  [Boost 1.70](https://www.boost.org/users/history/version_1_70_0.html)
- [PugiXML](https://github.com/zeux/pugixml/)
- A C++17 compiler

Dear ImGui, raylib/raygui, dr_wav and dr_flac are bundled under `src/`.

## Building

```sh
git clone <this-repo> 3d-audio-producer
cd 3d-audio-producer
mkdir build && cd build
cmake ..
make
./3d-audio-producer
```

The repo intentionally does **not** vendor raylib / OpenAL Soft / Boost /
PugiXML or their build trees — install them first (above), then `cmake ..`
will pick them up. UI style assets (`data/styles`) are also not shipped;
create projects with the built-in default look or drop themes in from the
raygui style gallery.

## Controls

### Camera

| input | action |
|---|---|
| Mouse wheel | zoom in / out |
| Mouse wheel pressed | pan |
| Alt + mouse wheel pressed | rotate |
| Alt + Ctrl + mouse wheel pressed | smooth zoom |
| Z | zoom to origin (0, 0, 0) |

### Listener (blue cube)

| keys | action |
|---|---|
| W A S D | move up / left / down / right |
| Q E | move forward / back |

### Sound producer (red cube)

Select a producer, then move it with **I J K L U O** (same axes as WASD/QE).

### Editor keys

| key | action |
|---|---|
| 1 | edit whatever is picked in 3D |
| Delete | remove the picked object / selected timeline point |
| Ctrl+S / Ctrl+O | save / load project |
| B | add a position point to the timeline |
| Z / X / C / V | add start / pause / resume / end playback markers |
| P | show timeline parameters |
| ← / → | timeline frame decrement / increment |
| Enter | select timeline frame |

### Timeline playback

1. Open the timeline and select the object (sound producer / listener).
2. Pick a timeline frame and press **Z** to mark the playback start.
3. Load a sound into that producer via the sound bank.
4. Press **Play**; audio starts when the timeline passes the marker.

## Coordinate system

Like OpenAL, the app uses a **right-handed** system: X points right, Y up,
Z toward the viewer. Up is +Y, back is +Z. A *frontal default view* shows
the soundstage from the listener's orientation.

## Multi-channel audio input

**Stereo (2-channel) audio does not get 3D spatialized** — it plays as
background music. Convert tracks to **mono** (Audacity or `sox`/ffmpeg) so
they can be placed and spatialized in the soundstage.

## Surround sound (experimental)

OpenAL Soft can remap the 3D audio to a 5.1 / 6.1 / 7.1 speaker setup — set
the output in `alsoft-config` (ships with OpenAL Soft) and see the
[OpenAL Soft 3D7.1 notes](https://github.com/kcat/openal-soft/blob/master/docs/3D7.1.txt).

## IMU orientation control

Listener orientation can be driven by a physical Adafruit BNO055 IMU —
see [`docs/external-orientation-imu.md`](docs/external-orientation-imu.md).

## Libraries & licenses

| library | license | where |
|---|---|---|
| Dear ImGui | MIT | `src/backends/imgui/` |
| raylib / raygui | zlib | `src/raygui/`, `src/backends/` |
| dr_wav / dr_flac | licence-free (public domain + MIT option) | `src/backends/` |
| OpenAL Soft | LGPL 2.1 | external dependency |
| PugiXML | MIT | external dependency |
| Boost | Boost Software License | external dependency |

The project itself is under the **BSD 3-Clause** license — see `LICENSE`
(© 2019, Pablo Antonio Camacho Jr.).