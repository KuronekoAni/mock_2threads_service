#В этом подпроекте создается исполняемый файл клиента
#Минимальная требуемая версия CMake
cmake_minimum_required(VERSION 3.16)
# Подсказки CMake
include(CMakePrintHelpers)
# Задание названиия, версии проекта и языка (C++)
project(client VERSION 0.1 LANGUAGES CXX)
# Требуем стандарт C++17
set(CMAKE_CXX_STANDARD 17)
# Обязательное использование стандарта
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# Автоматический запуск MOC для Qt-классов с Q_OBJECT
set(CMAKE_AUTOMOC ON)
# Создаем исполняемый файл client
add_executable(client
    main.cpp
    config_manager.cpp
    counter_manager.cpp
    logger.cpp
    thread_manager.cpp
    mainwindow.cpp
)
# Указываем заголовочные файлы
target_sources(client PRIVATE
    config_manager.h
    counter_manager.h
    logger.h
    thread_manager.h
    mainwindow.h
)
# Подключаем необходимые библиотеки Qt
target_link_libraries(client Qt5::Core Qt5::Network Qt5::Widgets)
# Разрешаем запуск под Windows (без консольного окна)
if(WIN32)
    set_target_properties(client PROPERTIES WIN32_EXECUTABLE TRUE)
endif()
