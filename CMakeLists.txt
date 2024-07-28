cmake_minimum_required(VERSION 3.5)
project(QQ LANGUAGES CXX)
set(CMAKE_PREFIX_PATH "C:/Qt/5.15.2/msvc2019_64")

function(add_subdirectories dir)
  file(
    GLOB SUBDIRS
    RELATIVE ${dir}
    ${dir}/*)
  foreach(subdir ${SUBDIRS})
    if(IS_DIRECTORY ${dir}/${subdir})
      include_directories(${dir}/${subdir})
      add_subdirectories(${dir}/${subdir})
    endif()
  endforeach()
endfunction()

add_subdirectory(QQLoginWidget)
add_subdirectory(QQDesktop)
