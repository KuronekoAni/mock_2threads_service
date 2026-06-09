////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// \file       config_manager.h
/// \brief      Проект MockService(клиент)
/// \date       07.06.26 - создан
/// \author     Намазов А.К.
///
#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include <QString>

class ConfigManager                                                                 ///< Класс для загрузки параметров конфигурации из INI-файла.
{
public:
    explicit ConfigManager( const QString &configFileName = "client_config.ini" );  ///< Конструктор по умолчанию: загружает конфиг из указанного файла
    int pollInterval1() const;                                                      ///< Возвращает период опроса для потока 1 (мс)
    int pollInterval2() const;                                                      ///< Возвращает период опроса для потока 2 (мс)
    int timeout1() const;                                                           ///< Возвращает таймаут ответа для потока 1 (мс)
    int timeout2() const;                                                           ///< Возвращает таймаут ответа для потока 2 (мс)
    QString serverHost() const;                                                     ///< Возвращает хост сервера (например, "localhost")
    quint16 serverPort() const;                                                     ///< Возвращает порт сервера

private:
    void load();                                                                    ///< Загрузка файла конфигурации, если не указать по умолчанию в конструкторе
    int m_pollInterval1;                                                            ///< Период опроса потока 1
    int m_pollInterval2;                                                            ///< Период опроса потока 2
    int m_timeout1;                                                                 ///< Таймаут потока 1
    int m_timeout2;                                                                 ///< Таймаут потока 2
    QString m_serverHost;                                                           ///< Хост сервера
    quint16 m_serverPort;                                                           ///< Порт сервера
};

#endif // CONFIG_MANAGER_H
