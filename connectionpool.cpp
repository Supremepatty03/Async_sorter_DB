#include "connectionpool.h"
#include <QSqlError>
#include <QDebug>

// Singleton
ConnectionPool& ConnectionPool::instance() {
    static ConnectionPool pool;
    return pool;
}

void ConnectionPool::init(const QString& driver,
                          const QString& dbName,
                          int poolSize,
                          const QString& connOptions)
{
    QMutexLocker locker(&m_mutex);
    m_driver      = driver;
    m_dbName      = dbName;
    m_connOptions = connOptions;
    m_poolSize    = poolSize;

    // Предварительное созданание соединения
    for (int i = 0; i < poolSize; ++i) {
        QString connName = QString("%1_conn_%2")
        .arg(driver)
            .arg(i);
        QSqlDatabase db = QSqlDatabase::addDatabase(driver, connName);
        db.setDatabaseName(dbName);
        if (!connOptions.isEmpty())
            db.setConnectOptions(connOptions);
        if (!db.open()) {
            qWarning() << "Cannot open DB:" << db.lastError().text();
        }
        m_freeConnections.enqueue(connName);
    }
}

QSqlDatabase ConnectionPool::acquire() {
    QMutexLocker locker(&m_mutex);
    while (m_freeConnections.isEmpty())
        m_waitCond.wait(&m_mutex);

    QString name = m_freeConnections.dequeue();
    return QSqlDatabase::database(name);
}

void ConnectionPool::release(const QSqlDatabase& db) {
    QMutexLocker locker(&m_mutex);
    QString name = db.connectionName();
    if (!name.isEmpty() && !m_freeConnections.contains(name))
        m_freeConnections.enqueue(name);
    locker.unlock();
    m_waitCond.wakeOne();
}

ConnectionPool::~ConnectionPool() {
    QMutexLocker locker(&m_mutex);
    for (int i = 0; i < m_poolSize; ++i) {
        QString connName = m_freeConnections.dequeue();
        QSqlDatabase db = QSqlDatabase::database(connName);
        db.close();
        QSqlDatabase::removeDatabase(connName);
    }
}
