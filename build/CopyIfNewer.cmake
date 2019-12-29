if (${SOURCE_FILE} IS_NEWER_THAN ${DESTINATION_FILE})
    message(STATUS "Copying ${SOURCE_FILE} to ${DESTINATION_FILE} since the former is newer...")
    configure_file(${SOURCE_FILE} ${DESTINATION_FILE} COPYONLY)
endif()
