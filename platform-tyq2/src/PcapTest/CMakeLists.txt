add_library(PcapTest SHARED pcapTest.c)
target_include_directories(PcapTest PUBLIC ${INCLUDE_PATH}/PcapTest)
target_link_libraries(PcapTest PUBLIC pthread pcap)