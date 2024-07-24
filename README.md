# Pulse

**Pulse** is a versatile C++ utility library designed to streamline common tasks and enhance the efficiency of your development workflow. Built with C++23, it leverages the latest features and improvements of the language to provide a modern and powerful toolkit.

## Features

- **Utility Functions**: A collection of common utility functions to simplify your code.
- **Data Structures**: Efficient and easy-to-use data structures.
- **Algorithms**: A set of optimized algorithms for various use cases.
- **Math Utilities**: Handy mathematical functions and constants.
- **File I/O**: Simplified file input/output operations.
- **Error Handling**: Robust error handling mechanisms.

## Installation

### Windows

1. Clone the repository:
    ```sh
    git clone --recursive https://github.com/ggjorven/Pulse.git
    cd Pulse
    ```

2. Navigate to the scripts folder:
    ```sh
    cd scripts
    ```

3. Choose what you want it build to:
    - Visual Studio 17 2022:
        ```sh
        ./gen-vs2022-windows.bat
        ```
    - MinGW make files: // TODO
        ```sh
        ./gen-make-windows.bat
        ```

### Linux

1. Clone the repository:
    ```sh
    git clone --recursive https://github.com/ggjorven/Pulse.git
    cd Pulse
    ```

2. Navigate to the scripts folder:
    ```sh
    cd scripts
    ```

3. Generate make files:
    ```sh
    chmod +x gen-make-linux.sh
    ./gen-make-linux.sh
    ```

## Usage

### Windows
- Visual Studio 17 2022:
    1. Navigate to the root of the directory
    2. Open the Pulse.sln file
    3. Start building in your desired configuration
    4. Build files can be in the bin/%Config#-windows/ folder.
    5. (Optional) Open a terminal and run the Sandbox project:
    ```sh
    ./Sandbox.exe
    ``` 
- MinGW Make:
    // TODO

### Linux
1. Navigate to the root of the directory
2. Open a terminal
3. Call make with desired configuration (debug or release):
    ```sh
    make config=release
    ```
4. Build files can be in the bin/%Config#-linux/ folder.
5. (Optional) Open a terminal and run the Sandbox project:
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