

#source files

file(GLOB_RECURSE incl includePublic/**.hpp include/**.hpp)
file(GLOB_RECURSE srcs src/**.cpp)

add_library(${PROJECT_NAME}
    ${incl}    
    ${srcs}
)


#lib files
add_subdirectory(libs)

#header files 
target_include_directories(${PROJECT_NAME} PUBLIC include includePublic)

target_link_libraries(${PROJECT_NAME} PUBLIC
    cefdll_wrapper
)