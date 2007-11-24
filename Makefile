# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.4

# Default target executed when no arguments are given to make.
default_target: all

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canoncical targets will work.
.SUFFIXES:

.SUFFIXES: .hpux_make_needs_suffix_list

# Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /usr/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/mongoose/Projects/freyja_0.9.5

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/mongoose/Projects/freyja_0.9.5

# Include the progress variables for this target.
include CMakeFiles/progress.make

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake cache editor..."
	/usr/bin/ccmake -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)

# Special rule for the target edit_cache
edit_cache/fast: edit_cache

# Special rule for the target install
install: preinstall
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Install the project..."
	/usr/bin/cmake -P cmake_install.cmake

# Special rule for the target install
install/fast: preinstall/fast
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Install the project..."
	/usr/bin/cmake -P cmake_install.cmake

# Special rule for the target install/local
install/local: preinstall
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Installing only the local directory..."
	/usr/bin/cmake -DCMAKE_INSTALL_LOCAL_ONLY=1 -P cmake_install.cmake

# Special rule for the target install/local
install/local/fast: install/local

# Special rule for the target list_install_components
list_install_components:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Only default component available"

# Special rule for the target list_install_components
list_install_components/fast: list_install_components

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /home/mongoose/Projects/freyja_0.9.5/CMakeFiles $(CMAKE_ALL_PROGRESS)
	$(MAKE) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /home/mongoose/Projects/freyja_0.9.5/CMakeFiles 0

# The main clean target
clean:
	$(MAKE) -f CMakeFiles/Makefile2 clean

# The main clean target
clean/fast: clean

# Prepare targets for installation.
preinstall: all
	$(MAKE) -f CMakeFiles/Makefile2 preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) -f CMakeFiles/Makefile2 preinstall

# clear depends
depend:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1

#=============================================================================
# Target rules for targets named luawrapper

# Build rule for target.
luawrapper: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 luawrapper

# fast build rule for target.
luawrapper/fast:
	$(MAKE) -f lua/CMakeFiles/luawrapper.dir/build.make lua/CMakeFiles/luawrapper.dir/build

#=============================================================================
# Target rules for targets named hel-0.2.2

# Build rule for target.
hel-0.2.2: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 hel-0.2.2

# fast build rule for target.
hel-0.2.2/fast:
	$(MAKE) -f libhel/CMakeFiles/hel-0.2.2.dir/build.make libhel/CMakeFiles/hel-0.2.2.dir/build

#=============================================================================
# Target rules for targets named mat44

# Build rule for target.
mat44: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 mat44

# fast build rule for target.
mat44/fast:
	$(MAKE) -f libhel/CMakeFiles/mat44.dir/build.make libhel/CMakeFiles/mat44.dir/build

#=============================================================================
# Target rules for targets named ray

# Build rule for target.
ray: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 ray

# fast build rule for target.
ray/fast:
	$(MAKE) -f libhel/CMakeFiles/ray.dir/build.make libhel/CMakeFiles/ray.dir/build

#=============================================================================
# Target rules for targets named freyja-0.9.5

# Build rule for target.
freyja-0.9.5: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 freyja-0.9.5

# fast build rule for target.
freyja-0.9.5/fast:
	$(MAKE) -f libfreyja/CMakeFiles/freyja-0.9.5.dir/build.make libfreyja/CMakeFiles/freyja-0.9.5.dir/build

#=============================================================================
# Target rules for targets named mgtk

# Build rule for target.
mgtk: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 mgtk

# fast build rule for target.
mgtk/fast:
	$(MAKE) -f libmgtk/CMakeFiles/mgtk.dir/build.make libmgtk/CMakeFiles/mgtk.dir/build

#=============================================================================
# Target rules for targets named mgtk-0.1.1

# Build rule for target.
mgtk-0.1.1: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 mgtk-0.1.1

# fast build rule for target.
mgtk-0.1.1/fast:
	$(MAKE) -f libmgtk/CMakeFiles/mgtk-0.1.1.dir/build.make libmgtk/CMakeFiles/mgtk-0.1.1.dir/build

#=============================================================================
# Target rules for targets named freyja

# Build rule for target.
freyja: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 freyja

# fast build rule for target.
freyja/fast:
	$(MAKE) -f freyja/CMakeFiles/freyja.dir/build.make freyja/CMakeFiles/freyja.dir/build

#=============================================================================
# Target rules for targets named image-plugin-bmp

