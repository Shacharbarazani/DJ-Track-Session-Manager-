# DJ Track Session Manager

A C++ systems project developed as part of an academic course, designed to simulate the core logic of a professional DJ controller.  
The project emphasizes clean system design, modular architecture, and efficient resource management.

## Project Description
The system manages DJ sessions and audio tracks (MP3/WAV) through well-defined components responsible for track parsing, session control, caching, and playlist handling.  
The implementation focuses on maintainability, memory safety, and clear separation of responsibilities between modules.

## Key Features
- Modular system architecture with clearly separated components
- Audio track abstraction supporting MP3 and WAV formats
- DJ session and playlist management
- LRU-based cache for efficient track reuse
- Memory-safe design using RAII principles
- Makefile-based build process

## Project Structure
- `include/` — Public interfaces and headers  
- `src/` — Core implementation  
- `Makefile` — Build configuration

## Technologies & Concepts
- C++
- Object-Oriented Design
- RAII and memory management
- Cache-based data structures (LRU)
- Modular systems programming
- Build automation using Makefile

## Build Instructions
To build the project, run:
```bash
make
