#include "triangleopenglwidget.h"

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

}

void TriangleOpenGLWidget::mouseReleaseEvent(QMouseEvent *e)
{

}

void TriangleOpenGLWidget::timerEvent(QTimerEvent *e)
{

}

void TriangleOpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();

    glClearColor(0, 0, 0, 1.0);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    initShaders();

    triangleEngine = new TriangleEngine;
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
    glEnable(GL_CULL_FACE);

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
