FIND_PACKAGE(Threads REQUIRED)

add_executable(osLab3 main.cpp CharacterBuffer.cpp)
TARGET_LINK_LIBRARIES(osLab3 Threads::Threads)
