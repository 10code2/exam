#include "server.h"
#include "ui_server.h"
#include "mytcpserver.h"

Server::Server(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Server)
{
    ui->setupUi(this);
    loadConfig();   // 加载配置文件
    // qDebug() << "配置文件：" << m_strIp << m_usPort;

    // 用重载后的TcpServer来监听
    MyTcpServer::getInstance().listen(QHostAddress(m_strIp), m_usPort);

}

Server::~Server()
{
    delete ui;
}

void Server::loadConfig()
{
    QFile file("://server.config");
    if(file.open(QIODevice::ReadOnly))
    {
        QByteArray Ba = file.readAll();
        file.close();
        QString Str = Ba.toStdString().c_str();
        Str.replace("\r\n", " ");
        QStringList SL = Str.split(' ');

        m_strIp = SL.at(0);
        m_usPort = SL.at(1).toUShort();
    }
    else
    {
        QMessageBox::critical(this, "打开配置文件", "打开配置文件失败");
    }
}

