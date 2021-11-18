cmake_minimum_required(VERSION 3.20)
project(Philosophers C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)
add_executable(Philosophers main.c ../includes/philo.h)
target_link_libraries(Philosophers PRIVATE Threads::Threads)


