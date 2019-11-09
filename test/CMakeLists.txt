find_package(SDL2 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

# Test Model
add_executable(test_model
	../src/glad.c
	../src/sdl/sdl_helper.cpp
	../src/model.cpp
	../src/mesh.cpp
	test_model.cpp
	)

target_include_directories(test_model PRIVATE ${gl00_SOURCE_DIR}/include)

target_link_libraries(test_model
	PRIVATE
	gl00-static
	SDL2::SDL2
	SDL2::SDL2main
	)

# Test Resources
add_executable(test_resources
	../src/sdl/sdl_helper.cpp
	../src/glad.c
	test_resources.cpp
	)

target_include_directories(test_resources PRIVATE ${gl00_SOURCE_DIR}/include)

target_link_libraries(test_resources
	gl00-static
	glm
	SDL2::SDL2
	SDL2::SDL2main
	)

# Test Scenes
add_executable(test_scenes
	../src/glad.c
	../src/sdl/sdl_helper.cpp
	test_scenes.cpp
	)

target_include_directories(test_scenes PRIVATE ${gl00_SOURCE_DIR}/include)

target_link_libraries(test_scenes
	gl00-static
	glm
	SDL2::SDL2
	SDL2::SDL2main
	)
	#${SDL2_LIBRARY} ${CMAKE_DL_LIBS})

# Test Scene Instance
add_executable(test_scene_instance
	../src/sdl/sdl_helper.cpp
	test_scene_instance.cpp
	)

target_include_directories(test_scene_instance PRIVATE ${gl00_SOURCE_DIR}/include)

target_link_libraries(test_scene_instance
	gl00-static
	SDL2::SDL2
	SDL2::SDL2main
	)

if (UNIX)
	add_executable(test_scene_linux
		tests/test_scene_instance.cpp
		src/sdl/sdl_helper.cpp
		)
	target_link_libraries(test_scene_linux
		gl00-static
		${assimp_LIBRARY}
		OpenGL::GL
		GLEW::GLEW
		${SDL2_LIBRARY}
		${CMAKE_DL_LIBS}
		)
	add_executable(test_sdl
		tests/test_sdl.cpp
	#	src/sdl/sdl_helper.cpp
		)
	target_link_libraries(test_sdl
		gl00-static
		glm
		OpenGL::GL
		GLEW::GLEW
		${SDL2_LIBRARY} ${CMAKE_DL_LIBS}
		)
	# TODO: This link statement needs to be used instead of the previously
	#	defined WIN32 link.
	target_link_libraries(test_model
		gl00-static
		glm
		OpenGL::GL
		GLEW::GLEW
		${assimp_LIBRARY}
		${SDL2_LIBRARY}
		${CMAKE_DL_LIBS}
		)
endif()
