.PHONY: all build run configure clean

# Default target: builds and then runs the executable
all: build run

# Optional: Run initial CMake configuration if the build folder doesn't exist yet
configure:
	cmake -B build

# Build the project using parallel jobs
build:
	cmake --build build --parallel

# Execute the compiled binary
run:
	.\build\lab6.exe

# Optional: Clean build artifacts
clean:
	cmake --build build --target clean
