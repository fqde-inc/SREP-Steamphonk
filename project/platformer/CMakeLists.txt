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

file(COPY UI/PixelatedFont.ttf UI/UISprites.json UI/UISprites.png DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY UI/PixelatedFont.ttf UI/UISprites.json UI/UISprites.png DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Debug)
file(COPY UI/PixelatedFont.ttf UI/UISprites.json UI/UISprites.png DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Release)

#copy files to graphics
file(COPY levels/dirtsheet.png DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY levels/dirtsheet.png DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Debug)
file(COPY levels/dirtsheet.png DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Release)

file(COPY Character_Anims/characterAnims.png DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY Character_Anims/characterAnims.png DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Debug)
file(COPY Character_Anims/characterAnims.png DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Release)

# copy level texture packer json
file(COPY levels/dirtsheet.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY levels/dirtsheet.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Debug)
file(COPY levels/dirtsheet.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Release)

# copy character texture packer json
file(COPY Character_Anims/characterAnims.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY Character_Anims/characterAnims.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Debug)
file(COPY Character_Anims/characterAnims.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Release)

# copy files to levels
file(COPY levels/testlvl.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY levels/testlvl.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Debug)
file(COPY levels/testlvl.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Release)

IF (WIN32)
    # Copy dll files to ${CMAKE_CURRENT_BINARY_DIR}/Debug
    file(COPY ${DLLFileList} DESTINATION Debug)
    file(COPY ${DLLFileList} DESTINATION Release)

    # Set working directory to ${CMAKE_CURRENT_BINARY_DIR}/Debug
    set_target_properties(SRE-Platformer PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Debug)
ENDIF(WIN32)
