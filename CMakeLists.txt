cmake_minimum_required(VERSION 3.10)

project(wui VERSION 0.0.1)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # Generate compile_commands.json for clangd
set_property(GLOBAL PROPERTY USE_FOLDERS ON) # Group projects into folders

if( NOT CMAKE_BUILD_TYPE )
    set(CMAKE_BUILD_TYPE Debug CACHE STRING
        "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel. Default: Debug"
        FORCE )
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_COLOR_DIAGNOSTICS ON)


include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/cef_cmake.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/get_cef_version.cmake)

if(NOT CEF_CMAKE_INCLUDED)
    message(FATAL_ERROR "Configuration not included. You need to include `cef_cmake` in your root CMakeLists.txt file")
endif()

# Note: Anything other than the version here is NOT supported officially, if you wanne build it, you're on your own
#options
set(WUI_CEF_VERSION "116.0.21+g9c7dc32+chromium-116.0.5845.181" CACHE STRING "CEF version to use")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/wui)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/wui)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/bin)

set(CEF_OUTPUT_DIR ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

# download/unpack CEF binary if not present for version and platform
get_cef_version(${WUI_CEF_VERSION} cefName)


#Build the libcef Wrapper
set(libcef_src_files)
set(libcef_src_dir ${cefName}/libcef_dll)
file(GLOB_RECURSE libcef_src_files "${libcef_src_dir}/*.cc" "${libcef_src_dir}/*.mm" "${libcef_src_dir}/*.h" "${cefName}/include/*.h")

add_library(cefdll_wrapper STATIC ${libcef_src_files})

target_compile_definitions(cefdll_wrapper
    PRIVATE
        # Creating the CEF wrapper library. Do not define this for dependent targets.
        -DWRAPPING_CEF_SHARED
    PUBLIC
        # Allow C++ programs to use stdint.h macros specified in the C99 standard that aren't
        # in the C++ standard (e.g. UINT8_MAX, INT64_MIN, etc)
        -D__STDC_CONSTANT_MACROS
        -D__STDC_FORMAT_MACROS
)

target_include_directories(cefdll_wrapper
    PUBLIC ${cefName}
    INTERFACE include
)

# add_custom_command(TARGET cefdll_wrapper_lib POST_BUILD
#     COMMENT "cefdll_wrapper: Copying CEF resources"
#     COMMAND ${CMAKE_COMMAND} -E
#         make_directory ${CEF_CMAKE_OUTPUT_DIR}
#     COMMAND ${CMAKE_COMMAND} -E copy_directory
#         ${CMAKE_CURRENT_SOURCE_DIR}/${cefName}/Resources/locales
#         ${CEF_CMAKE_OUTPUT_DIR}/locales
#     COMMAND ${CMAKE_COMMAND} -E copy_if_different
#         ${CMAKE_CURRENT_SOURCE_DIR}/${cefName}/Resources/cef.pak
#         ${CMAKE_CURRENT_SOURCE_DIR}/${cefName}/Resources/cef_100_percent.pak
#         ${CMAKE_CURRENT_SOURCE_DIR}/${cefName}/Resources/cef_200_percent.pak
#         ${CMAKE_CURRENT_SOURCE_DIR}/${cefName}/Resources/cef_extensions.pak
#         ${CMAKE_CURRENT_SOURCE_DIR}/${cefName}/Resources/devtools_resources.pak
#         ${CMAKE_CURRENT_SOURCE_DIR}/${cefName}/Resources/icudtl.dat
#         ${CEF_CMAKE_OUTPUT_DIR}
# )



if(CEF_CMAKE_OS_LINUX)

    find_package(X11 REQUIRED)
    find_package(Threads REQUIRED)

    target_link_libraries(cefdll_wrapper INTERFACE
        X11
        Threads::Threads
        debug ${CMAKE_CURRENT_SOURCE_DIR}/${cefName}/Debug/libcef.so
        optimized ${CMAKE_CURRENT_SOURCE_DIR}/${cefName}/Release/libcef.so
    )


    add_custom_command(TARGET cefdll_wrapper POST_BUILD
        COMMENT "Copy CEF build and runtime dependencies"
        COMMAND ${CMAKE_COMMAND} -E
            make_directory ${CEF_OUTPUT_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/wui/includePublic
            ${CEF_OUTPUT_DIR}/include
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_CURRENT_SOURCE_DIR}/${cefName}/$<CONFIG>/chrome-sandbox
            ${CMAKE_CURRENT_SOURCE_DIR}/${cefName}/$<CONFIG>/libcef.so
            ${CMAKE_CURRENT_SOURCE_DIR}/${cefName}/$<CONFIG>/libEGL.so
            ${CMAKE_CURRENT_SOURCE_DIR}/${cefName}/$<CONFIG>/libGLESv2.so
            ${CMAKE_CURRENT_SOURCE_DIR}/${cefName}/$<CONFIG>/libvk_swiftshader.so
            ${CMAKE_CURRENT_SOURCE_DIR}/${cefName}/$<CONFIG>/libvulkan.so.1
            ${CMAKE_CURRENT_SOURCE_DIR}/${cefName}/$<CONFIG>/snapshot_blob.bin
            ${CMAKE_CURRENT_SOURCE_DIR}/${cefName}/$<CONFIG>/v8_context_snapshot.bin
            ${CMAKE_CURRENT_SOURCE_DIR}/${cefName}/$<CONFIG>/vk_swiftshader_icd.json
            ${CMAKE_CURRENT_SOURCE_DIR}/${cefName}/Resources/*
            ${CEF_OUTPUT_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_CURRENT_SOURCE_DIR}/${cefName}/Resources/locales/*
            ${CEF_OUTPUT_DIR}/locales
        COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_CURRENT_SOURCE_DIR}/static_files/CMakeLists.X11.txt
            ${CEF_OUTPUT_DIR}/CMakeLists.txt
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_BINARY_DIR}/wui/libs/cJSON/include
            ${CEF_OUTPUT_DIR}/include
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_CURRENT_BINARY_DIR}/wui/libs/cJSON/lib/libcjson.so
            ${CEF_OUTPUT_DIR}
        )


    if(CEF_USE_SANDBOX)
        target_compile_definitions(cefdll_wrapper
            PUBLIC -DCEF_USE_SANDBOX
        )
    endif()

elseif(CEF_CMAKE_OS_WINDOWS)

    target_compile_definitions(cefdll_wrapper PUBLIC
        -DNOMINMAX
        -DWIN32_LEAN_AND_MEAN
        -DUNICODE
        -D_UNICODE
    )

    target_link_libraries(cefdll_wrapper PUBLIC
        comctl32.lib
        rpcrt4.lib
        shlwapi.lib
        ws2_32.lib
        debug ${CMAKE_CURRENT_SOURCE_DIR}/${cefName}/Debug/libcef.lib
        optimized ${CMAKE_CURRENT_SOURCE_DIR}/${cefName}/Release/libcef.lib
    )

    add_custom_command(TARGET cefdll_wrapper POST_BUILD
        COMMENT "cefdll_wrapper: Copying CEF binaries"
        COMMAND ${CMAKE_COMMAND} -E
            make_directory ${CEF_CMAKE_OUTPUT_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/${cefName}/$<CONFIG>/swiftshader
            ${CEF_CMAKE_OUTPUT_DIR}/swiftshader
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_CURRENT_SOURCE_DIR}/${cefName}/$<CONFIG>/chrome_elf.dll
            ${CMAKE_CURRENT_SOURCE_DIR}/${cefName}/$<CONFIG>/d3dcompiler_47.dll
            ${CMAKE_CURRENT_SOURCE_DIR}/${cefName}/$<CONFIG>/libcef.dll
            ${CMAKE_CURRENT_SOURCE_DIR}/${cefName}/$<CONFIG>/libEGL.dll
            ${CMAKE_CURRENT_SOURCE_DIR}/${cefName}/$<CONFIG>/libGLESv2.dll
            ${CMAKE_CURRENT_SOURCE_DIR}/${cefName}/$<CONFIG>/natives_blob.bin
            ${CMAKE_CURRENT_SOURCE_DIR}/${cefName}/$<CONFIG>/snapshot_blob.bin
            ${CMAKE_CURRENT_SOURCE_DIR}/${cefName}/$<CONFIG>/v8_context_snapshot.bin
            ${CEF_CMAKE_OUTPUT_DIR}
    )

    if(CEF_USE_SANDBOX)
        target_compile_definitions(cefdll_wrapper
            PUBLIC
                -DCEF_USE_SANDBOX
                -DPSAPI_VERSION=1
        )

        target_link_libraries(cefdll_wrapper PUBLIC
            dbghelp.lib
            psapi.lib
            version.lib
            wbemuuid.lib
            winmm.lib
            debug ${CMAKE_CURRENT_SOURCE_DIR}/${cefName}/Debug/cef_sandbox.lib
            optimized ${CMAKE_CURRENT_SOURCE_DIR}/${cefName}/Release/cef_sandbox.lib
        )
    endif()

else()
    # TODO: Copy macos shared libraries
    message(FATAL_ERROR "Build platform not supported yet")
endif()


add_subdirectory(wui)