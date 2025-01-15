#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_TriangleOpenGLWidget = new TriangleOpenGLWidget(this);
    ui->glWidgetLayout->addWidget(m_TriangleOpenGLWidget);
    m_fpslabel = new QLabel(m_TriangleOpenGLWidget);
    m_fpslabel->setMinimumSize(QSize(100, 50));
    QPalette pe;
    pe.setColor(QPalette::WindowText,Qt::red);
    m_fpslabel->setPalette(pe);
    connect(m_TriangleOpenGLWidget, &TriangleOpenGLWidget::updateFpsed, m_fpslabel, [=](int fps){

        m_fpslabel->setText(QString("fps: %1").arg(QString::number(fps)));
        //m_fpslabel->update();
    });
    m_TriangleOpenGLWidget1 = new TriangleOpenGLWidget(this);
    ui->glWidgetLayout->addWidget(m_TriangleOpenGLWidget1);
}

MainWindow::~MainWindow()
{
    delete ui;
}
