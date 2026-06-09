# В этом подпроекте создается исполняемый файл mock-сервера
#Минимальная требуемая версия CMake
cmake_minimum_required(VERSION 3.16)
#Подсказки CMake
include(CMakePrintHelpers)
# Название проекта и язык (C++)
project(mock_service VERSION 0.1 LANGUAGES CXX)
# Поиск библиотек Qt
find_package(Qt5 5.15 REQUIRED COMPONENTS Core Network)
# Требуем стандарт C++17
set(CMAKE_CXX_STANDARD 17)
# Обязательное использование стандарта
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# Автоматический запуск MOC для Qt-классов с Q_OBJECT
set(CMAKE_AUTOMOC ON)
# Создаем исполняемый файл mock_service (сервер)
add_executable(mock_service
    main.cpp
    mock_server.cpp
)
# Указываем заголовочные файлы
target_sources(mock_service PRIVATE
    mock_server.h
)
# Подключаем библиотеки Qt
target_link_libraries(mock_service Qt5::Core Qt5::Network)
