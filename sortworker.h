#ifndef SORTWORKER_H
#define SORTWORKER_H

#include <QObject>
#include <QTextEdit>
#include <QProgressBar>
#include <QLabel>
#include <QElapsedTimer>
#include <QStringList>
#include <vector>
#include <string>
#include "db_manager.h"
#include "sort.hpp"

class SortWorker : public QObject {
    Q_OBJECT

public:
    SortWorker(int testId, int count, bool fetchFromDb,int user_id, QObject* parent = nullptr);

public slots:
    void process();

signals:
    void progressUpdated(int);
    void statusUpdated(const QString&);
    void resultUpdated(const QString&);
    void timeUpdated(const QString&);
    void finished();

private:
    int testId;
    int count;
    bool fetchFromDb;
    int user_id;
    db_manager& dbManager;
};

#endif // SORTWORKER_H
