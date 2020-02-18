FIND_PATH (Gago_INCLUDE_DIR
        NAMES io
        HINTS /usr/include/gago/ /usr/local/include/gago/
        DOC "The Gago include directory" )

FIND_LIBRARY (Gago_LIBRARIES
        NAME  libgago.so libgago.a
        HINTS /usr/lib usr/local/lib
        DOC "The Gago library")