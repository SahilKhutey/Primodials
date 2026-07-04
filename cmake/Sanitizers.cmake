# cmake/Sanitizers.cmake
# Applies sanitizer flags to a target (Debug builds only).
# Controlled by SHAPE_ENABLE_SANITIZER, SHAPE_ENABLE_UBSAN, SHAPE_ENABLE_TSAN options.

include_guard(GLOBAL)

function(shape_apply_sanitizers target)
    if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
        return()
    endif()

    if(SHAPE_ENABLE_SANITIZER)
        if(MSVC)
            target_compile_options(${target} PRIVATE /fsanitize=address)
            target_link_options(${target}    PRIVATE /fsanitize=address)
        else()
            target_compile_options(${target} PRIVATE -fsanitize=address -fno-omit-frame-pointer)
            target_link_options(${target}    PRIVATE -fsanitize=address)
        endif()
        message(STATUS "[Shape] AddressSanitizer enabled for ${target}")
    endif()

    if(SHAPE_ENABLE_UBSAN AND NOT MSVC)
        target_compile_options(${target} PRIVATE -fsanitize=undefined)
        target_link_options(${target}    PRIVATE -fsanitize=undefined)
        message(STATUS "[Shape] UBSanitizer enabled for ${target}")
    endif()

    if(SHAPE_ENABLE_TSAN AND NOT MSVC)
        target_compile_options(${target} PRIVATE -fsanitize=thread)
        target_link_options(${target}    PRIVATE -fsanitize=thread)
        message(STATUS "[Shape] ThreadSanitizer enabled for ${target}")
    endif()
endfunction()
