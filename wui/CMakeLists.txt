set(srcs)

#source files
file(GLOB_RECURSE srcs src/*.cpp)

add_library(${PROJECT_NAME}
    ${srcs}
    ${CEF_CMAKE_EXECUTABLE_RESOURCES}
)

#lib files
add_subdirectory(libs)

#header files 
target_include_directories(${PROJECT_NAME} PRIVATE include)


target_link_libraries(${PROJECT_NAME} PUBLIC
    cefdll_wrapper_lib
)