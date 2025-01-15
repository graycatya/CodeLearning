#ifndef TRIANGLEOPENGLWIDGET_H
#define TRIANGLEOPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector2D>
#include <QBasicTimer>
#include <QElapsedTimer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>

#include "triangleengine.h"


class TriangleOpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    using QOpenGLWidget::QOpenGLWidget;
    explicit TriangleOpenGLWidget(QWidget *parent = nullptr);
    TriangleOpenGLWidget();
    ~TriangleOpenGLWidget();

private:
    void updateFps();

public slots:
    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);

signals:
    void xRotationChanged(int angle);
    void yRotationChanged(int angle);
    void zRotationChanged(int angle);
    void updateFpsed(int fps);

protected:
    void mouseMoveEvent(QMouseEvent *e) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void timerEvent(QTimerEvent *e) override;

    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void initShaders();
private:
    QBasicTimer timer;
    QOpenGLShaderProgram program;
    TriangleEngine *triangleEngine = nullptr;

    QMatrix4x4 m_Projection;
    QMatrix4x4 m_Camera;
    QMatrix4x4 m_View;
    int m_xRot = 0;
    int m_yRot = 0;
    int m_zRot = 0;
    QVector2D mousePressPosition;
    QVector3D rotationAxis;
    qreal angularSpeed = 0;
    QQuaternion rotation;
    QPoint m_lastPos;


    quint64 m_lastTime;
    int m_frameCount;
};

#endif // TRIANGLEOPENGLWIDGET_H
