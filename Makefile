# vars to be used
CXX = g++		# compiler
CXXFLAGS = -Iinclude -Wall -g	# extra info when running/errors
TARGET = new_test	# name of run (added in .gitignore)
SOURCES = src/main.cpp src/file_conv.cpp src/phys_sim.cpp src/PID.cpp include/file_conv.hpp include/phys_sim.hpp include/PID.hpp	# file name

# build rule
$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES)

# clean rule (safe version)
clean:
	rm -f $(TARGET)
