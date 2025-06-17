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
    this->archorPane = new ArchorPane(this);

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
    QVBoxLayout *layout = qobject_cast<QVBoxLayout*>(this->archorPane->layout());
    layout->addWidget(this->scrollPane);

    // 将 ArchorPane 添加到 MainScene 的布局中
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(availableRect.right() / 2 - 495, availableRect.bottom() - 80,
                                   availableRect.right() / 2 - 495, 10);
    mainLayout->addWidget(this->archorPane);

    setLayout(mainLayout);
}

MainScene::~MainScene()
{
    delete ui;
}

void MainScene::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
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

void MainScene::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        // 检查点击是否在某个 Image 控件上
        QWidget *child = childAt(event->pos());
        if (child) {
            Image *clickedImage = qobject_cast<Image*>(child);
            if (clickedImage) {
                // 处理 Image 点击
                bool ctrlPressed = qApp->keyboardModifiers() & Qt::ControlModifier;
                bool shiftPressed = qApp->keyboardModifiers() & Qt::ShiftModifier;

                if (ctrlPressed) {
                    // Ctrl 多选：切换选中状态
                    if (this->selectedFiles.contains(clickedImage)) {
                        this->selectedFiles.remove(clickedImage);
                        clickedImage->reset();
                    } else {
                        this->selectedFiles.insert(clickedImage);
                        clickedImage->setSelected(true);
                        clickedImage->setStyleSheet("background-color: rgba(255, 255, 255, 64);");
                    }
                } else if (shiftPressed && !this->selectedFiles.isEmpty()) {
                    // Shift 多选：从最后一个选中的 Image 到当前点击的 Image 之间的所有 Image 都被选中
                    // 这里需要实现 Shift 多选逻辑
                } else {
                    // 单选：清除之前的选中状态，选中当前点击的 Image
                    foreach (Image *img, this->selectedFiles) {
                        img->reset();
                    }
                    this->selectedFiles.clear();
                    this->selectedFiles.insert(clickedImage);
                    clickedImage->setSelected(true);
                }
            } else {
                // 点击了非 Image 控件（例如背景控件）
                foreach (Image *img, this->selectedFiles) {
                    img->reset();
                }
                this->selectedFiles.clear();
            }
        } else {
            qDebug() << "132";
        }
    } else if (event->button() == Qt::RightButton) {
        // 右键点击事件
    }
}

void MainScene::mouseReleaseEvent(QMouseEvent *event) {
    // 可以在这里处理鼠标释放事件
}
