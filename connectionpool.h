#pragma once
#include <QSqlDatabase>
#include <QMutex>
#include <QWaitCondition>
#include <QQueue>

class ConnectionPool {
public:
    static ConnectionPool& instance();
    QSqlDatabase acquire();               // получить соединение
    void release(const QSqlDatabase&);    // вернуть соединение

    void init(const QString& driver,
              const QString& dbName,
              int poolSize,
              const QString& connOptions = QString());

private:
    ConnectionPool() = default;
    ~ConnectionPool();

    QMutex               m_mutex;
    QWaitCondition       m_waitCond;
    QQueue<QString>      m_freeConnections;
    int                  m_poolSize = 0;
    QString              m_driver;
    QString              m_dbName;
    QString              m_connOptions;
};
