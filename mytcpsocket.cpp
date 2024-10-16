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
        strncpy(cName, pdu->caData, 32);  // 记录账号用于区分连接
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
    case MSG_TYPE_CHECK_STUDENT_REQUEST:{
        char ID[32] = {'\0'};
        char name[32] = {'\0'};
        strncpy(ID, pdu->caData, 32);
        strncpy(name, pdu->caData + 32, 32);

        PDU *resPDU = mkPDU(0);
        resPDU->uiMsgType = MSG_TYPE_ORDINARY_RESPOND;
        // 检查考试和学生信息是否存在
        if(!OpenDb::getInstance().handleCheckExam(ID)){
            strcpy(resPDU->caData, EXAM_FAIL);
        }
        else if(!OpenDb::getInstance().handleCheckSubmit(name, ID)){
            strcpy(resPDU->caData, STUDENT_FAIL);
        }
        else{
            resPDU->uiMsgType = MSG_TYPE_CHECK_STUDENT_RESPOND;
            strcpy(resPDU->caData, UPLOAD_READY);
        } // 重复提交?

        write((char*)resPDU, resPDU->uiPDULen);
        free(resPDU);
        resPDU = NULL;
        break;
    }
    case MSG_TYPE_SEND_FILE_REQUEST:  // 接受试卷文件
    {
        char path[64] = {'\0'};
        strcpy(path, pdu->caData);  // 得到路径
        qDebug() << "文件路径:" << path;

        // 把文件保存
        QByteArray receData((char*)(pdu->caMsg), pdu->uiMsgLen);//创建接收字节流

        QBuffer receBuffer(&receData);//
        QImageReader reader(&receBuffer,"png");
        QImage receImage=reader.read();

        PDU *resPDU = mkPDU(0);
        if(receImage.save(path)){
            resPDU->uiMsgType = MSG_TYPE_CHECK_STUDENT_RESPOND;
        }
        else{
            resPDU->uiMsgType = MSG_TYPE_ORDINARY_RESPOND;
            strcpy(resPDU->caData, IMAGE_SAVE_FAIL);
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
