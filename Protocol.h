#ifndef PROTOCOL_H
#define PROTOCOL_H
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
typedef unsigned uint;

#define REGIST_OK "注册成功"
#define REGIST_FAILED "注册失败，名称重复"
#define ENROLL_OK "登陆成功"
#define ENROLL_FAILED "登陆失败"
#define SEARCH_USR_FAIL "用户不存在"
#define EXAM_FAIL  "考试不存在"
#define STUDENT_FAIL "学生不存在"
#define UPLOAD_READY "上传准备"
#define IMAGE_SAVE_FAIL "图片保存失败"

enum MSG_TYPE
{
    MSG_TYPE_MIN = 0,
    MSG_TYPE_ORDINARY_RESPOND,  // 普通回复

    MSG_TYPE_REGIST_REQUEST , // 注册请求

    MSG_TYPE_ENROLL_REQUEST , // 登陆请求
    MSG_TYPE_ENROLL_RESPOND , // 登陆回复

    MSG_TYPE_CHECK_STUDENT_REQUEST , // 学生信息请求
    MSG_TYPE_CHECK_STUDENT_RESPOND , // 学生信息回复

    MSG_TYPE_SEND_FILE_REQUEST , // 文件传输请求
    MSG_TYPE_SEND_FILE_RESPOND , // 文件传输回复

    MSG_TYPE_GET_EXAM_REQUEST , // 试卷请求
    MSG_TYPE_GET_EXAM_RESPOND , // 试卷回复

    MSG_TYPE_CHOOSE_REQUEST,  // 选择题请求
    MSG_TYPE_CHOOSE_RESPOND,  // 选择题回复

    MSG_TYPE_FILL_REQUEST,  // 填空题请求
    MSG_TYPE_FILL_RESPOND,  // 填空题回复

    MSG_TYPE_JUDGMENT_REQUEST,  // 判断题请求
    MSG_TYPE_JUDGMENT_RESPOND,  // 判断题回复

    MSG_TYPE_MAX = 0x00ffffff
};

struct PDU
{
    uint uiPDULen;  // 数据单元大小
    uint uiMsgType;  // 数据类型
    char caData[64]; // 用户信息
    uint uiMsgLen;  // 实际数据大小
    int caMsg[];  // 实际消息
};

inline PDU *mkPDU(uint uiMsgLen)
{
    uint uiPDULen = sizeof(PDU) + uiMsgLen;
    PDU *pdu = (PDU*)malloc(uiPDULen);
    if(pdu == NULL)
    {
        exit(EXIT_FAILURE);
    }
    memset(pdu, 0, uiPDULen); // 清空
    pdu->uiMsgLen = uiMsgLen;
    pdu->uiPDULen = uiPDULen;
    return pdu;
}

#endif // PROTOCOL_H
