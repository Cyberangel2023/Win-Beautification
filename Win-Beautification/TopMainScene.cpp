#include "TopMainScene.h"

#include <QGuiApplication>
#include <QScreen>
#include <windows.h>
#include <shlobj.h>  // 包含SHGetKnownFolderPath所需的头文件
#include <wrl/client.h> // 用于COM接口的智能指针
#include <shellapi.h>
#include <QProcess>
#include <QSystemTrayIcon>
#include <QPushButton>

TopMainScene::TopMainScene(MainScene* mainScene, QWidget *parent)
    : QWidget(parent), mainScene(mainScene)
{
    // 获取屏幕尺寸
    this->screen = QGuiApplication::primaryScreen();
    this->screenRect = this->screen->geometry();
    this->availableRect = screen->availableGeometry();

    QRect getRect = screenRect;
    getRect.setRect(screenRect.x(), screenRect.y(), screenRect.width() + 20, screenRect.height() + 20);
    this->resize(getRect.size());
    this->move(0, 0);
    this->setWindowFlag(Qt::FramelessWindowHint); // 去除窗口边框
    this->setAttribute(Qt::WA_TranslucentBackground); // 去除窗口
    this->setWindowFlags(this->windowFlags() | Qt::Tool | Qt::WindowStaysOnTopHint); // 任务栏不显示，置顶窗口

    // 创建按钮
    QPushButton *button = new QPushButton("", this);
    button->setGeometry(this->screenRect.width() - 12, this->screenRect.height() - 48, 20, 60); // 设置按钮位置和大小
    // 设置按钮的样式
    button->setStyleSheet(
        "QPushButton {"
        "   background-color: rgba(255, 255, 255, 2); /* 白色背景，透明度1 */"
        "   border-radius: 0px; /* 圆角边框 */"
        "}"
        "QPushButton:hover {"
        "   background-color: rgba(255, 255, 255, 24); /* 鼠标悬停时背景颜色加深 */"
        "}"
        "QPushButton:pressed {"
        "   background-color: rgba(255, 255, 255, 16); /* 按下时背景颜色最深 */"
        "}"
        );

    // 设置 TopMainScene 为 mainScene 的父窗口
    mainScene->setParent(this);

    // 连接按钮点击信号到槽函数
    connect(button, &QPushButton::clicked, this, &TopMainScene::switchShow);
}

TopMainScene::~TopMainScene()
{

}

void TopMainScene::switchShow() {
    if (this->mainScene->isVisible()) {
        this->mainScene->hide();
    } else {
        this->mainScene->show();
    }
    this->raise();
}

// 打开“此电脑”
void TopMainScene::openThisPC() {
    // 使用QProcess启动explorer.exe
    QProcess::startDetached("explorer.exe");

    // 检查是否成功
    if (QProcess::startDetached("explorer.exe")) {
        qDebug() << "已成功打开'此电脑'";
    } else {
        qCritical() << "打开'此电脑'失败:" << GetLastError();
    }
}

