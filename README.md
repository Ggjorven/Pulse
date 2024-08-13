# Pulse

**Pulse** is a versatile C++ utility library designed to streamline common tasks and enhance the efficiency of your development workflow. Built with C++20, it leverages the latest features and improvements of the language to provide a modern and powerful toolkit.

## Features

- **Utility Functions**: A collection of common utility functions to simplify your code.
- **Data Structures**: Efficient and easy-to-use data structures.
- **Algorithms**: A set of optimized algorithms for various use cases.
- **Error Handling**: Robust error handling mechanisms.

## Getting Started

### Prerequisites

- C++20 compatible compiler

## Installation

### Windows

1. Clone the repository:
    ```sh
    git clone --recursive https://github.com/ggjorven/Pulse.git
    cd Pulse
    ```

2. Navigate to the scripts folder:
    ```sh
    cd scripts/windows
    ```

3. Choose what you want it build to:
    - Visual Studio 17 2022:
        ```sh
        ./gen-vs2022.bat
        ```
    - MinGW make files:
        ```sh
        ./gen-make.bat
        ```

### Linux

1. Clone the repository:
    ```sh
    git clone --recursive https://github.com/ggjorven/Pulse.git
    cd Pulse
    ```

2. Navigate to the scripts folder:
    ```sh
    cd scripts/linux
    ```

3. (Optional) If you haven't already installed the premake5 build system you can install it like this:
    ```sh
    chmod +x install-premake5.sh
    ./install-premake5.sh
    ```

4. Generate make files:
    ```sh
    chmod +x gen-make.sh
    ./gen-make.sh
    ```

## Building

### Windows
- Visual Studio 17 2022:
    1. Navigate to the root of the directory
    2. Open the Pulse.sln file
    3. Start building in your desired configuration
    4. Build files can be in the bin/%Config%-windows/ folder.
    5. (Optional) Open a terminal and run the Sandbox project:

        ```sh
        ./Sandbox.exe
        ```

- MinGW Make:
    1. Navigate to the root of the directory
    2. Open a terminal.
    3. Call make with desired configuration (debug, release or dist):

        ```sh
        make config=release
        ```

    5. Build files can be in the bin/%Config%-linux/ folder.
    6. (Optional) Open a terminal and run the Sandbox project:
        ```sh
        ./Sandbox.exe
        ```

### Linux

1. Navigate to the root of the directory
2. Open a terminal
3. Call make with desired configuration (debug, release or dist):

    ```sh
    make config=release
    ```

5. Build files can be in the bin/%Config%-linux/ folder.
6. (Optional) Open a terminal and run the Sandbox project:

    ```sh
    chmod +x Sandbox
    ./Sandbox
    ```

## Contributing
Contributions are welcome! Please fork the repository and submit a pull request with your changes. Make sure to follow the project's coding standards and optionally include tests for new features.

## License
This project is licensed under the Apache 2.0 License - see the [LICENSE](LICENSE.txt) file for details.

## Third party libraries
All libraries stated below are used to create the **Pulse** library:

- [magic_enum](https://github.com/Neargye/magic_enum.git) Licenced under the MIT LICENSE
- [entt](https://github.com/skypjack/entt.git) Licenced under the MIT LICENSE
