////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// \file       thread_manager.h
/// \brief      Проект MockService(клиент)
/// \date       07.06.26 - создан
/// \author     Намазов А.К.
///
#ifndef WORKER_H
#define WORKER_H

#include <QObject>

class QTcpSocket;
class QTimer;

class Worker : public QObject                                               ///< Класс Менеджер потоков, где Worker - объект, выполняющий опрос сервиса в отдельном потоке
{
    Q_OBJECT

public:
    explicit Worker( int id, int pollIntervalMs, int timeoutMs,             ///< Конструктор
                    const QString& serverHost, quint16 serverPort,
                    QObject *parent = nullptr );
    void start();                                                           ///< Запуск потока: создаём сокет и таймер, запускаем периодические запросы
    void stop();                                                            ///< Остановка потока: останавливаем таймер, закрываем сокет, сигнализируем о завершении

signals:    
    void resultReady( int workerId, int result );                           ///< Сигнал, испускаемый при получении ответа (или ошибке)
    void finished();                                                        ///< Сигнал о готовности к остановке потока

private slots:
    void doRequest();                                                       ///< Слот, выполняющий один цикл запроса-ответа

private:
    bool ensureConnected();                                                 ///< Проверяем соединение с сервером и при необходимости
                                                                            ///< переподключаемся для обеспечения соединения с сервером

    const int _id;                                                          ///< Bдентификатор рабочего (1 или 2)
    const int _pollInterval;                                                ///< Период опроса (мс)
    const int _timeout;                                                     ///< Таймаут ожидания ответа (мс)
    const QString _serverHost;                                              ///< Хост сервера
    const quint16 _serverPort;                                              ///< Порт сервера
    QTcpSocket* socket;                                                     ///< Указатель на TCP-сокет
    QTimer* pollTimer;                                                      ///< Таймер для периодических запросов
    bool stopping;                                                          ///< Флаг, сигнализирующий о необходимости остановки
};

#endif // WORKER_H
