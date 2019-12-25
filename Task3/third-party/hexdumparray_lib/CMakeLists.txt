add_executable(hexdumparray hexdumparray/hexdumparray.cpp)

function(convertIntoHeader sourceFile headerFile arrayName)
    add_custom_command(
            OUTPUT ${PROJECT_SOURCE_DIR}/${headerFile}
            DEPENDS hexdumparray
            COMMAND hexdumparray ${PROJECT_SOURCE_DIR}/${sourceFile} ${PROJECT_SOURCE_DIR}/${headerFile} ${arrayName}

            DEPENDS ${PROJECT_SOURCE_DIR}/${sourceFile} hexdumparray
    )
endfunction()