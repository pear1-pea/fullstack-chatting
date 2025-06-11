#include "mainwindow.h"
#include "global.h"

#include <QApplication>
#include <QFile>
#include <QSettings>
#include <QDir>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    // 读取配置文件
    QString app_path;
    if (QCoreApplication::applicationDirPath().contains(".app/Contents/MacOS")) {
        // macOS 下，配置文件在 Resources 目录
        app_path = QCoreApplication::applicationDirPath() + "/../Resources";
    } else {
        app_path = QCoreApplication::applicationDirPath();
    }
    
    QString config_path = QDir::toNativeSeparators(app_path + QDir::separator() + "config.ini");
    qDebug() << "Loading config from:" << config_path;
    
    QSettings settings(config_path, QSettings::IniFormat);
    QString gate_host = settings.value("GateServer/host").toString();
    QString gate_port = settings.value("GateServer/port").toString();
    gate_url_prefix = "http://" + gate_host + ":" + gate_port;
    qDebug() << "Gate URL prefix:" << gate_url_prefix;
    
    // 加载样式表
    QFile qss(":/style/stylesheet.qss");
    if(qss.open(QFile::ReadOnly)){
        qDebug("open success");
        QString style=QLatin1String(qss.readAll());
        a.setStyleSheet(style);
        qss.close();
    }else{
        qDebug("open failed");
    }
    
    MainWindow w;
    w.show();
    return a.exec();
}
