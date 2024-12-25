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

// 1.完成
bool OpenDb::handleLogin(const QString &name, const QString &pwd, const QString &type)
{
    if (name.isEmpty() || pwd.isEmpty())
    {
        return false;
    }

    // 确定表名
    QString tableName = type.toLower();
    if (tableName != "teachers" && tableName != "admins") {
        qDebug() << "Invalid user type:" << type;
        return false;
    }

    // 查找用户
    QSqlQuery query;
    QString selectCmd = QString("SELECT * FROM %1 WHERE Account=? AND Password=?").arg(tableName); //  AND online=0
    query.prepare(selectCmd);
    query.addBindValue(name);
    query.addBindValue(pwd);


    if (!query.exec()) {
        qDebug() << "Database error:" << query.lastError().text();
        return false;
    }

    if (query.next()) return true;
    else return false;
}

void OpenDb::handleGetExams(const QString &year, const QString &semester, const QString &teacher, const QString &course,
                            const QString &data, const QString &end,  QList<QJsonObject> &exams)
{
    QString queryStr = "SELECT e.AcademicYear, e.Term, c.CourseName, e.ExamDate, e.EndTime, e.Examiner, "
                           "CASE WHEN e.EndTime < datetime('now') THEN 'Completed' ELSE 'Scheduled' END AS Status "
                           "FROM Exams e "
                           "JOIN Courses c ON e.CourseID = c.CourseID "
                           "JOIN Teachers t ON e.Examiner = t.Name "
                           "WHERE 1=1 ";
    qDebug() << queryStr;
    QStringList conditions;

    if (!year.isEmpty()) {
        conditions.append(QString("e.AcademicYear = '%1'").arg(year));
    }
    if (!semester.isEmpty()) {
        conditions.append(QString("e.Term = '%1'").arg(semester));
    }
    if (!teacher.isEmpty()) {
        conditions.append(QString("t.Name LIKE '%%1%'").arg(teacher));
    }
    if (!course.isEmpty()) {
        conditions.append(QString("c.CourseName LIKE '%%1%'").arg(course));
    }
    if (!data.isEmpty()) {
        conditions.append(QString("e.ExamDate >= '%1'").arg(data));
    }
    if (!end.isEmpty()) {
        conditions.append(QString("e.EndTime <= '%1'").arg(end));
    }

    if (!conditions.isEmpty()) {
        queryStr += " AND " + conditions.join(" AND ");
    }


    queryStr += " ORDER BY e.ExamDate, e.endTime";
    // qDebug() << "queryStr of getexams:" << queryStr;

    QSqlQuery query;
    if (!query.exec(queryStr)) {
        qDebug() << "Error executing query:" << query.lastError().text();
        return;
    }

    // 打印可用字段，用于调试
//    for (int i = 0; i < query.record().count(); ++i) {
//        qDebug() << "Field" << i << ":" << query.record().fieldName(i);
//    }

    while (query.next()) {
        QJsonObject exam;
        // exam["ExamID"] = query.value("ExamID").toInt();
        exam["AcademicYear"] = query.value("AcademicYear").toString();
        exam["Term"] = query.value("Term").toString();
        exam["CourseName"] = query.value("CourseName").toString();
        exam["ExamDate"] = query.value("ExamDate").toDate().toString(Qt::ISODate);
        // exam["StartTime"] = query.value("StartTime").toTime().toString(Qt::ISODate);
        exam["EndTime"] = query.value("EndTime").toTime().toString(Qt::ISODate);
        // exam["Room"] = query.value("Room").toString();
        exam["Examiner"] = query.value("Examiner").toString();
        // exam["Status"] = query.value("Status").toString();

//        qDebug() << exam;
        exams.append(exam);
    }
}

