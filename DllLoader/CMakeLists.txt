project(injector)
FILE(GLOB injectorSources *.cpp)
add_library(injector SHARED ${injectorSources})
target_link_libraries(injector PRIVATE memorymodule)
