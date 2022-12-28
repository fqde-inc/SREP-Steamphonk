# Add hpp and cpp files to Snake variable
file(GLOB Platformer
        "*.hpp"
        "*.h"
        "*.cpp"
        )

# Compile cpp files
add_executable(SRE-Platformer ${Platformer})
target_link_libraries(SRE-Platformer ${all_libs})

# Original graphics
file(COPY Sound/phonkLoop.wav platformer-art-deluxe.json platformer-art-deluxe.png DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY Sound/phonkLoop.wav platformer-art-deluxe.json platformer-art-deluxe.png DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Debug)
file(COPY Sound/phonkLoop.wav platformer-art-deluxe.json platformer-art-deluxe.png DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Release)

file(COPY UI/PixelatedFont.ttf UI/heartEmpty.png UI/heartFull.png DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY UI/PixelatedFont.ttf UI/heartEmpty.png UI/heartFull.png DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Debug)
file(COPY UI/PixelatedFont.ttf UI/heartEmpty.png UI/heartFull.png DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Release)

# Graphics
file(COPY levels/dirtsheet.png levels/dirtsheet.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY levels/dirtsheet.png levels/dirtsheet.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Debug)
file(COPY levels/dirtsheet.png levels/dirtsheet.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Release)

# Level files
file(COPY levels/1.json levels/steamphonk.png levels/steamphonk.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY levels/1.json levels/steamphonk.png levels/steamphonk.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Debug)
file(COPY levels/1.json levels/steamphonk.png levels/steamphonk.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Release)

# Test level
file(COPY levels/testlvl.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY levels/testlvl.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Debug)
file(COPY levels/testlvl.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Release)

# Character animations
file(COPY Character_Anims/characterAnims.json Character_Anims/characterAnims.png DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY Character_Anims/characterAnims.json Character_Anims/characterAnims.png DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Debug)
file(COPY Character_Anims/characterAnims.json Character_Anims/characterAnims.png DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Release)

# Explosions
file(COPY explosion.json explosion.png DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY explosion.json explosion.png DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Debug)
file(COPY explosion.json explosion.png DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Release)


IF (WIN32)
    # Copy dll files to ${CMAKE_CURRENT_BINARY_DIR}/Debug
    file(COPY ${DLLFileList} DESTINATION Debug)
    file(COPY ${DLLFileList} DESTINATION Release)

    # Set working directory to ${CMAKE_CURRENT_BINARY_DIR}/Debug
    set_target_properties(SRE-Platformer PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Debug)
ENDIF(WIN32)