void OpenDb::handleGetStudents(const QString &major, const QString &name, const QString &StudentNumber, const QString &ClassName, QList<QJsonObject> &students)
{
    QString queryStr = "SELECT StudentNumber, Name, ClassName, MajorName "
                           "FROM Students "
                           "WHERE 1=1 ";
    qDebug() << queryStr;

    QStringList conditions;

    if (!major.isEmpty()) {
        conditions.append(QString("MajorName LIKE '%%1%'").arg(major));
    }
    if (!name.isEmpty()) {
        conditions.append(QString("Name LIKE '%%1%'").arg(name));
    }
    if (!StudentNumber.isEmpty()) {
        conditions.append(QString("StudentNumber = %1").arg(StudentNumber));
    }
    if (!ClassName.isEmpty()) {
        conditions.append(QString("ClassName LIKE '%%1%'").arg(ClassName));
    }

    if (!conditions.isEmpty()) {
        queryStr += " AND " + conditions.join(" AND ");
    }

    // queryStr += " ORDER BY EnrollmentDate";

    QSqlQuery query;
    if (!query.exec(queryStr)) {
        qDebug() << "Error executing query:" << query.lastError().text();
        return;
    }

    while (query.next()) {
        QJsonObject student;
        student["StudentNumber"] = query.value(0).toString();           // 使用字段索引
        student["Name"] = query.value(1).toString();             // 使用字段索引
        student["ClassName"] = query.value(2).toString();            // 使用字段索引
        student["MajorName"] = query.value(3).toString();            // 使用字段索引
//        student["Class"] = query.value(4).toString();            // 使用字段索引
//        student["EnrollmentDate"] = query.value(5).toDate().toString(Qt::ISODate); // 使用字段索引
//        student["Status"] = query.value(6).toString();           // 使用字段索引
        qDebug() << student;
        students.append(student);
    }
}

void OpenDb::handleGetGrades(const QString &StudentNumber, const QString &name, const QString &courseName, const QString &majorName, const QString &ClassName, QList<QJsonObject> &grades)
{
    QString queryStr = "SELECT s.StudentNumber, s.Name AS StudentName, c.CourseName, s.ClassName, s.MajorName, g.Score "
                           "FROM Grades g "
                           "JOIN Students s ON g.StudentID = s.StudentID "
                           "JOIN Courses c ON g.CourseID = c.CourseID "
                       "WHERE 1=1 ";

    QStringList conditions;

    if (!StudentNumber.isEmpty()) {
        conditions.append(QString("s.StudentNumber = %1").arg(StudentNumber));
    }
    if (!courseName.isEmpty()) {
        conditions.append(QString("c.CourseName LIKE '%%1%'").arg(courseName));
    }
    if (!majorName.isEmpty()) {
        conditions.append(QString("s.majorName = '%1'").arg(majorName));
    }
    if (!ClassName.isEmpty()) {
        conditions.append(QString("s.majorName = '%1'").arg(ClassName));
    }
    if (!name.isEmpty()) {
        conditions.append(QString("s.Name = '%1'").arg(name));
    }

    if (!conditions.isEmpty()) {
        queryStr += " AND " + conditions.join(" AND ");
    }

    // queryStr += " ORDER BY g.Term, g.Score DESC";

    QSqlQuery query;
    if (!query.exec(queryStr)) {
        qDebug() << "Error executing query:" << query.lastError().text();
        return;
    }
    // 打印可用字段，用于调试
    for (int i = 0; i < query.record().count(); ++i) {
        qDebug() << "Field" << i << ":" << query.record().fieldName(i);
    }
    while (query.next()) {
        QJsonObject grade;
        grade["StudentNumber"] = query.value(0).toString();          // 使用字段索引
        grade["StudentName"] = query.value(1).toString();         // 使用字段索引
        grade["CourseName"] = query.value(2).toString();    // 使用字段索引
        grade["ClassName"] = query.value(3).toString();          // 使用字段索引
        grade["CourseName"] = query.value(4).toString();     // 使用字段索引
        grade["Score"] = query.value(5).toDouble();          // 使用字段索引
        // grade["GradeLetter"] = query.value(7).toString();    // 使用字段索引
        qDebug() << grade;
        grades.append(grade);
    }
}

