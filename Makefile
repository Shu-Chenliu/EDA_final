# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -O2 -Wall

# Target executable
TARGET = main.out

# Automatically find all .cpp and .h files in current directory
SRCS := $(wildcard *.cpp)
HEADERS := $(wildcard *.h)
OBJS := $(SRCS:.cpp=.o)

# Default target
all: $(TARGET)

# Link object files to create final binary
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile each .cpp to .o (auto-depend on header files)
%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build artifacts
clean:
	rm -f $(OBJS) $(TARGET)


# # Compiler
# CXX = g++
# CXXFLAGS = -std=c++17 -Wall -Wextra -g -Iinc

# # Source and Object files
# SRCDIR = src
# OBJDIR = obj
# BINDIR = bin
# TARGET = $(BINDIR)/test

# SOURCES = $(wildcard $(SRCDIR)/*.cpp)
# OBJECTS = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SOURCES))

# # Default target
# all: $(TARGET)

# # Link all object files into final executable
# $(TARGET): $(OBJECTS)
# 	@mkdir -p $(BINDIR)
# 	$(CXX) $(CXXFLAGS) $^ -o $@

# # Compile source files to object files
# $(OBJDIR)/%.o: $(SRCDIR)/%.cpp
# 	@mkdir -p $(OBJDIR)
# 	$(CXX) $(CXXFLAGS) -c $< -o $@

# # Clean build artifacts
# clean:
# 	rm -rf $(OBJDIR) $(BINDIR)

# # Rebuild everything
# rebuild: clean all