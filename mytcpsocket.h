#ifndef MYTCPSOCKET_H
#define MYTCPSOCKET_H

#include <QObject>
#include<QTcpSocket>
#include<QFile>
#include<QBuffer>
#include<QImageReader>

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
};

#endif // MYTCPSOCKET_H
