option(ENABLE_TEST "Enable Unit Tests" OFF)
if(ENABLE_TEST AND NOT STORMBYTE_AS_DEPENDENCY)
	enable_testing()
	
	add_executable(ConfigTests config_test.cxx)
	target_link_libraries(ConfigTests StormByte::Config)
	add_test(NAME ConfigTests COMMAND ConfigTests)

	add_executable(SerializationTests serialization_test.cxx)
	target_link_libraries(SerializationTests StormByte::Config)
	add_test(NAME SerializationTests COMMAND SerializationTests)
endif()