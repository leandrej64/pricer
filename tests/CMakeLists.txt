# Spécifie la version minimale de CMake requise
cmake_minimum_required(VERSION 3.10)

# Nom du projet
project(PricerMonteCarlo)

# On impose l'utilisation du standard C++17 (très courant et recommandé)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# On demande de créer un exécutable nommé "pricer_test"
# et on liste TOUS les fichiers .cpp à compiler
add_executable(pricer_test main.cpp monte_carlo.cpp)