#include "triangleopenglwidget.h"

#include <QMouseEvent>
#include <QPointF>
#include <cmath>
#include <QDateTime>

#include <QDebug>

static void qNormalizeAngle(int &angle)
{
    while (angle < 0)
        angle += 360 * 16;
    while (angle > 360 * 16)
        angle -= 360 * 16;
}

TriangleOpenGLWidget::TriangleOpenGLWidget(QWidget *parent) :
    QOpenGLWidget(parent)
{

    m_lastTime = QDateTime::currentMSecsSinceEpoch();
    m_frameCount = 0;

    m_Camera.setToIdentity();
    m_Camera.translate(0, 0, -2.0);
}

TriangleOpenGLWidget::TriangleOpenGLWidget()
{

}

TriangleOpenGLWidget::~TriangleOpenGLWidget()
{
    makeCurrent();
    delete triangleEngine;
    doneCurrent();
}

void TriangleOpenGLWidget::updateFps()
{
    quint64 currentTime = QDateTime::currentMSecsSinceEpoch();
    m_frameCount++;
    if (currentTime - m_lastTime >= 1000) {

        int fps = m_frameCount * 1000 / (currentTime - m_lastTime);
        if(fps > 0)
        {
            emit updateFpsed(fps);
        }
        m_frameCount = 0;
        m_lastTime = currentTime;
    }
}

void TriangleOpenGLWidget::setXRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != m_xRot) {
        m_xRot = angle;
        emit xRotationChanged(angle);
        update();
    }
}

void TriangleOpenGLWidget::setYRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != m_yRot) {
        m_yRot = angle;
        emit yRotationChanged(angle);
        update();
    }
}

void TriangleOpenGLWidget::setZRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != m_zRot) {
        m_zRot = angle;
        emit zRotationChanged(angle);
        update();
    }
}

void TriangleOpenGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - m_lastPos.x();
    int dy = event->y() - m_lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
        setXRotation(m_xRot + 8 * dy);
        setYRotation(m_yRot + 8 * dx);
    } else if (event->buttons() & Qt::RightButton) {
        setXRotation(m_xRot + 8 * dy);
        setZRotation(m_zRot + 8 * dx);
    }
    m_lastPos = event->pos();
}

void TriangleOpenGLWidget::mousePressEvent(QMouseEvent *event)
{
    m_lastPos = event->pos();
    angularSpeed = 0;
    mousePressPosition = QVector2D(event->localPos());
}

void TriangleOpenGLWidget::mouseReleaseEvent(QMouseEvent *e)
{
    // Mouse release position - mouse press position
    QVector2D diff = QVector2D(e->localPos()) - mousePressPosition;

    // Rotation axis is perpendicular to the mouse position difference
    // vector
    QVector3D n = QVector3D(diff.y(), diff.x(), 0.0).normalized();

    // Accelerate angular speed relative to the length of the mouse sweep
    qreal acc = diff.length() / 100.0;

    // Calculate new rotation axis as weighted sum
    rotationAxis = (rotationAxis * angularSpeed + n * acc).normalized();

    // Increase angular speed
    angularSpeed += acc;
}

void TriangleOpenGLWidget::timerEvent(QTimerEvent *e)
{
    //angularSpeed *= 0.99;

    // Stop rotation when speed goes below threshold
    // if (angularSpeed < 0.01) {
    //     angularSpeed = 0.0;
    // } else {
    //     // Update rotation
    //     rotation = QQuaternion::fromAxisAndAngle(rotationAxis, angularSpeed) * rotation;

    //     // Request an update
    //     update();
    // }
    update();
}

void TriangleOpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();

    glClearColor(0, 0, 0, 1.0);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    initShaders();

    triangleEngine = new TriangleEngine;

    timer.start(12, this);
}

void TriangleOpenGLWidget::resizeGL(int w, int h)
{
    // Calculate aspect ratio
    qreal aspect = qreal(w) / qreal(h ? h : 1);

    // Set near plane to 3.0, far plane to 7.0, field of view 45 degrees
    const qreal zNear = 10.0, zFar = 7.0, fov = 45.0;

    // Reset projection
    m_Projection.setToIdentity();

    // Set perspective projection
    //m_Projection.perspective(fov, aspect, zNear, zFar);
    m_Projection.perspective(45.0f, GLfloat(w) / h, 0.01f, 100.0f);
}

void TriangleOpenGLWidget::paintGL()
{
    // Clear color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //! [2]
    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);

    // Enable back face culling
    //glEnable(GL_CULL_FACE);
    program.setUniformValue("lightPos", QVector3D(0, 0, 70));
    program.setUniformValue("lightColor", 1.0f, 1.0f, 1.0f);

    program.bind();



    //m_View.translate(0.0, 0.0, -6.0);
    m_View.setToIdentity();
    m_View.rotate(180.0f - (m_xRot / 16.0f), 1, 0, 0);
    m_View.rotate(m_yRot / 16.0f, 0, 1, 0);
    m_View.rotate(m_zRot / 16.0f, 0, 0, 1);

    // Set modelview-projection matrix
    program.setUniformValue("projectionMatrix", m_Projection);
    program.setUniformValue("mvpMatrix", m_Camera * m_View);


    // Draw cube geometry
    triangleEngine->drawEngine(&program);
    updateFps();

}

void TriangleOpenGLWidget::initShaders()
{
    // Compile vertex shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/triangle.vs"))
        close();

    // Compile fragment shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/triangle.fs"))
        close();

    // Link shader pipeline
    if (!program.link())
        close();

    // Bind shader pipeline for use
    if (!program.bind())
        close();
}
