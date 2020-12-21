project(MonsterLoader)
FILE(GLOB MonsterLoaderSources *.cpp)
add_library(MonsterLoader SHARED ${MonsterLoaderSources})
target_link_libraries(MonsterLoader PRIVATE loader minhook)
