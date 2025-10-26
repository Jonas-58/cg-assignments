cmake_minimum_required(VERSION 3.12)
project(basic-techniques)

# ===============================================
# global c++ settings
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# ===============================================
# custom stuff
set(BT_OPENGL_MAJOR "3" CACHE INTERNAL "OpenGL Major Version")
set(BT_OPENGL_MINOR "3" CACHE INTERNAL "OpenGL Minor Version")
option(BT_OPENGL_CORE "Use OpenGL Core Profile" ON)


# ===============================================
# add compile definition to use inside cpp
add_compile_definitions( OGL_MAJOR=${BT_OPENGL_MAJOR} )
add_compile_definitions( OGL_MINOR=${BT_OPENGL_MINOR} )
add_compile_definitions( CURR_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )

# ===============================================
# disable glfw stuff
option(GLFW_BUILD_EXAMPLES  OFF)
option(GLFW_BUILD_TESTS  OFF)
option(GLFW_BUILD_DOCS  OFF)
option(GLFW_INSTALL  OFF)


# ===============================================
# build using all cores
include(ProcessorCount)
ProcessorCount(N)
if(NOT N EQUAL 0)
  set(CTEST_BUILD_FLAGS -j${N})
  set(ctest_test_args ${ctest_test_args} PARALLEL_LEVEL ${N})
endif()

# ===============================================
# compiler and linker flags

set(COMMON_COMPILER_FLAGS "")

if (MSVC)
    list(APPEND COMMON_COMPILER_FLAGS /MP)
elseif(APPLE)
    list(APPEND COMMON_COMPILER_FLAGS -Wall -Wextra -Wunused-result -Wcast-align -Wunused  -Wpedantic )
else()
    list(APPEND COMMON_COMPILER_FLAGS -Wall -Wextra -Wunused-result -march=native -Wcast-align -Wunused  -Wpedantic )
endif()


# ===============================================
# glad flags
set(GLAD_VERSION "${BT_OPENGL_MAJOR}.${BT_OPENGL_MINOR}")
if(${BT_OPENGL_CORE})
    set(GLAD_PROFILE CORE)
    add_definitions( -DOGL_CORE )
    message("Loading OpenGL Version ${BT_OPENGL_MAJOR}.${BT_OPENGL_MINOR} Core Profile.")
else()
    set(GLAD_PROFILE COMPATIBILITY)
    message("Loading OpenGL Version ${BT_OPENGL_MAJOR}.${BT_OPENGL_MINOR} Compatibility Profile.")
endif()

# ===============================================
# add libs & stuff
add_subdirectory(extern/glfw)
add_subdirectory(extern/glad)
add_subdirectory(extern/glm)

function(build_assignment FOLDER)
    if(EXISTS "${CMAKE_SOURCE_DIR}/${FOLDER}")
    message("Adding Assignment ${FOLDER}")

    # ===============================================
    # configure executable
    file(GLOB_RECURSE SOURCES
        "common/*.cc"
        "common/*.hh"
        "${FOLDER}/*.cc"
        "${FOLDER}/*.hh"
        "${FOLDER}/*.cpp"
        "${FOLDER}/*.h"
        "${FOLDER}/*.fsh"
        "${FOLDER}/*.vsh"
    )

    # group sources according to folder structure
    source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})

    # ===============================================
    # add executable & set link libraries
    add_executable(${FOLDER} ${SOURCES})
    target_link_libraries(${FOLDER} PUBLIC
        glfw
        glad
        glm
        ${COMMON_LINKER_FLAGS}
    )


    # ===============================================
    # set compile flags
    target_include_directories(${FOLDER} PUBLIC "common")
    target_compile_options(${FOLDER} PUBLIC ${COMMON_COMPILER_FLAGS})
    else()
        message("Could not find folder ${CMAKE_SOURCE_DIR}/${FOLDER}, skipping.")
    endif()
endfunction()


file(GLOB_RECURSE ASSIGNMENT_MAINS assignment*/main.cc)
foreach(file ${ASSIGNMENT_MAINS})
    get_filename_component(path ${file} DIRECTORY)
    get_filename_component(assignmentname ${path} NAME)
    build_assignment(${assignmentname})
endforeach()
