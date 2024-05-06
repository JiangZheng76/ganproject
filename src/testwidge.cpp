#include "testwidge.h"
#include "ui_testwidge.h"

TestWidge::TestWidge(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TestWidge)
{
    ui->setupUi(this);
}

TestWidge::~TestWidge()
{
    delete ui;
}
