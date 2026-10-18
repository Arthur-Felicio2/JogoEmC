cmake_minimum_required(VERSION 3.20)
project(StarlightGames CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES OFF CACHE BOOL "" FORCE)
set(CUSTOMIZE_BUILD OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 5.0
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(raylib)

set(GAME_SOURCES
    src/main.cpp
    src/core/Application.cpp
    src/core/SceneManager.cpp
    src/ui/Theme.cpp
    src/ui/Button.cpp
    src/ui/Label.cpp
    src/ui/Panel.cpp
    src/ui/TextField.cpp
    src/fx/Tween.cpp
    src/fx/ParticleSystem.cpp
    src/games/OddEvenRules.cpp
    src/games/RpsRules.cpp
    src/games/HumanPlayer.cpp
    src/games/CpuPlayer.cpp
    src/games/Match.cpp
    src/render/ScoreBoard.cpp
    src/render/Trophy.cpp
    src/render/HandRenderer.cpp
    src/save/SaveData.cpp
    src/scenes/SplashScene.cpp
    src/scenes/MenuScene.cpp
    src/scenes/ModeScene.cpp
    src/scenes/PassDeviceScene.cpp
    src/scenes/NameEntryScene.cpp
    src/scenes/ScoreScene.cpp
    src/scenes/MatchScene.cpp
    src/scenes/ResultScene.cpp
    src/scenes/FlappyScene.cpp
    src/scenes/CreditsScene.cpp
)

add_executable(StarlightGames ${GAME_SOURCES})
target_include_directories(StarlightGames PRIVATE src)
target_link_libraries(StarlightGames PRIVATE raylib)

add_custom_command(TARGET StarlightGames POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:StarlightGames>/assets
)


set(TEST_SOURCES
    tests/rules_tests.cpp
    src/games/OddEvenRules.cpp
    src/games/RpsRules.cpp
    src/games/HumanPlayer.cpp
    src/games/CpuPlayer.cpp
    src/games/Match.cpp
)

add_executable(rules_tests ${TEST_SOURCES})
target_include_directories(rules_tests PRIVATE src)

add_executable(HandSprites_test tests/HandSprites_test.cpp)
target_include_directories(HandSprites_test PRIVATE src)

add_executable(ScoreStore_test tests/ScoreStore_test.cpp)
target_include_directories(ScoreStore_test PRIVATE src)

add_executable(generate_assets tools/generate_assets.cpp)
target_link_libraries(generate_assets PRIVATE raylib)

