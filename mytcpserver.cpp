#include "mytcpserver.h"


MyTcpServer::MyTcpServer(QObject *parent) : QTcpServer(parent)
{

}

MyTcpServer& MyTcpServer::getInstance()
{
    static MyTcpServer instance;
    return instance;
}

void MyTcpServer::incomingConnection(qintptr socketDescriptor)
{
    qDebug() << "a new connect";
    MyTcpSocket *ptcpSocket = new MyTcpSocket;
    ptcpSocket->setSocketDescriptor(socketDescriptor); // 将现有的套接字描述符关联到QTcpSocket对象上
    m_tcpSocketList.append(ptcpSocket);

    connect(ptcpSocket, SIGNAL(offline(MyTcpSocket*))
            ,this, SLOT(deleteSocket(MyTcpSocket*)));
}

void MyTcpServer::deleteSocket(MyTcpSocket* addr)
{
    QList<MyTcpSocket*>::iterator iter = m_tcpSocketList.begin();
    while(iter != m_tcpSocketList.end())
    {
        if(*iter == addr)
        {
            delete *iter;
            m_tcpSocketList.erase(iter);
            *iter = NULL;
            break;
        }
    }
}
