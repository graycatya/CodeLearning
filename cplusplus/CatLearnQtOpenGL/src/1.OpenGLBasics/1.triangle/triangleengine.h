#ifndef TRIANGLEENGINE_H
#define TRIANGLEENGINE_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

class TriangleEngine : protected QOpenGLFunctions
{
public:
    TriangleEngine();
    virtual ~TriangleEngine();

    void drawEngine(QOpenGLShaderProgram *program);

private:
    void initEngine();

private:
    QOpenGLBuffer arrayBuf;
    QOpenGLBuffer indexBuf;
};

#endif // TRIANGLEENGINE_H
