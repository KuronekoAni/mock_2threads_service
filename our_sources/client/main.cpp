////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// \file       main.cpp
/// \brief      Проект MockService(клиент)
/// \date       07.06.26 - создан
/// \author     Намазов А.К.
///
#include <QApplication>
#include "mainwindow.h"    /// Заголовок Главного окна программы

int main( int argc, char *argv[] )
{
    QApplication app( argc, argv );
    app.setApplicationName( "MockPollingClient" );
    app.setOrganizationName( "NAK Company" );

    MainWindow window;
    window.show();

    return app.exec();
}
