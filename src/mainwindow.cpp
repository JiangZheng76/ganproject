/*
 * @Author: jiangzheng 2440877322@example.com
 * @Date: 2024-04-08 15:30:46
 * @LastEditors: jiangzheng 2440877322@example.com
 * @LastEditTime: 2024-04-11 14:33:41
 * @FilePath: /GanProject/mainwindow.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "testwidge.h"
#include "imageview.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    // 将 UI 对象和当前的 window 做捆绑
    ui->setupUi(this);

    // 一般在 qt 的构造函数中进行初始化操作
    // 显示当前窗口的时候，显示另外的窗口 testWidget
 #if 0
     // 创建窗口对象 没有指定父对象，为一个独立窗口
     // 独立窗口需要 show 才会显示出来
     TestWidge* w = new TestWidge;
     w->show(); // 没有指定父对象所以需要 show 出来
 #else
     // 内嵌到this 中
     // 指定了父对象，所以父窗口创建之后，就会把子窗口显示出来
     ImageView* w = new ImageView(this);
     this->resize(w->size());
 #endif
}


MainWindow::~MainWindow()
{
    delete ui;
}