int OpenDb::handlePublishExam(const QString &courseName, const QDate &examDate,
                             const QTime &endTime, const QString &examiner)
{
    // 计算学年和学期
    QString academicYear = calculateAcademicYear(examDate);
    QString term = calculateTerm(examDate);

    // 获取课程ID
    QSqlQuery courseQuery;
    courseQuery.prepare("SELECT CourseID FROM Courses WHERE CourseName = :courseName");
    courseQuery.bindValue(":courseName", courseName);

    if (!courseQuery.exec()) {
        qDebug() << "Error executing course query:" << courseQuery.lastError().text();
        return 1;
    }

    int courseId = -1;
    if (courseQuery.next()) {
        courseId = courseQuery.value(0).toInt();
    } else {
        qDebug() << "Course not found:" << courseName;
        return 1;
    }

    // 验证老师
    QSqlQuery teacherQuery;
    teacherQuery.prepare("SELECT * FROM teachers WHERE name = :examiner");
    teacherQuery.bindValue(":examiner", examiner);

    if (!teacherQuery.exec()) {
        qDebug() << "Error executing teacher query:" << teacherQuery.lastError().text();
        return 2;
    }

    if (!teacherQuery.next()) {
        qDebug() << "Teacher not found:" << examiner;
        return 2;
    }

    // 插入考试信息
    QString room = "B305";
    QTime startTime(0, 0); // 确保小时和分钟部分正确初始化
    QSqlQuery examQuery;
    examQuery.prepare("INSERT INTO Exams (AcademicYear, Term, CourseID, ExamName, ExamDate, StartTime, EndTime, Room, Examiner) "
                      "VALUES (:academicYear, :term, :courseId, :examName, :examDate, :startTime, :endTime, :room, :examiner)");

    examQuery.bindValue(":academicYear", academicYear);
    examQuery.bindValue(":term", term);
    examQuery.bindValue(":courseId", courseId);
    examQuery.bindValue(":examName", courseName); // 假设考试名称与课程名称相同
    examQuery.bindValue(":examDate", examDate);
    examQuery.bindValue(":startTime", startTime);
    examQuery.bindValue(":endTime", endTime);
    examQuery.bindValue(":room", room);
    examQuery.bindValue(":examiner", examiner);

    if (!examQuery.exec()) {
        qDebug() << "Error inserting exam record:" << examQuery.lastError().text();
        return 3;
    }

    qDebug() << "Exam published successfully!";
    return 0;
}

// 获取符合指定条件的成绩记录
void OpenDb::handleGetScores(const QString &name, const QString &major, const QString &className,
                             const QString &studentNumber, const QString &course,
                             QList<QJsonObject> &scores)
{
    // 获取符合条件的学生ID列表
    QList<int> studentIds = getStudentIds(name, major, className, studentNumber);

    if (studentIds.isEmpty()) {
        qDebug() << "No scores found for the specified criteria.";
        return;
    }

    // 动态生成占位符
    QStringList placeholders;
    for (int i = 0; i < studentIds.size(); ++i) {
        placeholders.append(QString(":id%1").arg(i));
    }
    QString placeholderString = placeholders.join(",");

    // 构建成绩查询字符串
    QString scoreQueryStr = "SELECT s.ScoreID, s.StudentID, s.Score, "
                            "st.Name, st.MajorName, st.ClassName, st.StudentNumber "
                            "FROM Scores s "
                            "JOIN Students st ON s.StudentID = st.StudentID "
                            "WHERE s.StudentID IN (%1)";

    scoreQueryStr = scoreQueryStr.arg(placeholderString);

    QSqlQuery scoreQuery;
    scoreQuery.prepare(scoreQueryStr);

    // 绑定每个学生ID
    for (int i = 0; i < studentIds.size(); ++i) {
        scoreQuery.bindValue(placeholders[i], studentIds[i]);
    }

    qDebug() << "scoreQueryStr:" << scoreQueryStr;
    qDebug() << "Bound Values:" << studentIds;

    if (!scoreQuery.exec()) {
        qDebug() << "Error executing score query:" << scoreQuery.lastError().text();
        return;
    }

    scores.clear();
    while (scoreQuery.next()) {
        QJsonObject scoreObj;
        scoreObj["ScoreID"] = scoreQuery.value("ScoreID").toInt();
        scoreObj["StudentID"] = scoreQuery.value("StudentID").toInt();
        //scoreObj["Subject"] = scoreQuery.value("Subject").toString();
        scoreObj["Score"] = scoreQuery.value("Score").toDouble();
        scoreObj["Name"] = scoreQuery.value("Name").toString();
        scoreObj["Major"] = scoreQuery.value("MajorName").toString();
        scoreObj["ClassName"] = scoreQuery.value("ClassName").toString();
        scoreObj["StudentNumber"] = scoreQuery.value("StudentNumber").toString();

        scores.append(scoreObj);
        qDebug() << scoreObj["Score"];
    }

    if (scores.isEmpty()) {
        qDebug() << "No scores found for the specified student IDs.";
    } else {
        qDebug() << "Found scores:";
        for (const auto &score : scores) {
            qDebug() << score;
        }
    }

    qDebug() << "Scores retrieved successfully!";
}

