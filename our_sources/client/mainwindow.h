////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// \file       mainwindow.h
/// \brief      Проект MockService(клиент)
/// \date       07.06.26 - создан
/// \author     Намазов А.К.
///
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QSplitter;
class QFrame;
class QLabel;
class QPushButton;
class QThread;
class Worker;



class MainWindow : public QMainWindow                                                ///< Класс MainWindow - Главное окно приложения: две зоны, счётчик, кнопка сброса
{
    Q_OBJECT

public:

    explicit MainWindow( QWidget *parent = nullptr );                               ///< Конструктор главного окна
    ~MainWindow();                                                                  ///< Деструктор главного окна

protected:    
    void closeEvent( QCloseEvent *event ) override;                                 ///< Обработчик закрытия окна для корректной остановки потоков

private slots:    
    void onResultReady( int workerId, int result );                                 ///< Слот для обработки результата от Worker'а
    void onResetCounter();                                                          ///< Слот для обработки кнопки сброса счётчика

private:    
    void setupUi();                                                                 ///< Создание и настройка пользовательского интерфейса виджетами
    void startWorkers();                                                            ///< Запуск двух рабочих потоков
    void stopWorkers();                                                             ///< Остановка потоков

    QSplitter* splitter;                                                            ///< Разделитель для двух зон
    QFrame* frame1;                                                                 ///< Зона потока 1
    QFrame* frame2;                                                                 ///< Зона потока 2
    QLabel* counterLabel;                                                           ///< Отображение общего счётчика
    QPushButton* resetButton;                                                       ///< Кнопка сброса

    QThread* thread1;                                                               ///< Поток для Worker 1
    QThread* thread2;                                                               ///< Поток для Worker 2
    Worker* worker1;                                                                ///< Объект Worker 1
    Worker* worker2;                                                                ///< Объект Worker 2
};

#endif // MAINWINDOW_H
