find_package(Boost 1.71.0 REQUIRED COMPONENTS unit_test_framework)

add_executable(test_model
	${PROJECT_SOURCE_DIR}/src/model.cpp
	${PROJECT_SOURCE_DIR}/src/mesh.cpp
	test_model.cpp
	)

target_include_directories(test_model
	PRIVATE
		${gl00_SOURCE_DIR}/include)

target_link_libraries(test_model
	PRIVATE
		glad
		gl00-static
		Boost::unit_test_framework
	)

add_executable(test_state_solver
	${PROJECT_SOURCE_DIR}/src/gizmos/state_solver.cpp
	test_state_solver.cpp
	)

target_include_directories(test_state_solver
	PRIVATE
		${gl00_SOURCE_DIR}/include)

target_link_libraries(test_state_solver
	PRIVATE
		glad
		gl00-static
		Boost::unit_test_framework
	)