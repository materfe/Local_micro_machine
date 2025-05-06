file(GLOB_RECURSE SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc)
file(GLOB_RECURSE HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)




add_library(api STATIC ${SRC_FILES} ${HEADER_FILES})

file(GLOB_RECURSE DATA_FILES
        "data/*.json"
        "data/*.png"
        "data/*.jpg"
        "data/*.jpeg"
        "data/*.bmp"
        "data/*.hdr"
        "data/*.obj"
        "data/*.mtl"
        "data/*.gltf"
        "data/*.bin"
)

set(Data_OUTPUT_FILES "")  # Make sure this list is empty before appending

foreach(DATA ${DATA_FILES})
    get_filename_component(FILE_NAME ${DATA} NAME)
    get_filename_component(PATH_NAME ${DATA} DIRECTORY)
    get_filename_component(EXTENSION ${DATA} EXT)
    file(RELATIVE_PATH PATH_NAME "${CMAKE_CURRENT_SOURCE_DIR}" ${PATH_NAME})

    set(DATA_OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${PATH_NAME}/${FILE_NAME}")

    # Ensure the directory exists
    file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/${PATH_NAME}")

    add_custom_command(
            OUTPUT ${DATA_OUTPUT}
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${DATA} ${DATA_OUTPUT}
            DEPENDS ${DATA}
    )

    list(APPEND Data_OUTPUT_FILES ${DATA_OUTPUT})
endforeach()

# Create a custom target to copy all data files
add_custom_target(copy_data ALL DEPENDS ${Data_OUTPUT_FILES})
add_dependencies(api copy_data)

# Photon include and libs passed from parent
set(PHOTON_INCLUDE_DIR
        ${CMAKE_SOURCE_DIR}/externals/photon
        ${CMAKE_SOURCE_DIR}/externals/photon/Common-cpp
        ${CMAKE_SOURCE_DIR}/externals/photon/LoadBalancing-cpp
        ${CMAKE_SOURCE_DIR}/externals/photon/Photon-cpp
)

set(PHOTON_LIB_DIR ${CMAKE_SOURCE_DIR}/externals/photon/windows)

set(PHOTON_LIBS
        debug ${PHOTON_LIB_DIR}/Common-cpp_vc17_debug_windows_md_x64.lib
        debug ${PHOTON_LIB_DIR}/LoadBalancing-cpp_vc17_debug_windows_md_x64.lib
        debug ${PHOTON_LIB_DIR}/Photon-cpp_vc17_debug_windows_md_x64.lib
        optimized ${PHOTON_LIB_DIR}/Common-cpp_vc17_release_windows_md_x64.lib
        optimized ${PHOTON_LIB_DIR}/LoadBalancing-cpp_vc17_release_windows_md_x64.lib
        optimized ${PHOTON_LIB_DIR}/Photon-cpp_vc17_release_windows_md_x64.lib
)

# Ajout des includes de Photon
target_include_directories(api
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${PHOTON_INCLUDE_DIR}
)

# Lien avec Photon
target_link_libraries(api
        PRIVATE sfml-graphics sfml-window sfml-system
        PRIVATE ${PHOTON_LIBS}
        PUBLIC crackitos_physics
)


if(MSVC)
    target_compile_options(api PRIVATE "/W3" "/WX" "/permissive-")
endif ()


#add_executable(game tmp/game.cc)
#target_link_libraries(game PRIVATE api)