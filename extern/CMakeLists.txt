include(FetchContent)
include(cmake/add_FetchContent_MakeAvailableNotAll.cmake)
FetchContent_Declare(
        nlohmann-json
        GIT_REPOSITORY https://github.com/ArthurSonzogni/nlohmann_json_cmake_fetchcontent
        GIT_TAG v3.9.1
)
FetchContent_Declare(
        memorymodule
        GIT_REPOSITORY "https://github.com/fancycode/MemoryModule"
        GIT_TAG "5f83e41c3a3e7c6e8284a5c1afa5a38790809461"
)
FetchContent_Declare(
        minhook
        GIT_REPOSITORY "https://github.com/TsudaKageyu/minhook"
        GIT_TAG "3f2e34976c1685ee372a09f54c0c8c8f4240ef90"
)
FetchContent_MakeAvailableNotAll(nlohmann-json)
FetchContent_MakeAvailableNotAll(memorymodule)
FetchContent_MakeAvailableNotAll(minhook)
