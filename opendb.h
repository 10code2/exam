#ifndef OPENDB_H
#define OPENDB_H

#include <QObject>
#include<QSqlDatabase>
#include<QSqlQuery>
#include <QSqlError>
#include<QDebug>
#include <QDateTime>
#include<QStringList>
#include<QString>
#include<QList>
#include <QSqlRecord>
#include<QJsonObject>

class OpenDb : public QObject
{
    Q_OBJECT
public:
    static OpenDb& getInstance();
    void init();
    bool handleRegist(const char* name, const char* pwd);
    bool handleLogin(const QString &name, const QString &pwd, const QString &type);
    void handleGetExams(const QString &year, const QString &semester, const QString &teacher, const QString &course, const QString &data, const QString &end, QList<QJsonObject> &exams);
    void handleGetStudents(const QString &major, const QString &name, const QString &StudentNumber, const QString &ClassName, QList<QJsonObject> &students);
    void handleGetGrades(const QString &StudentNumber, const QString &name, const QString &courseName, const QString &majorName, const QString &ClassName, QList<QJsonObject> &grades);
    int handlePublishExam(const QString &courseName, const QDate &examDate,
                           const QTime &endTime, const QString &examiner);
    bool handleDeleteExam(int examID);
    int handleGetExamID(const QString &academicYear, const QString &term, const QString &courseName,
                                const QDate &examDate, const QTime &endTime, const QString &examiner);

    QList<int> getStudentIds(const QString &name, const QString &major,
                                     const QString &className, const QString &studentNumber);

    void handleGetScores(const QString &name, const QString &major, const QString &className,
                         const QString &studentNumber, const QString &course,
                                QList<QJsonObject> &scores);

    // 辅助函数：计算学年
    QString calculateAcademicYear(const QDate &date);
    // 辅助函数：计算学期
    QString calculateTerm(const QDate &date);

    void handleOffLine(const char* name);
    bool handleCheckExam(const char* name);
    bool handleCheckStudent(const char* name);
    bool handleCheckSubmit(const char* name, const char* id);
    void handleSubmit(const char* name, const char* id, const char* path, int count);
    int getStudentId(const char* examID, int type);
    char* getPath(const char* examID, int type);

signals:

private:
    explicit OpenDb(QObject *parent = nullptr);


    QSqlDatabase m_db; // 连接数据库

};

#endif // OPENDB_H
