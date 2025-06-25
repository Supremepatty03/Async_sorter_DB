#include "db_manager.h"
#include <QCryptographicHash>
#include <QRandomGenerator>

//QSqlDatabase db_manager::db;
//bool db_manager::connectionInitialized = false;

db_manager::db_manager() {
    dbPath = "C:/SQLite/my_database2.db";
}
db_manager::~db_manager() {
}
QSqlDatabase& db_manager::databaseForThisThread() {
    if (!threadLocalDb.hasLocalData()) {
        QString connName = QString("db_%1")
        .arg((quintptr)QThread::currentThreadId());

        // создаём локальное соединение
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", connName);
        db.setDatabaseName(dbPath);
        if (!db.open()) {
            qWarning() << "Нельзя открыть БД в потоке"
                       << QThread::currentThreadId()
                       << db.lastError().text();
        } else {
            db.exec("PRAGMA journal_mode = DELETE;");
            db.exec("PRAGMA busy_timeout = 3000;");
        }
        // В threadLocalDb хранится копия QSqlDatabase
        threadLocalDb.setLocalData(db);
    }
    return threadLocalDb.localData();
}

db_manager& db_manager::getInstance() {
    static db_manager instance;
    return instance;
}
void db_manager::closeDatabase() {
    if (threadLocalDb.hasLocalData()) {
        // Закрываем и удаляем соединение в этом потоке
        QSqlDatabase& db = threadLocalDb.localData();
        QString connName = db.connectionName();
        db.close();
        QSqlDatabase::removeDatabase(connName);
        //threadLocalDb.removeLocalData();
    }
}

QByteArray db_manager::hashPassword(const QString &password, const QByteArray &salt){
    QByteArray passwordWithSalt = password.toUtf8() + salt;
    return QCryptographicHash::hash(passwordWithSalt, QCryptographicHash::Sha256).toHex();
}

QByteArray db_manager::generateSalt() {
    int length = 16;
    QByteArray salt;
    for (int i = 0; i < length; ++i) {
        salt.append(QRandomGenerator::global()->bounded(33, 127)); // ASCII символы
    }
    return salt;
}
int db_manager::insertUser(const QString& username, const QString& password) {
    QSqlDatabase& db = databaseForThisThread();

    // 1) Проверка: нет ли уже такого пользователя
    {
        QSqlQuery chk(db);
        chk.prepare("SELECT COUNT(*) FROM users WHERE username = ?");
        chk.addBindValue(username);
        if (!chk.exec() || !chk.next()) {
            qDebug() << "[insertUser] Ошибка проверки:" << chk.lastError().text();
            return 2;
        }
        if (chk.value(0).toInt() > 0) {
            qDebug() << "[insertUser] Пользователь уже существует";
            return 3;
        }
    }

    // 2) Генерация соли и хэша
    QByteArray salt = QCryptographicHash::hash(
                          QDateTime::currentDateTime().toString("yyyyMMddhhmmsszzz").toUtf8(),
                          QCryptographicHash::Md5
                          ).toHex();
    QByteArray hash = hashPassword(password, salt);

    // 3) Вставка нового пользователя
    QSqlQuery ins(db);
    ins.prepare(
        "INSERT INTO users (username, salt, hash_password) "
        "VALUES (?, ?, ?)"
        );
    ins.addBindValue(username);
    ins.addBindValue(QString::fromUtf8(salt));
    ins.addBindValue(QString::fromUtf8(hash));

    if (!ins.exec()) {
        qDebug() << "[insertUser] Ошибка INSERT:" << ins.lastError().text();
        return 4;
    }

    qDebug() << "[insertUser] Успешная регистрация:" << username;
    return 0;
}
int db_manager::insertArray(int user_id, const QString& array, const QString& sortedArray){
    QSqlDatabase& db = databaseForThisThread();
    QSqlQuery query(db);
    query.prepare("INSERT INTO user_arrays (user_id, unsorted_array, sorted_array) VALUES (:userID, :unsortedArray, :sortedArray)");
    query.bindValue(":userID", user_id);
    query.bindValue(":unsortedArray", array);
    query.bindValue(":sortedArray", sortedArray);
    if (!query.exec()) {
        qDebug() << "Ошибка выполнения запроса:" << query.lastError().text();
        return 1;
    }
    return 0;
}

