////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// \file       mock_server.h
/// \brief      Проект MockService(сервер)
/// \date       06.06.26 - создан
/// \author     Намазов А.К.
///
#ifndef MOCKSERVER_H
#define MOCKSERVER_H

#include <QTcpServer>

class MockServer : public QTcpServer                                                    ///< Класс Mock-сервер, возвращающий 0 или 1 со случайной задержкой
{
    Q_OBJECT                                                                            ///< Макрос для метаобъектной системы Qt

public:
    explicit MockServer( int minDelayMs, int maxDelayMs, QObject *parent = nullptr );   ///< Конструктор по умолчанию: задаём диапазон задержки ответа (мин/макс в мс)
    bool start( quint16 port );                                                         ///< Запуск прослушивания сервера на указанном порту. Возвращаем true при успехе

protected:
    void incomingConnection( qintptr socketDescriptor ) override;                       ///< Обработка нового входящего соединения

private:
    int m_minDelay;                                                                     ///< Минимальная задержка в мс
    int m_maxDelay;                                                                     ///< Максимальная задержка в мс
};

#endif // MOCKSERVER_H
