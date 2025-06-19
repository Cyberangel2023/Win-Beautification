#ifndef TOPMAINSCENE_H
#define TOPMAINSCENE_H

#include <QWidget>

#include "MainScene.h"

class TopMainScene : public QWidget
{
    Q_OBJECT
public:
    TopMainScene(MainScene* mainScene, QWidget *parent = nullptr);
    ~TopMainScene();

private:
    void switchShow();
    void openThisPC();

private:
    QScreen* screen; // 屏幕
    QRect screenRect; // 屏幕大小
    QRect availableRect;  // 可用区域大小

private:
    MainScene* mainScene;
};

#endif // TOPMAINSCENE_H
