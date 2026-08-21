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
        )
        target_link_options(${target} PRIVATE
            /DYNAMICBASE  # Address Space Layout Randomization (ASLR)
            /NXCOMPAT     # Data Execution Prevention (DEP / NX)
            /HIGHENTROPYVA # 64-bit ASLR High Entropy
        )
    else()
        # GCC / Clang
        target_compile_options(${target} PRIVATE
            -fstack-protector-strong
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
            # Modern macOS linker handles hardening by default
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
