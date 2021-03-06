find_path(SOIL2_INCLUDE_DIR NAMES SOIL2.h PATH_SUFFIXES src/SOIL2)

if(WIN32)
	find_library(SOIL2_LIBRARY soil2 PATH_SUFFIXES lib/windows)
else(WIN32)
	find_library(SOIL2_LIBRARY soil2 PATH_SUFFIXES lib/linux)
endif(WIN32)

if (SOIL2_INCLUDE_DIR AND SOIL2_LIBRARY)
	set (SOIL2_FOUND TRUE)
endif (SOIL2_INCLUDE_DIR AND SOIL2_LIBRARY)
