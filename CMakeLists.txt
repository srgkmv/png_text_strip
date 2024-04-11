cmake_minimum_required(VERSION 3.14)

project(png_text_stripper LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(png_text_stripper
  main.cpp
)
target_link_libraries(png_text_stripper png)

include(GNUInstallDirs)
install(TARGETS png_text_stripper
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
