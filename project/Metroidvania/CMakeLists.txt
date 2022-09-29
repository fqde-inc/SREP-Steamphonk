# Add hpp and cpp files to Snake variable
file(GLOB Metroidvania
        "*.hpp"
        "*.h"
        "*.cpp"
        )

# Compile cpp files
add_executable(Metroidvania ${Metroidvania})
target_link_libraries(Metroidvania ${all_libs})

IF (WIN32)
    # Copy dll files to ${CMAKE_CURRENT_BINARY_DIR}/Debug
    file(COPY ${DLLFileList} DESTINATION Debug)
    file(COPY ${DLLFileList} DESTINATION Release)

    # Set working directory to ${CMAKE_CURRENT_BINARY_DIR}/Debug
    set_target_properties(Metroidvania PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Debug)
ENDIF(WIN32)
