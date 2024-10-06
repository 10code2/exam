#include "mytcpsocket.h"
#include "Protocol.h"
#include "opendb.h"

MyTcpSocket::MyTcpSocket(QObject *parent) : QTcpSocket(parent)
{
    cName[32] = {'\0'};
    connect(this, SIGNAL(readyRead()), this, SLOT(recvMsg()));
    connect(this, SIGNAL(disconnected()), this, SLOT(clientOffLine()));
}

void MyTcpSocket::clientOffLine() // 发出用户离线信号
{
    qDebug()<<"用户strName退出";
    OpenDb::getInstance().handleOffLine(cName);
    emit offline(this);  // 发送信号
}

void MyTcpSocket::recvMsg()  // 接受socket数据
{
    uint uiPDULen = this->bytesAvailable();  // 可接受信息大小
    // 接收PDU数据包
    this->read((char*)&uiPDULen, sizeof (uint));
    uint uiMsgLen = uiPDULen - sizeof(PDU);
    PDU* pdu = mkPDU(uiMsgLen);
    this->read((char*)pdu + sizeof (uint), uiPDULen - sizeof(uint));

    switch (pdu->uiMsgType) {
    case MSG_TYPE_REGIST_REQUEST:
    {
        char name[32] = {'\0'};
        char pwd[32] = {'\0'};
        strncpy(name, pdu->caData, 32);
        strncpy(pwd, pdu->caData + 32, 32);

        bool ret = OpenDb::getInstance().handleRegist(name, pwd);
        PDU *resPDU = mkPDU(0);
        resPDU->uiMsgType = MSG_TYPE_ORDINARY_RESPOND;
        if(ret)
        {
            strcpy(resPDU->caData, REGIST_OK);
        }
        else
        {
            strcpy(resPDU->caData, REGIST_FAILED);
        }
        write((char*)resPDU, resPDU->uiPDULen);
        free(resPDU);
        resPDU = NULL;
        break;
    }
    case MSG_TYPE_ENROLL_REQUEST:
    {
        char pwd[32] = {'\0'};
        strncpy(cName, pdu->caData, 32);
        strncpy(pwd, pdu->caData + 32, 32);

        bool ret = OpenDb::getInstance().handleEnroll(cName, pwd);
        PDU *resPDU = mkPDU(0);
        resPDU->uiMsgType = MSG_TYPE_ENROLL_RESPOND;
        if(ret)
        {
            strcpy(resPDU->caData, ENROLL_OK);
        }
        else
        {
            strcpy(resPDU->caData, ENROLL_FAILED);
        }
        write((char*)resPDU, resPDU->uiPDULen);
        free(resPDU);
        resPDU = NULL;
        break;
    }

    default: break;
    }
    free(pdu);
    pdu = NULL;
}