# Build rule for target.
image-plugin-bmp: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 image-plugin-bmp

# fast build rule for target.
image-plugin-bmp/fast:
	$(MAKE) -f plugins/CMakeFiles/image-plugin-bmp.dir/build.make plugins/CMakeFiles/image-plugin-bmp.dir/build

#=============================================================================
# Target rules for targets named image-plugin-dds

# Build rule for target.
image-plugin-dds: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 image-plugin-dds

# fast build rule for target.
image-plugin-dds/fast:
	$(MAKE) -f plugins/CMakeFiles/image-plugin-dds.dir/build.make plugins/CMakeFiles/image-plugin-dds.dir/build

#=============================================================================
# Target rules for targets named image-plugin-jpg

# Build rule for target.
image-plugin-jpg: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 image-plugin-jpg

# fast build rule for target.
image-plugin-jpg/fast:
	$(MAKE) -f plugins/CMakeFiles/image-plugin-jpg.dir/build.make plugins/CMakeFiles/image-plugin-jpg.dir/build

#=============================================================================
# Target rules for targets named image-plugin-pcx

# Build rule for target.
image-plugin-pcx: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 image-plugin-pcx

# fast build rule for target.
image-plugin-pcx/fast:
	$(MAKE) -f plugins/CMakeFiles/image-plugin-pcx.dir/build.make plugins/CMakeFiles/image-plugin-pcx.dir/build

#=============================================================================
# Target rules for targets named image-plugin-png

# Build rule for target.
image-plugin-png: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 image-plugin-png

# fast build rule for target.
image-plugin-png/fast:
	$(MAKE) -f plugins/CMakeFiles/image-plugin-png.dir/build.make plugins/CMakeFiles/image-plugin-png.dir/build

#=============================================================================
# Target rules for targets named image-plugin-ppm

# Build rule for target.
image-plugin-ppm: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 image-plugin-ppm

# fast build rule for target.
image-plugin-ppm/fast:
	$(MAKE) -f plugins/CMakeFiles/image-plugin-ppm.dir/build.make plugins/CMakeFiles/image-plugin-ppm.dir/build

#=============================================================================
# Target rules for targets named image-plugin-tga

# Build rule for target.
image-plugin-tga: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 image-plugin-tga

# fast build rule for target.
image-plugin-tga/fast:
	$(MAKE) -f plugins/CMakeFiles/image-plugin-tga.dir/build.make plugins/CMakeFiles/image-plugin-tga.dir/build

#=============================================================================
# Target rules for targets named model-plugin-3ds

# Build rule for target.
model-plugin-3ds: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 model-plugin-3ds

# fast build rule for target.
model-plugin-3ds/fast:
	$(MAKE) -f plugins/CMakeFiles/model-plugin-3ds.dir/build.make plugins/CMakeFiles/model-plugin-3ds.dir/build

#=============================================================================
# Target rules for targets named model-plugin-ase

# Build rule for target.
model-plugin-ase: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 model-plugin-ase

# fast build rule for target.
model-plugin-ase/fast:
	$(MAKE) -f plugins/CMakeFiles/model-plugin-ase.dir/build.make plugins/CMakeFiles/model-plugin-ase.dir/build

#=============================================================================
# Target rules for targets named model-plugin-bsa

# Build rule for target.
model-plugin-bsa: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 model-plugin-bsa

# fast build rule for target.
model-plugin-bsa/fast:
	$(MAKE) -f plugins/CMakeFiles/model-plugin-bsa.dir/build.make plugins/CMakeFiles/model-plugin-bsa.dir/build

#=============================================================================
# Target rules for targets named model-plugin-bvh

# Build rule for target.
model-plugin-bvh: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 model-plugin-bvh

# fast build rule for target.
model-plugin-bvh/fast:
	$(MAKE) -f plugins/CMakeFiles/model-plugin-bvh.dir/build.make plugins/CMakeFiles/model-plugin-bvh.dir/build

#=============================================================================
# Target rules for targets named model-plugin-dof

# Build rule for target.
model-plugin-dof: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 model-plugin-dof

# fast build rule for target.
model-plugin-dof/fast:
	$(MAKE) -f plugins/CMakeFiles/model-plugin-dof.dir/build.make plugins/CMakeFiles/model-plugin-dof.dir/build

#=============================================================================
# Target rules for targets named model-plugin-egg9

