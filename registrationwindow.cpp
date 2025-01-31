#include <QMainWindow>
#include "registrationwindow.h"
#include "./ui_registrationwindow.h"
//include "mainwindow.h"  // Подключаем главное окно приложения

registrationwindow::registrationwindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::registrationwindow),
    dbManager(),
    mainWindow(nullptr)   // Инициализация указателя на главное окно
{
    ui->setupUi(this);  // Настройка UI
    ui->errorLabel->setVisible(false);
    ui->registrationLabel->setVisible(false);
    ui->registrationLine->setVisible(false);
    ui->errorLabel->setStyleSheet("QLabel { color : red; }");

    // Подключаем кнопки к слотам
    connect(ui->AutorisationButton, &QPushButton::clicked, this, &registrationwindow::on_AutorisationButton_clicked);
}

registrationwindow::~registrationwindow()
{
    delete ui;
}

void registrationwindow::noDBConnection()
{
    ui->errorLabel->setVisible(true);
    ui->errorLabel->setText("Нет связи с базой данных!");
}
void registrationwindow::on_radioButton_clicked() {
    if (ui->radioButton->isChecked()) {
        ui->passwordLine->setEchoMode(QLineEdit::Normal); // Показать текст
        ui->registrationLine->setEchoMode(QLineEdit::Normal);
    } else {
        ui->passwordLine->setEchoMode(QLineEdit::Password); // Скрыть текст
        ui->registrationLine->setEchoMode(QLineEdit::Password);
    }
}
void registrationwindow::performSecondAction() {
    QString registrationPassword = ui->registrationLine->text();
    QString loginPassword = ui->passwordLine->text();
    QString username = ui->loginLine->text();
    int userID = -1;
    bool ok;

    if (registrationPassword == loginPassword) {
        QString queryStr1 = ("SELECT user_id FROM users WHERE username = :username");
        QVariant ID_result = dbManager.ExecuteSelectQuery_SingleData(queryStr1, username);
        if (ID_result.isValid()) {
            userID = ID_result.toString().toInt(&ok);
            return;
        }
        switch (dbManager.insertUser(username, loginPassword)) {
        case 0:
            queryStr1 = ("SELECT user_id FROM users WHERE username = :username");
            ID_result = dbManager.ExecuteSelectQuery_SingleData(queryStr1, username);
            userID = ID_result.toString().toInt(&ok);
            this->close();
            mainWindow = new MainWindow(userID);
            mainWindow->showMaximized();      // Закрываем текущее окно
            break;
        case 1:
            ui->errorLabel->setVisible(true);
            ui->errorLabel->setText("Нет соединения с БД!");
            break;
        case 2:
            ui->errorLabel->setVisible(true);
            ui->errorLabel->setText("Не удалось проверить пользователя!");
            break;
        case 3:
            ui->errorLabel->setVisible(true);
            ui->errorLabel->setText("Пользователь с таким именем уже существует!");
            break;
        case 4:
            ui->errorLabel->setVisible(true);
            ui->errorLabel->setText("Ошибка регистрации!");
            break;
        }
    } else {
        ui->errorLabel->setVisible(true);
        ui->errorLabel->setText("Пароли не совпадают!");
    }
}

void registrationwindow::on_AutorisationButton_clicked() {
    int userID;
    bool ok;
    if(isFirstClick_RegAuth) {
        QString loginText = ui->loginLine->text();
        QString passwordText = ui->passwordLine->text();

        if (loginText.isEmpty() || passwordText.isEmpty()) {
            ui->errorLabel->setText("Заполните все поля.");
            ui->errorLabel->setVisible(true);
            return;
        }

        QString queryStr1 = ("SELECT user_id FROM users WHERE username = :username");
        QVariant ID_result = dbManager.ExecuteSelectQuery_SingleData(queryStr1, loginText);
        if (ID_result.isValid()) {
            userID = ID_result.toString().toInt(&ok);
            
            
            QString queryStr2 = QString("SELECT user_password, salt FROM users WHERE username = '%1'").arg(loginText);
            QVector<QVariant> passwordResult = dbManager.ExecuteSelectQuery_UserHashPassword(queryStr2);
            QString stored_hash = passwordResult[0].toString();
            QString stored_salt = passwordResult[1].toString();

            QByteArray received_hash = dbManager.hashPassword(passwordText, stored_salt.toUtf8());

            if(received_hash == stored_hash) {
                // Создаем и показываем основное окно приложения

                mainWindow = new MainWindow(userID);
                mainWindow->showMaximized();

                // Закрываем окно регистрации
                isFirstClick_RegAuth = false;
                this->close();
            } else {
                ui->errorLabel->setVisible(true);
                ui->errorLabel->setText("Пароль неверный!");
            }
        } else {
            ui->errorLabel->setVisible(true);
            ui->errorLabel->setText("Логин не зарегистрирован!");
        }
    } else {
        performSecondAction();
    }
}

void registrationwindow::on_RegistrationButton_clicked() {
    if (isFirstClick_RegAuth) {
        ui->groupBox->setTitle("Регистрация");
        ui->loginLabel->setText("Придумайте логин");
        ui->passwordLabel->setText("Придумайте пароль");
        ui->registrationLine->setVisible(true);
        ui->registrationLabel->setVisible(true);
        ui->RegistrationButton->setText("Авторизация");
        ui->errorLabel->setVisible(false);
        ui->AutorisationButton->setText("Зарегистрироваться");
        isFirstClick_RegAuth = false;
    } else {
        ui->groupBox->setTitle("Авторизация");
        ui->loginLabel->setText("Логин");
        ui->passwordLabel->setText("Пароль");
        ui->registrationLine->setVisible(false);
        ui->registrationLabel->setVisible(false);
        ui->RegistrationButton->setText("Нет аккаунта? Зарегистрироваться");
        ui->errorLabel->setVisible(false);
        ui->AutorisationButton->setText("Авторизироваться");
        isFirstClick_RegAuth = true;
    }
}


