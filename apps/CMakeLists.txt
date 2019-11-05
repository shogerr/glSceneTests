add_executable(renderer
	test_app/app.cpp
	../src/sdl/sdl_helper.cpp
	../src/scenes/instance_test.cpp
	)
add_custom_command(TARGET renderer
	PRE_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	${PROJECT_SOURCE_DIR}/src/shaders $<TARGET_FILE_DIR:renderer>/shaders
	)
add_custom_command(TARGET renderer
	PRE_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	${PROJECT_SOURCE_DIR}/assets $<TARGET_FILE_DIR:renderer>/assets
	)
target_include_directories(renderer
	PRIVATE ${gl00_SOURCE_DIR}/include
	)
if(VCPKG_TARGET_TRIPLET MATCHES x64-windows-static)
	message("Static Triplet")
	set_target_properties(renderer PROPERTIES
		MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
		)
	target_link_libraries(renderer
		PRIVATE
		gl00-static
		assimp::assimp
		OpenGL::GL
		SDL2::SDL2-static
		SDL2::SDL2main
		ZLIB::ZLIB
		${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/$<$<CONFIG:Debug>:debug/>lib/IrrXML$<$<CONFIG:Debug>:d>.lib
		)
elseif(WIN32)
	message("Win32 Build")
	target_link_libraries(renderer
		PRIVATE
		gl00
		assimp::assimp
		OpenGL::GL
		SDL2::SDL2-static
		SDL2::SDL2main
		)
else()
	message("Unix Build")
	target_link_libraries(renderer
		PUBLIC
		gl00
		OpenGL::OpenGL
		SDL2::SDL2
		SDL2::SDL2main
		)
endif()

install(DIRECTORY ${PROJECT_SOURCE_DIR}/assets
	DESTINATION bin/$<IF:$<CONFIG:Debug>,Debug,Release>
	)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/src/shaders
	DESTINATION bin/$<IF:$<CONFIG:Debug>,Debug,Release>
	)
install(TARGETS renderer
	RUNTIME DESTINATION bin/$<IF:$<CONFIG:Debug>,Debug,Release>
	)

