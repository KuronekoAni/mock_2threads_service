////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// \file       main.cpp
/// \brief      Проект MockService(сервер)
/// \date       06.06.26 - создан
/// \author     Намазов А.К.
///
#include <QCoreApplication>
#include <QCommandLineParser>
#include <qcommandlineoption.h>
#include "mock_server.h"        /// Заголовок класса MockServer

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    app.setApplicationName( "MockService" );
    app.setApplicationVersion( "1.0" );

    /// Парсер командной строки
    QCommandLineParser parser;
    parser.addHelpOption();          /// добавляем ключ --help
    parser.addVersionOption();       /// добавляем ключ --version
    
    /// Определяем пользовательские опции
    parser.addOptions({
        QCommandLineOption( {"p", "port"}, "Порт для прослушивания", "port", "12345" ),   /// порт
        QCommandLineOption( "min", "Минимальная задержка (мс)", "ms", "10" ),             /// мин. задержка
        QCommandLineOption( "max", "Максимальная задержка (мс)", "ms", "10000" )          /// макс. задержка
    });
    parser.process( app );   /// разбираем аргументы

    /// Извлекаем значения из командной строки
    quint16 port = parser.value( "port" ).toUShort();   /// порт
    int minDelay = parser.value( "min" ).toInt();       /// мин. задержка
    int maxDelay = parser.value( "max" ).toInt();       /// макс. задержка

    /// Создаём экземпляр сервера
    MockServer server( minDelay, maxDelay );
    /// Запускаем сервер
    if( !server.start( port ) )
        return 1;   /// если не запустился, выходим с ошибкой

    /// Запускаем цикл обработки событий Qt
    return app.exec();
}
