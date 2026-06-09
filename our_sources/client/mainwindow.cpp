////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// \file       mainwindow.cpp
/// \brief      Проект MockService(клиент)
/// \date       07.06.26 - создан
/// \author     Намазов А.К.
///
#include "mainwindow.h"          /// Заголовок класса главного окна
#include "config_manager.h"      /// Класс конфигурации
#include "counter_manager.h"     /// Класс счётчика
#include "thread_manager.h"      /// Класс запуска потоков Worker
#include <QThread>
#include <QSplitter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QCloseEvent>

/// Конструктор главного окна
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setupUi();
    startWorkers();
}

/// Деструктор главного окна
MainWindow::~MainWindow()
{
    /// Останавливаем потоки
    stopWorkers();
}

/// Создание и настройка пользовательского интерфейса виджетами
void MainWindow::setupUi()
{
    setWindowTitle( "Опросник mock-сервиса" );
    resize( 800, 400 );

    QWidget* central = new QWidget( this );
    setCentralWidget( central );
    QVBoxLayout* mainLayout = new QVBoxLayout( central );

    /// Горизонтальный сплиттер для двух зон
    splitter = new QSplitter( Qt::Horizontal );
    mainLayout->addWidget( splitter );

    /// Зона потока 1
    frame1 = new QFrame();
    frame1->setFrameShape( QFrame::StyledPanel );
    QVBoxLayout* layout1 = new QVBoxLayout( frame1 );
    QLabel* title1 = new QLabel( "Поток 1" );
    title1->setAlignment( Qt::AlignCenter );
    layout1->addWidget( title1 );
    splitter->addWidget( frame1 );

    /// Зона потока 2
    frame2 = new QFrame();
    frame2->setFrameShape( QFrame::StyledPanel );
    QVBoxLayout* layout2 = new QVBoxLayout( frame2 );
    QLabel* title2 = new QLabel( "Поток 2" );
    title2->setAlignment(Qt::AlignCenter );
    layout2->addWidget( title2 );
    splitter->addWidget( frame2 );

    /// Нижняя панель: счётчик и кнопка сброса
    QHBoxLayout* bottomBar = new QHBoxLayout();
    counterLabel = new QLabel();
    resetButton = new QPushButton( "Сбросить счётчик" );
    bottomBar->addWidget( counterLabel );
    bottomBar->addStretch();
    bottomBar->addWidget( resetButton );
    mainLayout->addLayout( bottomBar );

    /// Подключаем сигнал кнопки к слоту
    connect( resetButton, &QPushButton::clicked, this, &MainWindow::onResetCounter );

    /// Начальный цвет зон (серый – нет данных)
    frame1->setStyleSheet( "background-color: gray;" );
    frame2->setStyleSheet( "background-color: gray;" );

    /// Обновляем отображение счётчика (сброс просто обновит текст)
    onResetCounter();
}

/// Запуск двух рабочих потоков
void MainWindow::startWorkers()
{
    ConfigManager config;   /// загружаем конфигурацию

    /// Создаём поток и воркер для потока 1
    thread1 = new QThread( this );
    worker1 = new Worker( 1, config.pollInterval1(), config.timeout1(), config.serverHost(), config.serverPort() );
    worker1->moveToThread( thread1 );   /// перемещаем Worker в поток
    
    /// Соединяем сигналы
    connect( thread1, &QThread::started, worker1, &Worker::start );
    connect( worker1, &Worker::resultReady, this, &MainWindow::onResultReady );
    connect( worker1, &Worker::finished, thread1, &QThread::quit );
    /// Очистка после завершения
    connect( thread1, &QThread::finished, worker1, &Worker::deleteLater );
    connect( thread1, &QThread::finished, thread1, &QThread::deleteLater );

    /// Аналогично для потока 2
    thread2 = new QThread( this );
    worker2 = new Worker( 2, config.pollInterval2(), config.timeout2(), config.serverHost(), config.serverPort() );
    worker2->moveToThread( thread2 );
    connect( thread2, &QThread::started, worker2, &Worker::start );
    connect( worker2, &Worker::resultReady, this, &MainWindow::onResultReady );
    connect( worker2, &Worker::finished, thread2, &QThread::quit );
    connect( thread2, &QThread::finished, worker2, &Worker::deleteLater);
    connect( thread2, &QThread::finished, thread2, &QThread::deleteLater );

    /// Запускаем потоки
    thread1->start();
    thread2->start();
}

/// Остановка потоков
void MainWindow::stopWorkers()
{
    if( worker1 ) worker1->stop();
    if( worker2 ) worker2->stop();
    
    /// Ждём завершения потоков (не более 3 секунд)
    if( thread1 ) { thread1->quit(); thread1->wait( 3000 ); }
    if( thread2 ) { thread2->quit(); thread2->wait( 3000 ); }
}

/// Слот для обработки результата от Worker'а
void MainWindow::onResultReady( int workerId, int result )
{
    /// Выбираем нужную зону по ID
    QFrame* frame = ( workerId == 1 ) ? frame1 : frame2;
    
    /// Определяем цвет в зависимости от результата
    QString color;
    if( result == 1 )
        color = "green";       /// зелёный
    else if( result == 0 )
        color = "red";         /// красный
    else
        color = "yellow";      /// жёлтый для ошибок/таймаутов
    
    /// Устанавливаем цвет фона зоны
    frame->setStyleSheet(QString( "background-color: %1;" ).arg( color ) );
    
    /// Обновляем отображение общего счётчика
    quint64 cnt = CounterManager::instance().value();
    counterLabel->setText( QString( "Всего запросов: %1" ).arg( cnt ) );
}

/// Слот для обработки кнопки сброса счётчика
void MainWindow::onResetCounter()
{
    CounterManager::instance().reset();
    counterLabel->setText( "Всего запросов: 0" );
    QMessageBox::information( this, "Сброс", "Счётчик обнулён" );
}

/// Обработчик закрытия окна для корректной остановки потоков
void MainWindow::closeEvent( QCloseEvent *event )
{
    stopWorkers();
    event->accept();  /// Разрешаем закрытие окна
}
