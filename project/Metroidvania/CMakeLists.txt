# Add hpp and cpp files to Snake variable
file(GLOB Metroidvania
        "*.hpp"
        "*.h"
        "*.cpp"
        )

# Compile cpp files
add_executable(Metroidvania ${Metroidvania})
target_link_libraries(Metroidvania ${all_libs})

file(COPY levels/testlvl.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

#TODO change this when we have a json file 
#file(COPY Metroidvania-art.json Metroidvania-art.png DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
#file(COPY Metroidvania-art.json Metroidvania-art.png DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Debug)
#file(COPY Metroidvania-art.json Metroidvania-art.png DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Release)

IF (WIN32)
    # Copy dll files to ${CMAKE_CURRENT_BINARY_DIR}/Debug
    file(COPY ${DLLFileList} DESTINATION Debug)
    file(COPY ${DLLFileList} DESTINATION Release)

    # Set working directory to ${CMAKE_CURRENT_BINARY_DIR}/Debug
    set_target_properties(Metroidvania PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Debug)
ENDIF(WIN32)