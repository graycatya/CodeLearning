#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "triangleopenglwidget.h"
#include <QLabel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    TriangleOpenGLWidget *m_TriangleOpenGLWidget = nullptr;
    TriangleOpenGLWidget *m_TriangleOpenGLWidget1 = nullptr;
    QLabel* m_fpslabel;
};

#endif // MAINWINDOW_H
