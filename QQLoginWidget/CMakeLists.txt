cmake_minimum_required(VERSION 3.5)
project(QQLoginWidget LANGUAGES CXX)

set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(Qt_DIR "C:/Qt/5.15.2/msvc2019_64")
set(CMAKE_PREFIX_PATH Qt_DIR)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG
      ${CMAKE_CURRENT_SOURCE_DIR}/../bin/win)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE
      ${CMAKE_CURRENT_SOURCE_DIR}/../bin/win)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG
      ${CMAKE_CURRENT_SOURCE_DIR}/../bin/linux)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE
      ${CMAKE_CURRENT_SOURCE_DIR}/../bin/linux)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG
      ${CMAKE_CURRENT_SOURCE_DIR}/../bin/mac)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE
      ${CMAKE_CURRENT_SOURCE_DIR}/../bin/mac)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Core Gui Network)

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Core Gui Network)

set(UI_DIR ./resource)
file(GLOB UI_FILES ${UI_DIR}/*.ui)
set(MOC_DIR ./include)
file(GLOB MOC_FILES ${MOC_DIR}/*.h)
set(RCC_DIR ./resource)
file(GLOB RCC_FILES ${RCC_DIR}/*.qrc)

qt_wrap_ui(UI_OUTPUT ${UI_FILES})
qt_wrap_cpp(MOC_OUTPUT ${MOC_FILES})
qt_add_resources(RCC_OUTPUT ${RCC_FILES})

include_directories(./include ../include)
aux_source_directory(./src SRC_FILES)
link_directories(../lib)

file(GLOB OUT_RCC_FILES ../resource/*.qrc)
qt_add_resources(RCC_OUTPUT ${OUT_RCC_FILES})

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
  qt_add_executable(${PROJECT_NAME} MANUAL_FINALIZATION ${SRC_FILES}
                    ${UI_OUTPUT} ${MOC_OUTPUT} ${RCC_OUTPUT})
else()
  if(ANDROID)
    add_library(${PROJECT_NAME} SHARED ${SRC_FILES} ${UI_OUTPUT} ${MOC_OUTPUT}
                                       ${RCC_OUTPUT})
  else()
    add_executable(${PROJECT_NAME} WIN32 ${SRC_FILES} ${UI_OUTPUT}
                                         ${MOC_OUTPUT} ${RCC_OUTPUT})
  endif()
endif()
target_compile_options(${PROJECT_NAME} PRIVATE -w)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  target_link_libraries(${PROJECT_NAME} PRIVATE elawidgettoolsd componentsd)
else()
  target_link_libraries(${PROJECT_NAME} PRIVATE elawidgettools components)
endif()

target_link_libraries(
  ${PROJECT_NAME}
  PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Core
          Qt${QT_VERSION_MAJOR}::Gui Qt${QT_VERSION_MAJOR}::Network)

include(GNUInstallDirs)
install(
  TARGETS ${PROJECT_NAME}
  BUNDLE DESTINATION .
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

if(${QT_VERSION_MAJOR} EQUAL 6)
  qt_finalize_executable(${PROJECT_NAME})
endif()
