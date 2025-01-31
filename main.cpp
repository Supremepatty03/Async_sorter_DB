#include "db_manager.h"
#include "registrationwindow.h"
#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QCoreApplication>
#include <QSqlDatabase>
#include <QtSql>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "untitled1_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    DB_config Default_config;
    db_manager database;
    registrationwindow regWindow;
    regWindow.show();
    //MainWindow w;
    if(!database.set_connection(Default_config)){
        qDebug("соединение не установлено");
        regWindow.noDBConnection();
    }
    //w.show();
    return a.exec();
}

