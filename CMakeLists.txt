cmake_minimum_required(VERSION 3.2)
project(MicroMachine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(externals/SFML)
add_subdirectory(externals/physics)
add_subdirectory(game)

set(IMGUI_SFML_FIND_SFML OFF)
set(IMGUI_DIR "${CMAKE_SOURCE_DIR}/externals/imgui/" CACHE STRING "")
add_subdirectory(externals/imgui-sfml)


# ---- Photon Libraries ----
set(PHOTON_INCLUDE_DIR
        ${CMAKE_SOURCE_DIR}/externals/photon
        ${CMAKE_SOURCE_DIR}/externals/photon/Common-cpp
        ${CMAKE_SOURCE_DIR}/externals/photon/LoadBalancing-cpp
        ${CMAKE_SOURCE_DIR}/externals/photon/Photon-cpp)

set(PHOTON_LIB_DIR ${CMAKE_SOURCE_DIR}/externals/photon/windows)

set(PHOTON_LIBS
        debug ${PHOTON_LIB_DIR}/Common-cpp_vc17_debug_windows_md_x64.lib
        debug ${PHOTON_LIB_DIR}/LoadBalancing-cpp_vc17_debug_windows_md_x64.lib
        debug ${PHOTON_LIB_DIR}/Photon-cpp_vc17_debug_windows_md_x64.lib
        optimized ${PHOTON_LIB_DIR}/Common-cpp_vc17_release_windows_md_x64.lib
        optimized ${PHOTON_LIB_DIR}/LoadBalancing-cpp_vc17_release_windows_md_x64.lib
        optimized ${PHOTON_LIB_DIR}/Photon-cpp_vc17_release_windows_md_x64.lib
)

# ---- Create executable for each main/ file ----
file(GLOB MAIN_FILES CONFIGURE_DEPENDS main/*.cc)

foreach(MAIN_FILE ${MAIN_FILES})
    get_filename_component(EXE_NAME ${MAIN_FILE} NAME_WE)
    add_executable(${EXE_NAME}
            ${MAIN_FILE}
            ${SRC_FILES}
    )

    # ---- Link Libraries ----
    target_link_libraries(${EXE_NAME} PRIVATE
            crackitos_core
            crackitos_physics
            ${PHOTON_LIBS}
            api
            ImGui-SFML
            sfml-graphics
            sfml-window
            sfml-system
    )

    target_include_directories(${EXE_NAME} PRIVATE
            ${PHOTON_INCLUDE_DIR}
            externals/imgui
            externals/imgui-sfml
            externals/SFML/include
            externals/SFML
            externals/physics
    )

    # Copy data/ folder next to executable
    #add_custom_command(TARGET ${EXE_NAME} POST_BUILD
    #        COMMAND ${CMAKE_COMMAND} -E copy_directory
    #        ${CMAKE_SOURCE_DIR}/data $<TARGET_FILE_DIR:${EXE_NAME}>/data
    #)
endforeach()


#copy resources in build
#file(COPY ${CMAKE_SOURCE_DIR}/data DESTINATION ${CMAKE_BINARY_DIR})