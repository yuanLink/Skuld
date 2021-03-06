function(binary_to_header bin header variable_name)
	message("-- Generating ${header} ...")
	execute_process(COMMAND python ${SKULD_UTILS_DIR}/BinaryToHeader.py ${bin} ${header} ${variable_name}
		RESULT_VARIABLE error)
	if (error)
		message("-- Generating ${header} ... -- Error")
	else()
		message("-- Generating ${header} ... -- Success")
	endif()
endfunction()