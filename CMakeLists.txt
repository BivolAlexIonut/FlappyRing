cmake_minimum_required(VERSION 3.15)

project(FlappyRing CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 4. Linia magică pentru Helix/clangd (să genereze compile_commands.json)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# -----------------------------------------------------------------
#  SECȚIUNEA FETCHCONTENT PENTRU SFML
# -----------------------------------------------------------------

# 5. Include modulul FetchContent
include(FetchContent)

# 6. Declară de unde să ia SFML
FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0 # Folosim cea mai recentă versiune stabilă (2.6.1)
)

# 7. Descarcă și configurează SFML
# Această comandă va crea țintele (targets) SFML::*
FetchContent_MakeAvailable(SFML)

# -----------------------------------------------------------------
#  PROIECTUL TĂU
# -----------------------------------------------------------------

# 8. Definește executabilul tău (ex: din main.cpp)
add_executable(FlappyRing
        main.cpp
        Player.cpp
        Obstacle.cpp
)

# 9. Leagă (link) executabilul tău de bibliotecile SFML
# Folosim țintele pe care FetchContent le-a creat
target_link_libraries(FlappyRing PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
)
# (Poți adăuga SFML::Audio sau SFML::Network dacă ai nevoie de ele)
