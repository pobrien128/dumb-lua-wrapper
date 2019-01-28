cmake_minimum_required(VERSION 3.0.0)
project(LuaWrapper)

include_directories(/home/pobrien/lua/5.3.5/include)
include_directories(/home/pobrien/lua_wrapper/include)

set ( PROJECT_LINK_LIBS liblua.a libdl.so )
link_directories( /home/pobrien/lua/5.3.5/lib )

add_executable(test_wrapper lua_test.cc)
target_link_libraries(test_wrapper ${PROJECT_LINK_LIBS} )
