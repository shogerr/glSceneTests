add_executable(renderer
	src/app.cpp
	src/scenes/click_bounce.cpp
	src/scenes/ground_scene.cpp
	../../src/sdl/sdl_helper.cpp
	../../src/scenes/instance_test.cpp
	../../src/scenes/test_signal.cpp
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
	PRIVATE ${PROJECT_SOURCE_DIR}/include
	)
set_target_properties(renderer PROPERTIES
	OUTPUT_NAME $<IF:$<BOOL:${STATIC_LINK}>,renderer-static,renderer>
	$<<BOOL:${MSVC}>:MSVC_RUNTIME_LIBRARY MultiThreaded$<$<CONFIG:Debug>:Debug>$<$<NOT:$<BOOL:${STATIC_LINK}>>:DLL>>
	)
target_link_libraries(renderer
	PRIVATE
		glad
		$<IF:$<BOOL:${STATIC_LINK}>,gl00-static,gl00>
		$<IF:$<BOOL:${STATIC_LINK}>,SDL2::SDL2-static,SDL2::SDL2>
		SDL2::SDL2main
		$<$<BOOL:${STATIC_LINK}>:ZLIB::ZLIB>
		$<$<BOOL:${STATIC_LINK}>:${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/$<$<CONFIG:Debug>:debug/>lib/IrrXML$<$<CONFIG:Debug>:d>.lib>
	)

install(
	DIRECTORY
		${PROJECT_SOURCE_DIR}/assets
		${PROJECT_SOURCE_DIR}/src/shaders
	DESTINATION
		bin/$<CONFIG>
	)
install(TARGETS renderer
	RUNTIME DESTINATION bin/$<CONFIG>
	)

