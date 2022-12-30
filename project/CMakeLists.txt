# Copy project data to output path
foreach(data_folder ../project_data)
	file(COPY ${data_folder} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Steamphonk)
endforeach()

# Pass the PROJECT_DATA_PATH variable to the C++ preprocessor
add_definitions(-DPRE_PROJECT_DATA_PATH="project_data")

IF (WIN32)
	## Copy dll's to output path
	file(COPY ${DLLFileList} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Debug)
	file(COPY ${DLLFileList} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Release)
ENDIF(WIN32)

add_subdirectory(Steamphonk)