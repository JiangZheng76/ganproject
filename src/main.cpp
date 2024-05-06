/*
 * @Author: jiangzheng 2440877322@example.com
 * @Date: 2024-04-08 15:30:46
 * @LastEditors: jiangzheng 2440877322@example.com
 * @LastEditTime: 2024-04-24 12:53:57
 * @FilePath: /GanProject/main.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%A
 */
#include "mainwindow.h"
#include "utils/ganconfig.h"
#include <QApplication>
#include <QDebug>
using namespace gan;
int main(int argc, char *argv[])
{
    // 应用程序类，只能有一个
    // 接收窗口的事件处理
    QApplication a(argc, argv);

    // SYLAR_LOG_INFO(g_logger) << "config init success.";

    // 窗口对象
    MainWindow w;
    // 显示窗口
    w.show();
    // 阻塞，应用程序进入到事件循环
    return a.exec();
}
