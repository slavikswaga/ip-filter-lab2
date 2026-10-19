cmake_minimum_required(VERSION 3.14)
project(ip-filter-lab2 VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.14.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(ip-filter-lab2 ip_filter.cpp)

enable_testing()

add_executable(ip_filter_tests
    test_ip_filter.cpp
)
target_link_libraries(ip_filter_tests PRIVATE gtest_main)

include(GoogleTest)
gtest_discover_tests(ip_filter_tests)

set(CPACK_PACKAGE_NAME "ip-filter-lab2")
set(CPACK_PACKAGE_VERSION "0.1.0")
set(CPACK_PACKAGE_VENDOR "slavik")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "vvk160@tpu.ru")
set(CPACK_GENERATOR "DEB")

if(DEFINED ENV{GITHUB_RUN_NUMBER})
    set(CPACK_PACKAGE_VERSION "0.1.0.$ENV{GITHUB_RUN_NUMBER}")
else()
    set(CPACK_PACKAGE_VERSION "0.1.0")
endif()

install(TARGETS ip-filter-lab2 RUNTIME DESTINATION bin)

include(CPack)