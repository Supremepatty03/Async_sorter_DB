#ifndef REGISTRATIONWINDOW_H
#define REGISTRATIONWINDOW_H

#include <QMainWindow>
#include "mainwindow.h"
#include "db_manager.h"  // Подключите ваш класс для работы с базой данных

namespace Ui {
class registrationwindow;
}

class registrationwindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit registrationwindow(QWidget *parent = nullptr);
    ~registrationwindow();
    void noDBConnection();   // Метод для отображения ошибки отсутствия соединения с БД

private slots:
    void on_AutorisationButton_clicked();      // Слот для кнопки авторизации
    void on_RegistrationButton_clicked();      // Слот для кнопки регистрации
    void performSecondAction();                // Дополнительная проверка для регистрации
    void on_radioButton_clicked();

private:
    Ui::registrationwindow *ui;
    db_manager dbManager;                       // Ваш менеджер базы данных
    bool isFirstClick_RegAuth = true;          // Флаг для отслеживания режима (авторизация/регистрация)
    MainWindow *mainWindow;                    // Указатель на главное окно приложения               
};

#endif // REGISTRATIONWINDOW_H
