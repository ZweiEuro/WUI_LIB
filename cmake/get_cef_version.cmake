# Download and extrackt CEF binary standard distribution
function(get_cef_version ARG_VERSION ret)

    # What system are we downloading?
    if("${CMAKE_SYSTEM_NAME}" STREQUAL "Darwin")
        set(CEF_CMAKE_OS_MACOSX 1)
        set(CEF_CMAKE_OS_POSIX 1)
    elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
        set(CEF_CMAKE_OS_LINUX 1)
        set(CEF_CMAKE_OS_POSIX 1)
    elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
        set(CEF_CMAKE_OS_WINDOWS 1)
    else()
        message(FATAL_ERROR "Unsupported target platform")
    endif()

    # set name extensions based on platform
    if(CEF_CMAKE_OS_LINUX)
        if(CMAKE_SIZEOF_VOID_P EQUAL 8)
            set(cefName cef_binary_${ARG_VERSION}_linux64)
        else()
            set(cefName cef_binary_${ARG_VERSION}_linux32)
        endif()
    elseif(CEF_CMAKE_OS_WINDOWS)
        if(CMAKE_SIZEOF_VOID_P EQUAL 8)
            set(cefName cef_binary_${ARG_VERSION}_windows64)
        else()
            set(cefName cef_binary_${ARG_VERSION}_windows32)
        endif()
    else()
        message(FATAL_ERROR "Download platform not supported yet")
    endif()

    set(${ret} ${cefName} PARENT_SCOPE)

    # Does the folder already exist?
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${cefName})
        message(STATUS "Using existing CEF ${cefName}")
        set(${ARG_VERSION} ${cefName} PARENT_SCOPE)
        return()
    endif()

    # download URL
    set(cefArchiveURL https://cef-builds.spotifycdn.com/${cefName}.tar.bz2)

    # fix the url as the version may contain pluses
    string(REGEX REPLACE "\\+" "%2B" cefArchiveURL ${cefArchiveURL})
    set(cefArchive ${CMAKE_CURRENT_SOURCE_DIR}/${cefName}.tar.bz2)

    if(NOT EXISTS ${cefArchive})
        # download cef version
        message(STATUS "Downloading CEF ${cefArchiveURL}")
        file(DOWNLOAD ${cefArchiveURL} ${cefArchive}
            SHOW_PROGRESS
        )

        # ... and extract
        message(STATUS "Extracting ${cefArchive}")
        execute_process(
            COMMAND ${CMAKE_COMMAND} -E tar xzf ${cefArchive}
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        )

        # delete archive
        file(REMOVE ${cefArchive})
    endif()


endfunction()

