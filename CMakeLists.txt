cmake_minimum_required(VERSION 3.26 FATAL_ERROR)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

set(PROJECT_NAME BatleCity)

project(${PROJECT_NAME} VERSION 1.0 LANGUAGES CXX)

add_executable(${PROJECT_NAME}

	src/main.cpp

	src/BatleCity/Render/ShaderProgram.h
	src/BatleCity/Render/ShaderProgram.cpp
	src/BatleCity/Render/Texture2D.h
	src/BatleCity/Render/Texture2D.cpp
	src/BatleCity/Render/IRenderObjectRect2D.h
	src/BatleCity/Render/IRenderObjectRect2D.cpp
	src/BatleCity/Render/Sprite2D.h
	src/BatleCity/Render/Sprite2D.cpp
	src/BatleCity/Render/SpriteAnimator.h
	src/BatleCity/Render/SpriteAnimator.cpp
	src/BatleCity/Render/VertexBuffer.h
	src/BatleCity/Render/VertexBuffer.cpp
	src/BatleCity/Render/ElementBuffer.h
	src/BatleCity/Render/ElementBuffer.cpp
	src/BatleCity/Render/VertexArray.h
	src/BatleCity/Render/VertexArray.cpp
	src/BatleCity/Render/VertexBufferLayout.h
	src/BatleCity/Render/VertexBufferLayout.cpp
	src/BatleCity/Render/Renderer.h
	src/BatleCity/Render/Renderer.cpp

	src/BatleCity/Physics/PhysicsEngine.h
	src/BatleCity/Physics/PhysicsEngine.cpp
	src/BatleCity/Physics/AxisAlignedBoundingBox.h
	src/BatleCity/Physics/AxisAlignedBoundingBox.cpp

	src/BatleCity/Resources/ResourceManager.h
	src/BatleCity/Resources/ResourceManager.cpp
	src/BatleCity/Resources/stb_image.h 

	src/BatleCity/Game/Game.h
	src/BatleCity/Game/Game.cpp
	
	src/BatleCity/Game/GameStates/Level.h
	src/BatleCity/Game/GameStates/Level.cpp
	src/BatleCity/Game/GameStates/IGameState.h
	src/BatleCity/Game/GameStates/IGameState.cpp
	src/BatleCity/Game/GameStates/StartScreen.h
	src/BatleCity/Game/GameStates/StartScreen.cpp

	src/BatleCity/Game/GameObjectContainers/BulletCollector.h
	src/BatleCity/Game/GameObjectContainers/BulletCollector.cpp

	src/BatleCity/Game/GameObjects/IGameObject.h
	src/BatleCity/Game/GameObjects/IGameObject.cpp
	src/BatleCity/Game/GameObjects/GameObjectCollider.h
	src/BatleCity/Game/GameObjects/GameObjectCollider.cpp
	src/BatleCity/Game/GameObjects/IDynamicGameObject.h
	src/BatleCity/Game/GameObjects/IDynamicGameObject.cpp
	src/BatleCity/Game/GameObjects/Tank.h
	src/BatleCity/Game/GameObjects/Tank.cpp
	src/BatleCity/Game/GameObjects/EnemyTank.h
	src/BatleCity/Game/GameObjects/EnemyTank.cpp
	src/BatleCity/Game/GameObjects/Bullet.h
	src/BatleCity/Game/GameObjects/Bullet.cpp
	src/BatleCity/Game/GameObjects/Block.h
	src/BatleCity/Game/GameObjects/Block.cpp
	src/BatleCity/Game/GameObjects/Border.h
	src/BatleCity/Game/GameObjects/Border.cpp
	src/BatleCity/Game/GameObjects/BrickWall.h
	src/BatleCity/Game/GameObjects/BrickWall.cpp
	src/BatleCity/Game/GameObjects/BetonWall.h
	src/BatleCity/Game/GameObjects/BetonWall.cpp
	src/BatleCity/Game/GameObjects/Tree.h
	src/BatleCity/Game/GameObjects/Tree.cpp
	src/BatleCity/Game/GameObjects/Water.h
	src/BatleCity/Game/GameObjects/Water.cpp
	src/BatleCity/Game/GameObjects/Ice.h
	src/BatleCity/Game/GameObjects/Ice.cpp
	src/BatleCity/Game/GameObjects/Eagle.h
	src/BatleCity/Game/GameObjects/Eagle.cpp

	src/BatleCity/Game/GameAI/EnemyTankAI.h
	src/BatleCity/Game/GameAI/EnemyTankAI.cpp

	src/BatleCity/System/Timer.h
	src/BatleCity/System/Timer.cpp
	src/BatleCity/System/Window.h
	src/BatleCity/System/Window.cpp
)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(external/glfw)
target_link_libraries(${PROJECT_NAME} glfw)

add_subdirectory(external/glad)
target_link_libraries(${PROJECT_NAME} glad)

target_link_libraries(${PROJECT_NAME} sfml-audio)

include_directories(external/glm)

include_directories(external/rapidjson)

target_include_directories(${PROJECT_NAME} PUBLIC src/)

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
					COMMAND ${CMAKE_COMMAND} -E copy_directory
					${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:${PROJECT_NAME}>/res)
