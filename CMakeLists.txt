cmake_minimum_required(VERSION 3.20)
project(c_cpp_learning LANGUAGES C CXX)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(LEARNING_ENABLE_SANITIZERS "Enable address and undefined sanitizers" OFF)
option(LEARNING_ENABLE_CLANG_TIDY "Run clang-tidy during compilation" OFF)
option(LEARNING_ENABLE_GCC_ANALYZER "Enable GCC's static analyzer" OFF)
option(LEARNING_BUILD_POSIX_NETWORKING "Build the POSIX TCP client example" OFF)
option(LEARNING_BUILD_OPENSSL_EXAMPLES "Build examples that use OpenSSL" OFF)

if(LEARNING_BUILD_OPENSSL_EXAMPLES)
    find_package(OpenSSL REQUIRED COMPONENTS Crypto)
endif()

if(LEARNING_ENABLE_SANITIZERS)
    if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang" AND
       CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer)
        add_link_options(-fsanitize=address,undefined)
    else()
        message(FATAL_ERROR "Sanitizers are supported here only with GCC or Clang")
    endif()
endif()

if(LEARNING_ENABLE_CLANG_TIDY)
    find_program(CLANG_TIDY_EXECUTABLE NAMES clang-tidy REQUIRED)
    set(CMAKE_C_CLANG_TIDY "${CLANG_TIDY_EXECUTABLE}")
    set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXECUTABLE}")
endif()

file(GLOB_RECURSE LEARNING_C_SOURCES CONFIGURE_DEPENDS
     "${CMAKE_CURRENT_SOURCE_DIR}/tracks/c/*.c"
     "${CMAKE_CURRENT_SOURCE_DIR}/algorithms/*.c"
     "${CMAKE_CURRENT_SOURCE_DIR}/data-structures/*.c"
     "${CMAKE_CURRENT_SOURCE_DIR}/projects/*.c")

add_library(int_stack STATIC data-structures/linear/int_stack.c)
target_include_directories(int_stack PUBLIC
                           "${CMAKE_CURRENT_SOURCE_DIR}/data-structures/linear")
add_library(int_list STATIC data-structures/linear/int_list.c)
target_include_directories(int_list PUBLIC
                           "${CMAKE_CURRENT_SOURCE_DIR}/data-structures/linear")
add_library(double_list STATIC data-structures/linear/double_list.c)
target_include_directories(double_list PUBLIC
                           "${CMAKE_CURRENT_SOURCE_DIR}/data-structures/linear")
add_library(lista4_lists STATIC data-structures/linear/lista4_lists.c)
target_include_directories(lista4_lists PUBLIC
                           "${CMAKE_CURRENT_SOURCE_DIR}/data-structures/linear")
add_library(char_stack STATIC data-structures/linear/char_stack.c)
target_include_directories(char_stack PUBLIC
                           "${CMAKE_CURRENT_SOURCE_DIR}/data-structures/linear")
add_library(int_min_max STATIC algorithms/statistics/int_min_max.c)
target_include_directories(int_min_max PUBLIC
                           "${CMAKE_CURRENT_SOURCE_DIR}/algorithms/statistics")
