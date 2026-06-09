////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// \file       mock_server.cpp
/// \brief      Проект MockService(сервер)
/// \date       06.06.26 - создан
/// \author     Намазов А.К.
///
#include "mock_server.h"
#include <QTcpSocket>
#include <QRandomGenerator>
#include <QTimer>
#include <QDebug>
#include <QPointer>

/// Конструктор по умолчанию: сохраняет диапазон задержки
MockServer::MockServer( int minDelayMs, int maxDelayMs, QObject *parent ) : QTcpServer( parent ), m_minDelay( minDelayMs ), m_maxDelay( maxDelayMs )
{
    /// Проверка корректности диапазона
    Q_ASSERT( minDelayMs >= 0 && maxDelayMs >= minDelayMs );
}

/// Запуск прослушивания порта
bool MockServer::start( quint16 port )
{
    /// Начинаем слушать localhost или л на указанном порту
    if( !listen( QHostAddress::Any, port ) ) {
        /// Если ошибка, выводим критическое сообщение
        qCritical() << "Не удалось запустить сервер на порту" << port << errorString();
        return false;   /// возвращаем неудачу
    }
    /// Логируем успешный запуск и диапазон задержек
    qDebug() << "Mock-сервис запущен на порту" << port
             << "Задержка:" << m_minDelay
             << ".." << m_maxDelay << "мс";
    return true;
}

/// Обработка нового входящего соединения
void MockServer::incomingConnection( qintptr socketDescriptor )
{
    /// Создаём объект сокета для этого клиента
    QTcpSocket *socket = new QTcpSocket( nullptr );
    /// Привязываем дескриптор к сокету
    if( !socket->setSocketDescriptor( socketDescriptor ) ) {
        delete socket;   /// если не удалось, удаляем сокет
        return;          /// и выходим
    }
    // Лямбда для обработки данных
    auto onReadyRead = [this, socket]() {
        socket->readAll(); // игнорируем

        int delay = QRandomGenerator::global()->bounded( m_minDelay, m_maxDelay + 1 );
        int result = QRandomGenerator::global()->bounded( 2 );

        // Отключаем обработчик readyRead, чтобы не реагировать на повторные данные
        QObject::disconnect( socket, &QTcpSocket::readyRead, nullptr, nullptr );

        QTimer::singleShot( delay, socket, [socket, result]() {
            if (socket->state() == QAbstractSocket::ConnectedState ) {
                socket->write( QByteArray::number( result ) + "\n" );
                socket->flush();
                socket->disconnectFromHost();
            }
            // Удаляем сокет после завершения
            socket->deleteLater();
        } );
    };

    connect( socket, &QTcpSocket::readyRead, socket, onReadyRead );
    /// При разрыве соединения удаляем сокет
    connect( socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater );
}
