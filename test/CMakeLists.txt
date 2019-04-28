add_executable(atomic_test Atomic_test.cc)
add_executable(mutex_test Mutex_test.cc)
add_executable(exception_test
			   Exception_test.cc
			   ../src/base/Exception.cc
			   ../src/base/CurrentThread.cc)
add_executable(timestamp_test
			   Timestamp_test.cc
			   ../src/base/Timestamp.cc)

add_executable(thread_test
			   Thread_test.cc
			   ../src/base/Thread.cc
			   ../src/base/CurrentThread.cc
			   ../src/base/Timestamp.cc
			   ../src/base/Condition.cc
			   ../src/base/CountDownLatch.cc
			   ../src/base/Exception.cc)

target_link_libraries(thread_test pthread)

include_directories(../src)

