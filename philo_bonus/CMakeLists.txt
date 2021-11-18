cmake_minimum_required(VERSION 3.20)
project(Philo_3_bonus C)

set(CMAKE_C_STANDARD 99)

add_executable(Philo_3_bonus main.c ../includes/philo_bonus.h)
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)
target_link_libraries(Philo_3_bonus PRIVATE Threads::Threads)