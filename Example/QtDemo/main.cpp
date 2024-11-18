#include <QApplication>
#include <QPushButton>
#include <QEvent>
#include <QMouseEvent>
#include <QResizeEvent>

class ButtonEventFilter : public QObject {
public:
    ButtonEventFilter(QPushButton* button, QObject* parent = nullptr)
        : QObject(parent), m_button(button), m_originalSize(button->size()) {}

protected:
    bool eventFilter(QObject* obj, QEvent* event) override {
        if (event->type() == QEvent::Enter) {
            m_button->resize(m_originalSize * 1.2); // 放大按钮
        }
        else if (event->type() == QEvent::Leave) {
            m_button->resize(m_originalSize); // 恢复按钮大小
        }
        return QObject::eventFilter(obj, event);
    }

    void setButton(QPushButton* button) {
        m_button = button;
        m_originalSize = button->size();
    }

private:
    QPushButton* m_button;
    QSize m_originalSize;
};

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    QPushButton button("Hover over me!");
    button.resize(200, 200); // 设置按钮的初始大小

    button.show();

    return app.exec();
}