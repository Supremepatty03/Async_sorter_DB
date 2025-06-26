#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "db_manager.h"
#include <QToolBar>
#include <QElapsedTimer>
#include <QTextEdit>
#include "sort.hpp"
#include <QtTest/QtTest>
#include "connectionpool.h"
#include "sortworker.h"
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
    void MainWindow::runSortWorker(int testId, int count, bool fetchFromDb,
                                   QTextEdit* outputEdit,
                                   QProgressBar* progressBar,
                                   QLineEdit* stateLabel,
                                   QLineEdit* timeLabel) {
        progressBar->setRange(0, count);
        progressBar->setValue(0);
        stateLabel  ->setText("Ожидание…");
        stateLabel->setStyleSheet("color: red;");
        timeLabel   ->clear();
        //outputEdit  ->clear();
        if (!fetchFromDb) {
            outputEdit->clear();
        }
        QThread* thread = new QThread;
        SortWorker* worker = new SortWorker(testId, count, fetchFromDb, userID);

        worker->moveToThread(thread);

        connect(thread, &QThread::started, worker, &SortWorker::process);
        connect(worker, &SortWorker::progressUpdated, this, [=](int val) {
            progressBar->setValue(val);
        });
        connect(worker, &SortWorker::statusUpdated, this, [=](const QString& status){
            stateLabel->setText(status);
            if (status.contains("Выполняется") || status.contains("В процессе")) {
                stateLabel->setStyleSheet("color: orange;");
            }
            else if (status.contains("Завершено")) {
                stateLabel->setStyleSheet("color: green;");
            }
            else {
                stateLabel->setStyleSheet("color: red;");
            }
        });
        if (fetchFromDb) {
            // дозаписываем каждый блок результатов
            connect(worker, &SortWorker::resultUpdated, this, [=](const QString& txt){
                outputEdit->append(txt);
            });
        } else {
            // один раз затираем и пишем
            connect(worker, &SortWorker::resultUpdated, this, [=](const QString& txt){
                outputEdit->setPlainText(txt);
            });
        }
        connect(worker, &SortWorker::timeUpdated, this, [=](const QString& text) {
            timeLabel->setText(text);
        });

        connect(worker, &SortWorker::finished, thread, &QThread::quit);
        connect(worker, &SortWorker::finished, worker, &SortWorker::deleteLater);
        connect(thread, &QThread::finished, thread, &QThread::deleteLater);

        thread->start();
    }
    void MainWindow::on_testButton_clicked(){
        ui->stackedWidget->setCurrentIndex(3);
        ui->progressBar->setRange(0, 100);    // для первого цикла
        ui->progressBar_2->setRange(0, 1000);   // для второго и тд
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
        // zgzg zgagdgaagad
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
        ui->testDataEdit->setPlainText(output);
    }
        QCoreApplication::processEvents();

    }
    void MainWindow::on_backButton_clicked(){
        ui->stackedWidget->setCurrentIndex(3);
        ui->testDataEdit->clear();
    }


    MainWindow::~MainWindow()
    {
        delete ui;
    }
    void MainWindow::on_testStartButton_clicked() {
        runSortWorker(2, 100, false, ui->resultEdit1, ui->progressBar, ui->state1, ui->time1);
        runSortWorker(2, 1000, false, ui->resultEdit1, ui->progressBar_2, ui->state2, ui->time2);
        runSortWorker(2, 10000, false, ui->resultEdit1, ui->progressBar_3, ui->state3, ui->time3);
        runSortWorker(2, 100, true, ui->resultEdit2, ui->progressBar_4, ui->state4, ui->time4);
        runSortWorker(2, 1000, true, ui->resultEdit2, ui->progressBar_5, ui->state5, ui->time5);
        runSortWorker(2, 10000, true, ui->resultEdit2, ui->progressBar_6, ui->state6, ui->time6);
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





















