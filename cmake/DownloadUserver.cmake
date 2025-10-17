include_guard(GLOBAL)

function(download_userver)
    set(OPTIONS)
    set(ONE_VALUE_ARGS VERSION GIT_TAG)
    set(MULTI_VALUE_ARGS)
    cmake_parse_arguments(ARG "${OPTIONS}" "${ONE_VALUE_ARGS}" "${MULTI_VALUE_ARGS}" ${ARGN})

    # CMP0077 and CMP0126 are required for correct option forwarding.
    cmake_minimum_required(VERSION 3.21)
    include(get_cpm)

    if(NOT DEFINED ARG_VERSION AND NOT DEFINED ARG_GIT_TAG)
        set(ARG_GIT_TAG develop)
    endif()

    if(NOT DEFINED CPM_USE_NAMED_CACHE_DIRECTORIES)
        set(CPM_USE_NAMED_CACHE_DIRECTORIES ON)
    endif()

    cpmaddpackage(
        NAME
        userver
        GITHUB_REPOSITORY
        userver-framework/userver
        VERSION
        ${ARG_VERSION}
        GIT_TAG
        ${ARG_GIT_TAG}
        ${ARG_UNPARSED_ARGUMENTS}
    )
endfunction()
