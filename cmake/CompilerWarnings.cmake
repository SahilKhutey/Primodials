# cmake/CompilerWarnings.cmake
# Applies compiler warning flags to a target.
# Usage: shape_apply_warnings(<target>)

include_guard(GLOBAL)

function(shape_apply_warnings target)
    if(MSVC)
        target_compile_options(${target} PRIVATE
            /W4
            /WX
            /permissive-
            /Zc:__cplusplus
            /utf-8
            /volatile:iso
            /EHsc
            /fp:precise
            $<$<CONFIG:Debug>:/RTC1>
        )
        target_compile_definitions(${target} PRIVATE
            _CRT_SECURE_NO_WARNINGS
            NOMINMAX
            WIN32_LEAN_AND_MEAN
        )
    else()
        target_compile_options(${target} PRIVATE
            -Wall
            -Wextra
            -Wpedantic
            -Wshadow
            -Wcast-align
            -Wunused
            -Wnon-virtual-dtor
            -Wformat=2
            -Wnull-dereference
            -Wdouble-promotion
            -fno-fast-math
            -Wno-c2y-extensions
            $<$<CONFIG:Debug>:-Werror>
        )
    endif()
endfunction()
