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
	src/Render/AnimatedSprite2D.h
	src/Render/AnimatedSprite2D.cpp
	src/Render/VertexBuffer.h
	src/Render/VertexBuffer.cpp
	src/Render/ElementBuffer.h
	src/Render/ElementBuffer.cpp
	src/Render/VertexArray.h
	src/Render/VertexArray.cpp
	src/Render/VertexBufferLayout.h
	src/Render/VertexBufferLayout.cpp

	src/Resources/ResourceManager.h
	src/Resources/ResourceManager.cpp
	src/Resources/stb_image.h 

	src/Game/Game.cpp
	src/Game/Game.h
	src/Game/Tank.h
	src/Game/Tank.cpp
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

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
					COMMAND ${CMAKE_COMMAND} -E copy_directory
					${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:${PROJECT_NAME}>/res)
