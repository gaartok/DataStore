cmake_minimum_required(VERSION 3.10)

# set the project name
project(DataStoreTest)

# add the executable
add_executable(DataStoreTest main.cpp DataStore.cpp sqlite3.c)
