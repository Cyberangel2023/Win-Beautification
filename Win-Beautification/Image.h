#ifndef IMAGE_H
#define IMAGE_H

#include <QWidget>
#include <QLabel>
#include <QMouseEvent>
#include <QPixmap>
#include <QMessageBox>
#include <QFileInfo>
#include <QIcon>
#include <QFileIconProvider>
#include <QCursor>
#include <QPoint>
#include <QRect>
#include <QUrl>
#include <QDesktopServices>

class Image : public QLabel {
    Q_OBJECT

public:
    Image(const QString &name, const QString& filePath, QWidget *parent)
        : QLabel(parent), m_filePath(filePath), m_selected(false), parent(parent) {
        // 设置图片
        QPixmap pixmap;
        QFileInfo fileInfo(filePath);
        QFileIconProvider iconProvider;
        QIcon icon = iconProvider.icon(fileInfo);

        // 强制图标大小
        pixmap = icon.pixmap(QSize(60, 60));
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
    }

    QString getFilePath() const {
        return m_filePath;
    }

    void setFilePath(const QString &filePath) {
        m_filePath = filePath;
    }

protected:
    bool event(QEvent *event) override {
        if (event->type() == QEvent::HoverEnter) {
            emit hovered();
        } else {
            if (!this->m_selected) {
                if (event->type() == QEvent::HoverLeave) {
                    setStyleSheet("background: transparent;");
                } else if (event->type() == QEvent::Wheel) {
                    setStyleSheet("background: transparent;");
                }
            } else {
                if (event->type() == QEvent::HoverLeave) {
                    setStyleSheet("background-color: rgba(255, 255, 255, 64);");
                } else if (event->type() == QEvent::Wheel) {
                    setStyleSheet("background-color: rgba(255, 255, 255, 64);");
                }
            }
        }
        return QLabel::event(event);
    }

signals:
    void hovered();
    void doubleClicked();

private slots:
    void onHovered() {
        // 在这里处理鼠标悬停事件
        setStyleSheet("background-color: rgba(255, 255, 255, 32);");
    }

public:
    void setSelected(bool selected)
    {
        this->m_selected = selected;
        // 更新 UI 以显示选中状态，例如改变背景色
        if (this->m_selected) {
            setStyleSheet("background-color: rgba(255, 255, 255, 64);");
        } else {
            setStyleSheet("background: transparent;");
        }
    }

    bool isSelected() const
    {
        return m_selected;
    }

    void reset() {
        setStyleSheet("background: transparent;");
        this->m_selected = false;
    }

    QString getFilePath() {
        return this->m_filePath;
    }

private:
    QString m_filePath;
    bool m_selected;
    QWidget parent;
};

#endif // IMAGE_H
