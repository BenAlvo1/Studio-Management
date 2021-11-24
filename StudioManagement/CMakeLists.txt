cmake_minimum_required(VERSION 3.20)
project(StudioManagement)

set(CMAKE_CXX_STANDARD 11)

add_executable(StudioManagement src/main.cpp src/Action.cpp include/Action.h src/Customer.cpp include/Customer.h src/Studio.cpp include/Studio.h src/Trainer.cpp include/Trainer.h src/Workout.cpp include/Workout.h)
