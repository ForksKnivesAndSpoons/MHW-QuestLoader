project(QuestLoader)
FILE(GLOB QuestLoaderSources *.cpp)
add_library(QuestLoader SHARED ${QuestLoaderSources})
target_link_libraries(QuestLoader PRIVATE loader minhook)
