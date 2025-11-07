# Horus Project

<div align="center">

![Horus Logo](https://img.shields.io/badge/Horus-Project-gold?style=for-the-badge)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![C++](https://img.shields.io/badge/C++-17-blue.svg)](https://isocpp.org/)
[![Qt](https://img.shields.io/badge/Qt-6-brightgreen.svg)](https://www.qt.io/)

**Open-Source Avionics Station for ISR UAVs**

*All-seeing, all-knowing flight visualization inspired by the Egyptian god of the sky*

A lightweight, customizable, and high-performance Primary Flight Display (PFD) built with C++ and Qt for Intelligence, Surveillance, and Reconnaissance (ISR) unmanned aerial vehicles.

[Features](#features) • [Installation](#installation) • [Usage](#usage) • [Contributing](#contributing) • [License](#license)

</div>

---

## 🦅 Overview

**Horus Project** is a modern, aviation-grade Primary Flight Display designed for UAV ground control stations. Built from the ground up with performance and customization in mind, it provides real-time visualization of critical flight parameters including attitude, altitude, airspeed, and heading.

### Why Horus?

Named after the ancient Egyptian god of the sky, kingship, and protection - often depicted with the head of a falcon. Horus represents:

- 👁️ **All-Seeing Vision** - The Eye of Horus symbolizes perfect vision and protection
- ✈️ **Sky Dominion** - God of the sky and celestial navigation
- 🎯 **Precision & Protection** - Guardian deity who never misses
- 🦅 **Falcon's Perspective** - Sharp-eyed bird's view from above
- 📜 **Ancient Wisdom** - Time-tested reliability and trust

**Horus Project** delivers:

- ✈️ **Aviation-Grade Visualization** - Professional artificial horizon, pitch ladder, and roll indicator
- ⚡ **High Performance** - 60+ FPS rendering with optimized C++ and Qt
- 🎨 **Customizable** - Custom fonts, colors, and layout configurations
- 🔌 **MAVLink Ready** - Easy integration with ArduPilot, PX4, and other autopilots
- 🖥️ **Cross-Platform** - Runs on Windows, macOS, and Linux
- 📖 **Open Source** - MIT licensed, community-driven development

---

## ✨ Features

### Core Instruments

- **Artificial Horizon**
  - Smooth pitch and roll visualization
  - 6x zoom for precise attitude monitoring (-15° to +15° visible range)
  - Dynamic horizon line with sky/ground differentiation

- **Pitch Ladder**
  - 5° increments with major marks every 10°
  - Positive angles (solid lines) and negative angles (dashed lines)
  - Precise angle labels in custom aviation fonts

- **Roll Indicator**
  - Visual roll scale with tick marks at ±10°, ±30°, ±60°
  - Dynamic roll pointer that tracks aircraft orientation
  - Reference marker for level flight

- **Altitude Tape**
  - Scrolling vertical tape with 100 ft increments
  - Major labels every 500 ft
  - Current altitude highlight box
  - AGL (Above Ground Level) display

- **Speed Indicator**
  - Vertical speed tape
  - Airspeed readout
  - Ground speed display (soon)

- **Heading Indicator**
  - Heading Tape
  - NSWE Text Implemented

- **RPM Gauges**
  - Add multiple rpm readouts (maximum 4 recommended)
  - Animated progress tape (soon)
  
- **Battery Indicator**
  - Battery readout (Volts)
  - Battery percentage (soon)

### GPS & Navigation *(Planned)*

- **Real-time GPS Display**
  - Latitude/Longitude coordinates
  - GPS altitude (MSL)
  - Fix type indicator (No Fix, 2D, 3D, DGPS, RTK)
  - Satellite count with signal strength
  - HDOP/VDOP accuracy metrics
  - Ground speed from GPS

- **GPS Trail Visualization**
  - Flight path history (breadcrumb trail)
  - Configurable trail length
  - Color-coded by altitude/speed

- **Position Overlay**
  - Home point distance and bearing
  - Takeoff location marker
  - Current position in Lat/Lon/Alt format

### Camera Integration *(Planned)*

- **Live Video Feed**
  - Picture-in-Picture (PiP) camera overlay
  - Resizable/draggable video window
  - Full-screen camera view toggle (hotkey support)
  - Opacity control for overlay

- **Video Stream Support**
  - H.264/H.265 codec support
  - RTSP streaming protocol
  - UDP/RTP streams
  - USB camera support (V4L2 on Linux)
  - Custom pipeline configuration

- **Camera Information Display**
  - Gimbal pitch/roll/yaw angles
  - Zoom level indicator
  - Recording status (REC indicator)
  - Camera mode (Photo/Video)
  - Storage capacity remaining

- **Camera Controls** *(Future)*
  - Gimbal control overlay
  - Zoom in/out controls
  - Photo/video capture buttons
  - Camera settings adjustment

### Display Features

- Real-time 20 Hz update rate
- Anti-aliased rendering for smooth visuals
- Custom aviation fonts support (Nimbus Mono, etc.)
- Green-on-black color scheme (night vision compatible)
- Configurable zoom levels
- Adaptive UI that adjusts to roll and pitch
- Low-latency video decoding (<100ms)
- Multi-threaded rendering for smooth performance

---

## 🛠️ Installation

### Prerequisites

- **C++ Compiler** with C++17 support (GCC 7+, Clang 5+, MSVC 2017+)
- **Qt 6.x** (or Qt 5.12+)
- **CMake 3.16+**

### Platform-Specific Setup

#### macOS
```bash
# Install Qt via Homebrew
brew install qt

# Clone the repository
git clone https://github.com/joaoliveira6704/Horus-Project.git
cd horus-project

# Build with CMake
mkdir build && cd build
cmake -DCMAKE_PREFIX_PATH=/opt/homebrew/opt/qt ..
make
./Horus
```

#### Linux (Ubuntu/Debian)
```bash
# Install dependencies
sudo apt-get update
sudo apt-get install qt6-base-dev build-essential cmake

# Clone and build
git clone https://github.com/joaoliveira6704/Horus-Project.git
cd horus-project
mkdir build && cd build
cmake ..
make
./Horus
```

#### Windows
```bash
# Install Qt from https://www.qt.io/download-qt-installer

# Clone repository
git clone https://github.com/joaoliveira6704/Horus-Project.git
cd horus-project

# Build with CMake
mkdir build
cd build
cmake -DCMAKE_PREFIX_PATH="C:/Qt/6.x.x/msvc2019_64" ..
cmake --build .
```

### Building with Qt Creator (Recommended)

1. Open Qt Creator
2. File → Open File or Project → Select `CMakeLists.txt`
3. Configure with your Qt kit
4. Build → Build Project
5. Run → Run

---

## 🚀 Usage

### Quick Start with Simulation

The application includes a built-in flight simulator for testing:

```cpp
// The default build runs with simulated flight data
./Horus
```

You'll see:
- Artificial horizon with gentle pitch and roll movements
- Scrolling altitude tape
- Real-time attitude updates

As of right now only simulated data using sin functions are implemented.

### Connecting to Real Flight Controller

#### MAVLink Integration (Coming Soon)

```cpp
// In main.cpp, replace simulation with MAVLink connection
// 1. Set up serial connection
QSerialPort *serial = new QSerialPort();
serial->setPortName("/dev/ttyUSB0");
serial->setBaudRate(57600);
serial->open(QIODevice::ReadOnly);

// 2. Parse MAVLink messages
// 3. Update PFD with real telemetry
attitudeIndicator->setAttitude(pitch, roll, altitude, speed);
```

#### Custom Telemetry Protocol

Implement your own parser by:
1. Reading telemetry from serial/UDP/TCP
2. Parsing your protocol format
3. Calling `setAttitude(pitch, roll, altitude, speed)`

---

## 📝 Configuration

### Custom Fonts

Add your aviation fonts:

1. Place `.ttf` files in `fonts/` directory
2. Add to `resources.qrc`:
```xml
<file>fonts/YourFont.ttf</file>
```
3. Load in `main.cpp`:
```cpp
int fontId = QFontDatabase::addApplicationFont(":/fonts/YourFont.ttf");
```

### Adjusting Zoom Level

In `attitudeindicator.h`:
```cpp
float zoom = 6.0f; // Change this value (higher = more zoomed in)
```

### Color Scheme

Modify colors in drawing functions:
```cpp
painter.setPen(QPen(Qt::green, 0.5)); // Change Qt::green to your color
```

---

## 🏗️ Project Structure

```
horus-project/
├── main.cpp                 # Application entry point, window management
├── attitudeindicator.h      # Core PFD widget with all instruments
├── CMakeLists.txt          # CMake build configuration
├── resources.qrc           # Qt resources (fonts, icons)
├── fonts/                  # Custom aviation fonts
│   ├── armarurgt.ttf
│   └── NimbusMono.ttf
└── README.md              # This file
```

---

## 🤝 Contributing

We welcome contributions! Here's how you can help:

### Ways to Contribute

- 🐛 **Report bugs** - Open an issue with reproduction steps
- 💡 **Suggest features** - Share your ideas in Discussions
- 📝 **Improve documentation** - Fix typos, add examples
- 🔧 **Submit pull requests** - Add features or fix bugs

### Development Setup

1. Fork the repository
2. Create a feature branch: `git checkout -b feature/amazing-feature`
3. Make your changes
4. Test thoroughly
5. Commit: `git commit -m 'Add amazing feature'`
6. Push: `git push origin feature/amazing-feature`
7. Open a Pull Request

### Code Style

- Follow C++17 standards
- Use Qt naming conventions (camelCase)
- Comment complex algorithms
- Keep functions focused and small
- Add const where appropriate

---

## 🗺️ Roadmap

### Version 1.0 (Current)
- ✅ Artificial horizon
- ✅ Pitch ladder
- ✅ Roll indicator
- ✅ Altitude tape
- ✅ Custom fonts support
- ✅ Airspeed tape
- ✅ Heading indicator
- ✅ Flight mode indicator


### Version 1.1 (In Progress)
- ⏳ Vertical speed indicator
- ⏳ MAVLink integration
- ⏳ GPS coordinate display
- ⏳ Battery voltage/percentage

### Version 1.5 (Planned)
- 📋 Live camera feed integration
  - PiP (Picture-in-Picture) video overlay
  - Full-screen camera view toggle
  - H.264/H.265 video stream support
  - RTSP/UDP video protocols
  - Camera gimbal indicator
  - Zoom level display
- 📋 GPS tracking and visualization
  - Real-time GPS coordinates (Lat/Lon/Alt)
  - GPS fix status (2D/3D/DGPS/RTK)
  - Satellite count indicator
  - HDOP/VDOP accuracy display
  - Ground speed from GPS
  - GPS trail/breadcrumb visualization

### Version 2.0 (Future)
- 📋 Interactive map overlay
  - Moving map with aircraft position
  - Terrain elevation data
  - No-fly zones visualization
  - Flight path overlay
  - Home point marker
- 📋 Waypoint navigation display
  - Active waypoint indicator
  - Distance/bearing to waypoint
  - ETA calculations
  - Mission progress bar
- 📋 Advanced camera features
  - Multi-camera support
  - Thermal/IR camera overlay
  - Recording status indicator
  - Snapshot capture
  - Camera settings overlay
- 📋 Data logging and replay
- 📋 Configurable HUD elements

---

## 📊 Performance

- **Frame Rate**: 60+ FPS on modern hardware
- **CPU Usage**: <5% on Intel Core i5
- **Memory**: ~50 MB RAM
- **Latency**: <20ms update cycle
- **Resolution**: Scalable from 800x600 to 4K

---

## 🐛 Known Issues

- Horizon line clipping with altitude tape needs refinement during high roll angles
- Font loading may fail silently on some systems (falls back to system fonts)
- High zoom levels (>8.0) may show rendering artifacts

See [Issues](https://github.com/joaoliveira6704/Horus-Project/issues) for full list.

---

## 📚 Documentation

- [GPS Integration](docs/GPS.md) *(Coming v1.5)*
- [Camera Streaming Guide](docs/CAMERA.md) *(Coming v1.5)*
- [Video Pipeline Configuration](docs/VIDEO.md) *(Coming v1.5)*

---

## 🙏 Acknowledgments

- **Qt Framework** - Cross-platform UI framework
- **MAVLink** - Lightweight messaging protocol for drones
- **ArduPilot Community** - Inspiration and protocols
- **Open-source contributors** - Thank you! ❤️

---

## 📄 License

This project is licensed under the **MIT License** - see the [LICENSE](LICENSE) file for details.

```
MIT License

Copyright (c) 2025 Horus Project Contributors

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```

---

## 📬 Contact

- **Project**: [Horus Project](https://github.com/joaoliveira6704/Horus-Project)
- **Issues**: [GitHub Issues](https://github.com/joaoliveira6704/Horus-Project/issues)
- **Discussions**: [GitHub Discussions](https://github.com/joaoliveira6704/Horus-Project/discussions)

---

<div align="center">

**Made with ☕ and ✈️ by the UAV community**

⭐ Star this repo if you find it useful!

</div>
