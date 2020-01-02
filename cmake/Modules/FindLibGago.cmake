FIND_PATH (Gago_INCLUDE_DIR
        NAMES gago.h
        PATH_SUFFIXES src
        HINTS ${CMAKE_CURRENT_SOURCE_DIR}/../../libgago
        DOC "The Gago include directory"
        )

FIND_LIBRARY (Gago_LIBRARIES
        NAME  libgago.so libgago.a
        HINTS ../../libgago/cmake-build-debug/src ../../libgago/cmake-build-release/src
        DOC "The Gago library"
        )