# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.9

# Default target executed when no arguments are given to make.
default_target: all

.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = "C:/Program Files/CMake/bin/cmake.exe"

# The command to remove a file.
RM = "C:/Program Files/CMake/bin/cmake.exe" -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = D:/Github/ImageProcess-CV

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = D:/Github/ImageProcess-CV

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake cache editor..."
	"C:/Program Files/CMake/bin/cmake-gui.exe" -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache

.PHONY : edit_cache/fast

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	"C:/Program Files/CMake/bin/cmake.exe" -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache

.PHONY : rebuild_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start D:/Github/ImageProcess-CV/CMakeFiles D:/Github/ImageProcess-CV/CMakeFiles/progress.marks
	$(MAKE) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start D:/Github/ImageProcess-CV/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean

.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named image

# Build rule for target.
image: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 image
.PHONY : image

# fast build rule for target.
image/fast:
	$(MAKE) -f CMakeFiles/image.dir/build.make CMakeFiles/image.dir/build
.PHONY : image/fast

test/BMPCodec.obj: test/BMPCodec.cpp.obj

.PHONY : test/BMPCodec.obj

# target to build an object file
test/BMPCodec.cpp.obj:
	$(MAKE) -f CMakeFiles/image.dir/build.make CMakeFiles/image.dir/test/BMPCodec.cpp.obj
.PHONY : test/BMPCodec.cpp.obj

test/BMPCodec.i: test/BMPCodec.cpp.i

.PHONY : test/BMPCodec.i

# target to preprocess a source file
test/BMPCodec.cpp.i:
	$(MAKE) -f CMakeFiles/image.dir/build.make CMakeFiles/image.dir/test/BMPCodec.cpp.i
.PHONY : test/BMPCodec.cpp.i

test/BMPCodec.s: test/BMPCodec.cpp.s

.PHONY : test/BMPCodec.s

# target to generate assembly for a file
test/BMPCodec.cpp.s:
	$(MAKE) -f CMakeFiles/image.dir/build.make CMakeFiles/image.dir/test/BMPCodec.cpp.s
.PHONY : test/BMPCodec.cpp.s

test/TestBase.obj: test/TestBase.cpp.obj

.PHONY : test/TestBase.obj

# target to build an object file
test/TestBase.cpp.obj:
	$(MAKE) -f CMakeFiles/image.dir/build.make CMakeFiles/image.dir/test/TestBase.cpp.obj
.PHONY : test/TestBase.cpp.obj

test/TestBase.i: test/TestBase.cpp.i

.PHONY : test/TestBase.i

# target to preprocess a source file
test/TestBase.cpp.i:
	$(MAKE) -f CMakeFiles/image.dir/build.make CMakeFiles/image.dir/test/TestBase.cpp.i
.PHONY : test/TestBase.cpp.i

test/TestBase.s: test/TestBase.cpp.s

.PHONY : test/TestBase.s

# target to generate assembly for a file
test/TestBase.cpp.s:
	$(MAKE) -f CMakeFiles/image.dir/build.make CMakeFiles/image.dir/test/TestBase.cpp.s
.PHONY : test/TestBase.cpp.s

test/TestMain.obj: test/TestMain.cpp.obj

.PHONY : test/TestMain.obj

# target to build an object file
test/TestMain.cpp.obj:
	$(MAKE) -f CMakeFiles/image.dir/build.make CMakeFiles/image.dir/test/TestMain.cpp.obj
.PHONY : test/TestMain.cpp.obj

test/TestMain.i: test/TestMain.cpp.i

.PHONY : test/TestMain.i

# target to preprocess a source file
test/TestMain.cpp.i:
	$(MAKE) -f CMakeFiles/image.dir/build.make CMakeFiles/image.dir/test/TestMain.cpp.i
.PHONY : test/TestMain.cpp.i

test/TestMain.s: test/TestMain.cpp.s

.PHONY : test/TestMain.s

# target to generate assembly for a file
test/TestMain.cpp.s:
	$(MAKE) -f CMakeFiles/image.dir/build.make CMakeFiles/image.dir/test/TestMain.cpp.s
.PHONY : test/TestMain.cpp.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... image"
	@echo "... edit_cache"
	@echo "... rebuild_cache"
	@echo "... test/BMPCodec.obj"
	@echo "... test/BMPCodec.i"
	@echo "... test/BMPCodec.s"
	@echo "... test/TestBase.obj"
	@echo "... test/TestBase.i"
	@echo "... test/TestBase.s"
	@echo "... test/TestMain.obj"
	@echo "... test/TestMain.i"
	@echo "... test/TestMain.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system
