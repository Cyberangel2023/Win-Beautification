#ifndef MAINSCENE_H
#define MAINSCENE_H

#include <QWidget>
#include <QPaintEvent>
#include <QPainter>
#include <QPixmap>
#include <QScreen>
#include <QFrame>
#include <QLabel>
#include <QMap>
#include <QList>
#include <QDir>
#include <QFileInfo>
#include <QIcon>
#include <QDebug>
#include <QStandardPaths>
#include <QMenu>

#include "ArchorPane.h"
#include "ScrollPane.h"
#include "Image.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainScene;
}
QT_END_NAMESPACE

class MainScene : public QWidget
{
    Q_OBJECT

public:
    MainScene(QWidget *parent = nullptr);
    ~MainScene();

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    void listDesktopFiles();
    QIcon getIconForFile(const QFileInfo &fileInfo);

public:
    // 其他成员函数
    QList<Image*> getSelected() {
        return selectedFiles;
    }

    void addSelected(Image file) {
        this->selectedFiles.push_back(&file);
    }

    int getSelectedSize() {
        return selectedFiles.size();
    }

    void resetFiles();

private slots:
    void onOpenActionTriggered();
    void onCopyActionTriggered();
    void onDeleteActionTriggered();

private:
    Ui::MainScene *ui;

    QMap<QString, Image*> icons;
    QList<Image*> selectedFiles;

    QScreen* screen; // 屏幕
    QRect screenRect; // 屏幕大小
    QRect availableRect;  // 可用区域大小

    ArchorPane* archorPane;
    ScrollPane* scrollPane;

    QMenu *contextMenu; // 右键菜单
    Image *contextMenuImage; // 用于存储右键点击的 Image 指针
};

#endif // MAINSCENE_H
