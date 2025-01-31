#include "db_manager.h"
#include <QCryptographicHash>
#include <QRandomGenerator>

QSqlDatabase db_manager::db;
bool db_manager::connectionInitialized = false;

db_manager::db_manager() {
    if (!connectionInitialized) {
        db = QSqlDatabase::addDatabase("QSQLITE", "my_connection");
        connectionInitialized = true;
    }
}

void db_manager::closeDatabase(){
    if (db.isOpen()){
        db.close();
        //вывод о закрытии соединения
    }
    QSqlDatabase::removeDatabase("my_connection");
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
    if (!db.isOpen()) {  // Проверка на открытость
        qDebug() << "Не удалось открыть соединение с базой данных.";
        return 1;
    }
    // Проверка: существует ли пользователь с таким именем
    QSqlQuery checkQuery(db);
    checkQuery.prepare("SELECT COUNT(*) FROM users WHERE username = :username");
    checkQuery.bindValue(":username", username);

    if (!checkQuery.exec() || !checkQuery.next()) {
        qDebug() << "Ошибка при проверке существования пользователя:" << checkQuery.lastError().text();
        return 2;
    }

    int count = checkQuery.value(0).toInt();
    if (count > 0) {
        qDebug() << "Пользователь с таким именем уже существует.";
        return 3;
    }
    QSqlQuery query(db);
    query.prepare("INSERT INTO users (username, user_password) Values (:username, :password)");
    query.bindValue(":username", username);
    query.bindValue(":password", password);
    if (!query.exec()) {
        qDebug() << "Ошибка выполнения запроса:" << query.lastError().text();
        return 4;
    }
    return 0;
}

int db_manager::insertArray(int user_id, const QString& array, const QString& sortedArray){
    QSqlQuery query(db);
    query.prepare("INSERT INTO user_arrays (user_id, unsorted_array, sorted_array) VALUES (:userID, :unsortedArray, :sortedArray)");
    query.bindValue(":userID", user_id);
    query.bindValue(":unsortedArray", array);
    query.bindValue(":sortedArray", sortedArray);
    if (!query.exec()) {
        qDebug() << "Ошибка выполнения запроса:" << query.lastError().text();
        return 1;  // Ошибка запроса
    }
    return 0;
}

db_manager::~db_manager(){
    closeDatabase();
}

bool db_manager::set_connection(DB_config Default_config){
    if (!QSqlDatabase::isDriverAvailable("QSQLITE")) {
        qDebug() << "QSQLITE драйвер не доступен!";
        return false;
    }
    db.setDatabaseName(Default_config.database_path);
    if (!db.open()){
        qDebug() << "Не удалось подключиться к базе данных:" << db.lastError().text();
        return false;
    }
    qDebug() << "Удалось подключиться к базе данных";
    return true;
}

QVariant db_manager::ExecuteSelectQuery_SingleData(const QString StrQuery, QString loginText){
    if (!db.isOpen()) {  // Проверка на открытость
        qDebug() << "Не удалось открыть соединение с базой данных.";
        return QVariant();
    }
    QSqlQuery query (db);
    query.prepare(StrQuery);
    query.bindValue(":username", loginText);

    if (!query.exec()){
        //err mess
        return QVariant();
    }
    if (query.next()){
        return query.value(0).toString();
    }
    else{
        // no res mess
        return QVariant();
    }
}

QVariant db_manager::ExecuteSelectQuery_SingleArray(const QString StrQuery, int arg, int user_id){
    if (!db.isOpen()) {  // Проверка на открытость
        qDebug() << "Не удалось открыть соединение с базой данных.";
        return QVariant();
    }
    QSqlQuery query (db);
    query.prepare(StrQuery);
    query.bindValue(":array", arg);
    query.bindValue(":user_id", user_id);
    if (!query.exec()){
        //err mess
        qDebug() << "Ошибка выполнения запроса:" << query.lastError();
        return QVariant();
    }
    if (query.next()){
        return query.value(0).toString();
    }
    else{
        // no res mess
        return QVariant();
    }
}

QVector<QVector<QVariant>> db_manager::ExecuteSelectQuery_allUserData(const QString StrQuery)
{
    QVector<QVector<QVariant>> resultMatrix;
    if (!db.isOpen()) {  // Проверка на открытость
        qDebug() << "Не удалось открыть соединение с базой данных.";
        return resultMatrix;
    }
    QSqlQuery query (db);
    query.prepare(StrQuery);


    if(!query.exec())
    {
        //error message
        return resultMatrix;
    }

    while(query.next())
    {
        QVector<QVariant> row;
        for (int i = 0; i < query.record().count(); ++i) {
            row.append(query.value(i));  // Добавляем значение столбца в строку
        }
        resultMatrix.append(row);  // Добавляем строку в матрицу
    }
    return resultMatrix;
}
QVector<QVariant> db_manager::ExecuteSelectQuery_UserHashPassword(const QString StrQuery)
{
    QVector<QVariant> result;
    if (!db.isOpen()) {  // Проверка на открытость
        qDebug() << "Не удалось открыть соединение с базой данных.";
        return result;
    }
    QSqlQuery query (db);
    query.prepare(StrQuery);

    if(!query.exec())
    {
        //error message
        return result;
    }

    while(query.next())
    {
        for (int i = 0; i < query.record().count(); ++i) {
            result.append(query.value(i));
        }
    }
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

    if (!db.isOpen()) {
        qDebug() << "База данных не открыта!";
        return arrays;
    }
    QSqlQuery query (db);
    query.prepare("SELECT unsorted_array, sorted_array FROM user_arrays WHERE user_id = :userID");
    query.bindValue(":userID", userID);

    if(!query.exec()){
        qDebug() << "Ошибка выполнения запроса: " << query.lastError().text();
        return arrays;
    }
    while (query.next()){
        QString unsorted = query.value(0).toString();
        QString sorted = query.value(1).toString();
        arrays.append(qMakePair(unsorted,sorted));
    }
    return arrays;

}

void db_manager::updateArray_query(const QString StrQuery, const QString unsortedArray, const QString sortedArray, int arrayID, int userID){
    if (!db.isOpen()) {
        qDebug() << "База данных не открыта!";
        return ;
    }
    QSqlQuery query (db);
    query.prepare(StrQuery);
    query.bindValue(":unsortedArray", unsortedArray);
    query.bindValue(":sortedArray", sortedArray);
    query.bindValue(":arrayID", arrayID);
    query.bindValue(":userID", userID);
    if(!query.exec())
    {
        //error message
        return;
    }
    else {
        qDebug() << "Данные успешно обновлены!";
    }
    return;
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
    if (!db.isOpen()) {  // Проверка на открытость
        qDebug() << "Не удалось открыть соединение с базой данных.";
        return false;
    }
    QSqlQuery query(db);
    int testUserId = 2; // TEST_ID

    QString deleteQuery = QString(
                              "DELETE FROM user_arrays "
                              "WHERE array_id IN ("
                              "    SELECT array_id "
                              "    FROM user_arrays "
                              "    WHERE user_id = %1 " // TEST_ID
                              "    ORDER BY array_id ASC "
                              "    LIMIT %2"
                              ")"
                              ).arg(testUserId).arg(recordCount);
    if (!query.exec(deleteQuery)) {
        qDebug() << "Ошибка удаления данных:" << query.lastError().text();
        return false;
    }

    return true;
}


int db_manager::getRandomArrayId(int user_id) {
    // 1. Найти минимальный и максимальный ID
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

int db_manager::getMinArrayId(int user_id)
{
    // 1. Найти минимальный и максимальный ID
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

    return minId;

}


