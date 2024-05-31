# Sql2CppHeader

A C++ application that uses Boost.Spirit X3 to parse SQL files and extract table definitions, then generates corresponding C++ header files.

## Features

- Parses SQL files to extract table definitions.
- Generates C++ header files based on parsed table definitions.

## Prerequisites

- CMake 3.28 or higher
- Boost 1.84 (tested with this version only)

## Installation

1. **Clone the repository:**

    ```sh
    git clone https://github.com/cyberium/Sql2CppHeader.git
    cd Sql2CppHeader
    ```

2. **Set up Boost:**

    - Ensure the `BOOST_ROOT` environment variable points to your Boost 1.84 installation directory.

## Building the Project

1. **Run CMake and build:**

    ```sh
    cmake -H. -Bbuild
    cmake --build build --config Release
    ```

## Usage

Run the application with the input SQL file and the desired output header file:

```sh
./build/src/parser/Release/Sql2CppHeader -i <input_sql_file> -o <output_header_file>
```

Example:

```sh
./build/src/parser/Release/Sql2CppHeader input.sql output.h
```

### Command-Line Options

- `-i, --input <file>`: Specify the input SQL file.
- `-o, --output <file>`: Specify the output header file.
- `-v, --verbose`: Enable verbose output.
- `-h, --help`: Show help message.

## Running Tests

**Command to use:**

  ```sh
    ctest -C Release --output-on-failure --test-dir build
  ```

## Libraries Used

- **Boost.Test**: Used for unit testing the application.
- **Boost.Spirit X3**: Used for parsing SQL files.

## License

This project is licensed under the MIT License.