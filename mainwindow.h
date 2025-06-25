#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "db_manager.h"
#include <QObject>
#include <QTextEdit>
#include <QProgressBar>
#include <QLabel>
#include <QElapsedTimer>
#include <QStringList>
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(const int userID, QWidget *parent = nullptr);
    ~MainWindow(); 
private slots:
    void on_newArrButton_clicked();
    void on_toLobby_clicked();
    void on_confirmInput_clicked();
    void on_saveButton_clicked();
    void on_viewButton_clicked();
    void on_changeButton_clicked();
    void on_numberButton_clicked();
    void on_changeAccButton_clicked();
    void on_testButton_clicked();
    void on_testStartButton_clicked();
    void on_clearDatabaseButton_clicked();
    void on_testViewButton_clicked();
    void on_backButton_clicked();

    private : Ui::MainWindow *ui;
    db_manager dbManager;
    int userID;
    void runSortWorker(int testId, int count, bool fetchFromDb,
                       QTextEdit* output, QProgressBar* progress,
                       QLineEdit* stateLabel, QLineEdit* timeLabel);
};
#endif // MAINWINDOW_H
