cmake_minimum_required(VERSION 3.14)

project(png_text_stripper LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(png_text_strip
  main.cpp
)
target_link_libraries(png_text_strip png)

include(GNUInstallDirs)
install(TARGETS png_text_strip
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
install(FILES png_text_strip.desktop DESTINATION ${CMAKE_INSTALL_PREFIX}/share/kio/servicemenus)
