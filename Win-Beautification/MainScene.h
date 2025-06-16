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
#include <QDir>
#include <QFileInfo>
#include <QIcon>
#include <QDebug>
#include <QStandardPaths>

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

private:
    void listDesktopFiles();
    QIcon getIconForFile(const QFileInfo &fileInfo);

private:
    Ui::MainScene *ui;

    QScreen* screen; // 屏幕
    QRect screenRect; // 屏幕大小
    QRect availableRect;  // 可用区域大小

    QScopedPointer<ArchorPane> anchorPane;
    ScrollPane* scrollPane;
    QMap<QString, Image*> icons;
};

#endif // MAINSCENE_H
