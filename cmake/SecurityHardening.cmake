# cmake/SecurityHardening.cmake
# Applies compiler and linker security hardening flags to a target.
# Usage: shape_apply_hardening(<target>)

include_guard(GLOBAL)

function(shape_apply_hardening target)
    if(NOT SHAPE_ENABLE_HARDENING)
        return()
    endif()

    if(MSVC)
        target_compile_options(${target} PRIVATE
            /GS           # Buffer Security Check
            /guard:cf     # Control Flow Guard
            /sdl          # Additional Security Development Lifecycle checks
        )
        target_link_options(${target} PRIVATE
            /DYNAMICBASE  # Address Space Layout Randomization (ASLR)
            /NXCOMPAT     # Data Execution Prevention (DEP / NX)
            /HIGHENTROPYVA # 64-bit ASLR High Entropy
            /guard:cf     # Control Flow Guard Linker flag
        )
        target_compile_definitions(${target} PRIVATE
            _FORTIFY_SOURCE=2
        )
    else()
        # GCC / Clang
        target_compile_options(${target} PRIVATE
            -fstack-protector-strong
            -U_FORTIFY_SOURCE
            -D_FORTIFY_SOURCE=2
        )
        
        # Linker hardening flags
        if(WIN32)
            # MinGW / Clang on Windows (PE/COFF)
            target_link_options(${target} PRIVATE
                -Wl,--dynamicbase
                -Wl,--nxcompat
                -Wl,--high-entropy-va
            )
        elseif(APPLE)
            target_link_options(${target} PRIVATE
                -Wl,-bind_at_load
            )
        else()
            # Linux (ELF)
            target_compile_options(${target} PRIVATE -fPIE)
            target_link_options(${target} PRIVATE
                -fPIE
                -pie
                -Wl,-z,relro
                -Wl,-z,now
                -Wl,-z,noexecstack
            )
        endif()
    endif()
endfunction()
