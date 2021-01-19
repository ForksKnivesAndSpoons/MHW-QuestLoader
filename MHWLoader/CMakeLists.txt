project(loader)
FILE(GLOB loaderSources CONFIGURE_DEPENDS *.cpp *.h)
add_library(loader SHARED ${loaderSources})
target_compile_definitions(loader PRIVATE DINPUT8MHW_EXPORTS)
target_include_directories(loader PUBLIC .)
target_compile_features(loader PRIVATE cxx_std_17)
find_package(OpenSSL COMPONENTS Crypto)
target_link_libraries(loader PRIVATE OpenSSL::applink)
target_link_libraries(loader
                      PRIVATE MemoryModule OpenSSL::Crypto
                      PUBLIC nlohmann_json::nlohmann_json minhook
)
