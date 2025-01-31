#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "db_manager.h"
#include <QToolBar>
#include <QElapsedTimer>
#include <QTextEdit>
#include "sort.hpp"
#include <QtTest/QtTest>
#include "C:\Users\User\Documents\tests_untitled1\tst_db_productivity.h"


MainWindow::MainWindow(const int userID, QWidget *parent)
    : QMainWindow(parent), userID(userID)
    , ui(new Ui::MainWindow),
    dbManager()
    {
        ui->setupUi(this);
        ui->stackedWidget->setCurrentIndex(0);
        connect(ui->newArrButton, &QPushButton::clicked, this, &MainWindow::on_newArrButton_clicked);
        connect(ui->toLobby, &QPushButton::clicked, this, &MainWindow::on_toLobby_clicked);
        connect(ui->confirmInput, &QPushButton::clicked, this, &MainWindow::on_confirmInput_clicked);
        this->setStyleSheet("background-color: rgb(255, 255, 255);");        // Настройка UI
        ui->inputLine->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        ui->inputLine->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        ui->newLabel->setVisible(false);
        ui->inputLine->setVisible(false);
        ui->toLobby->setVisible(false);
        ui->confirmInput->setVisible(false);
        ui->sortTextEdit->setVisible(false);
        ui->sortLabel->setVisible(false);
        ui->errorLabel2->setVisible(false);
        ui->saveButton->setVisible(false);
        ui->saveLabel->setVisible(false);
        ui->saveButton->setVisible(false);
        ui->saveLabel->setVisible(false);
        ui->arrayLabel->setVisible(false);
        ui->changeLabel->setVisible(false);
        ui->changeEdit->setVisible(false);
        ui->outputLabel->setVisible(false);
        ui->changeAccButton->setVisible(false);
        ui->textEdit22->setVisible(false);
    }
    void MainWindow::on_newArrButton_clicked() {
        ui->stackedWidget->setCurrentIndex(0);
        ui->saveButton->setVisible(false);
        ui->saveLabel->setVisible(false);
        ui->saveButton->setVisible(false);
        ui->saveLabel->setVisible(false);
        ui->arrayLabel->setVisible(false);
        ui->changeLabel->setVisible(false);
        ui->changeEdit->setVisible(false);
        ui->outputLabel->setVisible(false);
        ui->changeAccButton->setVisible(false);
        ui->newLabel->setVisible(true);
        ui->inputLine->setVisible(true);
        ui->toLobby->setVisible(true);
        ui->confirmInput->setVisible(true);
        //ui->newArrButton->setVisible(false);
        //ui->viewButton->setVisible(false);
        //ui->testButton->setVisible(false);
    }
    void MainWindow::on_toLobby_clicked() {
        ui->newArrButton->setVisible(true);
        ui->viewButton->setVisible(true);
        ui->testButton->setVisible(true);
        ui->newLabel->setVisible(false);
        ui->inputLine->setVisible(false);
        ui->inputLine->clear();
        ui->toLobby->setVisible(false);
        ui->confirmInput->setVisible(false);
        ui->sortTextEdit->setVisible(false);
        ui->sortLabel->setVisible(false);
        ui->sortTextEdit->clear();
        ui->errorLabel2->setVisible(false);
        ui->saveButton->setVisible(false);
        ui->saveLabel->setVisible(false);
        ui->saveButton->setVisible(false);
        ui->saveLabel->setVisible(false);
        ui->saveButton->setVisible(false);
        ui->saveLabel->setVisible(false);
        ui->arrayLabel->setVisible(false);
        ui->changeLabel->setVisible(false);
        ui->changeEdit->setVisible(false);
        ui->outputLabel->setVisible(false);
        ui->changeAccButton->setVisible(false);
    }
    void MainWindow::on_confirmInput_clicked() {
        ui->errorLabel2->setVisible(false);
        ui->saveLabel->setVisible(false);
        QString inputLine = ui->inputLine->toPlainText();
        QVector<QString> arr = dbManager.convertToVec(inputLine);
        if (arr.size()!=0)
        {
            ui->sortTextEdit->setVisible(true);
            ui->sortLabel->setVisible(true);
            std::vector<std::string> stdVec;
            for (const QString& str : arr) {
                stdVec.push_back(str.toStdString()); // Преобразование для my_sort_lib (шаблонный вектор)
            }
            std::vector<std::string> res;

            res = my_sort_lib::shaker_sort(stdVec);
            res = my_sort_lib::number_sort(res);

            QStringList sortedList;
            for (const std::string& str : res) {
                sortedList << QString::fromStdString(str);
            }

            // Объединение в одну строку и вывод в TextEdit
            QString output = sortedList.join(" ");
            ui->sortTextEdit->setPlainText(output);
            ui->saveButton->setVisible(true);

        }
        else{
            ui->errorLabel2->setVisible(true);
        }

    }
    void MainWindow::on_saveButton_clicked(){
        int result = dbManager.insertArray(userID, ui->inputLine->toPlainText(), ui->sortTextEdit->toPlainText());
        if (result == 1){
            ui->saveLabel->setVisible(true);
            ui->saveLabel->setStyleSheet("QLabel { color: red; }");
            ui->saveLabel->setText("Произошла ошибка, попробуйте снова!");
        }
        if (result == 0){
            ui->saveLabel->setVisible(true);
            ui->saveLabel->setStyleSheet("QLabel { color: green; }");
            ui->saveLabel->setText("Сохранение прошло успешно!");
        }
    }
    void MainWindow::on_viewButton_clicked(){
        ui->saveButton->setVisible(false);
        ui->saveLabel->setVisible(false);
        ui->saveButton->setVisible(false);
        ui->saveLabel->setVisible(false);
        ui->arrayLabel->setVisible(false);
        ui->changeLabel->setVisible(false);
        ui->changeEdit->setVisible(false);
        ui->outputLabel->setVisible(false);
        ui->changeAccButton->setVisible(false);
        ui->numberButton->setVisible(false);
        ui->numberLine->setVisible(false);
        ui->numberLabel->setVisible(false);
        ui->textEdit22->setVisible(false);

        ui->stackedWidget->setCurrentIndex(1);
        ui->arrayBox->setReadOnly(true);
        ui->arrayBox->clear();
        QVector<QPair<QString, QString>> arrays = dbManager.getUserArrays(userID);

        if (arrays.size()==0){
            ui->arrayBox->setText("Данные пока отсутствуют!");
            return;
        }
        QString output;
        for (int i = 0; i<arrays.size(); ++i){
            output += QString("Список №%1:\n").arg(i+1);
            output += "Неотсортированный : " + arrays[i].first + '\n';
            output += "Отсортированный : " + arrays[i].second + "\n\n";
            QCoreApplication::processEvents();

        }
        ui->arrayBox->setPlainText(output);
    }


    void MainWindow::on_changeButton_clicked(){
        ui->numberLine->setVisible(true);
        ui->numberLabel->setVisible(true);
        ui->numberButton->setVisible(true);

    }

    void MainWindow::on_numberButton_clicked(){
        ui->changeAccButton->setVisible(true);
        ui->changeLabel->setVisible(true);
        ui->changeEdit->setVisible(true);
        ui->textEdit22->setVisible(true);
        ui->outputLabel->setVisible(false);
        ui->textEdit22->setVisible(false);
        ui->textEdit22->clear();
        ui->changeEdit->clear();
        bool ok;
        QString argument = ui->numberLine->text().trimmed().simplified();
        int zazor = dbManager.getMinArrayId(userID);
        int argum = argument.toInt(&ok);

        if (!ok || QString::number(argum) != argument) {
            ui->outputLabel->setVisible(true);
            ui->outputLabel->setText("Некорректное число введено.");
            ui->outputLabel->setStyleSheet("color: rgb(167, 58, 58);");
            return;
        }
        argum = zazor-1+argum;

        QString StrQuery = ("SELECT unsorted_array FROM user_arrays WHERE array_id = :array AND user_id = :user_id");
        QVariant array_res = dbManager.ExecuteSelectQuery_SingleArray(StrQuery, argum, userID);

        if (array_res.isValid()) {
            ui->outputLabel->setVisible(false);
            qDebug() << "Массив найден:" << array_res.toString();
            ui->changeEdit->setText(array_res.toString());
        } else {
            ui->outputLabel->setVisible(true);
            ui->outputLabel->setText("Некорректное число введено.");
        }
        QString array = array_res.toString();
        //QString output;
        ui->changeEdit->setText(array);
    }

    void MainWindow::on_changeAccButton_clicked(){
        //ui->outputLabel->setVisible(true);
        QString changed_array = ui->changeEdit->toPlainText();
        if (changed_array.trimmed().isEmpty()) {
            ui->outputLabel->setText("Ошибка: поле ввода пустое");
            ui->outputLabel->setStyleSheet("color: rgb(167, 58, 58);");
            ui->outputLabel->setVisible(true);
            return; // Выход из метода, если строка пуста
        }
        QVector<QString> vec = dbManager.convertToVec(changed_array);
        std::vector<std::string> stdVec;
        for (const QString& str : vec) {
            stdVec.push_back(str.toStdString()); // Преобразование для my_sort_lib (шаблонный вектор)
        }
        std::vector<std::string> res;

        res = my_sort_lib::shaker_sort(stdVec);
        res = my_sort_lib::number_sort(res);

        QStringList sortedList;
        for (const std::string& str : res) {
            sortedList << QString::fromStdString(str);
        }

        // вывод в TextEdit
        QString output = sortedList.join(" ");
        ui->textEdit22->setText(output);

        QString StrQuery = "UPDATE user_arrays "
        "SET unsorted_array  = :unsortedArray, sorted_array = :sortedArray "
        "WHERE array_id = :arrayID AND user_id = :userID";

        bool ok;
        int zazor = dbManager.getMinArrayId(userID);
        QString argument = ui->numberLine->text().trimmed();
        int argum = argument.toInt(&ok);


        if (!ok || QString::number(argum) != argument) {
            ui->outputLabel->setVisible(true);
            ui->outputLabel->setText("Некорректное число введено.");
            ui->outputLabel->setStyleSheet("color: rgb(167, 58, 58);");
            return;
        }
        argum = zazor-1+argum;
        dbManager.updateArray_query(StrQuery, changed_array, output, argum, userID);
        ui->arrayBox->setReadOnly(false);
        ui->outputLabel->setVisible(true);
        ui->outputLabel->setText("Изменения внесены!");

        ui->arrayBox->clear();
        QVector<QPair<QString, QString>> arrays = dbManager.getUserArrays(userID);

        if (arrays.size()==0){
            ui->arrayBox->setText("Данные пока отсутствуют!");
            return;
        }
        QString output2;
        for (int i = 0; i<arrays.size(); ++i){
            output2 += QString("Список №%1:\n").arg(i+1);
            output2 += "Неотсортированный : " + arrays[i].first + '\n';
            output2 += "Отсортированный : " + arrays[i].second + "\n\n";

        }
        qDebug() << output2;
        ui->arrayBox->setPlainText(output2);
        ui->arrayBox->setReadOnly(true);
    }

    void MainWindow::on_testButton_clicked(){
        ui->stackedWidget->setCurrentIndex(3);
        ui->progressBar->setRange(0, 100);    // для первого цикла
        ui->progressBar_2->setRange(0, 1000);   // для второго
        ui->progressBar_3->setRange(0, 10000);
        ui->progressBar_4->setRange(0, 100);
        ui->progressBar_5->setRange(0, 1000);
        ui->progressBar_6->setRange(0, 10000);
        ui->state1->setText("Ожидает запуска");
        ui->state2->setText("Ожидает запуска");
        ui->state3->setText("Ожидает запуска");
        ui->state4->setText("Ожидает запуска");
        ui->state5->setText("Ожидает запуска");
        ui->state6->setText("Ожидает запуска");
        ui->progressBar->setValue(0);
        ui->progressBar_2->setValue(0);
        ui->progressBar_3->setValue(0);
        ui->progressBar_4->setValue(0);
        ui->progressBar_5->setValue(0);
        ui->progressBar_6->setValue(0);
    }
    void MainWindow::on_testViewButton_clicked(){
        ui->stackedWidget->setCurrentIndex(2);
        QVector<QPair<QString, QString>> arrays = dbManager.getUserArrays(2);

        if (arrays.size()==0){
            ui->testDataEdit->setText("Данные пока отсутствуют!");
            return;
        }
        QString output;
        for (int i = 0; i<arrays.size(); ++i){
            output += QString("Список №%1:\n").arg(i+1);
            output += "Неотсортированный : " + arrays[i].first + '\n';
            output += "Отсортированный : " + arrays[i].second + "\n\n";
            QCoreApplication::processEvents();

        }
        ui->testDataEdit->setPlainText(output);
    }
    void MainWindow::on_backButton_clicked(){
        ui->stackedWidget->setCurrentIndex(3);
        ui->testDataEdit->clear();
    }


    MainWindow::~MainWindow()
    {
        delete ui;
    }































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































    void MainWindow::on_testStartButton_clicked(){
        srand(static_cast<unsigned int>(time(nullptr)));

        int TEST_ID = 2;

        ui->state1->setText("Выполняется...");
        ui->state1->setStyleSheet("color: yellow;");
        ui->state2->setText("Ожидает");
        ui->state2->setStyleSheet("color: red;");
        ui->state3->setText("Ожидает");
        ui->state3->setStyleSheet("color: red;");
        ui->state4->setText("Ожидает");
        ui->state4->setStyleSheet("color: red;");
        ui->state5->setText("Ожидает");
        ui->state5->setStyleSheet("color: red;");
        ui->state6->setText("Ожидает");
        ui->state6->setStyleSheet("color: red;");
        QElapsedTimer timer1;
        QString output;
        timer1.start();
        for (int i = 0; i<=100; ++i)
        {
            //db_productivity testObject;

            //QTest::qExec(&testObject);
            std::vector<std::string> stdVec = dbManager.default_data();
            std::vector<std::string> res;

            res = my_sort_lib::shaker_sort(stdVec);
            res = my_sort_lib::number_sort(res);

            QStringList List;
            for (const std::string& str : stdVec){
                List << QString::fromStdString(str);
            }
            QStringList sortedList;
            for (const std::string& str : res){
                sortedList << QString::fromStdString(str);
            }

            // вывод в TextEdit
            QString first = List.join(" ");
            QString second = sortedList.join(" ");

            int result = dbManager.insertArray(TEST_ID, first, second);
            if (result == 1){
                qDebug ("Произошла ошибка", i);
            }

            ui->progressBar->setValue(i);
            QCoreApplication::processEvents();


            output += QString("Список №%1:\n").arg(i + 1);
            output += "Неотсортированный : " + first + '\n';
            output += "Отсортированный : " + second + "\n\n";
            QCoreApplication::processEvents();

            }
        ui->resultEdit1->setPlainText(output);

        qint64 time1 = timer1.elapsed();
        ui->time1->setText(QString::number(time1) + "мс");
        ui->state1->setText("Завершен!");
        ui->state1->setStyleSheet("color: green;");
        ui->state2->setText("Выполняется...");
        ui->state2->setStyleSheet("color: yellow;");
        QElapsedTimer timer2;
        QString output2;
        timer2.start();
        for (int i = 0; i<=1000; ++i){
            std::vector<std::string> stdVec = dbManager.default_data();
            std::vector<std::string> res;

            res = my_sort_lib::shaker_sort(stdVec);
            res = my_sort_lib::number_sort(res);

            QStringList List;
            for (const std::string& str : stdVec){
                List << QString::fromStdString(str);
            }
            QStringList sortedList;
            for (const std::string& str : res){
                sortedList << QString::fromStdString(str);
            }

            // вывод в TextEdit
            QString first = List.join(" ");
            QString second = sortedList.join(" ");

            int result = dbManager.insertArray(TEST_ID, first, second);
            if (result == 1){
                qDebug ("Произошла ошибка", i);
            }
            ui->progressBar_2->setValue(i);

            output2 += QString("Список №%1:\n").arg(i + 1);
            output2 += "Неотсортированный : " + first + '\n';
            output2 += "Отсортированный : " + second + "\n\n";
            QCoreApplication::processEvents();
        }
        ui->resultEdit1->append(output2);
        qint64 time2 = timer2.elapsed();
        ui->time2->setText(QString::number(time2) + "мс");
        ui->state2->setText("Завершен!");
        ui->state2->setStyleSheet("color: green;");
        ui->state3->setText("Выполняется...");
        ui->state3->setStyleSheet("color: yellow;");
        QElapsedTimer timer3;
        QString output3;
        timer3.start();
        for (int i = 0; i<=10000; ++i){
            std::vector<std::string> stdVec = dbManager.default_data();
            std::vector<std::string> res;

            res = my_sort_lib::shaker_sort(stdVec);
            res = my_sort_lib::number_sort(res);

            QStringList List;
            for (const std::string& str : stdVec){
                List << QString::fromStdString(str);
            }
            QStringList sortedList;
            for (const std::string& str : res){
                sortedList << QString::fromStdString(str);
            }

            // вывод в TextEdit
            QString first = List.join(" ");
            QString second = sortedList.join(" ");

            int result = dbManager.insertArray(TEST_ID, first, second);
            if (result == 1){
                qDebug ("Произошла ошибка", i);
            }
            ui->progressBar_3->setValue(i);
            output3 += QString("Список №%1:\n").arg(i + 1);
            output3 += "Неотсортированный : " + first + '\n';
            output3 += "Отсортированный : " + second + "\n\n";
            QCoreApplication::processEvents();
        }

        qint64 time3 = timer3.elapsed();
        ui->resultEdit1->append(output3);
        ui->time3->setText(QString::number(time3) + "мс");
        ui->state3->setText("Завершен!");
        ui->state3->setStyleSheet("color: green;");
        ui->state4->setText("Выполняется...");
        ui->state4->setStyleSheet("color: yellow;");

        /////////////////

        ui->state4->setText("Выполняется...");
        ui->state4->setStyleSheet("color: yellow;");
        QElapsedTimer timer4;
        timer4.start();
        QString output4;
        for (int i = 0; i <= 100; ++i) {
            QString StrQuery = "SELECT unsorted_array FROM user_arrays WHERE array_id = :array AND user_id = :user_id";
            QVariant arrayRes = dbManager.ExecuteSelectQuery_SingleArray(StrQuery, dbManager.getRandomArrayId(TEST_ID), TEST_ID);

            if (arrayRes.isValid()) {

                qDebug() << i << "Массив найден:" << arrayRes.toString();

            } else {
                qDebug() << ("Некорректное число введено.");
            }
            QString array = arrayRes.toString(); // first

            QVector<QString> Qarray = array.split(" ");

            // QVector<QString> -> std::vector<std::string>
            std::vector<std::string> stdVec;
            for (const QString& item : Qarray) {
                stdVec.push_back(item.toStdString());
            }

            std::vector<std::string> res = my_sort_lib::shaker_sort(stdVec);
            res = my_sort_lib::number_sort(res);
            ui->progressBar_4->setValue(i);
            QCoreApplication::processEvents();
            QStringList sortedList;
            for (const auto& item : res) {
                sortedList << QString::fromStdString(item);
            }
            output4 += QString("Список №%1:\n").arg(i + 1);
            output4 += "Неотсортированный массив: " + array+ '\n';
            output4 += "Отсортированный массив: " + sortedList.join(" ")+ '\n';
        }
        qint64 elapsedTime4 = timer4.elapsed();
        ui->time4->setText(QString("Время: %1 мс").arg(elapsedTime4));
        ui->state4->setText("Завершен!");
        ui->state4->setStyleSheet("color: green;");
        ui->resultEdit2->append(output);
        QCoreApplication::processEvents();

        // Тест на 1000 записей
        ui->state5->setText("Выполняется...");
        ui->state5->setStyleSheet("color: yellow;");
        QElapsedTimer timer5;
        QString output5;
        timer5.start();

        for (int i = 0; i <= 1000; ++i) {
            QString StrQuery = "SELECT unsorted_array FROM user_arrays WHERE array_id = :array AND user_id = :user_id";
            QVariant arrayRes = dbManager.ExecuteSelectQuery_SingleArray(StrQuery, dbManager.getRandomArrayId(TEST_ID), TEST_ID);

            if (arrayRes.isValid()) {

                qDebug() << i << "Массив найден:" << arrayRes.toString();

            } else {
                //ui->outputLabel->setVisible(true);
                qDebug() << ("Некорректное число введено.");
            }
            QString array = arrayRes.toString();
            QVector<QString> Qarray = array.split(" ");

            // QVector<QString> -> std::vector<std::string>
            std::vector<std::string> stdVec;
            for (const QString& item : Qarray) {
                stdVec.push_back(item.toStdString());
            }

            std::vector<std::string> res = my_sort_lib::shaker_sort(stdVec);
            res = my_sort_lib::number_sort(res);
            ui->progressBar_5->setValue(i);
            QCoreApplication::processEvents();
            QStringList sortedList;
            for (const auto& item : res) {
                sortedList << QString::fromStdString(item);
            }
            output5 += QString("Список №%1:\n").arg(i + 1);
            output5 += "Неотсортированный массив: " + array+ '\n';
            output5 += "Отсортированный массив: " + sortedList.join(" ")+ '\n';
        }
        qint64 elapsedTime5 = timer5.elapsed();

        ui->time5->setText(QString("Время: %1 мс").arg(elapsedTime5));
        ui->state5->setText("Завершен!");
        ui->state5->setStyleSheet("color: green;");
        ui->resultEdit2->append(output5);
        QCoreApplication::processEvents();

        // Тест на 10000 записей
        ui->state6->setText("Выполняется...");
        ui->state6->setStyleSheet("color: yellow;");
        QElapsedTimer timer6;
        QString output6;
        timer6.start();

        for (int i = 0; i <= 10000; ++i) {
            QString StrQuery = "SELECT unsorted_array FROM user_arrays WHERE array_id = :array AND user_id = :user_id";
            QVariant arrayRes = dbManager.ExecuteSelectQuery_SingleArray(StrQuery, dbManager.getRandomArrayId(TEST_ID), TEST_ID);

            if (arrayRes.isValid()) {
                qDebug() << i << "Массив найден:" << arrayRes.toString();
            } else {
                qDebug() << ("Некорректное число введено.");
            }
            QString array = arrayRes.toString();
            QVector<QString> Qarray = array.split(" ");

            // QVector<QString> -> std::vector<std::string>
            std::vector<std::string> stdVec;
            for (const QString& item : Qarray) {
                stdVec.push_back(item.toStdString());
            }

            std::vector<std::string> res = my_sort_lib::shaker_sort(stdVec);
            res = my_sort_lib::number_sort(res);
            ui->progressBar_6->setValue(i);
            QCoreApplication::processEvents();
            QStringList sortedList;
            for (const auto& item : res) {
                sortedList << QString::fromStdString(item);
            }
            output6 += QString("Список №%1:\n").arg(i + 1);
            output6 += "Неотсортированный массив: " + array+ '\n';
            output6 += "Отсортированный массив: " + sortedList.join(" ")+ '\n';
        }

        qint64 elapsedTime6 = timer6.elapsed();
        ui->time6->setText(QString("Время: %1 мс").arg(elapsedTime6));
        ui->state6->setText("Завершен!");
        ui->state6->setStyleSheet("color: green;");
        ui->resultEdit2->append(output6);

        QCoreApplication::processEvents();
    }
    void MainWindow::on_clearDatabaseButton_clicked() {
        // Тест очистки базы на 100 записей
        ui->state7->setText("Очистка...");
        ui->state7->setStyleSheet("color: yellow;");
        QElapsedTimer timer100;

        timer100.start();
        bool success100 = dbManager.clearDatabase(100);
        qint64 totalTime100 = timer100.elapsed();
        QString resultFlag100 = success100 ? "Успешно" : "Ошибка";

        ui->state7->setText(QString("%1").arg(resultFlag100));
        ui->time7->setText(QString("Время: %1 мс").arg(totalTime100));
        ui->state7->setStyleSheet(success100 ? "color: green;" : "color: red;");
        QCoreApplication::processEvents();

        // Тест очистки базы на 1000 записей
        ui->state8->setText("Очистка...");
        ui->state8->setStyleSheet("color: yellow;");
        QElapsedTimer timer1000;

        timer1000.start();
        bool success1000 = dbManager.clearDatabase(1000);
        qint64 totalTime1000 = timer1000.elapsed();
        QString resultFlag1000 = success1000 ? "Успешно" : "Ошибка";

        ui->state8->setText(QString(" %1").arg(resultFlag1000));
        ui->time8->setText(QString("Время: %1 мс").arg(totalTime1000));
        ui->state8->setStyleSheet(success1000 ? "color: green;" : "color: red;");
        QCoreApplication::processEvents();

        // Тест очистки базы на 10000 записей
        ui->state9->setText("Очистка...");
        ui->state9->setStyleSheet("color: yellow;");
        QElapsedTimer timer10000;

        timer10000.start();
        bool success10000 = dbManager.clearDatabase(10000);
        qint64 totalTime10000 = timer10000.elapsed();
        QString resultFlag10000 = success10000 ? "Успешно" : "Ошибка";

        ui->state9->setText(QString("%1").arg(resultFlag10000));
        ui->time9->setText(QString("Время: %1 мс").arg(totalTime10000));
        ui->state9->setStyleSheet(success10000 ? "color: green;" : "color: red;");
        QCoreApplication::processEvents();
    }





















