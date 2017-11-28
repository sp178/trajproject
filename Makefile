# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/sp178/project/trajproject

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/sp178/project/trajproject

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target install
install: preinstall
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Install the project..."
	/usr/bin/cmake -P cmake_install.cmake
.PHONY : install

# Special rule for the target install
install/fast: preinstall/fast
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Install the project..."
	/usr/bin/cmake -P cmake_install.cmake
.PHONY : install/fast

# Special rule for the target list_install_components
list_install_components:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Available install components are: \"Unspecified\""
.PHONY : list_install_components

# Special rule for the target list_install_components
list_install_components/fast: list_install_components

.PHONY : list_install_components/fast

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake cache editor..."
	/usr/bin/cmake-gui -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache

.PHONY : edit_cache/fast

# Special rule for the target install/local
install/local: preinstall
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Installing only the local directory..."
	/usr/bin/cmake -DCMAKE_INSTALL_LOCAL_ONLY=1 -P cmake_install.cmake
.PHONY : install/local

# Special rule for the target install/local
install/local/fast: install/local

.PHONY : install/local/fast

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache

.PHONY : rebuild_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /home/sp178/project/trajproject/CMakeFiles /home/sp178/project/trajproject/CMakeFiles/progress.marks
	$(MAKE) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /home/sp178/project/trajproject/CMakeFiles 0
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
# Target rules for targets named main

# Build rule for target.
main: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 main
.PHONY : main

# fast build rule for target.
main/fast:
	$(MAKE) -f CMakeFiles/main.dir/build.make CMakeFiles/main.dir/build
.PHONY : main/fast

# Manual pre-install relink rule for target.
main/preinstall:
	$(MAKE) -f CMakeFiles/main.dir/build.make CMakeFiles/main.dir/preinstall
.PHONY : main/preinstall

plantform/main.o: plantform/main.cpp.o

.PHONY : plantform/main.o

# target to build an object file
plantform/main.cpp.o:
	$(MAKE) -f CMakeFiles/main.dir/build.make CMakeFiles/main.dir/plantform/main.cpp.o
.PHONY : plantform/main.cpp.o

plantform/main.i: plantform/main.cpp.i

.PHONY : plantform/main.i

# target to preprocess a source file
plantform/main.cpp.i:
	$(MAKE) -f CMakeFiles/main.dir/build.make CMakeFiles/main.dir/plantform/main.cpp.i
.PHONY : plantform/main.cpp.i

plantform/main.s: plantform/main.cpp.s

.PHONY : plantform/main.s

# target to generate assembly for a file
plantform/main.cpp.s:
	$(MAKE) -f CMakeFiles/main.dir/build.make CMakeFiles/main.dir/plantform/main.cpp.s
.PHONY : plantform/main.cpp.s

plantform/modelbase.o: plantform/modelbase.cpp.o

.PHONY : plantform/modelbase.o

# target to build an object file
plantform/modelbase.cpp.o:
	$(MAKE) -f CMakeFiles/main.dir/build.make CMakeFiles/main.dir/plantform/modelbase.cpp.o
.PHONY : plantform/modelbase.cpp.o

plantform/modelbase.i: plantform/modelbase.cpp.i

.PHONY : plantform/modelbase.i

# target to preprocess a source file
plantform/modelbase.cpp.i:
	$(MAKE) -f CMakeFiles/main.dir/build.make CMakeFiles/main.dir/plantform/modelbase.cpp.i
.PHONY : plantform/modelbase.cpp.i

plantform/modelbase.s: plantform/modelbase.cpp.s

.PHONY : plantform/modelbase.s

# target to generate assembly for a file
plantform/modelbase.cpp.s:
	$(MAKE) -f CMakeFiles/main.dir/build.make CMakeFiles/main.dir/plantform/modelbase.cpp.s
.PHONY : plantform/modelbase.cpp.s

