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

add_executable(threadlocal_test
			   ThreadLocal_test.cc
			   ../src/base/CurrentThread.cc
			   ../src/base/CountDownLatch.cc
			   ../src/base/Thread.cc)

add_executable(threadlocalsingleton_test
			   ThreadLocalSingleton_test.cc
			   ../src/base/CurrentThread.cc
			   ../src/base/CountDownLatch.cc
			   ../src/base/Thread.cc)

add_executable(singleton_test
			   Singleton_test.cc
			   ../src/base/CurrentThread.cc
			   ../src/base/CountDownLatch.cc
			   ../src/base/Thread.cc)

add_executable(singletonthreadlocal_test
			   SingletonThreadLocal_test.cc
			   ../src/base/CurrentThread.cc
			   ../src/base/CountDownLatch.cc
			   ../src/base/Thread.cc)

add_executable(threadpool_test
			   ThreadPool_test.cc
			   ../src/base/ThreadPool.cc
			   ../src/base/CurrentThread.cc
			   ../src/base/CountDownLatch.cc
			   ../src/base/Thread.cc)

target_link_libraries(thread_test pthread)
target_link_libraries(threadlocal_test pthread)
target_link_libraries(threadlocalsingleton_test pthread)
target_link_libraries(singleton_test pthread)
target_link_libraries(singletonthreadlocal_test pthread)
target_link_libraries(threadpool_test pthread)

include_directories(../src)

