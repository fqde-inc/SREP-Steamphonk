# Add hpp and cpp files to Snake variable
file(GLOB Platformer
        "*.hpp"
        "*.h"
        "*.cpp"
        )

# Compile cpp files
add_executable(SRE-Platformer ${Platformer})
target_link_libraries(SRE-Platformer ${all_libs})

# copy files to dest
file(COPY levels/testlvl.json platformer-art-deluxe.json platformer-art-deluxe.png DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY levels/testlvl.json platformer-art-deluxe.json platformer-art-deluxe.png DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Debug)
file(COPY levels/testlvl.json platformer-art-deluxe.json platformer-art-deluxe.png DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Release)

#copy files to graphics
file(COPY graphics/dirtsheet.png DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/graphics)
file(COPY graphics/dirtsheet.png DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Debug/graphics)
file(COPY graphics/dirtsheet.png DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Release/graphics)

# copy files to levels
file(COPY levels/testlvl.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/levels)
file(COPY levels/testlvl.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Debug/levels)
file(COPY levels/testlvl.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Release/levels)

IF (WIN32)
    # Copy dll files to ${CMAKE_CURRENT_BINARY_DIR}/Debug
    file(COPY ${DLLFileList} DESTINATION Debug)
    file(COPY ${DLLFileList} DESTINATION Release)

    # Set working directory to ${CMAKE_CURRENT_BINARY_DIR}/Debug
    set_target_properties(SRE-Platformer PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Debug)
ENDIF(WIN32)
