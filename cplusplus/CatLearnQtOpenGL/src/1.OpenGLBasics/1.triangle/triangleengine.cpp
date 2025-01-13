#include "triangleengine.h"

#include <QVector4D>
#include <QVector3D>
#include <QVector2D>

struct VertexData
{
    QVector3D position;
    QVector4D color;
};


TriangleEngine::TriangleEngine()
    : indexBuf(QOpenGLBuffer::IndexBuffer)
{
    initializeOpenGLFunctions();

    arrayBuf.create();
    indexBuf.create();

    initEngine();
}

TriangleEngine::~TriangleEngine()
{
    arrayBuf.destroy();
    indexBuf.destroy();
}

void TriangleEngine::initEngine()
{

    VertexData vertices[] = {
        // Vertex data for face 0
        {QVector3D(-0.5f, -0.5f, 0.0f), QVector4D(1.0f, 0.5f, 1.0f, 1.0f)},
        {QVector3D(0.5f, -0.5f, 0.0f), QVector4D(1.0f, 0.5f, 1.0f, 0.2f)},
        {QVector3D(0.0f,  0.5f, 0.0f), QVector4D(1.0f, 0.5f, 1.0f, 0.2f)}
    };
    GLushort indices[] = { 0,  1,  2 };
    arrayBuf.setUsagePattern(QOpenGLBuffer::DynamicDraw);
    arrayBuf.bind();
    arrayBuf.allocate(vertices, 3 * sizeof(VertexData));

    // Transfer index data to VBO 1
    indexBuf.bind();
    indexBuf.allocate(indices, 3  * sizeof(GLushort));


}

void TriangleEngine::drawEngine(QOpenGLShaderProgram *program)
{
    // Tell OpenGL which VBOs to use
    arrayBuf.bind();
    indexBuf.bind();

    // Offset for position
    quintptr offset = 0;

    // Tell OpenGL programmable pipeline how to locate vertex position data
    int vertexLocation = program->attributeLocation("aPos");
    program->enableAttributeArray(vertexLocation);
    program->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

    // Offset for texture coordinate
    offset += sizeof(QVector3D);

    // Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
    int colorLocation = program->attributeLocation("aColor");
    program->enableAttributeArray(colorLocation);
    program->setAttributeBuffer(colorLocation, GL_FLOAT, offset, 4, sizeof(VertexData));

    glDrawElements(GL_TRIANGLE_STRIP, 3, GL_UNSIGNED_SHORT, nullptr);

}


