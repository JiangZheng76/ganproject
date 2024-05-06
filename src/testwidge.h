#ifndef TESTWIDGE_H
#define TESTWIDGE_H

#include <QWidget>

namespace Ui {
class TestWidge;
}

class TestWidge : public QWidget
{
    Q_OBJECT

public:
    explicit TestWidge(QWidget *parent = nullptr);
    ~TestWidge();

private:
    Ui::TestWidge *ui;
};

#endif // TESTWIDGE_H