bool db_manager::set_connection(const DB_config& cfg) {
    dbPath = cfg.database_path;
    return true;
}

QVariant db_manager::ExecuteSelectQuery_SingleData(const QString StrQuery, QString loginText) {
    QSqlDatabase& db = databaseForThisThread();
    QSqlQuery query(db);
    query.prepare(StrQuery);
    query.bindValue(":username", loginText);
    if (!query.exec() || !query.next())
        return QVariant();
    return query.value(0);
}

QMutex db_manager::s_dbMutex;

QVariant db_manager::ExecuteSelectQuery_SingleArray(
    const QString StrQuery, int arg, int user_id)
{
    QMutexLocker locker(&s_dbMutex);
    QSqlDatabase& db = databaseForThisThread();
    QSqlQuery query(db);
    query.prepare(StrQuery);
    query.bindValue(":array",   arg);
    query.bindValue(":user_id", user_id);
    if (!query.exec() || !query.next())
        return QVariant();
    return query.value(0);
}

QVector<QVector<QVariant>> db_manager::ExecuteSelectQuery_allUserData(const QString StrQuery) {
    QVector<QVector<QVariant>> resultMatrix;

    // Берём или создаём соединение в этом потоке
    QSqlDatabase& db = databaseForThisThread();
    if (!db.isOpen()) {
        qDebug() << "[allUserData] БД не открыта в потоке"
                 << QThread::currentThreadId();
        return resultMatrix;
    }

    QSqlQuery query(db);
    query.prepare(StrQuery);
    if (!query.exec()) {
        qDebug() << "[allUserData] Ошибка запроса:" << query.lastError().text();
        return resultMatrix;
    }

    while (query.next()) {
        QVector<QVariant> row;
        for (int i = 0; i < query.record().count(); ++i)
            row.append(query.value(i));
        resultMatrix.append(row);
    }
    return resultMatrix;
}
QVector<QVariant> db_manager::ExecuteSelectQuery_UserHashPassword(const QString StrQuery) {
    QVector<QVariant> result;
    QSqlDatabase& db = databaseForThisThread();
    QSqlQuery query(db);
    query.prepare(StrQuery);
    if (!query.exec())
        return result;
    while (query.next())
        for (int i = 0; i < query.record().count(); ++i)
            result.append(query.value(i));
    return result;
}
QVector<QString> db_manager::convertToVec (QString input){
    QVector<QString> result = input.split(" ", Qt::SkipEmptyParts);
    return result;
}

QString convertToString (QVector<QString> input){
    QString result;
    QStringList tmp = QStringList::fromVector(input);
    result = tmp.join(" ");
    return result;
}

QVector<QPair<QString, QString>> db_manager::getUserArrays(int userID) {
    QVector<QPair<QString, QString>> arrays;
    QSqlDatabase& db = databaseForThisThread();
    QSqlQuery query(db);
    query.prepare(
        "SELECT unsorted_array, sorted_array "
        "FROM user_arrays WHERE user_id = :userID"
        );
    query.bindValue(":userID", userID);
    if (!query.exec())
        return arrays;
    while (query.next()) {
        arrays.append(qMakePair(
            query.value(0).toString(),
            query.value(1).toString()
            ));
    }
    return arrays;
}

void db_manager::updateArray_query(
    const QString StrQuery,
    const QString unsortedArray,
    const QString sortedArray,
    int arrayID,
    int userID)
{
    QSqlDatabase& db = databaseForThisThread();
    QSqlQuery query(db);
    query.prepare(StrQuery);
    query.bindValue(":unsortedArray", unsortedArray);
    query.bindValue(":sortedArray",   sortedArray);
    query.bindValue(":arrayID",       arrayID);
    query.bindValue(":userID",        userID);
    if (!query.exec())
        qDebug() << "updateArray_query ошибка:" << query.lastError().text();
}

