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

    setMouseTracking(true);

    // 创建右键菜单
    contextMenu = new QMenu(this);
    QAction *openAction = contextMenu->addAction("打开");
    QAction *copyAction = contextMenu->addAction("复制");
    QAction *deleteAction = contextMenu->addAction("删除");

    // 连接菜单项的信号到槽函数
    connect(openAction, &QAction::triggered, this, &MainScene::onOpenActionTriggered);
    connect(copyAction, &QAction::triggered, this, &MainScene::onCopyActionTriggered);
    connect(deleteAction, &QAction::triggered, this, &MainScene::onDeleteActionTriggered);
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
    QPoint clickPos = event->pos();
    if (event->button() == Qt::LeftButton) {
        // 检查点击是否在某个 Image 控件上
        QWidget *child = childAt(clickPos);
        if (child) {
            Image *clickedImage = qobject_cast<Image*>(child);
            if (clickedImage) {
                // 处理 Image 点击
                bool ctrlPressed = qApp->keyboardModifiers() & Qt::ControlModifier;
                bool shiftPressed = qApp->keyboardModifiers() & Qt::ShiftModifier;

                if (ctrlPressed) {
                    // Ctrl 多选：切换选中状态
                    if (this->selectedFiles.contains(clickedImage)) {
                        this->selectedFiles.removeOne(clickedImage);
                        clickedImage->reset();
                    } else {
                        this->selectedFiles.push_back(clickedImage);
                        clickedImage->setSelected(true);
                    }
                } else if (shiftPressed && !this->selectedFiles.isEmpty()) {
                    // Shift 多选：从最后一个选中的 Image 到当前点击的 Image 之间的所有 Image 都被选中
                    // 这里需要实现 Shift 多选逻辑
                } else {
                    // 单选：清除之前的选中状态，选中当前点击的 Image
                    resetFiles();
                    this->selectedFiles.push_back(clickedImage);
                    clickedImage->setSelected(true);
                }
            } else {
                // 点击了非 Image 控件（例如背景控件）
                resetFiles();
            }
        } else {
            // 点击了 MainScene 的背景区域（不在任何子控件上）
            resetFiles();
        }
    } else if (event->button() == Qt::RightButton) {
        // 右键点击事件
        QWidget *child = childAt(clickPos);
        if (child) {
            Image *clickedImage = qobject_cast<Image*>(child);
            if (clickedImage) {
                // 点击了 Image，显示 Image 的右键菜单
                contextMenuImage = clickedImage; // 存储当前点击的 Image 指针
                resetFiles();
                this->selectedFiles.push_back(clickedImage);
                clickedImage->setSelected(true);
                contextMenu->exec(clickPos); // 在鼠标点击位置显示菜单
            } else {
                // 点击了非 Image 控件，可以隐藏菜单或执行其他操作
                resetFiles();
                contextMenu->hide();
            }
        } else {
            // 点击了 MainScene 的背景区域，隐藏菜单
            resetFiles();
            contextMenu->hide();
        }
    }
}

void MainScene::mouseReleaseEvent(QMouseEvent *event) {
    // 可以在这里处理鼠标释放事件
}

void MainScene::resetFiles() {
    foreach (Image* img, this->selectedFiles) {
        img->reset();
    }
    this->selectedFiles.clear();
}

// 菜单槽函数实现
void MainScene::onOpenActionTriggered() {
    if (contextMenuImage) {
        QString filePath = contextMenuImage->getFilePath();
        QUrl url = QUrl::fromLocalFile(filePath);
        if (!QDesktopServices::openUrl(url)) {
            //QMessageBox::warning(this, "错误", "无法使用系统程序打开文件！");
        }
    }
}

void MainScene::onCopyActionTriggered() {
    if (contextMenuImage) {
        QString filePath = contextMenuImage->getFilePath();
        // 在这里添加复制文件的逻辑
    }
}

void MainScene::onDeleteActionTriggered() {
    if (contextMenuImage) {
        QString filePath = contextMenuImage->getFilePath();
        // 在这里添加删除文件的逻辑
        QFile file(filePath);
        if (file.remove()) {
            qDebug() << "文件删除成功";
        } else {
            qDebug() << "文件删除失败";
        }
    }
}
