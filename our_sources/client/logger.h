////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// \file       logger.h
/// \brief      Проект MockService(клиент)
/// \date       07.06.26 - создан
/// \author     Намазов А.К.
///
#ifndef LOGGER_H
#define LOGGER_H

#include <QMutex>
#include <QFile>
#include <QString>

class Logger                                                    ///< Класс Logger для сохранения логирования в файл
{
public:    
    static Logger& instance();                                  ///< Возвращает единственный экземпляр логгера
    void log( const QString& message );                         ///< Логирование сообщения в лог-файл с добавлением текущего времени

private:
    Logger();                                                   ///< Приватный конструктор (синглтон)
    ~Logger();                                                  ///< Деструктор закрывает файл

    Logger(const Logger&) = delete;                             ///< Запрет копирования
    Logger& operator=(const Logger&) = delete;                  ///< Запрет присваивания

    QMutex mutex;                                               ///< Мьютекс для защиты доступа к файлу
    QFile file;                                                 ///< Файл для записи логов
};

#endif // LOGGER_H
