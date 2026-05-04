In this project, I developed a C++ systems application that simulates the core logic of a professional DJ controller. The system manages DJ sessions and audio tracks (MP3/WAV) through a modular architecture, including components for track parsing, playlist handling, and session control. To improve efficiency, the software implements an LRU-based caching mechanism for track reuse, while emphasizing memory-safe design using RAII principles. The project focuses on clean separation of responsibilities, maintainable system structure, and efficient resource management.

## How to Run

Build the project:
make

Run in interactive mode:
./bin/dj_manager -I

Run all playlists (Play All mode):
./bin/dj_manager -I -A > output.txt

## Requirements

Make sure the following folder exists in the project root:

input_2/
  dj_config.txt

This file contains the configuration and playlists used by the system.
