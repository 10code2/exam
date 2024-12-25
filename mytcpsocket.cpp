#include "mytcpsocket.h"
#include "Protocol.h"
#include "opendb.h"
#include "CommunicationProtocol.h"

MyTcpSocket::MyTcpSocket(QObject *parent) : QTcpSocket(parent)
{
    cName[0] = '\0';
    connect(this, SIGNAL(readyRead()), this, SLOT(recvMsg()));
    connect(this, SIGNAL(disconnected()), this, SLOT(clientOffLine()));
}

void MyTcpSocket::clientOffLine() // 发出用户离线信号
{
    qDebug()<<"用户strName退出";
    OpenDb::getInstance().handleOffLine(cName);
    emit offline(this);  // 发送信号
}

void MyTcpSocket::handleLogin(const QJsonObject &request, QTcpSocket *socket)
{
    // 1 读取信息
    CommunicationProtocol::LoginRequest loginRequest;
    loginRequest = CommunicationProtocol::LoginRequest::fromJson(request);
    qDebug() << "LoginRequest:" << loginRequest.username << "," << loginRequest.userType;

    // 2 这里应该验证用户名和密码是否正确
    if (OpenDb::getInstance().handleLogin(loginRequest.username, loginRequest.password, loginRequest.userType)) {
        CommunicationProtocol::LoginResponse response;
        response.success = true;
        response.userId = loginRequest.username;
        response.userType = loginRequest.userType;
        sendResponse(socket, response.toJson());
    } else {
        sendErrorResponse(socket, "用户名或密码错误");
    }
}

void MyTcpSocket::handleGetExams(const QJsonObject &request)
{
    // 1 读取信息
    CommunicationProtocol::GetExamsRequest getExamsRequest;
    getExamsRequest = CommunicationProtocol::GetExamsRequest::fromJson(request);
    qDebug() << "LoginRequest:" << getExamsRequest.course << "," << getExamsRequest.teacher;

    // 2 处理消息
    QList<QJsonObject> exams;
    OpenDb::getInstance().handleGetExams(getExamsRequest.academicYear, getExamsRequest.semester, getExamsRequest.teacher,
                                         getExamsRequest.course, getExamsRequest.data, getExamsRequest.end, exams);

    // 3 回复
    CommunicationProtocol::GetExamsResponse response;
    response.success = true;
    response.exams = exams;
    qDebug() << "exams num: " << response.exams.size();
    sendResponse(this, response.toJson());
}

void MyTcpSocket::handleCreateExam(const QJsonObject &request)
{
    // 1 读取信息
    CommunicationProtocol::CreateExamRequest createExamsRequest;
    createExamsRequest = CommunicationProtocol::CreateExamRequest::fromJson(request);
    qDebug() << "createExamsRequest:" << createExamsRequest.course << "," << createExamsRequest.teacher;

    // 2 使用 ISO 标准格式（YYYY-MM-DD）将 QString 转换为 QDate
    QDate date = QDate::fromString(createExamsRequest.data, "yyyy-MM-dd");
    if (date.isValid()) {
        // 日期转换成功
        qDebug() << "Date is:" << date.toString("yyyy-MM-dd");
    } else {
        // 日期转换失败
        qDebug() << "Invalid date string";
        sendErrorResponse(this, "日期格式不对");
        return;
    }

    // 3 使用 HH:mm:ss 格式将 QString 转换为 QTime
    QTime time = QTime::fromString(createExamsRequest.end, "HH:mm");

    if (time.isValid()) {
        // 时间转换成功
        qDebug() << "Time is:" << time.toString("HH:mm:ss");
    } else {
        // 时间转换失败
        qDebug() << "Invalid time string";
        sendErrorResponse(this, "时间格式不对");
        return;
    }

    // 4 这里发布
    int ret = OpenDb::getInstance().handlePublishExam(createExamsRequest.course, date, time, createExamsRequest.teacher);
    if (!ret) {
        CommunicationProtocol::SuccessResponse response;
        response.action = CommunicationProtocol::CREATE_EXAM;
        response.successMessage = "发布考试成功";
        sendResponse(this, response.toJson());
    } else {
        if(ret == 1) sendErrorResponse(this, "课程不存在");
        if(ret == 2) sendErrorResponse(this, "老师不存在");
        if(ret == 3) sendErrorResponse(this, "发布考试失败");
    }
}

