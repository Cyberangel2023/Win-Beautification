#include "MainScene.h"
#include "./ui_MainScene.h"

#include <windows.h>
#include <shlobj.h>
#include <shellapi.h>

MainScene::MainScene(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainScene)
{
    ui->setupUi(this);
    // 获取屏幕尺寸
    this->screen = QGuiApplication::primaryScreen();
    this->screenRect = this->screen->geometry();
    this->availableRect = screen->availableGeometry();

    this->resize(availableRect.size());
    this->move(0, 0);
    this->setWindowFlag(Qt::FramelessWindowHint); // 去除窗口边框
    this->setAttribute(Qt::WA_TranslucentBackground); // 去除窗口

    // 初始化 ArchorPane
    this->anchorPane.reset(new ArchorPane(this));

    // 初始化 scrollPane
    this->scrollPane = new ScrollPane();

    // 创建一个容器 widget 用于放置滚动内容
    QWidget *contentWidget = new QWidget();
    QHBoxLayout *contentLayout = new QHBoxLayout(contentWidget);
    contentLayout->setContentsMargins(20, 0, 20, 0);
    contentLayout->setSpacing(20);

    // 加载文件按钮
    listDesktopFiles();

    foreach (Image* img, this->icons) {
        contentLayout->addWidget(img);
    }

    this->scrollPane->setWidget(contentWidget);

    // 将 ScrollPane 添加到 ArchorPane 的布局中
    QVBoxLayout *layout = qobject_cast<QVBoxLayout*>(this->anchorPane->layout());
    layout->addWidget(this->scrollPane);

    // 将 ArchorPane 添加到 MainScene 的布局中
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(availableRect.right() / 2 - 495, availableRect.bottom() - 80,
                                   availableRect.right() / 2 - 495, 10);
    mainLayout->addWidget(this->anchorPane.data());

    setLayout(mainLayout);
}

MainScene::~MainScene()
{
    delete ui;
}

void MainScene::paintEvent(QPaintEvent *event)
{

}

void MainScene::listDesktopFiles() {
    // 获取桌面路径
    QString desktopPath = QStandardPaths::standardLocations(QStandardPaths::DesktopLocation).first();

    // 创建桌面目录对象
    QDir desktopDir(desktopPath);

    // 检查桌面目录是否存在
    if (!desktopDir.exists()) {
        qDebug() << "Desktop directory does not exist.";
        return;
    }

    // 遍历桌面目录中的文件
    foreach (QFileInfo fileInfo, desktopDir.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot | QDir::System)) {
        // 获取文件路径
        QString filePath = fileInfo.absoluteFilePath();
        // 获取文件名字
        QString fileName = fileInfo.fileName();

        //qDebug() << fileName;

        Image* img = new Image(fileName, filePath, this);
        icons.insert(fileName, img);
    }
}
