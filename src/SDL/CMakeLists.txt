set(GUI_SOURCES GUI.cpp)

find_package(SDL2 REQUIRED)

add_library(gui ${GUI_SOURCES})
target_link_libraries(gui PRIVATE ${SDL2_LIBRARIES})
target_include_directories(gui PRIVATE ${SDL2_INCLUDE_DIRS})
