#AlgoEngine executable
include_directories(${ODB_COMPILE_OUTPUT_DIR})
add_executable(${EXE_FILE_NAME} Main.C)

set_target_properties("${EXE_FILE_NAME}" PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries("${EXE_FILE_NAME}" ${ODB_LIB_NAME}
                                         ${LIBRARY_NAME}
                                         ${IB_API_LIB}
                                         ${Boost_LIBRARIES}
                                         "/usr/local/lib/libodb-boost.so"
                                         "/usr/local/lib/libodb-mysql.so"
                                         "/usr/local/lib/libodb.so")
