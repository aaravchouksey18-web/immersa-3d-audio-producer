cmake_minimum_required(VERSION 3.16)
set (CMAKE_CXX_STANDARD 17)
set(PROJECT_NAME 3daudio-producer)
project (${PROJECT_NAME} LANGUAGES C CXX)
# Build-time compiler flags for all build types.
# (The old version overwrote CMAKE_CXX_FLAGS_{DEBUG,RELEASE} with an
# undefined -DDATADIR=${DATAPATH}, which defeated the code-local
# #ifndef DATADIR resource fallbacks - do not reintroduce that.)
add_compile_options(-Wall -fPIE)
# Let CMake search for a raylib-config.cmake
find_package(raylib 3.5 QUIET)
find_package(Threads REQUIRED)
#Pugi XML parsing
find_path(pugixml_INCLUDE_DIRS pugixml.hpp)
#find_library(pugixml_LIBRARIES NAMES pugixml)
#For the shared library:
set ( PROJECT_LINK_LIBS  raylib openal ${CMAKE_THREAD_LIBS_INIT} -lpugixml)
#where to find library files .so 
link_directories( /usr/lib /usr/local/lib ) 
if(WIN32)
    set ( PROJECT_LINK_LIBS  ${PROJECT_LINK_LIBS} opengl32 gdi32 winmm)
endif()
set ( PROJECT_INCLUDE_DIR ./src)
#for where to find header files for source and library
include_directories(/usr/include /usr/local/include ./include ./src/timeline-track-editor/include ${PROJECT_INCLUDE_DIR} src/backends/imgui)
set(SOURCES src/main.cpp
            src/global_variables.cpp
            src/main_gui_editor.cpp
            src/timeline.cpp
            src/SaveSystem.cpp src/LoadSystem.cpp src/XMLCreator.cpp src/XMLReader.cpp
            src/CreateStandardReverbZoneDialog.cpp src/EditMultipleStandardReverbZonesDialog.cpp 
            src/CreateEAXReverbZoneDialog.cpp src/EditMultipleEAXReverbZonesDialog.cpp 
            src/CreateEchoZoneDialog.cpp src/EditMultipleEchoZonesDialog.cpp
            src/echo-zone.cpp
            src/reverb-zone.cpp
            src/effect-zone.cpp
            src/effects-manager.cpp
            src/soundproducer-registry.cpp
            src/immediate_mode_sound_player.cpp src/sound_bank.cpp
            src/CreateSoundProducerDialog.cpp src/EditMultipleSoundProducersDialog.cpp 
            src/Change-HRTF-Dialog.cpp src/HRTF-Test-Dialog.cpp 
            src/EditListenerDialog.cpp
            src/openalsoftaudioengine.cpp src/soundproducer.cpp src/listener.cpp
            src/backends/openalsoft-player-drwav.cpp
            src/backends/imgui_input_extended.cpp
            src/backends/rlImGui.cpp 
            src/backends/imgui/imgui.cpp src/backends/imgui/imgui_demo.cpp
            src/backends/imgui/imgui_draw.cpp src/backends/imgui/imgui_tables.cpp
            src/backends/imgui/imgui_widgets.cpp
             )
#make executable
add_executable(${PROJECT_NAME} ${SOURCES})
#link libraries
target_link_libraries(${PROJECT_NAME} ${PROJECT_LINK_LIBS} ${CMAKE_DL_LIBS} )
#make install ops
install(TARGETS 3daudio-producer DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)