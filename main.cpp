#include "server.h"
#include "opendb.h"
#include <QDebug>
#include <QApplication>
#include<QList>
#include<QJsonObject>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Server w;
    w.show();

//    int examIDToDelete = 3; // 假设要删除的考试ID为1

//    QList<QJsonObject> success;
//    OpenDb::getInstance().handleGetScores("", "软件工程", "", "", success);
//    if (success.size()) {
//        qDebug() << "Exam deleted successfully!\n";
//    } else {
//        qDebug() << "Failed to delete exam.";
//    }

    return a.exec();
}
