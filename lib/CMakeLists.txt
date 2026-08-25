include(GNUInstallDirs)

file(GLOB_RECURSE STORMBYTE_CONFIG_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/*.cxx")

add_library(StormByte-Config SHARED ${STORMBYTE_CONFIG_SOURCES})
add_library(StormByte::Config ALIAS StormByte-Config)

set_target_properties(StormByte-Config PROPERTIES
	LINKER_LANGUAGE CXX
	SOVERSION ${CMAKE_PROJECT_VERSION}
	VERSION ${CMAKE_PROJECT_VERSION}
	POSITION_INDEPENDENT_CODE ON
	CXX_VISIBILITY_PRESET hidden
	VISIBILITY_INLINES_HIDDEN ON
)

if(MSVC)
	target_compile_options(StormByte-Config PRIVATE
		/EHsc
		$<$<CONFIG:Release>:/O2>
		$<$<CONFIG:Debug>:/Od>
	)
	target_compile_definitions(StormByte-Config PUBLIC UNICODE NOMINMAX)
else()
	target_compile_options(StormByte-Config PRIVATE
		-fvisibility=hidden
		$<$<COMPILE_LANGUAGE:CXX>:-fvisibility-inlines-hidden>
		$<$<CONFIG:Debug>:-pipe -g -ggdb -Wall -Wextra -Wnon-virtual-dtor -pedantic -pedantic-errors -O0>
	)
endif()

target_include_directories(StormByte-Config
	SYSTEM BEFORE PUBLIC
		"${CMAKE_CURRENT_LIST_DIR}/public"
		"${CMAKE_CURRENT_LIST_DIR}/private"
)
