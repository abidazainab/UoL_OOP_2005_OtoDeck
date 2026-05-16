# UoL_OOP_2005_OtoDeckDJ

A two-deck DJ application built in **C++ with JUCE**, featuring dual audio players, a crossfader, EQ processing, a searchable playlist with CSV persistence, waveform displays, and custom visual components.

![Project Preview](homepage.jpg)
---

## Features

### Playback
- Load audio files via **file browser button** or **drag and drop**
- Play, pause, stop on two independent decks
- Volume and speed sliders per deck
- **Crossfader** to blend between the two players
- Loop and cue point controls

### Effects
- **3-band EQ** (bass, mid, treble) via `EQProcessor`
- Per-deck audio routed through `MixerAudioSource`

### Playlist
- Track library with duration display and waveform preview per row
- **Duplicate detection** — warns before re-adding a track
- **Favourites filter** — mark and filter tracks
- Search/filter by All, Favourites, or Custom
- **CSV persistence** via `CSVTrack` — playlist survives app restarts

### Visualisations
- `WaveFormDisplay` — scrolling waveform with playhead
- `JogWheel` — animated scrub wheel (`AnimatedAppComponent`)
- `CircularMeter` — radial level indicator
- `LevelMeter` + `LevelMeterBulb` — segmented vertical meter

### UI
- Custom theme via `CustomLookAndFeel` (extends `LookAndFeel_V4`)
- Custom font rendering ("Otodeck DJ" branding)
- Fully responsive layout via `resized()` callbacks

---

## Architecture

| Class | Role |
|---|---|
| `MainComponent` | App root, owns both players and mixer |
| `DJAudioPlayer` | Audio transport, EQ, gain, speed |
| `DeckGUI` | Per-deck UI (load, play, waveform, knobs) |
| `MiddleDeck` | Crossfader and shared transport controls |
| `PlaylistComponent` | Track table, search, drag-to-deck |
| `PlaylistFilterManager` | All / Favourites / Custom filter logic |
| `EQProcessor` | Bass / mid / treble IIR filter chain |
| `WaveFormDisplay` | Audio thumbnail waveform renderer |
| `JogWheel` | Animated jog wheel component |
| `CircularMeter` | Circular VU meter |
| `LevelMeter` / `LevelMeterBulb` | Segmented level meter |
| `CSVTrack` | CSV serialisation for playlist persistence |
| `CustomLookAndFeel` | Custom colours, fonts, and control styles |

---

## Setup

**Requirements:** JUCE 7+, C++17, CMake or Projucer

```bash
git clone <repo-url>
# Open OtoDecks.jucer in Projucer, export to your IDE, then build
# Or with CMake:
cmake -B build && cmake --build build
```

---

## License

Academic use only · University of London OOP-2005
