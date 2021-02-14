
# Author: Alexander Miro
# Date: Feb-2021

include(FetchContent)
include(ExternalProject)

set(FETCHCONTENT_QUIET OFF)

function(fetch p_content p_git_repo p_git_tag p_source_dir)
    message("Fetching dependency ===> ${p_content} | ${p_git_repo} | ${p_git_tag} | ${p_source_dir}")
    set(FETCHCONTENT_BASE_DIR ${p_source_dir})
    FetchContent_Declare(
        ${p_content}
        GIT_REPOSITORY ${p_git_repo}
        GIT_TAG        ${p_git_tag}
    ) 
    FetchContent_GetProperties(${p_content})
    if (NOT ${p_content}_POPULATED)
        FetchContent_Populate(${p_content})
        add_subdirectory(${${p_content}_SOURCE_DIR} ${${p_content}_BINARY_DIR})
    endif()
endfunction()

function(add_external_proj p_content p_zip_url p_version p_source_dir)
    message("Adding external project ===> ${p_content} | ${p_zip_url} | ${p_source_dir}")
    set(dest_dir ${p_source_dir}/${p_content})
    ExternalProject_Add(
        ${p_content}
        URL               ${p_zip_url}
        PREFIX            ${dest_dir}
        INSTALL_DIR       ${dest_dir}
        LOG_BUILD         1
        BUILD_IN_SOURCE   1
        CONFIGURE_COMMAND COMMAND ./configure "--prefix=${dest_dir}" 
        BUILD_COMMAND     make "CXXFLAGS=-Wno-deprecated-declarations"
        BUILD_ALWAYS      OFF
        UPDATE_COMAND     ""
        INSTALL_COMMAND   make install
    ) 
endfunction()

function(fetch_cpm)
    set(CPM_DOWNLOAD_LOCATION "${CMAKE_SOURCE_DIR}/cmake/CPM.cmake")
    set(CPM_DOWNLOAD_VERSION 0.25.1)

    if(NOT (EXISTS ${CPM_DOWNLOAD_LOCATION} AND CPM_VERSION STREQUAL CPM_DOWNLOAD_VERSION))
    message(STATUS "Downloading CPM.cmake")
    file(DOWNLOAD https://github.com/TheLartians/CPM.cmake/releases/download/v${CPM_DOWNLOAD_VERSION}/CPM.cmake ${CPM_DOWNLOAD_LOCATION})
    endif()

    include(${CPM_DOWNLOAD_LOCATION})
endfunction()

function(fetch_lua p_url p_version p_source_dir)
    message("Adding external project ===> lua | ${p_url} | ${p_version} | ${p_source_dir}")
    CPMAddPackage(
        NAME           lua
        GIT_REPOSITORY ${p_url}
        VERSION        ${p_version}
        DOWNLOAD_ONLY  YES
        SOURCE_DIR     ${p_source_dir}
    ) 
    if (lua_ADDED)
        set(lua_ADDED lua_ADDED CACHE INTERNAL "")
        set(lua_SOURCE_DIR ${lua_SOURCE_DIR} CACHE INTERNAL "")

        # lua has no CMake support, so we create our own target
        FILE(GLOB lua_sources ${lua_SOURCE_DIR}/*.c)
        add_library(lua STATIC ${lua_sources})

        target_include_directories(lua
            PUBLIC
            $<BUILD_INTERFACE:${lua_SOURCE_DIR}>
        )
    endif()
endfunction()


function(fetch_dependencies)
    set(prefix          ARG)
    set(noValues        )
    set(singleValues    DIR GOOGLETEST GRPC CATCH2 LUA)
    set(multiValues     )

    include(CMakeParseArguments)
    cmake_parse_arguments(${prefix}
                         "${noValues}"
                         "${singleValues}"
                         "${multiValues}"
                         ${ARGN})

    if("${${prefix}_DIR}" STREQUAL "") 
        message(FATAL_ERROR "Missing DIR parameter in fetch_dependencies()")
        return()
    endif()

    set(source_dir ${${prefix}_DIR})

    if(NOT "${${prefix}_GOOGLETEST}" STREQUAL "") 
        fetch(googletest "https://github.com/google/googletest.git" ${${prefix}_GOOGLETEST} ${source_dir})
    endif()

    if(NOT "${${prefix}_GRPC}" STREQUAL "") 
        fetch(grpc "https://github.com/grpc/grpc.git" ${${prefix}_GRPC} ${source_dir})
    endif()

    if(NOT "${${prefix}_CATCH2}" STREQUAL "") 
        fetch(catch2 "https://github.com/catchorg/Catch2" ${${prefix}_CATCH2} ${source_dir})
    endif()

    if(NOT "${${prefix}_LUA}" STREQUAL "") 
        fetch_lua("https://github.com/lua/lua.git" ${${prefix}_LUA} ${source_dir}/lua)
    endif()

endfunction()

