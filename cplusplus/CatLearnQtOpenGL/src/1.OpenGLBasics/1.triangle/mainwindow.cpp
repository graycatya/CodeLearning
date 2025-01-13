#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_TriangleOpenGLWidget = new TriangleOpenGLWidget(this);
    ui->glWidgetLayout->addWidget(m_TriangleOpenGLWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}
