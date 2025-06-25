#include "sortworker.h"
#include <QThread>
#include <QDebug>
#include <QCoreApplication>
#include "db_manager.h"
#include <QLineEdit>

SortWorker::SortWorker(int testId, int count, bool fetchFromDb, int user_iD, QObject* parent)
    : QObject(parent),
    testId(testId),
    count(count),
    fetchFromDb(fetchFromDb),
    user_id(user_iD),
    dbManager(db_manager::getInstance()) {}

void SortWorker::process() {
    emit statusUpdated("Выполняется...");
    QElapsedTimer timer;
    timer.start();
    QString output;

    for (int i = 0; i <= count; ++i) {
        std::vector<std::string> stdVec;

        if (fetchFromDb) {
            QString query = "SELECT unsorted_array FROM user_arrays "
                            "WHERE array_id = :array AND user_id = :user_id";
            int randomId = dbManager.getRandomArrayId(testId);
            QVariant res = dbManager.ExecuteSelectQuery_SingleArray(query, randomId, user_id);

            if (!res.isValid()) {emit progressUpdated(i); continue; }

            QString raw = res.toString();
            for (const QString& s : raw.split(" ", Qt::SkipEmptyParts))
                stdVec.push_back(s.toStdString());

        } else {
            stdVec = dbManager.default_data();  // случайные строки
        }

        auto res = my_sort_lib::shaker_sort(stdVec);
        res = my_sort_lib::number_sort(res);

        QStringList sortedList;
        for (const auto& s : res)
            sortedList << QString::fromStdString(s);

        if (!fetchFromDb) {
            QStringList original;
            for (const auto& s : stdVec)
                original << QString::fromStdString(s);

            dbManager.insertArray(testId, original.join(" "), sortedList.join(" "));
        }

        // Формируем результат для TextEdit
        QString unsorted = QString::fromStdString(std::accumulate(
            stdVec.begin(), stdVec.end(), std::string(),
            [](std::string a, std::string b) { return a + " " + b; }));

        output += QString("Список №%1:\n").arg(i + 1);
        output += "Неотсортированный: " + unsorted.trimmed() + "\n";
        output += "Отсортированный:   " + sortedList.join(" ") + "\n\n";

        emit progressUpdated(i);  // Обновление прогрессбара
    }

    emit resultUpdated(output);  // Вывод в QTextEdit
    emit timeUpdated(QString::number(timer.elapsed()) + " мс");
    emit statusUpdated("Завершено!");
    emit finished();
}
