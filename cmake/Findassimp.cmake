if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(ASSIMP_ARCHITECTURE "64")
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
	set(ASSIMP_ARCHITECTURE "32")
endif(CMAKE_SIZEOF_VOID_P EQUAL 8)

if(WIN32)
	# Find path of each library
	find_path(assimp_INCLUDE_DIR
		NAMES
			assimp/anim.h
		HINTS
			${assimp_ROOT}/include
	)

	if(MSVC12)
		set(assimp_MSVC_VERSION "vc120")
	elseif(MSVC14 OR MSVC15)	
		set(assimp_MSVC_VERSION "vc140")
	endif(MSVC12)

	if(MSVC12 OR MSVC14 OR MSVC15)
		find_path(assimp_LIBRARY_DIR
			NAMES
				assimp-${assimp_MSVC_VERSION}-mt.lib
				PATH_SUFFIXES lib
		)

		find_library(assimp_LIBRARY_RELEASE assimp-${assimp_MSVC_VERSION}-mt.lib
			PATHS ${assimp_LIBRARY_DIR})
		find_library(assimp_LIBRARY_DEBUG assimp-${assimp_MSVC_VERSION}-mtd.lib
			PATHS ${assimp_LIBRARY_DIR})
		
		set(assimp_LIBRARY 
			optimized 	${assimp_LIBRARY_RELEASE}
			debug		${assimp_LIBRARY_DEBUG}
		)
		
		set(ASSIMP_LIBRARIES "ASSIMP_LIBRARY_RELEASE" "ASSIMP_LIBRARY_DEBUG")
	
		FUNCTION(ASSIMP_COPY_BINARIES TargetDirectory)
			ADD_CUSTOM_TARGET(AssimpCopyBinaries
				COMMAND ${CMAKE_COMMAND} -E copy ${assimp_ROOT}/bin/assimp-${assimp_MSVC_VERSION}-mtd.dll
					${TargetDirectory}/Debug/assimp-${assimp_MSVC_VERSION}-mtd.dll
				COMMAND ${CMAKE_COMMAND} -E copy ${assimp_ROOT}/bin/assimp-${assimp_MSVC_VERSION}-mt.dll
					${TargetDirectory}/Release/assimp-${assimp_MSVC_VERSION}-mt.dll
			COMMENT "Copying Assimp binaries to '${TargetDirectory}'"
			VERBATIM)
		ENDFUNCTION(ASSIMP_COPY_BINARIES)
	
	endif()
	
else(WIN32)
	find_path(
		assimp_INCLUDE_DIR
		NAMES postprocess.h scene.h version.h config.h cimport.h
		PATHS /usr/local/include
		PATH_SUFFIXES assimp
	)

	find_library(
		assimp_LIBRARY
		NAMES assimp
		PATHS /usr/local/lib/
	)

	if (assimp_INCLUDE_DIR AND assimp_LIBRARY)
		set(assimp_FOUND TRUE)
	endif (assimp_INCLUDE_DIR AND assimp_LIBRARY)

	if (assimp_FOUND)
	  if (NOT assimp_FIND_QUIETLY)
		message(STATUS "Found asset importer library: ${assimp_LIBRARIES}")
	  endif (NOT assimp_FIND_QUIETLY)
	else (assimp_FOUND)
	  if (assimp_FIND_REQUIRED)
		message(FATAL_ERROR "Could not find asset importer library")
	  endif (assimp_FIND_REQUIRED)
	endif (assimp_FOUND)
	
endif(WIN32)
