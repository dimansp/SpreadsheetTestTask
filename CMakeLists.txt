cmake_minimum_required(VERSION 3.7)

project(task)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/bin)

if(APPLE)
  add_compile_options("-std=c++17" "-Wall")
elseif(APPLE)
  message(WARNING "NOTE! Please add compile_options if needed (default for your platform)")
endif(APPLE)

set(core_code src/app/spreadsheet.cpp
              src/app/spreadsheet_loader.cpp
              src/app/helpers.cpp
              src/app/thread_pool.cpp
              src/app/cell_changer.cpp
              src/app/cells_map.cpp
              src/app/formula/parser.cpp
              src/app/formula/cell_value.cpp
              src/app/formula/cell_proxy.cpp
              src/app/formula/core_elements.cpp)

add_executable(task_app src/app/main.cpp
                        ${core_code})
target_link_libraries(task_app)


include_directories(src
                    3party/gtest/include 
                    3party/gmock/include)
add_executable(tests src/tests/test_main.cpp
                     src/tests/spreadsheet_tests.cpp
                     src/tests/cells_map_tests.cpp
                     src/tests/helpers_tests.cpp
                     ${core_code})

if(APPLE)
target_link_libraries(tests ${CMAKE_SOURCE_DIR}/3party/gtest/libgtest.a
                            ${CMAKE_SOURCE_DIR}/3party/gmock/libgmock.a)
elseif(APPLE)
  message(FATAL_ERROR "There aren't gtest/gmock libs for your platform!")
endif(APPLE)
