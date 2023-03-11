cmake_minimum_required(VERSION 3.26 FATAL_ERROR)

set(PROJECT_NAME GameOpenGL)

project(${PROJECT_NAME})

add_executable(${PROJECT_NAME}
	src/main.cpp
	src/Render/ShaderProgram.h
	src/Render/ShaderProgram.cpp
	src/Render/Texture2D.h
	src/Render/Texture2D.cpp
	src/Render/Sprite2D.h
	src/Render/Sprite2D.cpp
	src/Render/SpriteAnimator.h
	src/Render/SpriteAnimator.cpp
	src/Render/VertexBuffer.h
	src/Render/VertexBuffer.cpp
	src/Render/ElementBuffer.h
	src/Render/ElementBuffer.cpp
	src/Render/VertexArray.h
	src/Render/VertexArray.cpp
	src/Render/VertexBufferLayout.h
	src/Render/VertexBufferLayout.cpp
	src/Render/Renderer.h
	src/Render/Renderer.cpp

	src/Resources/ResourceManager.h
	src/Resources/ResourceManager.cpp
	src/Resources/stb_image.h 

	src/Game/Game.h
	src/Game/Game.cpp
	src/Game/Level.h
	src/Game/Level.cpp

	src/Game/GameObjects/IGameObject.h
	src/Game/GameObjects/IGameObject.cpp
	src/Game/GameObjects/Tank.h
	src/Game/GameObjects/Tank.cpp
	src/Game/GameObjects/Block.h
	src/Game/GameObjects/Block.cpp
	src/Game/GameObjects/Border.h
	src/Game/GameObjects/Border.cpp
	src/Game/GameObjects/BrickWall.h
	src/Game/GameObjects/BrickWall.cpp
	src/Game/GameObjects/BetonWall.h
	src/Game/GameObjects/BetonWall.cpp
	src/Game/GameObjects/Tree.h
	src/Game/GameObjects/Tree.cpp
	src/Game/GameObjects/Water.h
	src/Game/GameObjects/Water.cpp
	src/Game/GameObjects/Ice.h
	src/Game/GameObjects/Ice.cpp
	src/Game/GameObjects/Eagle.h
	src/Game/GameObjects/Eagle.cpp
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(external/glfw)
target_link_libraries(${PROJECT_NAME} glfw)

add_subdirectory(external/glad)
target_link_libraries(${PROJECT_NAME} glad)

include_directories(external/glm)

include_directories(external/rapidjson)

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
					COMMAND ${CMAKE_COMMAND} -E copy_directory
					${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:${PROJECT_NAME}>/res)
