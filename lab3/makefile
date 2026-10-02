CXX = g++
CXXFLAGS = -std=c++20
LDFLAGS = -lgdi32 -lcomctl32
TARGET = main.exe

# Locate all source and corresponding object files
SRCS = main.cpp shapes/dot.cpp shapes/elipse.cpp shapes/line.cpp shapes/rect.cpp create.cpp
OBJS = $(SRCS:.cpp=.o)

# Default target links object files into executable
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Rule to compile individual .cpp files into .o object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Run target
run: $(TARGET)
	.\$(TARGET)

# Clean up build artifacts
clean:
	cmd /c "del /S *.o $(TARGET)"