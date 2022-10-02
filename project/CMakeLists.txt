# Copy project data to output path
file(COPY ../project_data DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Debug)
file(COPY ../project_data DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Release)
file(COPY ../project_data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

IF (WIN32)
	## Copy dll's to output path
	file(COPY ${DLLFileList} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Debug)
	file(COPY ${DLLFileList} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Release)
ENDIF(WIN32)

add_subdirectory(Metroidvania)
add_subdirectory(platformer)