plantform/odeengin.o: plantform/odeengin.cpp.o

.PHONY : plantform/odeengin.o

# target to build an object file
plantform/odeengin.cpp.o:
	$(MAKE) -f CMakeFiles/main.dir/build.make CMakeFiles/main.dir/plantform/odeengin.cpp.o
.PHONY : plantform/odeengin.cpp.o

plantform/odeengin.i: plantform/odeengin.cpp.i

.PHONY : plantform/odeengin.i

# target to preprocess a source file
plantform/odeengin.cpp.i:
	$(MAKE) -f CMakeFiles/main.dir/build.make CMakeFiles/main.dir/plantform/odeengin.cpp.i
.PHONY : plantform/odeengin.cpp.i

plantform/odeengin.s: plantform/odeengin.cpp.s

.PHONY : plantform/odeengin.s

# target to generate assembly for a file
plantform/odeengin.cpp.s:
	$(MAKE) -f CMakeFiles/main.dir/build.make CMakeFiles/main.dir/plantform/odeengin.cpp.s
.PHONY : plantform/odeengin.cpp.s

plantform/recorder.o: plantform/recorder.cpp.o

.PHONY : plantform/recorder.o

# target to build an object file
plantform/recorder.cpp.o:
	$(MAKE) -f CMakeFiles/main.dir/build.make CMakeFiles/main.dir/plantform/recorder.cpp.o
.PHONY : plantform/recorder.cpp.o

plantform/recorder.i: plantform/recorder.cpp.i

.PHONY : plantform/recorder.i

# target to preprocess a source file
plantform/recorder.cpp.i:
	$(MAKE) -f CMakeFiles/main.dir/build.make CMakeFiles/main.dir/plantform/recorder.cpp.i
.PHONY : plantform/recorder.cpp.i

plantform/recorder.s: plantform/recorder.cpp.s

.PHONY : plantform/recorder.s

# target to generate assembly for a file
plantform/recorder.cpp.s:
	$(MAKE) -f CMakeFiles/main.dir/build.make CMakeFiles/main.dir/plantform/recorder.cpp.s
.PHONY : plantform/recorder.cpp.s

plantform/rk1.o: plantform/rk1.c.o

.PHONY : plantform/rk1.o

# target to build an object file
plantform/rk1.c.o:
	$(MAKE) -f CMakeFiles/main.dir/build.make CMakeFiles/main.dir/plantform/rk1.c.o
.PHONY : plantform/rk1.c.o

plantform/rk1.i: plantform/rk1.c.i

.PHONY : plantform/rk1.i

# target to preprocess a source file
plantform/rk1.c.i:
	$(MAKE) -f CMakeFiles/main.dir/build.make CMakeFiles/main.dir/plantform/rk1.c.i
.PHONY : plantform/rk1.c.i

plantform/rk1.s: plantform/rk1.c.s

.PHONY : plantform/rk1.s

# target to generate assembly for a file
plantform/rk1.c.s:
	$(MAKE) -f CMakeFiles/main.dir/build.make CMakeFiles/main.dir/plantform/rk1.c.s
.PHONY : plantform/rk1.c.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... install"
	@echo "... list_install_components"
	@echo "... main"
	@echo "... edit_cache"
	@echo "... install/local"
	@echo "... rebuild_cache"
	@echo "... plantform/main.o"
	@echo "... plantform/main.i"
	@echo "... plantform/main.s"
	@echo "... plantform/modelbase.o"
	@echo "... plantform/modelbase.i"
	@echo "... plantform/modelbase.s"
	@echo "... plantform/odeengin.o"
	@echo "... plantform/odeengin.i"
	@echo "... plantform/odeengin.s"
	@echo "... plantform/recorder.o"
	@echo "... plantform/recorder.i"
	@echo "... plantform/recorder.s"
	@echo "... plantform/rk1.o"
	@echo "... plantform/rk1.i"
	@echo "... plantform/rk1.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

