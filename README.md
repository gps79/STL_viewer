# STL File Viewer for Windows

This is a simple **STL file viewer** application built for **Windows** using the **Win32 API** and the **FreeGLUT library**. The application allows you to load and view STL files (commonly used in 3D printing) interactively. It provides basic visualization of 3D models, allowing users to rotate and inspect STL files.

## Screenshot

Here is a screenshot of the STL file viewer application in action:

![STL Viewer](image1.png)

## Features
- Load STL files and view them in 3D.
- Rotate and zoom the model for better inspection.
- Uses freeglut for OpenGL rendering.

## Prerequisites
Before running the application, make sure that the following libraries are installed:

- **Win32 API** (pre-installed with Visual Studio for Windows development)
- **freeglut library** (for OpenGL rendering)

## Installation

1. **Clone the repository:**

    ```bash
    git clone https://github.com/gps79/STL_viewer.git
    ```

2. **Get the build environment:**

    Download and install MinGW with GCC-6.3.0-1
    Download [freeGlut-MinGW-3.0.0-1](https://www.transmissionzero.co.uk/files/software/development/GLUT/) library

3. **Build the application:**

    Open the project in **Code::Blocks** IDE
   or in the MinGW console type
   ```bash
   make all
   ```

4. **Run the application:**

    Once the project is built, you can run the executable `stl_viewer.exe <file>.stl` to view the STL file.

## Documentation

  Developer's documentation can be found [here](doc/html/index.html).

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