std::vector<std::string> db_manager::default_data() {
    std::string tmp;
    std::vector<std::string> default_list;

    int length = rand() % 20 + 2;

    for (int i = 0; i < length; i++) {
        int j = rand() % 7 + 2;
        tmp = "";
        for (int k = 1; k < j; k++) {
            int random_char_type = rand() % 4;
            char random_char;

            if (random_char_type == 0) {
                random_char = rand() % 10 + '0'; // Numbers
            } else if (random_char_type == 1) {
                random_char = rand() % 26 + 'A'; // Uppercase
            } else if (random_char_type == 2) {
                random_char = rand() % 26 + 'a'; // Lowercase
            } else {
                random_char = rand() % 15 + 33;  // Special characters
            }
            tmp += random_char;
        }
        default_list.emplace_back(tmp);
    }
    return default_list;
}

bool db_manager::clearDatabase(int recordCount) {
    QSqlDatabase& db = databaseForThisThread();
    QSqlQuery query(db);
    // пример для тестового user_id=2, лучше параметризовать
    QString deleteQuery = QString(
                              "DELETE FROM user_arrays "
                              "WHERE array_id IN ( "
                              "  SELECT array_id "
                              "  FROM user_arrays "
                              "  WHERE user_id = 2 "
                              "  ORDER BY array_id ASC "
                              "  LIMIT %1 "
                              ")"
                              ).arg(recordCount);
    if (!query.exec(deleteQuery)) {
        qDebug() << "clearDatabase ошибка:" << query.lastError().text();
        return false;
    }
    return true;
}

int db_manager::getRandomArrayId(int user_id) {
    // 1. Найти минимальный и максимальный ID
    QSqlDatabase& db = databaseForThisThread();
    QSqlQuery query(db);
    QString queryString = QString(
        "SELECT MIN(array_id) AS min_id, MAX(array_id) AS max_id "
        "FROM user_arrays WHERE user_id = %1"
        ).arg(user_id);

    if (!query.exec(queryString)) {
        qDebug() << "Ошибка получения диапазона ID:" << query.lastError().text();
        return -1; // Ошибка
    }

    if (!query.next()) {
        qDebug() << "Таблица user_arrays пуста!";
        return -1; // Пустая таблица
    }

    int minId = query.value("min_id").toInt();
    int maxId = query.value("max_id").toInt();

    // 2. Генерация случайного числа в диапазоне
    int randomId = QRandomGenerator::global()->bounded(minId, maxId + 1);

    // 3. Проверить существование ID (необязательно, если IDs непрерывны)
    QSqlQuery checkQuery(db);
    checkQuery.prepare("SELECT COUNT(*) FROM user_arrays WHERE array_id = :random_id");
    checkQuery.bindValue(":random_id", randomId);

    if (!checkQuery.exec()) {
        qDebug() << "Ошибка проверки существования ID:" << checkQuery.lastError().text();
        return -1; // Ошибка
    }

    if (checkQuery.next() && checkQuery.value(0).toInt() > 0) {
        return randomId; // Возвращаем случайный существующий ID
    } else {
        qDebug() << "Случайный ID не существует в таблице, попробуйте снова.";
        return getRandomArrayId(user_id); // Рекурсия для поиска нового случайного ID
    }
}

int db_manager::getMinArrayId(int user_id) {
    QSqlDatabase& db = databaseForThisThread();
    QSqlQuery query(db);
    query.prepare(
        "SELECT MIN(array_id) FROM user_arrays WHERE user_id = :uid"
        );
    query.bindValue(":uid", user_id);
    if (!query.exec() || !query.next())
        return -1;
    return query.value(0).toInt();
}

