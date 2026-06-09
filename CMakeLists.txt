#Минимальная требуемая версия CMake
cmake_minimum_required(VERSION 3.16)

# Название проекта и язык (C++)
project(MockService VERSION 0.1 LANGUAGES CXX)

# Если не указан тип сборки, собрать в Release
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# Требуемый стандарт C++17
set(CMAKE_CXX_STANDARD 17)
# Обязательное использование стандарта
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# Автоматический запуск MOC для Qt-классов с Q_OBJECT
set(CMAKE_AUTOMOC ON)
# Автоматическая компиляция ресурсов .qrc
set(CMAKE_AUTORCC ON)
# Автоматическая компиляция .ui файлов
set(CMAKE_AUTOUIC ON)
# Вывод сообщения с определенным типом сборки
message("Build mode: ${CMAKE_BUILD_TYPE}")

#Задание путей для выходных файлов
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/libs/${CMAKE_BUILD_TYPE})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/libs/${CMAKE_BUILD_TYPE})
if(WIN32)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/win/${CMAKE_BUILD_TYPE})
elseif(UNIX AND NOT APPLE)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/linux/${CMAKE_BUILD_TYPE})
elseif(ANDROID)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/android/${CMAKE_BUILD_TYPE})
endif()
# Опции для оптимизации компиляции
add_compile_options(
    -Wall
    $<$<CONFIG:RELEASE>:-O3>
    $<$<CONFIG:DEBUG>:-O0>
)

# Ищем необходимые пакеты Qt5 и его компоненты
find_package(Qt5 REQUIRED COMPONENTS Core Network Widgets)

# Добавляем подпроекты mock_service и client
add_subdirectory(our_sources/mock_service)
add_subdirectory(our_sources/client)