bool OpenDb::handleDeleteExam(int examID)
{
    // 开始事务
    m_db.transaction();

    bool result = true;

    // 检查是否存在该考试记录
    QSqlQuery checkQuery(m_db);
    checkQuery.prepare("SELECT COUNT(*) FROM Exams WHERE ExamID = :examID");
    checkQuery.bindValue(":examID", examID);

    if (!checkQuery.exec()) {
        qDebug() << "Error executing check query:" << checkQuery.lastError().text();
        result = false;
    } else {
        int count = 0;
        if (checkQuery.next()) {
            count = checkQuery.value(0).toInt();
        }

        if (count == 0) {
            qDebug() << "Exam with ID" << examID << "does not exist.";
            result = false;
        }
    }

    // 删除考试记录
    if (result) {
        QSqlQuery deleteQuery(m_db);
        deleteQuery.prepare("DELETE FROM Exams WHERE ExamID = :examID");
        deleteQuery.bindValue(":examID", examID);

        if (!deleteQuery.exec()) {
            qDebug() << "Error deleting exam record:" << deleteQuery.lastError().text();
            result = false;
        }
    }

    // 提交或回滚事务
    if (result) {
        m_db.commit();
        qDebug() << "Exam deleted successfully!";
    } else {
        m_db.rollback();
        qDebug() << "Failed to delete exam.";
    }

    return result;
}

// 获取符合指定条件的学生ID列表
QList<int> OpenDb::getStudentIds(const QString &name, const QString &major,
                                 const QString &className, const QString &studentNumber)
{
    // 构建查询字符串
    QString queryStr = "SELECT StudentID FROM Students WHERE 1=1";
    QStringList conditions;
    QList<QVariant> values;

    if (!name.isEmpty()) {
        conditions.append("Name LIKE :name");
        values.append("%" + name + "%");
    }
    if (!major.isEmpty()) {
        conditions.append("MajorName LIKE :major");
        values.append("%" + major + "%");
    }
    if (!className.isEmpty()) {
        conditions.append("ClassName LIKE :className");
        values.append("%" + className + "%");
    }
    if (!studentNumber.isEmpty()) {
        conditions.append("StudentNumber LIKE :studentNumber");
        values.append("%" + studentNumber + "%");
    }

    if (!conditions.isEmpty()) {
        queryStr += " AND (" + conditions.join(" AND ") + ")";
    }

    QSqlQuery studentQuery;
    studentQuery.prepare(queryStr);

    for (int i = 0; i < conditions.size(); ++i) {
        QString paramName = conditions[i].split(' ').last();
        studentQuery.bindValue(paramName, values[i]);
    }

//    qDebug() << "queryStr:" << queryStr;
//    qDebug() << "Bound Values:" << values;

    if (!studentQuery.exec()) {
        qDebug() << "Error executing student query:" << studentQuery.lastError().text();
        return QList<int>();
    }

    QList<int> studentIds;
    while (studentQuery.next()) {
        int studentId = studentQuery.value(0).toInt();
        studentIds.append(studentId);
    }

    if (studentIds.isEmpty()) {
        qDebug() << "No students found with the given details.";
    } else {
        qDebug() << "Found students with IDs:" << studentIds;
    }

    return studentIds;
}

