add_library(banking
src/centralBank.cpp
src/localBank.cpp
src/localClient.cpp
src/loan.cpp
src/dice.cpp
src/loggerClass.cpp)


target_include_directories(banking PUBLIC include)