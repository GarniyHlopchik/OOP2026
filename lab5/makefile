CXX = g++
CXXFLAGS = -std=c++20 -DUNICODE -D_UNICODE
LDFLAGS = -lgdi32 -lcomctl32 -lcomdlg32
TARGET = main.exe

# Locate all source and corresponding object files
SRCS = main.cpp shapes/dot.cpp shapes/elipse.cpp shapes/line.cpp shapes/rect.cpp shapes/circled_line.cpp shapes/cube.cpp create.cpp windows/window.cpp windows/paint_window.cpp windows/dialogue_window.cpp windows/table.cpp
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