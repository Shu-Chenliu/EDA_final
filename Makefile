# Compiler
CXX = g++
CXXFLAGS = -std=c++11 -O2 -Wall

# Target executable
TARGET = main.out

# Source and object files
SRCS = kmean.cpp MBFFgeneration.cpp classes.cpp main.cpp
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