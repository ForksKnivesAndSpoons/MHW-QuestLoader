project(injector)
FILE(GLOB injectorSources CONFIGURE_DEPENDS *.cpp)
add_library(injector SHARED ${injectorSources})
target_compile_features(injector PRIVATE cxx_std_17)
target_link_libraries(injector PRIVATE MemoryModule)