# Build rule for target.
model-plugin-egg9: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 model-plugin-egg9

# fast build rule for target.
model-plugin-egg9/fast:
	$(MAKE) -f plugins/CMakeFiles/model-plugin-egg9.dir/build.make plugins/CMakeFiles/model-plugin-egg9.dir/build

#=============================================================================
# Target rules for targets named model-plugin-et

# Build rule for target.
model-plugin-et: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 model-plugin-et

# fast build rule for target.
model-plugin-et/fast:
	$(MAKE) -f plugins/CMakeFiles/model-plugin-et.dir/build.make plugins/CMakeFiles/model-plugin-et.dir/build

#=============================================================================
# Target rules for targets named model-plugin-glm

# Build rule for target.
model-plugin-glm: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 model-plugin-glm

# fast build rule for target.
model-plugin-glm/fast:
	$(MAKE) -f plugins/CMakeFiles/model-plugin-glm.dir/build.make plugins/CMakeFiles/model-plugin-glm.dir/build

#=============================================================================
# Target rules for targets named model-plugin-halflife

# Build rule for target.
model-plugin-halflife: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 model-plugin-halflife

# fast build rule for target.
model-plugin-halflife/fast:
	$(MAKE) -f plugins/CMakeFiles/model-plugin-halflife.dir/build.make plugins/CMakeFiles/model-plugin-halflife.dir/build

#=============================================================================
# Target rules for targets named model-plugin-llm

# Build rule for target.
model-plugin-llm: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 model-plugin-llm

# fast build rule for target.
model-plugin-llm/fast:
	$(MAKE) -f plugins/CMakeFiles/model-plugin-llm.dir/build.make plugins/CMakeFiles/model-plugin-llm.dir/build

#=============================================================================
# Target rules for targets named model-plugin-lwo

# Build rule for target.
model-plugin-lwo: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 model-plugin-lwo

# fast build rule for target.
model-plugin-lwo/fast:
	$(MAKE) -f plugins/CMakeFiles/model-plugin-lwo.dir/build.make plugins/CMakeFiles/model-plugin-lwo.dir/build

#=============================================================================
# Target rules for targets named model-plugin-md2

# Build rule for target.
model-plugin-md2: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 model-plugin-md2

# fast build rule for target.
model-plugin-md2/fast:
	$(MAKE) -f plugins/CMakeFiles/model-plugin-md2.dir/build.make plugins/CMakeFiles/model-plugin-md2.dir/build

#=============================================================================
# Target rules for targets named model-plugin-md3

# Build rule for target.
model-plugin-md3: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 model-plugin-md3

# fast build rule for target.
model-plugin-md3/fast:
	$(MAKE) -f plugins/CMakeFiles/model-plugin-md3.dir/build.make plugins/CMakeFiles/model-plugin-md3.dir/build

#=============================================================================
# Target rules for targets named model-plugin-md5

# Build rule for target.
model-plugin-md5: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 model-plugin-md5

# fast build rule for target.
model-plugin-md5/fast:
	$(MAKE) -f plugins/CMakeFiles/model-plugin-md5.dir/build.make plugins/CMakeFiles/model-plugin-md5.dir/build

#=============================================================================
# Target rules for targets named model-plugin-mdl

# Build rule for target.
model-plugin-mdl: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 model-plugin-mdl

# fast build rule for target.
model-plugin-mdl/fast:
	$(MAKE) -f plugins/CMakeFiles/model-plugin-mdl.dir/build.make plugins/CMakeFiles/model-plugin-mdl.dir/build

#=============================================================================
# Target rules for targets named model-plugin-milkshape

# Build rule for target.
model-plugin-milkshape: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 model-plugin-milkshape

# fast build rule for target.
model-plugin-milkshape/fast:
	$(MAKE) -f plugins/CMakeFiles/model-plugin-milkshape.dir/build.make plugins/CMakeFiles/model-plugin-milkshape.dir/build

#=============================================================================
# Target rules for targets named model-plugin-nad

# Build rule for target.
model-plugin-nad: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 model-plugin-nad

# fast build rule for target.
model-plugin-nad/fast:
	$(MAKE) -f plugins/CMakeFiles/model-plugin-nad.dir/build.make plugins/CMakeFiles/model-plugin-nad.dir/build

#=============================================================================
# Target rules for targets named model-plugin-nif4

# Build rule for target.
model-plugin-nif4: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 model-plugin-nif4

