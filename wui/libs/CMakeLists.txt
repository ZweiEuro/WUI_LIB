# Put other libs here that this depends on
include(ExternalProject)

# cJSON external dependency
#-------------------------------------------------------------------------------
ExternalProject_Add(libcJSON
    GIT_REPOSITORY https://github.com/DaveGamble/cJSON.git
    GIT_TAG "aafb64a1c549b7b927e339df6d35b1d5059dc235"
    CMAKE_ARGS += -DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/cJSON -DBUILD_TESTING=OFF
)

add_dependencies(${PROJECT_NAME} libcJSON)  

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/cJSON/include)
