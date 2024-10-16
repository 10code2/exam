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

bool OpenDb::handleCheckExam(const char *name)
{  // 检查考试是否存在
    if(name == NULL)
    {
        qDebug() << "name is blank";
        return false;
    }
    else
    {
        QString cmd = QString("select * from exam where name=%1").arg(name);
        qDebug() << cmd;
        QSqlQuery query;
        query.exec(cmd);
        if(query.next())
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}

bool OpenDb::handleCheckStudent(const char *name)
{ // 检查学生是否存在，应该改为学生是否在考试表里
    if(name == NULL)
    {
        qDebug() << "name is blank";
        return false;
    }
    else
    {
        QString cmd = QString("select * from student where name=%1").arg(name);
        qDebug() << cmd;
        QSqlQuery query;
        query.exec(cmd);
        if(query.next())
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}

bool OpenDb::handleCheckSubmit(const char *name, const char *id)
{ // 提交表应该提前登记好考试学生信息
    if(name == NULL || id == NULL){
        return false;
    }
    else{
        QString cmd = QString("select * from submit where examName=\'%1\' and studentId=%2").arg(name).arg(id);
        qDebug() << cmd;
        QSqlQuery query;
        query.exec(cmd);
        if(query.next())
        {
            return true;
        }
        else
        {
            return false;
        }
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
