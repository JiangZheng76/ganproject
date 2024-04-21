/*
 * @Author: jiangzheng 2440877322@example.com
 * @Date: 2024-04-11 14:47:47
 * @LastEditors: jiangzheng 2440877322@example.com
 * @LastEditTime: 2024-04-11 23:40:43
 * @FilePath: /GanProject/imageview.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include <QWidget>
#include <QLabel>
#include <QTableWidget>
namespace Ui {
class ImageView;
}

class ImageView : public QWidget
{
    Q_OBJECT

public:
    explicit ImageView(QWidget *parent = nullptr);
    ~ImageView();

private slots:
    void chooseImage();

private:
    QLabel *imageLabel;
    QLabel *resolutionLabel;

    QString selectedImagePath;

    void updateImage(const QString &imagePath);
    void updateResolutionLabel(const QString &imagePath);

private:// pararmstable
    int curRow = 0;
    void initParamsTable();
    void addParams();
    void deleteParams();
    void insertNewParamsItem(QString param,QString val);
    
    
private:
    Ui::ImageView *ui;
    
    QLabel* m_image;
    QTableWidget* m_paramsTable;
};

#endif // IMAGEVIEW_H
