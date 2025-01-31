#ifndef DB_MANAGER_H
#define DB_MANAGER_H
#include <QApplication>
#include <QCoreApplication>
#include <QDebug>
#include <QtSql>
#include <QSqlQuery>
#include <QSqlDatabase>

struct DB_config{
    QString database_path = "C:/SQLite/my_database2.db";
};
class db_manager
{
public: // methods
    db_manager();
    ~db_manager();
    void closeDatabase();
    int insertUser(const QString& username, const QString& password);
    bool set_connection (DB_config Default_config);

public: //functions
    QVector<QVector<QVariant>> ExecuteSelectQuery_allUserData(const QString StrQuery);
    QVariant ExecuteSelectQuery_SingleData(const QString StrQuery, QString loginText);
    QVector<QString> convertToVec (QString input);
    QString convertToString (QVector<QString> input);
    int insertArray(int user_id, const QString& username, const QString& password);
    QVector<QPair<QString, QString>> getUserArrays(int userID);
    QVariant ExecuteSelectQuery_SingleArray(const QString StrQuery, int arg, int user_id);
    void updateArray_query(const QString StrQuery, const QString unsortedArray, const QString sortedArray, int arrayID, int userID);
    std::vector<std::string> default_data();
    bool clearDatabase(int recordCount);
    int getRandomArrayId(int user_id);
    int getMinArrayId(int user_id);
    QByteArray hashPassword(const QString &password, const QByteArray &salt);
    QVector<QVariant> ExecuteSelectQuery_UserHashPassword(const QString StrQuery);
    QByteArray generateSalt();
private:
    static QSqlDatabase db;
    static bool connectionInitialized;

};

#endif // DB_MANAGER_H
