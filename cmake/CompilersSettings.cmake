# ==============================================================================
# Aswin 2DEngine playground - Top-level compilers settings
# ==============================================================================

include(CheckCXXSourceCompiles)

if(MSVC)
	# ============================================================================
	# MSVC compiler settings
	# ============================================================================

	#----------------------------------------------
	# Basic
	#----------------------------------------------

	add_compile_options(/MP)                                                 # Multi-processor compilation
	add_compile_options(/W4)                                                 # High warning level 0/4
	add_compile_options(/Wall)                                               # All warnings
	add_compile_options(/WX-)                                                # Warnings not as errors
	add_compile_options(/permissive-)                                        # Strict conformance mode
	add_compile_options(/fp:fast)                                            # Fast floating point
	add_compile_options(/Zc:__cplusplus)                                     # Correct __cplusplus macro
	add_compile_options(/Zc:inline)                                          # Remove unreferenced COMDAT
	add_compile_options(/Zc:preprocessor)                                    # Conforming preprocessor
	add_compile_options(/external:anglebrackets)                             # Treat angle bracket includes as external
	add_compile_options(/external:W0)                                        # No warnings for external headers
	add_compile_options(/arch:AVX2)                                          # Enable AVX2 (covers AVX, SSE4.2, etc.)

	#----------------------------------------------
	# Code analysis
	#----------------------------------------------

#	add_compile_options(/analyze:WX-)                                        # Static analysis warnings not as errors
#	add_compile_options(/analyze:external-)                                  # Don't analyze external headers
#	add_compile_options(/analyze)                                            # Enable static analysis

	#----------------------------------------------
	# Opimisations by build configuration
	#----------------------------------------------

	#-----------------------
	# Release builds
	#-----------------------

	add_compile_options($<$<CONFIG:Release>:/MD>)                            # Runtime library
	add_compile_options($<$<CONFIG:Release>:/O2>)                            # Maximum speed optimization
	add_compile_options($<$<CONFIG:Release>:/Oi>)                            # Enable intrinsic functions
	add_compile_options($<$<CONFIG:Release>:/Ot>)                            # Favor fast code over small code
#	add_compile_options($<$<CONFIG:Release>:/Ob3>)                           # Default inlining
	add_compile_options($<$<CONFIG:Release>:/Ob3>)                           # Aggressive inlining
#	add_compile_options($<$<CONFIG:Release>:/GL>)                            # Whole program optimization
	add_compile_options($<$<CONFIG:Release>:/Gy>)                            # Function-Level Linking
	add_compile_options($<$<CONFIG:Release>:/Qpar>)                          # Auto-parallelization
#	add_compile_options($<$<CONFIG:Release>:/Qvec-report:2>)                 # Vectorization reporting
	add_compile_options($<$<CONFIG:Release>:/DNDEBUG>)                       # Disable debug assertions
	add_compile_options($<$<CONFIG:Release>:/GS->)                           # Disable buffer security checks
	add_compile_options($<$<CONFIG:Release>:/Gw>)                            # Optimize global data
	add_compile_options($<$<CONFIG:Release>:/GT>)                            # Support fiber-safe TLS
	add_compile_options($<$<CONFIG:Release>:/fp:except->)                    # Disable floating point exceptions

	# Linker optimizations for Release
	add_link_options($<$<CONFIG:Release>:/OPT:REF>)                          # Remove unreferenced functions
	add_link_options($<$<CONFIG:Release>:/OPT:ICF>)                          # Identical COMDAT folding
	add_link_options($<$<CONFIG:Release>:/LTCG>)                             # Link Time Code Generation
	add_link_options($<$<CONFIG:Release>:/INCREMENTAL:NO>)                   # Disable incremental linking

	#-----------------------
	# MinSizeRel builds
	#-----------------------

	add_compile_options($<$<CONFIG:MinSizeRel>:/Os>)                         # Optimize for size
	add_compile_options($<$<CONFIG:MinSizeRel>:/DNDEBUG>)                    # Disable debug assertions

	#-----------------------
	# debug builds
	#-----------------------

	add_compile_options($<$<CONFIG:Debug>:/Od>)                              # Disable optimizations
	add_compile_options($<$<CONFIG:Debug>:/MDd>)                             # Debug runtime library
	add_compile_options($<$<CONFIG:Debug>:/Zi>)                              # Debug information
	add_compile_options($<$<CONFIG:Debug>:/RTC1>)                            # Runtime checks

	#-----------------------
	# RelWithDebInfo builds
	#-----------------------

	add_compile_options($<$<CONFIG:RelWithDebInfo>:/O2>)                     # Maximum speed optimization
	add_compile_options($<$<CONFIG:RelWithDebInfo>:/Zi>)                     # Debug information
	add_compile_options($<$<CONFIG:RelWithDebInfo>:/DNDEBUG>)                # Disable debug assertions

	#----------------------------------------------
	# Warning suppressions
	#----------------------------------------------

