cmake_minimum_required(VERSION 3.20)
project(MyGreatProject)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)




# Указываем исходные файлы
set(SOURCE_FILES  main.cpp)

# Указываем заголовочные файлы
set(HEADER_FILES FixedBlockMemoryResource.hpp Stack.hpp)

# Добавляем исходные файлы к исполняемому файлу
add_executable(my_laba_5 ${SOURCE_FILES} ${HEADER_FILES})

# тесты
enable_testing()

add_executable(
  googltest
  googltest.cpp
  FixedBlockMemoryResource.hpp
  Stack.hpp
)
target_link_libraries(
  googltest
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(googltest)