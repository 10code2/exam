#ifndef MYTCPSOCKET_H
#define MYTCPSOCKET_H

#include <QObject>
#include<QTcpSocket>
#include<QFile>
#include<QBuffer>
#include<QImageReader>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include <QList>


class MyTcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit MyTcpSocket(QObject *parent = nullptr);

signals:
    void offline(MyTcpSocket* addr);

private slots:
    void recvMsg();
    void clientOffLine();

private:
    char cName[32];
    char examId[32] = {'\0'};
    char studentId[32] = {'\0'};
    int count = 0;

private:
    void handleLogin(const QJsonObject &request, QTcpSocket *socket);
    void handleGetExams(const QJsonObject &request);
    void handleCreateExam(const QJsonObject &jsonObj);
    void handleDeleteExam(const QJsonObject &jsonObj);
    void handleGetScores(const QJsonObject &request);


    void sendResponse(QTcpSocket *socket, const QJsonObject &response);
    void sendErrorResponse(QTcpSocket *socket, const QString &errorMessage);
};

#endif // MYTCPSOCKET_H
