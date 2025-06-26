#ifndef REGISTRATIONWINDOW_H
#define REGISTRATIONWINDOW_H

#include <QMainWindow>
#include "mainwindow.h"
#include "db_manager.h"

namespace Ui {
class registrationwindow;
}

class registrationwindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit registrationwindow(QWidget *parent = nullptr);
    ~registrationwindow();
    void noDBConnection();

private slots:
    void on_AutorisationButton_clicked();
    void on_RegistrationButton_clicked();
    void performSecondAction();
    void on_radioButton_clicked();

private:
    Ui::registrationwindow *ui;
    db_manager dbManager;
    bool isFirstClick_RegAuth = true;
    MainWindow *mainWindow;
};

#endif // REGISTRATIONWINDOW_H