add_library(binary_search_tree STATIC data-structures/trees/binary_search_tree.c)
target_include_directories(binary_search_tree PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/data-structures/trees")
add_library(bounded_queue STATIC
    tracks/c/11-systems-programming/concurrency/bounded_queue.c)
target_include_directories(bounded_queue PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/tracks/c/11-systems-programming/concurrency")
find_package(Threads REQUIRED)
target_link_libraries(bounded_queue PUBLIC Threads::Threads)
add_library(calendar_date STATIC tracks/c/06-structures/calendar_date.c)
target_include_directories(calendar_date PUBLIC
                           "${CMAKE_CURRENT_SOURCE_DIR}/tracks/c/06-structures")
add_library(int_vector STATIC
    tracks/c/04-arrays-strings/arrays/int_vector.c)
target_include_directories(int_vector PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/tracks/c/04-arrays-strings/arrays")
add_library(truco STATIC projects/card-game-c/truco.c
                         projects/card-game-c/card_renderer.c)
target_include_directories(truco PUBLIC
                           "${CMAKE_CURRENT_SOURCE_DIR}/projects/card-game-c")
add_library(matrix_operations STATIC
            tracks/c/10-algorithms/linear-algebra/01-matrix-operations.c)
target_compile_definitions(matrix_operations PRIVATE MATRIX_OPERATIONS_NO_MAIN)
target_include_directories(matrix_operations PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/tracks/c/10-algorithms/linear-algebra")
if(MSVC)
    target_compile_options(int_vector PRIVATE /W4)
    target_compile_options(int_list PRIVATE /W4)
    target_compile_options(double_list PRIVATE /W4)
    target_compile_options(lista4_lists PRIVATE /W4)
    target_compile_options(char_stack PRIVATE /W4)
    target_compile_options(int_min_max PRIVATE /W4)
    target_compile_options(calendar_date PRIVATE /W4)
    target_compile_options(truco PRIVATE /W4)
    target_compile_options(matrix_operations PRIVATE /W4)
    target_compile_options(binary_search_tree PRIVATE /W4)
    target_compile_options(bounded_queue PRIVATE /W4)
else()
    target_compile_options(int_vector PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(int_list PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(double_list PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(lista4_lists PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(char_stack PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(int_min_max PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(calendar_date PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(truco PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(matrix_operations PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(binary_search_tree PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(bounded_queue PRIVATE -Wall -Wextra -Wpedantic)
endif()
if(LEARNING_ENABLE_GCC_ANALYZER AND CMAKE_C_COMPILER_ID STREQUAL "GNU")
    target_compile_options(int_vector PRIVATE -fanalyzer)
    target_compile_options(int_list PRIVATE -fanalyzer)
    target_compile_options(double_list PRIVATE -fanalyzer)
    target_compile_options(lista4_lists PRIVATE -fanalyzer)
    target_compile_options(char_stack PRIVATE -fanalyzer)
    target_compile_options(int_min_max PRIVATE -fanalyzer)
    target_compile_options(calendar_date PRIVATE -fanalyzer)
    target_compile_options(truco PRIVATE -fanalyzer)
    target_compile_options(matrix_operations PRIVATE -fanalyzer)
endif()
if(MSVC)
    target_compile_options(int_stack PRIVATE /W4)
else()
    target_compile_options(int_stack PRIVATE -Wall -Wextra -Wpedantic)
endif()
if(LEARNING_ENABLE_GCC_ANALYZER AND CMAKE_C_COMPILER_ID STREQUAL "GNU")
    target_compile_options(int_stack PRIVATE -fanalyzer)
endif()

foreach(source IN LISTS LEARNING_C_SOURCES)
    if(source MATCHES "/data-structures/linear/(int_stack|int_list|char_stack|double_list|lista4_lists).c$" OR
       source MATCHES "/tracks/c/04-arrays-strings/arrays/int_vector.c$" OR
       source MATCHES "/data-structures/trees/binary_search_tree.c$" OR
       source MATCHES "/concurrency/bounded_queue.c$" OR
       source MATCHES "/algorithms/statistics/int_min_max.c$" OR
       source MATCHES "/tracks/c/06-structures/calendar_date.c$" OR
       source MATCHES "/projects/card-game-c/(truco|card_renderer)[.]c$" OR
       source MATCHES "/projects/card-game-c/render-card[.]c$")
        continue()
    endif()
    if(source MATCHES "/networking/")
        if(NOT LEARNING_BUILD_POSIX_NETWORKING OR NOT UNIX)
            continue()
        endif()
    elseif(source MATCHES "/security/")
        if(NOT LEARNING_BUILD_OPENSSL_EXAMPLES)
            continue()
        endif()
    endif()
    if(source MATCHES "/concurrency/" AND NOT UNIX)
        message(STATUS "Skipping POSIX thread example on non-UNIX platform: ${source}")
        continue()
    endif()

    file(RELATIVE_PATH relative_source "${CMAKE_CURRENT_SOURCE_DIR}"
         "${source}")
    string(REPLACE "/" "_" target_name "${relative_source}")
    string(REPLACE ".c" "" target_name "${target_name}")
    add_executable("${target_name}" "${source}")
    if(source MATCHES "/tracks/c/10-algorithms/linear-algebra/02-matrix-menu[.]c$")
        target_link_libraries("${target_name}" PRIVATE matrix_operations)
    endif()
    if(relative_source MATCHES
       "^tracks/c/04-arrays-strings/arrays/02-dynamic-int-vector[.]c$")
        target_link_libraries("${target_name}" PRIVATE int_vector)
    endif()
    set_property(GLOBAL APPEND PROPERTY LEARNING_EXECUTABLES "${target_name}")
    if(MSVC)
        target_compile_options("${target_name}" PRIVATE /W4)
    else()
        target_compile_options("${target_name}" PRIVATE
                               -Wall -Wextra -Wpedantic)
    endif()
    if(LEARNING_ENABLE_GCC_ANALYZER AND CMAKE_C_COMPILER_ID STREQUAL "GNU")
        target_compile_options("${target_name}" PRIVATE -fanalyzer)
    endif()
    if(relative_source MATCHES "concurrency/")
        target_link_libraries("${target_name}" PRIVATE Threads::Threads)
    endif()
    if(relative_source MATCHES "^data-structures/trees/01-btree[.]c$")
        target_link_libraries("${target_name}" PRIVATE binary_search_tree)
    endif()
    if(relative_source MATCHES "/concurrency/05-queue[.]c$")
        target_link_libraries("${target_name}" PRIVATE bounded_queue)
    endif()
    if(relative_source MATCHES "/security/")
        target_link_libraries("${target_name}" PRIVATE OpenSSL::Crypto)
    endif()
    if(relative_source MATCHES "09-data-structures/stack/.*\.c$")
        if(relative_source MATCHES "03-reverse-line|04-palindrome")
            target_link_libraries("${target_name}" PRIVATE char_stack)
        else()
            target_link_libraries("${target_name}" PRIVATE int_stack)
        endif()
    endif()
    if(relative_source MATCHES
       "09-data-structures/linked-list/0[1-6]-.*[.]c$")
        target_link_libraries("${target_name}" PRIVATE int_list)
        target_include_directories("${target_name}" PRIVATE
                                   "${CMAKE_CURRENT_SOURCE_DIR}")
    endif()
    if(relative_source MATCHES
       "09-data-structures/linked-list/(07-concatenate-real|08-interleave-real)[.]c$")
        target_link_libraries("${target_name}" PRIVATE double_list)
    endif()
    if(relative_source MATCHES
       "09-data-structures/linked-list/(09-copy-characters|10-singly-circular|11-remove-doubly-ends|12-circular-doubly)[.]c$")
        target_link_libraries("${target_name}" PRIVATE lista4_lists)
    endif()
    if(relative_source MATCHES
       "projects/card-game-c/(deal-hand|auto-hand|auto-match|play-match)[.]c$")
        target_link_libraries("${target_name}" PRIVATE truco)
    endif()
    if(relative_source MATCHES "tracks/c/03-functions/01-min-max[.]c$")
        target_link_libraries("${target_name}" PRIVATE int_min_max)
    endif()
    if(relative_source MATCHES "tracks/c/06-structures/01-calendar-date[.]c$")
        target_link_libraries("${target_name}" PRIVATE calendar_date)
    endif()
endforeach()

add_executable(render_card projects/card-game-c/render-card.c)
target_link_libraries(render_card PRIVATE truco)
if(MSVC)
    target_compile_options(render_card PRIVATE /W4)
else()
    target_compile_options(render_card PRIVATE -Wall -Wextra -Wpedantic)
endif()
if(LEARNING_ENABLE_GCC_ANALYZER AND CMAKE_C_COMPILER_ID STREQUAL "GNU")
    target_compile_options(render_card PRIVATE -fanalyzer)
endif()

file(GLOB_RECURSE LEARNING_CXX_SOURCES CONFIGURE_DEPENDS
     "${CMAKE_CURRENT_SOURCE_DIR}/tracks/cpp/*.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/projects/*.cpp")

foreach(source IN LISTS LEARNING_CXX_SOURCES)
    file(RELATIVE_PATH relative_source "${CMAKE_CURRENT_SOURCE_DIR}"
         "${source}")
    string(REPLACE "/" "_" target_name "${relative_source}")
    string(REPLACE ".cpp" "" target_name "${target_name}")
    add_executable("${target_name}" "${source}")
    set_property(GLOBAL APPEND PROPERTY LEARNING_EXECUTABLES "${target_name}")
    if(MSVC)
        target_compile_options("${target_name}" PRIVATE /W4)
    else()
        target_compile_options("${target_name}" PRIVATE
                               -Wall -Wextra -Wpedantic)
    endif()
endforeach()

include(CTest)
if(BUILD_TESTING)
    add_executable(test_int_vector tests/unit/test_int_vector.c)
    target_link_libraries(test_int_vector PRIVATE int_vector)
    if(MSVC)
        target_compile_options(test_int_vector PRIVATE /W4 /UNDEBUG)
    else()
        target_compile_options(test_int_vector PRIVATE
                               -Wall -Wextra -Wpedantic -UNDEBUG)
    endif()
    add_test(NAME unit_int_vector COMMAND test_int_vector)

    add_executable(test_binary_search_tree
                   tests/unit/test_binary_search_tree.c)
    target_link_libraries(test_binary_search_tree PRIVATE binary_search_tree)
    add_test(NAME unit_binary_search_tree COMMAND test_binary_search_tree)

    add_executable(test_bounded_queue tests/unit/test_bounded_queue.c)
    target_link_libraries(test_bounded_queue PRIVATE bounded_queue)
    add_test(NAME unit_bounded_queue COMMAND test_bounded_queue)

    add_executable(test_truco tests/unit/test_truco.c)
    target_link_libraries(test_truco PRIVATE truco)
    if(MSVC)
        target_compile_options(test_truco PRIVATE /W4)
    else()
        target_compile_options(test_truco PRIVATE -Wall -Wextra -Wpedantic)
    endif()
    if(LEARNING_ENABLE_GCC_ANALYZER AND CMAKE_C_COMPILER_ID STREQUAL "GNU")
        target_compile_options(test_truco PRIVATE -fanalyzer)
    endif()
    add_test(NAME unit_truco COMMAND test_truco)

    add_executable(test_card_renderer tests/unit/test_card_renderer.c)
    target_link_libraries(test_card_renderer PRIVATE truco)
    if(MSVC)
        target_compile_options(test_card_renderer PRIVATE /W4)
    else()
        target_compile_options(test_card_renderer PRIVATE
                               -Wall -Wextra -Wpedantic)
    endif()
    if(LEARNING_ENABLE_GCC_ANALYZER AND CMAKE_C_COMPILER_ID STREQUAL "GNU")
        target_compile_options(test_card_renderer PRIVATE -fanalyzer)
    endif()
    add_test(NAME unit_card_renderer COMMAND test_card_renderer)

    add_executable(test_int_stack tests/unit/test_int_stack.c)
    target_link_libraries(test_int_stack PRIVATE int_stack)
    if(MSVC)
        target_compile_options(test_int_stack PRIVATE /W4)
    else()
        target_compile_options(test_int_stack PRIVATE -Wall -Wextra -Wpedantic)
    endif()
    if(LEARNING_ENABLE_GCC_ANALYZER AND CMAKE_C_COMPILER_ID STREQUAL "GNU")
        target_compile_options(test_int_stack PRIVATE -fanalyzer)
    endif()
    add_test(NAME unit_int_stack COMMAND test_int_stack)

    add_executable(test_int_list tests/unit/test_int_list.c)
    target_link_libraries(test_int_list PRIVATE int_list)
    target_include_directories(test_int_list PRIVATE
                               "${CMAKE_CURRENT_SOURCE_DIR}")
    if(MSVC)
        target_compile_options(test_int_list PRIVATE /W4)
    else()
        target_compile_options(test_int_list PRIVATE -Wall -Wextra -Wpedantic)
    endif()
    if(LEARNING_ENABLE_GCC_ANALYZER AND CMAKE_C_COMPILER_ID STREQUAL "GNU")
        target_compile_options(test_int_list PRIVATE -fanalyzer)
    endif()
    add_test(NAME unit_int_list COMMAND test_int_list)

    add_executable(test_double_list tests/unit/test_double_list.c)
    target_link_libraries(test_double_list PRIVATE double_list)
    if(MSVC)
        target_compile_options(test_double_list PRIVATE /W4)
    else()
        target_compile_options(test_double_list PRIVATE -Wall -Wextra -Wpedantic)
    endif()
    if(LEARNING_ENABLE_GCC_ANALYZER AND CMAKE_C_COMPILER_ID STREQUAL "GNU")
        target_compile_options(test_double_list PRIVATE -fanalyzer)
    endif()
    add_test(NAME unit_double_list COMMAND test_double_list)

    add_executable(test_lista4_lists tests/unit/test_lista4_lists.c)
    target_link_libraries(test_lista4_lists PRIVATE lista4_lists)
    if(MSVC)
        target_compile_options(test_lista4_lists PRIVATE /W4)
    else()
        target_compile_options(test_lista4_lists PRIVATE -Wall -Wextra -Wpedantic)
    endif()
    if(LEARNING_ENABLE_GCC_ANALYZER AND CMAKE_C_COMPILER_ID STREQUAL "GNU")
        target_compile_options(test_lista4_lists PRIVATE -fanalyzer)
    endif()
    add_test(NAME unit_lista4_lists COMMAND test_lista4_lists)

    add_executable(test_matrix_determinant tests/unit/test_matrix_determinant.c)
    target_link_libraries(test_matrix_determinant PRIVATE matrix_operations)
    if(MSVC)
        target_compile_options(test_matrix_determinant PRIVATE /W4)
    else()
        target_compile_options(test_matrix_determinant PRIVATE
                               -Wall -Wextra -Wpedantic)
    endif()
    if(LEARNING_ENABLE_GCC_ANALYZER AND CMAKE_C_COMPILER_ID STREQUAL "GNU")
        target_compile_options(test_matrix_determinant PRIVATE -fanalyzer)
    endif()
    add_test(NAME unit_matrix_determinant COMMAND test_matrix_determinant)

    add_executable(test_char_stack tests/unit/test_char_stack.c)
    target_link_libraries(test_char_stack PRIVATE char_stack)
    if(MSVC)
        target_compile_options(test_char_stack PRIVATE /W4)
    else()
        target_compile_options(test_char_stack PRIVATE -Wall -Wextra -Wpedantic)
    endif()
    if(LEARNING_ENABLE_GCC_ANALYZER AND CMAKE_C_COMPILER_ID STREQUAL "GNU")
        target_compile_options(test_char_stack PRIVATE -fanalyzer)
    endif()
    add_test(NAME unit_char_stack COMMAND test_char_stack)

    add_executable(test_int_min_max tests/unit/test_int_min_max.c)
    target_link_libraries(test_int_min_max PRIVATE int_min_max)
    if(MSVC)
        target_compile_options(test_int_min_max PRIVATE /W4)
    else()
        target_compile_options(test_int_min_max PRIVATE -Wall -Wextra -Wpedantic)
    endif()
    if(LEARNING_ENABLE_GCC_ANALYZER AND CMAKE_C_COMPILER_ID STREQUAL "GNU")
        target_compile_options(test_int_min_max PRIVATE -fanalyzer)
    endif()
    add_test(NAME unit_int_min_max COMMAND test_int_min_max)

    add_executable(test_calendar_date tests/unit/test_calendar_date.c)
    target_link_libraries(test_calendar_date PRIVATE calendar_date)
    if(MSVC)
        target_compile_options(test_calendar_date PRIVATE /W4)
    else()
        target_compile_options(test_calendar_date PRIVATE
                               -Wall -Wextra -Wpedantic)
    endif()
    if(LEARNING_ENABLE_GCC_ANALYZER AND CMAKE_C_COMPILER_ID STREQUAL "GNU")
        target_compile_options(test_calendar_date PRIVATE -fanalyzer)
    endif()
    add_test(NAME unit_calendar_date COMMAND test_calendar_date)

    find_package(Python3 COMPONENTS Interpreter QUIET)
    if(Python3_Interpreter_FOUND)
        add_test(NAME sorting_benchmark_correctness
                 COMMAND "${Python3_EXECUTABLE}"
                         "${CMAKE_CURRENT_SOURCE_DIR}/tools/benchmarking/compare_sort.py"
                         --count 257 --warmups 0 --repetitions 1)
        add_test(NAME exercise_catalog
                 COMMAND "${Python3_EXECUTABLE}"
                         "${CMAKE_CURRENT_SOURCE_DIR}/tools/validation/catalog.py")
        add_test(NAME exercise_catalog_validation_tests
                 COMMAND "${Python3_EXECUTABLE}"
                         "${CMAKE_CURRENT_SOURCE_DIR}/tools/validation/test_catalog.py")
        add_test(NAME smoke_example_validation_tests
                 COMMAND "${Python3_EXECUTABLE}"
                         "${CMAKE_CURRENT_SOURCE_DIR}/tools/validation/test_smoke_example.py")
        add_test(NAME integration_matrix_determinant_exact
                 COMMAND "${Python3_EXECUTABLE}"
                         "${CMAKE_CURRENT_SOURCE_DIR}/tools/validation/test_matrix_determinant_exact.py"
                         --executable
                         "$<TARGET_FILE:tracks_c_10-algorithms_linear-algebra_02-matrix-menu>")
        set_tests_properties(integration_matrix_determinant_exact
                             PROPERTIES TIMEOUT 15)
        add_test(NAME smoke_card_renderer
                 COMMAND "${Python3_EXECUTABLE}"
                         "${CMAKE_CURRENT_SOURCE_DIR}/tools/validation/smoke_example.py"
                         --source projects/card-game-c/render-card.c
                         --executable "$<TARGET_FILE:render_card>")
        add_test(NAME integration_file_line_count
                 COMMAND "${Python3_EXECUTABLE}"
                         "${CMAKE_CURRENT_SOURCE_DIR}/tools/validation/test_file_line_count.py"
                         --executable
                         "$<TARGET_FILE:tracks_c_07-file-io_line-count_01-count-lines>")
        add_test(NAME integration_cpp_file_line_count
                 COMMAND "${Python3_EXECUTABLE}"
                         "${CMAKE_CURRENT_SOURCE_DIR}/tools/validation/test_file_line_count.py"
                         --executable
                         "$<TARGET_FILE:tracks_cpp_09-modern-cpp_03-count-file-lines>")
        get_property(learning_executables GLOBAL PROPERTY LEARNING_EXECUTABLES)
        foreach(target_name IN LISTS learning_executables)
            get_target_property(target_sources "${target_name}" SOURCES)
            list(GET target_sources 0 target_source)
            file(RELATIVE_PATH relative_source "${CMAKE_CURRENT_SOURCE_DIR}"
                 "${target_source}")
            if(relative_source MATCHES
               "^projects/card-game-c/play-match[.]c$")
                add_test(
                    NAME integration_play_match
                    COMMAND "${Python3_EXECUTABLE}"
                            "${CMAKE_CURRENT_SOURCE_DIR}/tools/validation/test_interactive_match.py"
                            --executable "$<TARGET_FILE:${target_name}>")
                set_tests_properties(integration_play_match
                                     PROPERTIES TIMEOUT 30)
            elseif(relative_source MATCHES
                   "^tracks/c/10-algorithms/linear-algebra/02-matrix-menu[.]c$")
                add_test(
                    NAME integration_matrix_menu
                    COMMAND "${Python3_EXECUTABLE}"
                            "${CMAKE_CURRENT_SOURCE_DIR}/tools/validation/test_matrix_menu.py"
                            --executable "$<TARGET_FILE:${target_name}>")
                set_tests_properties(integration_matrix_menu
                                     PROPERTIES TIMEOUT 15)
            elseif(relative_source MATCHES "/networking/")
                add_test(NAME "integration_${target_name}"
                         COMMAND "${Python3_EXECUTABLE}"
                                 "${CMAKE_CURRENT_SOURCE_DIR}/tools/validation/test_socket_client.py"
                                 --executable "$<TARGET_FILE:${target_name}>")
                set_tests_properties("integration_${target_name}"
                                     PROPERTIES TIMEOUT 15 SKIP_RETURN_CODE 77)
            else()
                add_test(NAME "smoke_${target_name}"
                         COMMAND "${Python3_EXECUTABLE}"
                                 "${CMAKE_CURRENT_SOURCE_DIR}/tools/validation/smoke_example.py"
                                 --source "${relative_source}"
                                 --executable "$<TARGET_FILE:${target_name}>")
                set_tests_properties("smoke_${target_name}"
                                     PROPERTIES SKIP_RETURN_CODE 77)
            endif()
        endforeach()
        set_tests_properties(
            smoke_tracks_c_07-file-io_line-count_01-count-lines
            PROPERTIES WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
        set_tests_properties(
            smoke_tracks_cpp_09-modern-cpp_03-count-file-lines
            PROPERTIES WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
    endif()
endif()
