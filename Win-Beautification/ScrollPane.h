#ifndef SCROLLPANE_H
#define SCROLLPANE_H

#include <QScrollArea>
#include <QHBoxLayout>
#include <QLabel>
#include <QWheelEvent>
#include <QScrollBar>

class ScrollPane : public QScrollArea {
    Q_OBJECT
public:
    explicit ScrollPane(QWidget* parent = nullptr) : QScrollArea(parent) {
        // 设置横向滚条
        setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setWidgetResizable(true);

        // 隐藏滚动条
        setStyleSheet("QScrollBar:horizontal { height: 0px; }");

        QWidget* scrollContent = new QWidget(this);
        QHBoxLayout* scrollLayout = new QHBoxLayout(scrollContent);
        scrollLayout->setContentsMargins(0, 0, 0, 0);
        scrollLayout->setSpacing(10);

        setWidget(scrollContent);
    }

protected:
    void wheelEvent(QWheelEvent* event) override {
        // 处理滚轮事件，实现横向滚动
        if (event->angleDelta().y() != 0) {
            int scrollAmount = event->angleDelta().y() > 0 ? -40 : 40;
            horizontalScrollBar()->setValue(horizontalScrollBar()->value() + scrollAmount);
            event->accept();
        } else {
            QScrollArea::wheelEvent(event);
        }
    }
};

#endif // SCROLLPANE_H