#	add_compile_options(/wd4061)                                             # switch not handled
	add_compile_options(/wd4100)                                             # unreferenced formal parameter
#	add_compile_options(/wd4189)                                             # local variable is initialized but not referenced
#	add_compile_options(/wd4244)                                             # conversion from 'type1' to 'type2', possible loss of data
#	add_compile_options(/wd4267)                                             # conversion from 'size_t' to 'type', possible loss of data
#	add_compile_options(/wd4365)                                             # conversion signed/unsigned mismatch
#	add_compile_options(/wd4388)                                             # signed/unsigned mismatch
#	add_compile_options(/wd4456)                                             # declaration hides previous local declaration
#	add_compile_options(/wd4505)                                             # unreferenced local function has been removed
	add_compile_options(/wd4514)                                             # unreferenced inline function has been removed
	add_compile_options(/wd4625)                                             # copy constructor was implicitly defined as deleted
	add_compile_options(/wd4626)                                             # assignment operator was implicitly defined as deleted
	add_compile_options(/wd4702)                                             # unreachable code
	add_compile_options(/wd4710)                                             # function not inlined
	add_compile_options(/wd4711)                                             # function 'function' selected for inline expansion
	add_compile_options(/wd4820)                                             # padding
#	add_compile_options(/wd4834)                                             # discarding return value of function with 'nodiscard' attribute
	add_compile_options(/wd4866)                                             # compiler may not enforce left-to-right evaluation order for call to operator_name
	add_compile_options(/wd4868)                                             # compiler may not enforce left-to-right evaluation order in braced initializer list
	add_compile_options(/wd5026)                                             # move constructor was implicitly defined as deleted
	add_compile_options(/wd5027)                                             # move assignment operator was implicitly defined as deleted
#	add_compile_options(/wd5038)                                             # will be initialized after
	add_compile_options(/wd5045)                                             # Qspectre
	add_compile_options(/wd5054)                                             # operator 'operator-name': deprecated between enumerations of different types
