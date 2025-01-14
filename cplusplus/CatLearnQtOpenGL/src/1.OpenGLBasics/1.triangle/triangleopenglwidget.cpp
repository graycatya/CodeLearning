#include "triangleopenglwidget.h"

#include <QMouseEvent>
#include <QPointF>
#include <cmath>

TriangleOpenGLWidget::TriangleOpenGLWidget(QWidget *parent) :
    QOpenGLWidget(parent)
{

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

void TriangleOpenGLWidget::mousePressEvent(QMouseEvent *e)
{
    angularSpeed = 0;
    mousePressPosition = QVector2D(e->localPos());
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
    if (angularSpeed < 0.01) {
        angularSpeed = 0.0;
    } else {
        // Update rotation
        rotation = QQuaternion::fromAxisAndAngle(rotationAxis, angularSpeed) * rotation;

        // Request an update
        update();
    }
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
    const qreal zNear = 3.0, zFar = 7.0, fov = 45.0;

    // Reset projection
    projection.setToIdentity();

    // Set perspective projection
    projection.perspective(fov, aspect, zNear, zFar);
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

    program.bind();

    QMatrix4x4 matrix;
    matrix.translate(0.0, 0.0, -5.0);
    matrix.rotate(rotation);

    // Set modelview-projection matrix
    program.setUniformValue("mvp_matrix", projection * matrix);


    // Draw cube geometry
    triangleEngine->drawEngine(&program);
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
