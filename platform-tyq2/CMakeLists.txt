cmake_minimum_required(VERSION 3.23)  
project(platform-tyq2 C)

cmake_path(GET platform-tyq2_SOURCE_DIR PARENT_PATH ROOT_PATH)
set(DPDK_PATH ${ROOT_PATH}/dpdk)
set(DPDK_INCLUDE_PATH ${DPDK_PATH}/x86_64-native-linuxapp-gcc/include)
set(DPDK_LIB_PATH ${DPDK_PATH}/x86_64-native-linuxapp-gcc/lib)
set(INCLUDE_PATH ${platform-tyq2_SOURCE_DIR}/include)

add_subdirectory(apps)
add_subdirectory(src)