#	add_compile_options(/wd5267)                                             # definition of implicit copy constructor/assignment operator is deprecated because it has a user-provided assignment operator/copy constructor
#	add_compile_options(/wd6031)                                             # return value ignored: 'called-function' could return unexpected value
#	add_compile_options(/wd6246)                                             # local declaration of 'variable' hides declaration of same name in outer scope
#	add_compile_options(/wd6326)                                             # potential comparison of a constant with another constant
#	add_compile_options(/wd6387)                                             # this does not adhere to the specification for the function
else()
	# ============================================================================
	# GCC/Clang settings
	# ============================================================================

	#----------------------------------------------
	# Basic
	#----------------------------------------------

	add_compile_options(-fPIC)                                               # Position independent code
	add_compile_options(-Wall)                                               # All warnings
	add_compile_options(-Wextra)                                             # Extra warnings
	add_compile_options(-Wpedantic)                                          # Pedantic warnings
	add_compile_options(-Wconversion)                                        # Conversion warnings
	add_compile_options(-Wsign-conversion)                                   # Sign conversion warnings
	add_compile_options(-Wcast-qual)                                         # Cast quality warnings
	add_compile_options(-Wcast-align)                                        # Cast alignment warnings
	add_compile_options(-Wunused)                                            # Unused variable warnings
	add_compile_options(-Wuninitialized)                                     # Uninitialized variable warnings
	add_compile_options(-Wshadow)                                            # Variable shadowing warnings
	add_compile_options(-Woverloaded-virtual)                                # Overloaded virtual warnings
	add_compile_options(-Wnon-virtual-dtor)                                  # Non-virtual destructor warnings
	add_compile_options(-Wold-style-cast)                                    # Old-style cast warnings
	add_compile_options(-Wformat=2)                                          # Format string warnings
	add_compile_options(-Wformat-security)                                   # Format security warnings
	add_compile_options(-Wno-unknown-pragmas)                                # Ignore MSVC pragmas

	#----------------------------------------------
	# SIMD
	#----------------------------------------------

	add_compile_options(-msse4.2)                                            # SSE4.2 support
	add_compile_options(-mavx)                                               # AVX support
	add_compile_options(-mavx2)                                              # AVX2 support
	add_compile_options(-mfma)                                               # Fused multiply-add

	#----------------------------------------------
	# Optimizations by build configuration
	#----------------------------------------------

	#-----------------------
	# Release builds
	#-----------------------

	add_compile_options($<$<CONFIG:Release>:-O3>)                            # Maximum optimization
	add_compile_options($<$<CONFIG:Release>:-march=native>)                  # Use all available CPU features
	add_compile_options($<$<CONFIG:Release>:-mtune=native>)                  # Tune for current CPU
	add_compile_options($<$<CONFIG:Release>:-ffast-math>)                    # Fast math operations
	add_compile_options($<$<CONFIG:Release>:-funroll-loops>)                 # Unroll loops
	add_compile_options($<$<CONFIG:Release>:-ftree-vectorize>)               # Auto-vectorization
	add_compile_options($<$<CONFIG:Release>:-finline-functions>)             # Aggressive inlining
	add_compile_options($<$<CONFIG:Release>:-fomit-frame-pointer>)           # Remove frame pointers
	add_compile_options($<$<CONFIG:Release>:-flto>)                          # Link-time optimization
	add_compile_options($<$<CONFIG:Release>:-DNDEBUG>)                       # Disable assertions

	# Linker optimizations for Release
	add_link_options($<$<CONFIG:Release>:-flto>)                             # Link-time optimization
	add_link_options($<$<CONFIG:Release>:-s>)                                # Strip symbols
	add_link_options($<$<CONFIG:Release>:-Wl,--gc-sections>)                 # Remove unused sections

	#-----------------------
	# MinSizeRel builds
	#-----------------------

	add_compile_options($<$<CONFIG:MinSizeRel>:-Os>)                         # Optimize for size
	add_compile_options($<$<CONFIG:MinSizeRel>:-flto>)                       # Link-time optimization for size
	add_compile_options($<$<CONFIG:MinSizeRel>:-DNDEBUG>)                    # Disable assertions

	#-----------------------
	# Debug builds
	#-----------------------

	add_compile_options($<$<CONFIG:Debug>:-Og>)                              # Debug-friendly optimization
	add_compile_options($<$<CONFIG:Debug>:-g3>)                              # Maximum debug info
	add_compile_options($<$<CONFIG:Debug>:-fno-omit-frame-pointer>)          # Keep frame pointers
	add_compile_options($<$<CONFIG:Debug>:-fstack-protector-strong>)         # Stack protection
	add_compile_options($<$<CONFIG:Debug>:-rdynamic>)                        # Export symbols for debugging

	#-----------------------
	# RelWithDebInfo builds
	#-----------------------

	add_compile_options($<$<CONFIG:RelWithDebInfo>:-O2>)                     # Moderate optimization
	add_compile_options($<$<CONFIG:RelWithDebInfo>:-g1>)                     # Minimal debug info
	add_compile_options($<$<CONFIG:RelWithDebInfo>:-fno-omit-frame-pointer>) # Keep frames for profiling
	add_compile_options($<$<CONFIG:RelWithDebInfo>:-DNDEBUG>)                # Disable assertions

	#----------------------------------------------
	# Compiler-specific settings
	#----------------------------------------------

	if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
		#-----------------------
		# GCC-specific
		#-----------------------

		add_compile_options(-fdiagnostics-color=always)                      # Colored diagnostics
		add_compile_options(-Wlogical-op)                                    # Logical operation warnings
		add_compile_options(-Wduplicated-cond)                               # Duplicated condition warnings
		add_compile_options(-Wduplicated-branches)                           # Duplicated branch warnings
		add_compile_options(-Wnull-dereference)                              # Null dereference warnings

		# Debug-specific GCC settings
		add_compile_options($<$<CONFIG:Debug>:-ggdb>)                        # GDB-specific debug info
		add_compile_options($<$<CONFIG:Debug>:-D_GLIBCXX_DEBUG>)             # STL debug mode
		add_compile_options($<$<CONFIG:Debug>:-D_GLIBCXX_DEBUG_PEDANTIC>)    # Pedantic STL debug

		# Static linking for GCC
		add_link_options(-static-libgcc)                                     # Static libgcc
		add_link_options(-static-libstdc++)                                  # Static libstdc++

	elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
		#-----------------------
		# Clang-specific
		#-----------------------

		add_compile_options(-fcolor-diagnostics)                             # Colored diagnostics
		add_compile_options(-Wno-pre-c++17-compat)                           # Disable pre-C++17 warnings
		add_compile_options(-Wno-c++98-compat)                               # Disable C++98 compatibility warnings
		add_compile_options(-Wno-c++98-compat-pedantic)                      # Disable pedantic C++98 warnings
		add_compile_options(-Wno-padded)                                     # Disable padding warnings
		add_compile_options(-Wno-exit-time-destructors)                      # Disable exit-time destructor warnings
		add_compile_options(-Wno-global-constructors)                        # Disable global constructor warnings

		# Debug-specific Clang settings
		add_compile_options($<$<CONFIG:Debug>:-glldb>)                       # LLDB-specific debug info
		add_compile_options($<$<CONFIG:Debug>:-fno-optimize-sibling-calls>)  # Better stack traces

		# Clang-specific optimizations
		add_compile_options($<$<CONFIG:Release>:-fvectorize>)                # Enable vectorization
		add_compile_options($<$<CONFIG:Release>:-fslp-vectorize>)            # SLP vectorization
		add_compile_options($<$<CONFIG:Release>:-ffp-contract=fast>)         # Fast FP contraction
		add_compile_options($<$<CONFIG:Release>:-flto=thin>)                 # Thin link-time optimization

		# Thin LTO for Clang
		add_link_options($<$<CONFIG:Release>:-flto=thin>)                    # Thin LTO linking
	endif()
endif()
