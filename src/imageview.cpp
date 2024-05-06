/*
 * @Author: jiangzheng 2440877322@example.com
 * @Date: 2024-04-11 15:37:37
 * @LastEditors: jiangzheng 2440877322@example.com
 * @LastEditTime: 2024-04-27 15:44:14
 * @FilePath: /GanProject/imageview.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "imageview.h"
#include "ui_imageview.h"

#include "utils/ganconfig.h"

#include <QFileDialog>
#include <QPixmap>
#include <QFileInfo>
#include <QPushButton>
#include <QVBoxLayout>
#include <QString>
#include <QInputDialog>
#include <QDebug>
#include <QMessageBox>

using namespace gan;

ImageView::ImageView(QWidget *parent) : QWidget(parent),ui(new Ui::ImageView)
{
    ui->setupUi(this);
    m_paramsTable = ui->paramsTable;

    connect(ui->chooseBtn, &QPushButton::clicked, this, &ImageView::chooseImage);
    connect(ui->addParamsBtn, &QPushButton::clicked, this, &ImageView::addParams);
    connect(ui->deleteParamsBtn, &QPushButton::clicked, this, &ImageView::deleteParams);
    connect(ui->yamlLoadBtn, &QPushButton::clicked, this, &ImageView::chooseYamlFile);
    connect(ui->genBtn,&QPushButton::clicked,this,&ImageView::genImage);

    connect(ui->paramsTable,&QTableWidget::itemChanged,this,&ImageView::onParamItemChange);

    initParamsTable();
    loadYamlParam(YAML_PATH);
    
    // refreshParamsTable();
    m_image = ui->imageLabel;
}
void ImageView::genImage(){
    GanConfig::OverrideCurYaml();
}


void ImageView::onParamItemChange(QTableWidgetItem* item){
    int name_row = item->row();
    int name_clo = item->column() - 1;
    qDebug() << "Item changed " << name_row << " : " << name_clo;
    if(name_row < 0 || name_clo < 0){
        // 防止初始化的时候出现问题
        return;
    }
    QTableWidgetItem* name_item = m_paramsTable->item(name_row,name_clo);
    if(name_item){
        std::string name = name_item->text().toStdString();
        std::string val = item->text().toStdString();
        qDebug() << "onParamItemChange() Item changed " << name_item->text() << " : " << item->text();
        GanConfig::Lookup<std::string>(name,val,"");
    }else {
        qDebug() << "onParamItemChange() name_item is nullptr";
    }
}

void ImageView::chooseYamlFile(){
    QString YamlFile = QFileDialog::getOpenFileName(this, tr("Choose YAML"), "", tr("YAML Files (*.yaml)"));
    if(!YamlFile.isEmpty()){
        qInfo() << "load yaml files " << YamlFile;
        loadYamlParam(YamlFile.toStdString());
    }
}

void ImageView::chooseImage()
{
    QString imagePath = QFileDialog::getOpenFileName(this, tr("Choose Image"), "", tr("Image Files (*.png *.jpg *.bmp)"));
    if (!imagePath.isEmpty()) {
        selectedImagePath = imagePath;
        updateImage(imagePath);
        updateResolutionLabel(imagePath);
    }
}

void ImageView::updateImage(const QString &imagePath)
{
    QPixmap pixmap(imagePath);
    if (!pixmap.isNull()) {
        m_image->setPixmap(pixmap.scaled(m_image->size(), Qt::KeepAspectRatio));
        m_image->resize(pixmap.size());
        
    }
}

void ImageView::updateResolutionLabel(const QString &imagePath)
{
    QPixmap pixmap(imagePath);
    QString resolution = QString("%1 x %2").arg(QString::number(pixmap.width()), QString::number(pixmap.height()));
    ui->resolutionLabel->setText(resolution);
}
void ImageView::addParams(){
    int row = m_paramsTable->rowCount();
    row+=1;
    m_paramsTable->setRowCount(row);
}
void ImageView::deleteParams(){
    QList<QTableWidgetItem *> selectedItems = m_paramsTable->selectedItems();
        
    if(selectedItems.empty()){
        QMessageBox::question(this,"question","请双击选择需要删除的行");
        return;
    }
    // 删除选中行
    for (QTableWidgetItem *item : selectedItems) {
        int row = item->row();
        m_paramsTable->removeRow(row);
    }
}
void ImageView::insertNewParamsItem(const std::string& param,const std::string& val){
    QString qparam = QString::fromStdString(param);
    QString qval = QString::fromStdString(val);
    QTableWidgetItem* paramsItem = new QTableWidgetItem(qparam);
    QTableWidgetItem* valItem = new QTableWidgetItem(qval);
    m_paramsTable->setItem(curRow,0,paramsItem);
    m_paramsTable->setItem(curRow,1,valItem);
    curRow++;
}
void ImageView::refreshParamsTable(){
    curRow = 0;
    m_paramsTable->setRowCount(GanConfig::GetSdatas().size());
    for(auto p : GanConfig::GetSdatas()){
        qInfo() <<"refreshParamsTable() " << QString::fromStdString(p.first) << " " <<  QString::fromStdString(p.second->toString());
        insertNewParamsItem(p.first,p.second->toString());
    }
}
void ImageView::initParamsTable(){
    QStringList header;
    header << "params" << "value";
    m_paramsTable->setRowCount(4);
    m_paramsTable->setColumnCount(2);
    m_paramsTable->setHorizontalHeaderLabels(header);
    m_paramsTable->setSelectionMode(QAbstractItemView::ExtendedSelection);
    
    // 设置水平头拉伸模式为 Stretch
    m_paramsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    // 设置列拉伸模式为 Stretch
    m_paramsTable->horizontalHeader()->setStretchLastSection(true);

    auto size = m_paramsTable->size();
    m_paramsTable->setColumnWidth(0,size.width()/2);
    m_paramsTable->setColumnWidth(1,size.width()/2);
    
    insertNewParamsItem("resolution_u","10000");
    insertNewParamsItem("resolution_v","10000");
    for(auto p : GanConfig::GetSdatas()){
        insertNewParamsItem(p.first,p.second->toString());
    }
}

void ImageView::loadYamlParam(const std::string& path){
    // static _InitConfig initCfg;
    if(!GanConfig::LoadFromYaml(path)){
        QString error = QString::fromStdString(path) + "导入 YAML 参数失败.";
        QMessageBox::question(this,"question",error);
        return;
    }
    refreshParamsTable();
}