#ifndef GLOBAL_H
#define GLOBAL_H
#include <QWidget>
#include <functional>
#include <QRegularExpression>
#include "QStyle"
#include <memory>
#include <iostream>
#include <mutex>
#include <QByteArray>
#include <QNetworkReply>
#include <QString>
/*********************************************************************
* @file       global.h
* @brief      用来刷新qss
*
* @author     Pear
* @date       2025/05/18
* @history
**********************************************************************/

extern std::function<void(QWidget*)> repolish;
extern QString gate_url_prefix;

enum ReqId{
  ID_GET_VARIFY_CODE =1001,//获取验证码
  ID_REG_USER = 1002,//注册用户
};

enum Modules{
    REGISTERMOD=0,
};

enum ErrorCodes{
    SUCCESS = 0,
    ERR_JSON = 1,//JSON解析失败
    ERR_NETWORK = 2,//网络错误
};

#endif // GLOBAL_H
