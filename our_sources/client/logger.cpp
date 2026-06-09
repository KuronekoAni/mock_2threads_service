////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// \file       logger.cpp
/// \brief      Проект MockService(клиент)
/// \date       07.06.26 - создан
/// \author     Намазов А.К.
///
#include "logger.h"          /// Заголовок класса логирования
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QDebug>

/// Конструктор
Logger::Logger()
{
    file.setFileName( "client_log.txt" );   /// Имя файла логов
    /// Открываем лог-файл на добавление в текстовом режиме
    if( !file.open( QIODevice::Append | QIODevice::Text ) ) {
        /// Если не удалось, выводим предупреждение
        qWarning() << "Не удалось открыть лог-файл:" << file.errorString();
    }
}

/// Деструктор
Logger::~Logger()
{
    /// Закрываем лог-файл (с мьютексом)
    QMutexLocker locker( &mutex );   /// блокируем на время закрытия
    if( file.isOpen() )
        file.close();
}

/// Получаем доступ к синглтону
Logger& Logger::instance()
{
    static Logger logger;
    return logger;
}

/// Логирование сообщения с добавлением текущего времени
void Logger::log( const QString& message )
{
    QMutexLocker locker( &mutex );
    if( !file.isOpen() ) return;     /// Если файл не открыт, ничего не делаем
    QTextStream out( &file );        /// Создаём текстовый поток для файла
    /// Выводим временную метку в формате ISO с миллисекундами и сообщение
    out << QDateTime::currentDateTime().toString( "yyyy-MM-dd hh:mm:ss.zzz" ) << " " << message << "\n";
    out.flush();   /// Сбрасываем буфер записав данные в файл на диске
}
