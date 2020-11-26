


function(SPICE_MOC trgt prefix)

	set(MOC_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/__cmake__build__moc__")

	set (EXEC_PATH "")
	if (SpiceEngine_BINARY_DIR) 
		set(EXEC_PATH "${SpiceEngine_BINARY_DIR}/bin/")
	endif()
	


	string(MAKE_C_IDENTIFIER ${trgt} TARGET_IDENTIFIER)
	add_definitions(-D${TARGET_IDENTIFIER})


	set(TARGET_NAME "${trgt}-MOC")
	add_custom_target(${TARGET_NAME}
                            COMMAND ${EXEC_PATH}spcMOC --prefix ${prefix}  --path ${MOC_DIRECTORY}/${prefix} --export ${TARGET_IDENTIFIER}
			WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	)
	if (SpiceEngine_BINARY_DIR) 
		# this is an in-engine build
		add_dependencies(${TARGET_NAME} spcMOC)
	endif()
	add_dependencies(${trgt} ${TARGET_NAME})


    target_include_directories(${trgt} BEFORE PUBLIC ${MOC_DIRECTORY})

	
endfunction(SPICE_MOC)