# fast build rule for target.
model-plugin-nif4/fast:
	$(MAKE) -f plugins/CMakeFiles/model-plugin-nif4.dir/build.make plugins/CMakeFiles/model-plugin-nif4.dir/build

#=============================================================================
# Target rules for targets named model-plugin-nif5

# Build rule for target.
model-plugin-nif5: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 model-plugin-nif5

# fast build rule for target.
model-plugin-nif5/fast:
	$(MAKE) -f plugins/CMakeFiles/model-plugin-nif5.dir/build.make plugins/CMakeFiles/model-plugin-nif5.dir/build

#=============================================================================
# Target rules for targets named model-plugin-nod

# Build rule for target.
model-plugin-nod: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 model-plugin-nod

# fast build rule for target.
model-plugin-nod/fast:
	$(MAKE) -f plugins/CMakeFiles/model-plugin-nod.dir/build.make plugins/CMakeFiles/model-plugin-nod.dir/build

#=============================================================================
# Target rules for targets named model-plugin-obj

# Build rule for target.
model-plugin-obj: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 model-plugin-obj

# fast build rule for target.
model-plugin-obj/fast:
	$(MAKE) -f plugins/CMakeFiles/model-plugin-obj.dir/build.make plugins/CMakeFiles/model-plugin-obj.dir/build

#=============================================================================
# Target rules for targets named model-plugin-psk

# Build rule for target.
model-plugin-psk: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 model-plugin-psk

# fast build rule for target.
model-plugin-psk/fast:
	$(MAKE) -f plugins/CMakeFiles/model-plugin-psk.dir/build.make plugins/CMakeFiles/model-plugin-psk.dir/build

#=============================================================================
# Target rules for targets named model-plugin-sims

# Build rule for target.
model-plugin-sims: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 model-plugin-sims

# fast build rule for target.
model-plugin-sims/fast:
	$(MAKE) -f plugins/CMakeFiles/model-plugin-sims.dir/build.make plugins/CMakeFiles/model-plugin-sims.dir/build

#=============================================================================
# Target rules for targets named model-plugin-skel

# Build rule for target.
model-plugin-skel: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 model-plugin-skel

# fast build rule for target.
model-plugin-skel/fast:
	$(MAKE) -f plugins/CMakeFiles/model-plugin-skel.dir/build.make plugins/CMakeFiles/model-plugin-skel.dir/build

#=============================================================================
# Target rules for targets named model-plugin-skn

# Build rule for target.
model-plugin-skn: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 model-plugin-skn

# fast build rule for target.
model-plugin-skn/fast:
	$(MAKE) -f plugins/CMakeFiles/model-plugin-skn.dir/build.make plugins/CMakeFiles/model-plugin-skn.dir/build

#=============================================================================
# Target rules for targets named model-plugin-smd

# Build rule for target.
model-plugin-smd: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 model-plugin-smd

# fast build rule for target.
model-plugin-smd/fast:
	$(MAKE) -f plugins/CMakeFiles/model-plugin-smd.dir/build.make plugins/CMakeFiles/model-plugin-smd.dir/build

#=============================================================================
# Target rules for targets named model-plugin-tombraider

# Build rule for target.
model-plugin-tombraider: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 model-plugin-tombraider

# fast build rule for target.
model-plugin-tombraider/fast:
	$(MAKE) -f plugins/CMakeFiles/model-plugin-tombraider.dir/build.make plugins/CMakeFiles/model-plugin-tombraider.dir/build

#=============================================================================
# Target rules for targets named model-plugin-trmesh

# Build rule for target.
model-plugin-trmesh: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 model-plugin-trmesh

# fast build rule for target.
model-plugin-trmesh/fast:
	$(MAKE) -f plugins/CMakeFiles/model-plugin-trmesh.dir/build.make plugins/CMakeFiles/model-plugin-trmesh.dir/build

#=============================================================================
# Target rules for targets named model-plugin-utpackage

# Build rule for target.
model-plugin-utpackage: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 model-plugin-utpackage

# fast build rule for target.
model-plugin-utpackage/fast:
	$(MAKE) -f plugins/CMakeFiles/model-plugin-utpackage.dir/build.make plugins/CMakeFiles/model-plugin-utpackage.dir/build

#=============================================================================
# Target rules for targets named ui-plugin-lineageIIextract

# Build rule for target.
ui-plugin-lineageIIextract: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 ui-plugin-lineageIIextract

