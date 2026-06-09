////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// \file       thread_manager.cpp
/// \brief      Проект MockService(клиент)
/// \date       07.06.26 - создан
/// \author     Намазов А.К.
///
#include "thread_manager.h"       /// Заголовок класса Менеджер потоков
#include "counter_manager.h"      /// Заголовок класса Счётчика
#include "logger.h"               /// Заголовок класса Логирования
#include <QTcpSocket>
#include <QTimer>
#include <QDebug>

/// Конструктор
Worker::Worker( int id, int pollIntervalMs, int timeoutMs, const QString& serverHost, quint16 serverPort, QObject *parent)
    : QObject(parent), _id( id ), _pollInterval( pollIntervalMs ) , _timeout( timeoutMs ), _serverHost( serverHost )
    , _serverPort( serverPort ), socket( nullptr ) , pollTimer( nullptr ), stopping( false )
{
}

/// Запуск потока: создаём сокет и таймер, запускаем периодические запросы
void Worker::start()
{
    socket = new QTcpSocket( this );
    pollTimer = new QTimer( this );
    pollTimer->setInterval( _pollInterval );
    connect( pollTimer, &QTimer::timeout, this, &Worker::doRequest );
    QMetaObject::invokeMethod( this, &Worker::doRequest, Qt::QueuedConnection );
    pollTimer->start();
}

/// Остановка потока: останавливаем таймер, закрываем сокет, сигнализируем о завершении
void Worker::stop()
{
    stopping = true;
    if( pollTimer ) pollTimer->stop();
    if( socket) socket->disconnectFromHost();
    emit finished();
}

/// Запрос к серверу
void Worker::doRequest()
{
    if( stopping ) return;
    /// Проверяем или устанавливаем соединение с сервером
    if( !ensureConnected() )
    {
        /// Если не удалось подключиться, логируем ошибку
        Logger::instance().log( QString( "Thread %1: Cannot connect to server" ).arg( _id ) );
        emit resultReady( _id, -1 );
        return;
    }

    /// Отправляем запрос
    socket->write( "REQUEST\n" );
    socket->flush();

    /// Ожидаем готовности данных для чтения с таймаутом
    if( socket->waitForReadyRead( _timeout ) ) {
        QByteArray data = socket->readAll().trimmed();   /// Читаем полученный ответ и обрезаем пробелы/переводы
        bool ok;
        int result = data.toInt( &ok );
        if( !ok || ( result != 0 && result != 1 ) ) {
            /// Если некорректный ответ, то логируем
            Logger::instance().log(QString( "Thread %1: Invalid response '%2'" ).arg( _id ).arg( QString( data ) ) );
            emit resultReady( _id, -1 );
            return;
        }
        /// Увеличиваем общий счётчик при успешном ответе
        CounterManager::instance().increment();
        /// Логируем результат
        Logger::instance().log(QString( "Thread %1: result=%2" ).arg( _id ).arg( result ) );
        emit resultReady(_id, result);
    } else {
        /// Превышение таймаута фиксируем в логах
        Logger::instance().log(QString( "Thread %1: timeout after %2 ms" ).arg( _id ).arg( _timeout ) );
        emit resultReady( _id, -1 );
        /// Прерываем текущее соединение
        socket->abort();
    }
}

/// Проверяем соединение с сервером и при необходимости переподключаемся для обеспечения соединения с сервером
bool Worker::ensureConnected()
{
    if ( socket->state() == QAbstractSocket::ConnectedState )
        return true;
    /// Если соединение не устанавливается в данный момент, инициируем подключение
    if ( socket->state() != QAbstractSocket::ConnectingState ) {
         socket->connectToHost( _serverHost, _serverPort );
    }
    /// Записываем в лог соединение
    Logger::instance().log( QString( "Thread %1: trying to connect to %2:%3" ).arg( _id ).arg( _serverHost ).arg( _serverPort ) );

    /// Ожидаем установки соединения не более 2 секунд
    if( socket->waitForConnected( 2000 ) ) {
        /// Подключились успешно
        Logger::instance().log( QString( "Thread %1: connected to server").arg( _id ) );
        return true;
    } else {
        /// Не удалось подключиться, выводим отладочное сообщение
        qDebug() << "Thread" << _id << "connection failed:" << socket->errorString();
        return false;
    }
}
