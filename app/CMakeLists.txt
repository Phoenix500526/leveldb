link_directories(${CMAKE_BINARY_DIR})

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(open_db_if_not_exists_then_create open_db_if_not_exists_then_create.cc)
target_include_directories(open_db_if_not_exists_then_create PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(open_db_if_not_exists_then_create PRIVATE Threads::Threads libleveldb.a)

add_executable(example02 example02.cc)
target_include_directories(example02 PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(example02 PRIVATE Threads::Threads libleveldb.a)

add_executable(put_and_get put_and_get.cc)
target_include_directories(put_and_get PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(put_and_get PRIVATE Threads::Threads libleveldb.a)

add_executable(batch_process batch_process.cc)
target_include_directories(batch_process PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(batch_process PRIVATE Threads::Threads libleveldb.a)

add_executable(Iterator_demo Iterator_demo.cc)
target_include_directories(Iterator_demo PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(Iterator_demo PRIVATE Threads::Threads libleveldb.a)

add_executable(write write.cc)
target_include_directories(write PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(write PRIVATE Threads::Threads libleveldb.a)

add_executable(read read.cc)
target_include_directories(read PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(read PRIVATE Threads::Threads libleveldb.a)

add_executable(snapshot snapshot.cc)
target_include_directories(snapshot PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(snapshot PRIVATE Threads::Threads libleveldb.a)

add_executable(memtable_demo memtable_demo.cc)
target_include_directories(memtable_demo PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(memtable_demo PRIVATE Threads::Threads libleveldb.a)