void MyTcpSocket::handleDeleteExam(const QJsonObject &request)
{
    // 1 读取信息
    CommunicationProtocol::DeleteExamRequest deleteExamsRequest;
    deleteExamsRequest = CommunicationProtocol::DeleteExamRequest::fromJson(request);
    qDebug() << "handleDeleteExam:" << deleteExamsRequest.course << "," << deleteExamsRequest.teacher;

    // 2 使用 ISO 标准格式（YYYY-MM-DD）将 QString 转换为 QDate
    QDate date = QDate::fromString(deleteExamsRequest.data, "yyyy-MM-dd");
    if (date.isValid()) {
        // 日期转换成功
        qDebug() << "Date is:" << date.toString("yyyy-MM-dd");
    } else {
        // 日期转换失败
        qDebug() << "Invalid date string";
        sendErrorResponse(this, "日期格式不对");
        return;
    }

    // 3 使用 HH:mm:ss 格式将 QString 转换为 QTime
    QTime time = QTime::fromString(deleteExamsRequest.end, "HH:mm:ss");

    if (time.isValid()) {
        // 时间转换成功
        qDebug() << "Time is:" << time.toString("HH:mm:ss");
    } else {
        // 时间转换失败
        qDebug() << "Invalid time string";
        sendErrorResponse(this, "时间格式不对");
        return;
    }

    // 4 这里删除
    QString room = "B3305";
    int examID = OpenDb::getInstance().handleGetExamID(deleteExamsRequest.academicYear, deleteExamsRequest.semester, deleteExamsRequest.course,
                                    date, time, deleteExamsRequest.teacher);
    if(examID == -1){
        // 考试不存在
        qDebug() << "Invalid time string";
        sendErrorResponse(this, "考试不存在");
        return;
    }
    qDebug() << "examID = " << examID;
    if (OpenDb::getInstance().handleDeleteExam(examID)) {
        CommunicationProtocol::SuccessResponse response;
        response.action = CommunicationProtocol::CREATE_EXAM;
        response.successMessage = "删除考试成功";
        sendResponse(this, response.toJson());
    } else {
        sendErrorResponse(this, "删除考试失败");
    }
}

void MyTcpSocket::handleGetScores(const QJsonObject &request)
{
    // 1 读取信息
    CommunicationProtocol::GetScoresRequest getScoresRequest;
    getScoresRequest = CommunicationProtocol::GetScoresRequest::fromJson(request);
    qDebug() << "LoginRequest:" << getScoresRequest.name << "," << getScoresRequest.major;

    // 2 处理消息
    QList<QJsonObject> scores;
    OpenDb::getInstance().handleGetScores(getScoresRequest.name, getScoresRequest.major,getScoresRequest.className,
                                          getScoresRequest.number, getScoresRequest.course, scores);

    // 3 回复
    CommunicationProtocol::GetScoresResponse response;
    response.success = true;
    response.scores = scores;
    qDebug() << "students num: " << response.scores.size();
    sendResponse(this, response.toJson());
}

void MyTcpSocket::sendResponse(QTcpSocket *socket, const QJsonObject &response)
{
    QJsonDocument jsonDoc(response);
    QByteArray jsonData = jsonDoc.toJson();
    socket->write(jsonData);
    socket->flush();
}

void MyTcpSocket::sendErrorResponse(QTcpSocket *socket, const QString &errorMessage)
{
    CommunicationProtocol::ErrorResponse response;
    response.errorMessage = errorMessage;
    sendResponse(socket, response.toJson());
}

void MyTcpSocket::recvMsg()
{
    QByteArray data = this->readAll();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data);

    if (jsonDoc.isNull()) {
        qWarning() << "Failed to parse JSON data:" << data;
        return;
    }

    QJsonObject jsonObj = jsonDoc.object();

    // 提取 action 字段
    int action = jsonObj.value("action").toInt(-1); // 默认值设为 -1 表示未知动作

    switch(action){
    case CommunicationProtocol::LOGIN: {
        handleLogin(jsonObj, this);
        break;
    }
    case CommunicationProtocol::GET_EXAMS: {
        handleGetExams(jsonObj);
        break;
    }
    case CommunicationProtocol::ADD_TEACHER: {
        // handleAddTeacher(jsonObj);
        break;
    }
    case CommunicationProtocol::CREATE_EXAM: {
        // qDebug() << "CREATE_EXAM 请求";
        handleCreateExam(jsonObj);
        break;
    }
    case CommunicationProtocol::DELETE_EXAM: {
        // qDebug() << "DELETE_EXAM 请求";
        handleDeleteExam(jsonObj);
        break;
    }
    case CommunicationProtocol::GET_SCORES: {
        // qDebug() << "DELETE_EXAM 请求";
        handleGetScores(jsonObj);
        break;
    }
    // 添加其他动作的处理分支
    default:
        qWarning() << "Unhandled action:" << action;
        break;
    }
}
