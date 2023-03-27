add_executable(CaptureTest CaptureTest.c)
target_link_libraries(CaptureTest PUBLIC DpdkTest PcapTest)
target_include_directories(CaptureTest PUBLIC ${INCLUDE_PATH}/CaptureTest)