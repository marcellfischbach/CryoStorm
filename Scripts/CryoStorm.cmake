



function(CS_MOC trgt javaPath)

	set(MOC_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/__cmake__build__moc__/${trgt}")

	target_include_directories(${trgt} PUBLIC "${CMAKE_CURRENT_BINARY_DIR}/__cmake__build__moc__")

	#message("MOC: ${CMAKE_CURRENT_SOURCE_DIR}")
	#message("MOC: ${CMAKE_SOURCE_DIR}")

	set (EXEC_PATH "")
	if (CryoStorm_BINARY_DIR)
		set(EXEC_PATH "${CryoStorm_BINARY_DIR}/bin/")
	endif()
	
	#string(REGEX REPLACE ".*/" "" ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

	string(MAKE_C_IDENTIFIER ${trgt} TARGET_IDENTIFIER)

	# construct java path parameter when parameter javaPath is not empty
	set(CMD_JAVA_PATH "")
	set(CMD_JAVA_PATH_VALUE "")
	#string(COMPARE NOTEQUAL ${javaPath} "" JAVA_PATH_EMPTY)
	if (javaPath)
		set (CMD_JAVA_PATH --javaPath ${javaPath})
	endif()
	#message ("JavaPathCmd: ${CMD_JAVA_PATH}")

	#Get the source files associated with the executable and write it to .../sources.txt
	get_property(SOURCE_FILES TARGET ${trgt} PROPERTY SOURCES)
	set(OUTPUT_FILE "${MOC_DIRECTORY}/sources.txt")
	file(WRITE ${OUTPUT_FILE} "")
	foreach(FILE ${SOURCE_FILES})
		file(APPEND ${OUTPUT_FILE} "${FILE}\n")
    endforeach()

	# Print the source files
	message(STATUS "Source files for ${trgt}: ${SOURCE_FILES}")

	set(TARGET_NAME "${trgt}-MOC")
	add_custom_target(${TARGET_NAME}
                            COMMAND ${EXEC_PATH}csMOC --module ${trgt} --path ${MOC_DIRECTORY} --sourcepath ${CMAKE_CURRENT_SOURCE_DIR} --sourceInput ${MOC_DIRECTORY}/sources.txt --javaConverter ${CryoStorm_SOURCE_DIR}/Scripts\;${CryoStorm_SOURCE_DIR}/data ${CMD_JAVA_PATH}
			WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
			BYPRODUCTS "${MOC_DIRECTORY}/.csCache"
	)
	if (CryoStorm_BINARY_DIR)
		# this is an in-engine build
		add_dependencies(${TARGET_NAME} csMOC)
	endif()
	add_dependencies(${trgt} ${TARGET_NAME})


    target_include_directories(${trgt} BEFORE PUBLIC ${MOC_DIRECTORY})

	file(GLOB_RECURSE GENERATED_SOURCES "${MOC_DIRECTORY}/*.cc")
	message(STATUS "Generated sources: ${GENERATED_SOURCES}")
	
endfunction(CS_MOC)
