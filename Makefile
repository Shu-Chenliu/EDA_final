# Compiler
CXX = g++
CXXFLAGS = -std=c++11 -O2 -Wall

# Target executable
TARGET = kmean.out

# Source and object files
SRCS = kmean.cpp MBFFgeneration.cpp classes.cpp
OBJS = $(SRCS:.cpp=.o)

# Default target
all: $(TARGET)

# Link object files to create final binary
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Compile .cpp to .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build artifacts
clean:
	rm -f $(OBJS) $(TARGET)
