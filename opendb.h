#ifndef OPENDB_H
#define OPENDB_H

#include <QObject>
#include<QSqlDatabase>
#include<QSqlQuery>
#include<QDebug>
#include<QStringList>

class OpenDb : public QObject
{
    Q_OBJECT
public:
    static OpenDb& getInstance();
    void init();
    bool handleRegist(const char* name, const char* pwd);
    bool handleEnroll(const char* name, const char* pwd);
    void handleOffLine(const char* name);
    bool handleCheckExam(const char* name);
    bool handleCheckStudent(const char* name);
    bool handleCheckSubmit(const char* name, const char* id);

signals:

private:
    explicit OpenDb(QObject *parent = nullptr);


    QSqlDatabase m_db; // 连接数据库

};

#endif // OPENDB_H
