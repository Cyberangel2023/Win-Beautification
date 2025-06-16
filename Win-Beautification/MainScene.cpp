#include "MainScene.h"
#include "./ui_MainScene.h"

MainScene::MainScene(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainScene)
{
    ui->setupUi(this);
}

MainScene::~MainScene()
{
    delete ui;
}
