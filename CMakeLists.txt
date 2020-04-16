cmake_minimum_required(VERSION 2.8)
project( 3DGameEngine )

#####################################
# Function for finding packages (Windows)
#####################################

function(myFindPackage VARIABLE DEBUG_NAME RELEASE_NAME HINT_DIR)
	find_library(LIBRARY_RELEASE_${RELEASE_NAME} "${RELEASE_NAME}" HINTS ${HINT_DIR})
	find_library(LIBRARY_DEBUG_${DEBUG_NAME} "${DEBUG_NAME}" HINTS ${HINT_DIR})
	set(ALL_LIBRARY optimized ${LIBRARY_RELEASE_${RELEASE_NAME}} debug ${LIBRARY_DEBUG_${DEBUG_NAME}})	
	set(${VARIABLE} ${ALL_LIBRARY} PARENT_SCOPE)	
endfunction(myFindPackage)

#####################################
# Add directory to list of places CMake looks for config files
#####################################

if(WIN32)
	set(MASTER_DEPEND "" CACHE PATH "Set this to your directory for dependencies.")	
endif()

set(CMAKE_PREFIX_PATH ${MASTER_DEPEND} ${CMAKE_PREFIX_PATH})

#####################################
# LIST OF DEPENDENCIES:
# - OpenGL
# - GLFW (static)
# - GLEW (static)
# - GLM
# - Assimp (static)
# INTERNALLY INCLUDES -
# - DearImgui - https://github.com/ocornut/imgui
# - json.hpp - https://github.com/nlohmann/json
# - tiny_gltf.h - https://github.com/syoyo/tinygltf
# - imfilebrowser.h - https://github.com/AirGuanZ/imgui-filebrowser
# - stb_image
# - stb_image_write
#####################################

#####################################
# OpenGL
#####################################

find_package(OpenGL REQUIRED)

#####################################
# GLFW (static) 
#####################################

if(WIN32 AND NOT MINGW)	
	myFindPackage("GLFW_LIBRARY" "glfw3_d" "glfw3" ${MASTER_DEPEND}/lib)
elseif(WIN32 AND MINGW)
	myFindPackage("GLFW_LIBRARY" "glfw3" "glfw3" ${MASTER_DEPEND}/lib)
elseif(NOT WIN32)
	find_package(PkgConfig REQUIRED)
	pkg_search_module(GLFW REQUIRED glfw3)
	set(GLFW_LIBRARY ${GLFW_STATIC_LIBRARIES})
endif()

#####################################
# GLEW (static) 
#####################################

if(WIN32 AND NOT MINGW)	
	myFindPackage("GLEW_LIBRARY" "libglew32d" "libglew32" ${MASTER_DEPEND}/lib)
elseif(WIN32 AND MINGW)
	myFindPackage("GLEW_LIBRARY" "libglew32" "libglew32" ${MASTER_DEPEND}/lib)
elseif(NOT WIN32)
	find_package(GLEW REQUIRED)	
	set(GLEW_LIBRARY ${GLEW_LIBRARIES})
endif()

add_definitions(-DGLEW_STATIC)

#####################################
# Assimp (static)
#####################################

if(WIN32 AND NOT MINGW)	

	message(${MSVC_TOOLSET_VERSION})
	set(ASSIMP_MSVC_VERSION "-vc${MSVC_TOOLSET_VERSION}")

	myFindPackage("ASSIMP_LIBRARY" "assimp${ASSIMP_MSVC_VERSION}-mt_d" "assimp${ASSIMP_MSVC_VERSION}-mt" ${MASTER_DEPEND}/lib)
	myFindPackage("ASSIMP_ZLIB" "zlibstaticd" "zlibstatic" ${MASTER_DEPEND}/lib)
	myFindPackage("ASSIMP_IRR" "IrrXML_d" "IrrXML" ${MASTER_DEPEND}/lib)
elseif(WIN32 AND MINGW)
	myFindPackage("ASSIMP_LIBRARY" "assimp" "assimp" ${MASTER_DEPEND}/lib)
	myFindPackage("ASSIMP_ZLIB" "zlibstatic" "zlibstatic" ${MASTER_DEPEND}/lib)
	myFindPackage("ASSIMP_IRR" "IrrXML" "IrrXML" ${MASTER_DEPEND}/lib)
elseif(NOT WIN32)
	find_package(assimp REQUIRED)
	set(ASSIMP_LIBRARY ${ASSIMP_LIBRARIES})
	set(ASSIMP_ZLIB "")
	set(ASSIMP_IRR "")
endif()

#####################################
# Require C++11
#####################################

if(WIN32)	
	set(CMAKE_CXX_STANDARD 17)
	set(CMAKE_CXX_STANDARD_REQUIRED ON)
elseif(WIN32 AND MINGW)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
elseif(LINUX)	
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")	
elseif(APPLE)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z")	
endif()

#####################################
# Include any includes and libraries we missed
#####################################

include_directories(${MASTER_DEPEND}/include)
link_directories(${MASTER_DEPEND}/lib)

#####################################
# Get general sources
#####################################

file(GLOB GENERAL_SOURCES
       ${PROJECT_SOURCE_DIR}/*.cpp
       ${PROJECT_SOURCE_DIR}/*.hpp
       ${PROJECT_SOURCE_DIR}/*.h
       ${PROJECT_SOURCE_DIR}/core/*.cpp
       ${PROJECT_SOURCE_DIR}/core/*.hpp
       ${PROJECT_SOURCE_DIR}/core/*.h
       ${PROJECT_SOURCE_DIR}/physics/*.cpp
       ${PROJECT_SOURCE_DIR}/physics/*.hpp
       ${PROJECT_SOURCE_DIR}/physics/*.h
       ${PROJECT_SOURCE_DIR}/libs/*.cpp
       ${PROJECT_SOURCE_DIR}/libs/*.hpp
       ${PROJECT_SOURCE_DIR}/libs/*.h
       ${PROJECT_SOURCE_DIR}/libs/dearimgui/*.cpp
       ${PROJECT_SOURCE_DIR}/libs/dearimgui/*.hpp
       ${PROJECT_SOURCE_DIR}/libs/dearimgui/*.h)

#####################################
# Set general libraries
#####################################

#set(GENERAL_LIBRARIES ${OPENGL_LIBRARY} ${GLFW_LIBRARY} ${GLEW_LIBRARY} ${ASSIMP_LIBRARY} ${ASSIMP_ZLIB} ${ASSIMP_IRR})

set(GENERAL_LIBRARIES ${GLFW_LIBRARY} ${GLEW_LIBRARY} ${ASSIMP_LIBRARY} ${ASSIMP_ZLIB} ${ASSIMP_IRR} ${OPENGL_LIBRARY})

#####################################
# Create executable
#####################################

# Create executable and link libraries
add_executable(3DGameEngine ${GENERAL_SOURCES})
target_link_libraries(3DGameEngine ${GENERAL_LIBRARIES})

#####################################
# Set install target 
#####################################

install(TARGETS 3DGameEngine RUNTIME DESTINATION bin)