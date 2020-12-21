project(ClutchRework)
FILE(GLOB ClutchReworkSources *.cpp)
add_library(ClutchRework SHARED ${ClutchReworkSources})
target_link_libraries(ClutchRework PRIVATE loader minhook)