int OpenDb::handleGetExamID(const QString &academicYear, const QString &term, const QString &courseName,
                            const QDate &examDate, const QTime &endTime, const QString &examiner)
{
    // 获取课程ID
    QSqlQuery courseQuery;
    courseQuery.prepare("SELECT CourseID FROM Courses WHERE CourseName = :courseName");
    courseQuery.bindValue(":courseName", courseName);

    if (!courseQuery.exec()) {
        qDebug() << "Error executing course query:" << courseQuery.lastError().text();
        return -1;
    }

    int courseId = -1;
    if (courseQuery.next()) {
        courseId = courseQuery.value(0).toInt();
    } else {
        qDebug() << "Course not found:" << courseName;
        return -1;
    }

    // 查询 ExamID
    QSqlQuery examQuery;
    examQuery.prepare(
        "SELECT ExamID "
        "FROM Exams "
        "WHERE AcademicYear = :academicYear AND Term = :term AND CourseID = :courseId "
        "AND ExamDate = :examDate AND EndTime = :endTime AND Examiner = :examiner"
    );

    examQuery.bindValue(":academicYear", academicYear);
    examQuery.bindValue(":term", term);
    examQuery.bindValue(":courseId", courseId);
    examQuery.bindValue(":examDate", examDate.toString("yyyy-MM-dd")); // 确保日期格式正确
    examQuery.bindValue(":endTime", endTime.toString("hh:mm:ss"));     // 确保时间格式正确
    examQuery.bindValue(":examiner", examiner);

    if (!examQuery.exec()) {
        qDebug() << "Error executing exam query:" << examQuery.lastError().text();
        return -1;
    }

    qDebug() << "examQuery:" << examQuery.lastQuery(); // 输出完整的查询语句

    int examID = -1;
    if (examQuery.next()) {
        examID = examQuery.value(0).toInt();
    } else {
        qDebug() << "No matching exam found.";
    }

    return examID;
}

QString OpenDb::calculateAcademicYear(const QDate &date)
{
    int year = date.year();
    if (date.month() >= 9) {
        return QString("%1-%2").arg(year).arg(year + 1);
    } else {
        return QString("%1-%2").arg(year - 1).arg(year);
    }
}

QString OpenDb::calculateTerm(const QDate &date)
{
    int month = date.month();
    if (month >= 9 || month <= 12) {
        return "Fall";
    } else if (month >= 1 && month <= 5) {
        return "Spring";
    } else {
        return "Summer";
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

void OpenDb::handleSubmit(const char *name, const char *id, const char *path, int count)
{
    if(name == NULL || id == NULL || path == NULL){
        return;
    }
    QString cmd;
    switch (count) {
    case 0:{
        cmd = QString("update submit set choosePath=\'%1\' where examName=\'%2\' and studentId=%3").arg(path).arg(name).arg(id);
        break;
    }
    case 1:{
        cmd = QString("update submit set fillPath=\'%1\' where examName=\'%2\' and studentId=%3").arg(path).arg(name).arg(id);
        break;
    }
    case 2:{
        cmd = QString("update submit set judgmentPath=\'%1\' where examName=\'%2\' and studentId=%3").arg(path).arg(name).arg(id);
        break;
    }
    default:
        return;
    }
    qDebug() << cmd;
    QSqlQuery query;
    query.exec(cmd);
}

char *OpenDb::getPath(const char *examID, int type)
{
    char path[64] = {"\0"};
    int studentId = getStudentId(examID, type);
    if(studentId < 0) return "\0";
}

int OpenDb::getStudentId(const char *examID, int type)
{
    if(examID == NULL){
        return -1;
    }

    QString cmd;
    switch (type) {
    case 0:{
        cmd = QString("select studentID from score where examName=\'%1\' and choose=NULL").arg(examID);
        break;
    }
    case 1:{
        cmd = QString("select studentID from score where examName=\'%1\' and fill=NULL").arg(examID);
        break;
    }
    case 2:{
        cmd = QString("select studentID from score where examName=\'%1\' and judgment=NULL").arg(examID);
        break;
    }
    default:
        return -1;  // 参数错误，没有对应的试卷部分
    }
    qDebug() << cmd;
    QSqlQuery query;
    query.exec(cmd);

    if(query.next()){
        return query.value(0).toInt();
    }
    return -2;  // 没有未录取成绩的学生
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
