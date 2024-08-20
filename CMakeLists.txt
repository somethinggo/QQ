cmake_minimum_required(VERSION 3.5)
project(QQ LANGUAGES CXX)

set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_PREFIX_PATH $ENV{Qt_DIR})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 解析依赖配置文件 读取 JSON 文件内容
file(READ ${CMAKE_CURRENT_SOURCE_DIR}/three/config.json JSON_CONTENT)
set(PACKAGES_NAME)
set(PACKAGES_VERSION)
set(PACKAGES_INCLUDEPATH)
set(PACKAGES_LIBPATH)
set(PACKAGES_LIBS)
set(PACKAGES_BINPATH)
set(PACKAGES_RESOURCEDIR)
foreach(PACKAGE_INDEX RANGE 0)
  string(JSON PACKAGE GET ${JSON_CONTENT} packages ${PACKAGE_INDEX})
  if(PACKAGE STREQUAL "")
    continue()
  endif()
  string(JSON PACKAGE_NAME GET ${PACKAGE} packageName)
  if(NOT PACKAGE_NAME STREQUAL "")
    list(APPEND PACKAGES_NAME ${PACKAGE_NAME})
  endif()
  string(JSON PACKAGE_VERSION GET ${PACKAGE} version)
  if(NOT PACKAGE_VERSION STREQUAL "")
    list(APPEND PACKAGES_VERSION ${PACKAGE_VERSION})
  endif()
  string(JSON PACKAGE_INCLUDEPATH GET ${PACKAGE} includePath)
  if(NOT PACKAGE_INCLUDEPATH STREQUAL "")
    list(APPEND PACKAGES_INCLUDEPATH ${PACKAGE_INCLUDEPATH})
  endif()
  string(JSON PACKAGE_LIBPATH GET ${PACKAGE} libraryPath)
  if(NOT PACKAGE_LIBPATH STREQUAL "")
    list(APPEND PACKAGES_LIBPATH ${PACKAGE_LIBPATH})
  endif()
  string(JSON PACKAGE_BINPATH GET ${PACKAGE} binaryPath)
  if(NOT PACKAGE_BINPATH STREQUAL "")
    list(APPEND PACKAGES_BINPATH ${PACKAGE_BINPATH})
  endif()
  string(JSON PACKAGE_RESOURCEDIR GET ${PACKAGE} resourcePath)
  if(NOT PACKAGE_RESOURCEDIR STREQUAL "")
    list(APPEND PACKAGES_RESOURCEDIR ${PACKAGE_RESOURCEDIR})
  endif()
  if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    foreach(PACKAGE_LIBPATH ${PACKAGE_LIBPATH})
      string(JSON PACKAGE_DEBUGEXTENSION GET ${PACKAGE} debugExtension)
      file(GLOB PACKAGE_LIBS_TEMP
           ${PACKAGE_LIBPATH}/*${PACKAGE_DEBUGEXTENSION}.lib)
      if(NOT PACKAGE_LIBS_TEMP STREQUAL "")
        list(APPEND PACKAGES_LIBS ${PACKAGE_LIBS_TEMP})
      endif()
    endforeach()
  else()
    foreach(PACKAGE_LIBPATH ${PACKAGE_LIBPATH})
      string(JSON PACKAGE_RELEASEEXTENSION GET ${PACKAGE} releaseExtension)
      file(GLOB PACKAGE_LIBS_TEMP
           ${PACKAGE_LIBPATH}/*${PACKAGE_RELEASEEXTENSION}.lib)
      if(NOT PACKAGE_LIBS_TEMP STREQUAL "")
        list(APPEND PACKAGES_LIBS ${PACKAGE_LIBS_TEMP})
      endif()
    endforeach()
  endif()
endforeach()

# 设置模块路径
set(MOUDLE_DIRS
    ${CMAKE_SOURCE_DIR}/common
    ${CMAKE_SOURCE_DIR}/audio
    ${CMAKE_SOURCE_DIR}/chat
    ${CMAKE_SOURCE_DIR}/emoji
    ${CMAKE_SOURCE_DIR}/file
    ${CMAKE_SOURCE_DIR}/friend
    ${CMAKE_SOURCE_DIR}/login
    ${CMAKE_SOURCE_DIR}/network
    ${CMAKE_SOURCE_DIR}/setting
    ${CMAKE_SOURCE_DIR}/storage
    ${CMAKE_SOURCE_DIR}/system)

# 查询Qt库
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Core Gui Network
                                                  Multimedia Concurrent)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Core Gui Network
                                                       Multimedia Concurrent)

# 设置内部UI、MOC、RCC文件路径
set(UI_DIR ${CMAKE_SOURCE_DIR}/ui)
file(GLOB UI_FILES ${UI_DIR}/*.ui)
set(MOC_FILES)
foreach(MOC_DIR ${MOUDLE_DIRS})
  file(GLOB MOC_TEMP_FILES ${MOC_DIR}/*.h)
  list(APPEND MOC_FILES ${MOC_TEMP_FILES})
endforeach()
list(APPEND MOC_FILES QQMain.h)
set(RCC_DIR ${CMAKE_SOURCE_DIR}/resource)
file(GLOB RCC_FILES ${RCC_DIR}/*.qrc)
qt_wrap_ui(UI_OUTPUT ${UI_FILES})
qt_wrap_cpp(MOC_OUTPUT ${MOC_FILES})
qt_add_resources(RCC_OUTPUT ${RCC_FILES})

# 设置模块头文件路径
foreach(INCLUDE_DIR ${MOUDLE_DIRS})
  include_directories(${INCLUDE_DIR})
endforeach()
include_directories(${CMAKE_SOURCE_DIR})

# 设置源文件路径
set(SRC_FILES)
foreach(MOUDLE_DIR ${MOUDLE_DIRS})
  file(GLOB MOUDLE_SRC_FILES ${MOUDLE_DIR}/*.cpp)
  list(APPEND SRC_FILES ${MOUDLE_SRC_FILES})
endforeach()
list(APPEND SRC_FILES QQMain.cpp)
list(APPEND SRC_FILES main.cpp)

# 设置外部依赖库
include_directories(${PACKAGES_INCLUDEPATH})
foreach(PACKAGE_RESOURCEDIR ${PACKAGES_RESOURCEDIR})
  file(GLOB PACKAGE_QRCFILES ${PACKAGE_RESOURCEDIR}/*.qrc)
  qt_add_resources(RCC_OUTPUT ${PACKAGE_QRCFILES})
endforeach()
link_directories(${PACKAGES_LIBPATH})

# 设置编译目标
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
# 禁用警告
target_compile_options(${PROJECT_NAME} PRIVATE -w)

# 将外部依赖的二进制文件复制到生成目录--便于调试
foreach(BINS_DIR ${PACKAGES_BINPATH})
  file(GLOB BIN_FILES ${BINS_DIR}/*)
  foreach(BIN_FILE ${BIN_FILES})
    add_custom_command(
      TARGET ${PROJECT_NAME}
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different ${BIN_FILE}
              $<TARGET_FILE_DIR:${PROJECT_NAME}>)
  endforeach()
endforeach()

# 链接 Qt 库
target_link_libraries(
  ${PROJECT_NAME}
  PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Core
          Qt${QT_VERSION_MAJOR}::Gui Qt${QT_VERSION_MAJOR}::Network
          Qt${QT_VERSION_MAJOR}::Multimedia Qt${QT_VERSION_MAJOR}::Concurrent)

# 链接外部依赖库
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  target_link_libraries(${PROJECT_NAME} PRIVATE elawidgettoolsd componentsd)
else()
  target_link_libraries(${PROJECT_NAME} PRIVATE elawidgettools components)
endif()

# 链接包管理器的依赖库
find_package(FFTW3 CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE FFTW3::fftw3)

# 安装
include(GNUInstallDirs)
install(
  TARGETS ${PROJECT_NAME}
  BUNDLE DESTINATION .
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

if(${QT_VERSION_MAJOR} EQUAL 6)
  qt_finalize_executable(${PROJECT_NAME})
endif()
