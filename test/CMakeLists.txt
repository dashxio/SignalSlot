add_executable(TestSignal testSignalSlot.cpp)
message(${CMAKE_SOURCE_DIR}/include)
target_include_directories(TestSignal PRIVATE ${CMAKE_SOURCE_DIR}/include)
