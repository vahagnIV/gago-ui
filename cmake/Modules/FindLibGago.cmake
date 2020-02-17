FIND_PATH (Gago_INCLUDE_DIR
        NAMES gago.h
        PATH_SUFFIXES src
        HINTS /usr/include/gago /usr/local/include/gao
        DOC "The Gago include directory" )

FIND_LIBRARY (Gago_LIBRARIES
        NAME  libgago.so libgago.a
        HINTS /usr/lib usr/local/lib
        DOC "The Gago library")