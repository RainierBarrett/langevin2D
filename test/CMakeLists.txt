file (GLOB_RECURSE TEST_SRC *.cpp *.cxx *.cc *.C *.c *.h *.hpp)
set (TEST_BIN ${PROJECT_NAME}_test)
set (TEST_LIBS ${PROJECT_NAME})

# configure the executable
link_directories(${MAINFOLDER}/lib)
include_directories("${CATCH_DIR}/include")
add_executable(${TEST_BIN} ${TEST_SRC})
target_link_libraries(${TEST_BIN} ${TEST_LIBS})

# configure unit tests via CTest
add_test(NAME AllTests COMMAND ${TEST_BIN})

# run catch tests directly
file(GLOB TXT_FILES "${MAINFOLDER}/*.txt")
file(COPY ${TXT_FILES} DESTINATION "${MAINFOLDER}/build/test" )
file(COPY ${TXT_FILES} DESTINATION "${MAINFOLDER}/bin")
add_custom_target(catch "${MAINFOLDER}/bin/${TEST_BIN}" DEPENDS ${TEST_BIN} COMMENT "Executing unit tests..." VERBATIM SOURCES ${TEST_SRC})
add_custom_target(coverage "${MAINFOLDER}/coverage.sh")

if(EXISTS "${MAINFOLDER}/build/test/output.dat")
  file(COPY "${MAINFOLDER}/build/test/output.dat" DESTINATION "${MAINFOLDER}/build")
endif()