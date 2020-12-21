# Need to set these env vars CC=CL CXX=CL | CC=CL.exe && CXX=CL.exe
cmake_minimum_required(VERSION 3.19)

project(MHW_QuestLoader CXX)
set(CMAKE_CXX_STANDARD 14)
set(CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
        nlohmann-json
        GIT_REPOSITORY "https://github.com/nlohmann/json"
        GIT_TAG "v3.9.1"
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

FetchContent_MakeAvailable(nlohmann-json memorymodule minhook)

add_subdirectory(DllLoader)
add_subdirectory(MHWLoader)
add_subdirectory(Plugins/MonsterLoader)
add_subdirectory(Plugins/QuestLoader)
# add_subdirectory(Plugins/ClutchRework)
