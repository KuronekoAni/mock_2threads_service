////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// \file       config_manager.cpp
/// \brief      Проект MockService(клиент)
/// \date       07.06.26 - создан
/// \author     Намазов А.К.
#include "config_manager.h"      /// Заголовок класса конфигурации
#include <QSettings>
#include <QDebug>

/// Конструктор по умолчанию
ConfigManager::ConfigManager( const QString &configFileName )
{
    /// Создаём объект QSettings, указывая формат Ini и имя файла
    QSettings settings( configFileName, QSettings::IniFormat );
    
    /// Читаем значения с теми, что по умолчанию
    m_pollInterval1 = settings.value( "Polling/Interval1", 1000 ).toInt();
    m_pollInterval2 = settings.value( "Polling/Interval2", 1500 ).toInt();
    m_timeout1 = settings.value( "Polling/Timeout1", 800 ).toInt();
    m_timeout2 = settings.value( "Polling/Timeout2", 1200 ).toInt();
    m_serverHost = settings.value( "Server/Host", "localhost" ).toString();
    m_serverPort = settings.value( "Server/Port", 12345 ).toUInt();

    /// Отладочный вывод загруженной конфигурации
    qDebug() << "Конфигурация загружена:"
             << "Поток1: период = " << m_pollInterval1 << "таймаут = " << m_timeout1
             << "Поток2: период = " << m_pollInterval2 << "таймаут = " << m_timeout2;
}

/// Реализации методов
int ConfigManager::pollInterval1() const { return m_pollInterval1; }
int ConfigManager::pollInterval2() const { return m_pollInterval2; }
int ConfigManager::timeout1() const { return m_timeout1; }
int ConfigManager::timeout2() const { return m_timeout2; }
QString ConfigManager::serverHost() const { return m_serverHost; }
quint16 ConfigManager::serverPort() const { return m_serverPort; }

/// Загрузка файла конфигурации, если не указать по умолчанию в конструкторе
void ConfigManager::load() {}
