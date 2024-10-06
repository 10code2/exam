#include "opendb.h"

OpenDb::OpenDb(QObject *parent) : QObject(parent)
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    init();
}

bool OpenDb::handleRegist(const char *name, const char *pwd)
{
    if(name == NULL || pwd == NULL)
    {
        return false;
    }
    QString cmd = QString("insert into teacher(teacherId, pwd) values(%1, \'%2\')").arg(name).arg(pwd);
    qDebug() << cmd;
    QSqlQuery query;
    return query.exec(cmd);
}

bool OpenDb::handleEnroll(const char *name, const char *pwd)
{
    if(name == NULL || pwd == NULL)
    {
        return false;
    }
    // 查找用户
    QString cmd = QString("select * from teacher where teacherId=%1 and pwd=\'%2\' and online=0").arg(name).arg(pwd);
    QSqlQuery query;
    query.exec(cmd);
    // 跟新用户状态
    if(query.next())
    {
        cmd = QString("update teacher set online=1 where teacherId=%1 and pwd=\'%2\'").arg(name).arg(pwd);
        qDebug() << cmd;
        query.exec(cmd);
        return true;
    }
    else
    {
        return false;
    }
}

void OpenDb::handleOffLine(const char *name)
{
    if(name == NULL)
    {
        qDebug() << "name is blank";
        return;
    }
    else
    {
        QString cmd = QString("update teacher set online=0 where teacherId=%1").arg(name);
        qDebug() << cmd;
        QSqlQuery query;
        query.exec(cmd);
    }
}

OpenDb& OpenDb::getInstance()
{
    static OpenDb instance;
    return instance;
}

void OpenDb::init()
{
    m_db.setHostName("localhost");
    m_db.setDatabaseName("D:\\lan\\ExamSystemServer\\cloud.db");
    if(!m_db.open())
    {
        qDebug() << "打开数据库失败";
    }
}
