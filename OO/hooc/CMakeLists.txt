cmake_minimum_required (VERSION 2.6)
project (HandyObjectOrientedC)
set (source main.c MyPlayer.c ChildPlayer.c logger.c ConcretePlayer.c Object.c)
add_executable (main ${source})
