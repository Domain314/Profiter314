cmake_minimum_required(VERSION 3.22)
project(Profiter3)

set(CMAKE_CXX_STANDARD 23)

add_executable(Profiter3 main.cpp Profiter/Profit.cpp Profiter/Profit.hpp Profiter/Holder.cpp Profiter/Holder.hpp Profiter/TimeEngine.cpp Profiter/TimeEngine.hpp Profiter/Misc/Config.hpp Profiter/Misc/Helper.h Profiter/FileOrganizer.cpp Profiter/FileOrganizer.hpp Profiter/Transfer.cpp Profiter/Transfer.hpp)
