# Faith Saturator

A simple audio saturation plugin built with **C++20**, **JUCE 8.0.6**, and **CMake** as a personal DSP learning project.

## Overview

Faith Saturator is a stereo audio effect that applies `tanh`-based soft clipping to an input signal. It was developed to explore nonlinear audio processing, plugin architecture, and DAW integration. It is an **early-stage educational prototype**, not a commercial-ready plugin or a novel saturation algorithm.

## Features

- **Drive:** increases input level before the waveshaper (0–10; internal input multiplier 1–11x).
- **Mix:** blends the original dry signal and saturated wet signal (0–100%).
- **Output:** post-effect gain adjustment (-24 dB to +24 dB).
- Stereo signal processing, graphical controls, and host session parameter recall.
- Build targets: **VST3** and **Audio Unit (AU)** for **macOS Apple Silicon (arm64)**.

## How the audio is processed

Each sample is processed in this order:

1. Input gain: `driven = input * (1 + drive)`
2. Soft clipping: `wet = tanh(driven)`
3. Dry/wet mix: `mixed = input * (1 - mix) + wet * mix`
4. Output gain: `output = mixed * gainFromDecibels(outputDb)`

The current code uses a standard waveshaping curve. Even at Drive = 0, 100% wet applies `tanh` to the signal. Set Mix = 0% for a fully dry signal.

## Build (macOS)

Requirements: Xcode command-line tools, CMake 3.24+, and an internet connection for CMake to fetch JUCE 8.0.6.

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

CMake is configured to build AU and VST3 formats for arm64 and copy built plugins to local plugin folders after a successful build. This setup has not been independently rebuilt or tested in the preparation of this repository README.

## Test status

The author previously compiled the plugin and tested it in **REAPER** on macOS. The original local project archive included VST3 and AU build artifacts dated March 2026. No automated tests or cross-platform compatibility claims are made.

## Current limitations

- No oversampling; nonlinear processing may introduce aliasing.
- No explicit parameter smoothing; rapid parameter changes can produce artifacts.
- Output gain can exceed 0 dBFS; manage levels in the host.
- No advanced tone shaping or independent distortion modes.
- Designed and configured around a stereo bus and Apple Silicon macOS.

## Project structure

- `Source/PluginProcessor.cpp` — core DSP processing and parameter state
- `Source/PluginEditor.cpp` — plugin interface and parameter controls
- `CMakeLists.txt` — JUCE/CMake configuration

## About

Created as a first step into audio plugin development and real-time DSP. The focus is learning and documenting a functional signal-processing prototype.