# fast build rule for target.
ui-plugin-lineageIIextract/fast:
	$(MAKE) -f plugins/CMakeFiles/ui-plugin-lineageIIextract.dir/build.make plugins/CMakeFiles/ui-plugin-lineageIIextract.dir/build

#=============================================================================
# Target rules for targets named ui-plugin-octree

# Build rule for target.
ui-plugin-octree: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 ui-plugin-octree

# fast build rule for target.
ui-plugin-octree/fast:
	$(MAKE) -f plugins/CMakeFiles/ui-plugin-octree.dir/build.make plugins/CMakeFiles/ui-plugin-octree.dir/build

#=============================================================================
# Target rules for targets named ui-plugin-pakreader

# Build rule for target.
ui-plugin-pakreader: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 ui-plugin-pakreader

# fast build rule for target.
ui-plugin-pakreader/fast:
	$(MAKE) -f plugins/CMakeFiles/ui-plugin-pakreader.dir/build.make plugins/CMakeFiles/ui-plugin-pakreader.dir/build

#=============================================================================
# Target rules for targets named ui-plugin-perlinnoise

# Build rule for target.
ui-plugin-perlinnoise: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 ui-plugin-perlinnoise

# fast build rule for target.
ui-plugin-perlinnoise/fast:
	$(MAKE) -f plugins/CMakeFiles/ui-plugin-perlinnoise.dir/build.make plugins/CMakeFiles/ui-plugin-perlinnoise.dir/build

#=============================================================================
# Target rules for targets named ui-plugin-reverseengineer

# Build rule for target.
ui-plugin-reverseengineer: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 ui-plugin-reverseengineer

# fast build rule for target.
ui-plugin-reverseengineer/fast:
	$(MAKE) -f plugins/CMakeFiles/ui-plugin-reverseengineer.dir/build.make plugins/CMakeFiles/ui-plugin-reverseengineer.dir/build

#=============================================================================
# Target rules for targets named win32

# Build rule for target.
win32: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 win32

# fast build rule for target.
win32/fast:
	$(MAKE) -f plugins/CMakeFiles/win32.dir/build.make plugins/CMakeFiles/win32.dir/build

# Help Target
help::
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... edit_cache"
	@echo "... install"
	@echo "... install/local"
	@echo "... list_install_components"
	@echo "... rebuild_cache"
	@echo "... luawrapper"
	@echo "... hel-0.2.2"
	@echo "... mat44"
	@echo "... ray"
	@echo "... freyja-0.9.5"
	@echo "... mgtk"
	@echo "... mgtk-0.1.1"
	@echo "... freyja"
	@echo "... image-plugin-bmp"
	@echo "... image-plugin-dds"
	@echo "... image-plugin-jpg"
	@echo "... image-plugin-pcx"
	@echo "... image-plugin-png"
	@echo "... image-plugin-ppm"
	@echo "... image-plugin-tga"
	@echo "... model-plugin-3ds"
	@echo "... model-plugin-ase"
	@echo "... model-plugin-bsa"
	@echo "... model-plugin-bvh"
	@echo "... model-plugin-dof"
	@echo "... model-plugin-egg9"
	@echo "... model-plugin-et"
	@echo "... model-plugin-glm"
	@echo "... model-plugin-halflife"
	@echo "... model-plugin-llm"
	@echo "... model-plugin-lwo"
	@echo "... model-plugin-md2"
	@echo "... model-plugin-md3"
	@echo "... model-plugin-md5"
	@echo "... model-plugin-mdl"
	@echo "... model-plugin-milkshape"
	@echo "... model-plugin-nad"
	@echo "... model-plugin-nif4"
	@echo "... model-plugin-nif5"
	@echo "... model-plugin-nod"
	@echo "... model-plugin-obj"
	@echo "... model-plugin-psk"
	@echo "... model-plugin-sims"
	@echo "... model-plugin-skel"
	@echo "... model-plugin-skn"
	@echo "... model-plugin-smd"
	@echo "... model-plugin-tombraider"
	@echo "... model-plugin-trmesh"
	@echo "... model-plugin-utpackage"
	@echo "... ui-plugin-lineageIIextract"
	@echo "... ui-plugin-octree"
	@echo "... ui-plugin-pakreader"
	@echo "... ui-plugin-perlinnoise"
	@echo "... ui-plugin-reverseengineer"
	@echo "... win32"



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0

