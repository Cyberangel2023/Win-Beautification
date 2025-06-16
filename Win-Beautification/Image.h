#ifndef IMAGE_H
#define IMAGE_H

#include <QLabel>
#include <QMouseEvent>
#include <QPixmap>
#include <QMessageBox>
#include <QFileInfo>
#include <QIcon>
#include <QFileIconProvider>

class Image : public QLabel {
    Q_OBJECT

public:
    Image(const QString &name, const QString& filePath, QWidget *parent = nullptr)
        : QLabel(parent), m_filePath(filePath) {
        // 设置图片
        QPixmap pixmap;
        QFileInfo fileInfo(filePath);
        QFileIconProvider iconProvider;
        QIcon icon = iconProvider.icon(fileInfo);

        // 强制图标大小
        pixmap = icon.pixmap(QSize(50, 50));
        setMinimumSize(60, 60);
        setMaximumSize(60, 60);
        setPixmap(pixmap);

        // 设置背景透明
        setStyleSheet("background: transparent;");
        // 图片居中显示
        setAlignment(Qt::AlignCenter);

        // 设置点击事件
        setMouseTracking(true);
        setAttribute(Qt::WA_Hover);
        connect(this, &Image::hovered, this, &Image::onHovered);
        connect(this, &Image::clicked, this, &Image::onClicked);
        connect(this, &Image::doubleClicked, this, &Image::onDoubleClicked);
        connect(this, &Image::rightClicked, this, &Image::onMenuEvent);
    }

    QString getFilePath() const {
        return m_filePath;
    }

    void setFilePath(const QString &filePath) {
        m_filePath = filePath;
    }

protected:
    void mousePressEvent(QMouseEvent *event) override {
        if (event->button() == Qt::LeftButton) {
            emit clicked();
        }
        QLabel::mousePressEvent(event);
    }

    void mouseDoubleClickEvent(QMouseEvent *event) override {
        if (event->button() == Qt::LeftButton) {
            emit doubleClicked();
        }
        QLabel::mouseDoubleClickEvent(event);
    }

    void contextMenuEvent(QContextMenuEvent *event) override {
        emit rightClicked(event->pos());
        QLabel::contextMenuEvent(event);
    }

    bool event(QEvent *event) override {
        if (event->type() == QEvent::HoverEnter) {
            emit hovered();
        } else if (event->type() == QEvent::HoverLeave) {
            setStyleSheet("background: transparent;");
        }
        return QLabel::event(event);
    }

signals:
    void clicked();
    void hovered();
    void doubleClicked();
    void rightClicked(const QPoint &pos);

private slots:
    void onHovered() {
        // 在这里处理鼠标悬停事件
        setStyleSheet("background-color: rgba(255, 255, 255, 64);");
    }

    void onClicked() {
        // 在这里处理鼠标左键点击事件
    }

    void onDoubleClicked() {
        // 在这里处理鼠标左键双击事件
    }

    void onMenuEvent() {
        // 在这里处理鼠标右键点击事件
    }

private:
    QString m_filePath;
};

#endif // IMAGE_H
