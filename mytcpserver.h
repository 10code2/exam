#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include <QObject>
#include<QTcpServer>
#include<QDebug>
#include<QList>
#include "mytcpsocket.h"

class MyTcpServer : public QTcpServer
{
    Q_OBJECT
public:
    static MyTcpServer& getInstance();

    void incomingConnection(qintptr socketDescriptor);


signals:

public slots:
    void deleteSocket(MyTcpSocket* addr);

private:
    explicit MyTcpServer(QObject *parent = nullptr);

    QList<MyTcpSocket*> m_tcpSocketList;  // 记录每一个连接
};

#endif // MYTCPSERVER_H
