function(add_merged_library output_library)
	set(output_target "${output_library}")
	string(REGEX REPLACE "-" "_" output_library ${output_library})
	
	set(dummyfile ${CMAKE_CURRENT_BINARY_DIR}/${output_library}_dummy.c)
	file(WRITE ${dummyfile} "const char * dummy = \"${dummyfile}\";")
	
	add_library(${output_target} STATIC ${dummyfile})
endfunction()

function(merge_static_libs output_library)
	add_dependencies(${ARGV})
	set(output_target "${output_library}")
	string(REGEX REPLACE "-" "_" output_library ${output_library})
	set(libs ${ARGV})
	list(REMOVE_AT libs 0)

	foreach(lib ${libs})
		get_target_property(libtype ${lib} TYPE)
		if(NOT libtype STREQUAL "STATIC_LIBRARY")
			message(FATAL_ERROR "Merge_static_libs can only process static libraries")
		endif()
		if(SKULD_MULTICONFIG)
			foreach(CONFIG_TYPE ${CMAKE_CONFIGURATION_TYPES})
				get_target_property("libfile_${CONFIG_TYPE}" ${lib} "LOCATION_${CONFIG_TYPE}")
				list(APPEND libfiles_${CONFIG_TYPE} ${libfile_${CONFIG_TYPE}})
			endforeach()
		else()
			get_target_property(libfile ${lib} LOCATION)
			list(APPEND libfiles "${libfile}")
		endif()
	endforeach()

	if(SKULD_MULTICONFIG)	
		foreach(CONFIG_TYPE ${CMAKE_CONFIGURATION_TYPES})
			list(REMOVE_DUPLICATES libfiles_${CONFIG_TYPE})
			set(libfiles ${libfiles} ${libfiles_${CONFIG_TYPE}})
		endforeach()
	endif()
	list(REMOVE_DUPLICATES libfiles)

  if(MSVC)
	foreach(CONFIG_TYPE ${CMAKE_CONFIGURATION_TYPES})
		set(flags "")
		foreach(lib ${libfiles_${CONFIG_TYPE}})
			set(flags "${flags} ${lib}")
		endforeach()
		string(TOUPPER "STATIC_LIBRARY_FLAGS_${CONFIG_TYPE}" PROPNAME)
		set_target_properties(${output_target} PROPERTIES ${PROPNAME} "${flags}")
	endforeach()
	
  elseif(APPLE)
	if(SKULD_MULTICONFIG)
		message(FATAL_ERROR "Multiple configurations are not supported")
	endif()
	get_target_property(outfile ${output_target} LOCATION)  
	add_custom_command(TARGET ${output_target} POST_BUILD
		COMMAND rm ${outfile}
		COMMAND /usr/bin/libtool -static -o ${outfile} 
		${libfiles}
	)
  else()
	if(multiconfig)
		message(FATAL_ERROR "Multiple configurations are not supported")
	endif()
	get_target_property(outfile ${output_target} LOCATION)

	add_custom_command(TARGET ${output_target} POST_BUILD
			COMMAND python ${SKULD_UTILS_DIR}/MergeStaticLibs/PrepareLibs.py ${CMAKE_AR} ${outfile} ${libfiles}
			COMMAND ${CMAKE_RANLIB} ${outfile})
  endif()
endfunction()