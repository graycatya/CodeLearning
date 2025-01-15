#include "triangleengine.h"

#include <QVector4D>
#include <QVector3D>
#include <QVector2D>

struct VertexData
{
    QVector3D position;
    QVector4D color = QVector4D(1.0f, 0.5f, 1.0f, 1.0f);
    QVector3D normal;
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
    GLfloat v0;
    VertexData vertices[3];
    vertices[0].position = QVector3D(-0.5f, -0.5f, 0.0f);
    vertices[1].position = QVector3D(0.5f, -0.5f, 0.0f);
    vertices[2].position = QVector3D(0.0f,  0.5f, 0.0f);

    QVector3D e1 = QVector3D(vertices[1].position.x() - vertices[0].position.x(),
                             vertices[1].position.y() - vertices[0].position.y(),
                             vertices[1].position.z() - vertices[0].position.z());
    QVector3D e2 = QVector3D(vertices[2].position.x() - vertices[0].position.x(),
                             vertices[2].position.y() - vertices[0].position.y(),
                             vertices[2].position.z() - vertices[0].position.z());

    QVector3D normal;
    normal.setX(e1.y() * e2.z() - e1.z() * e2.y());
    normal.setY(e1.z() * e2.x() - e1.x() * e2.z());
    normal.setZ(e1.x() * e2.y() - e1.y() * e2.x());

    GLfloat length = sqrt(normal.x() * normal.x() + normal.y() * normal.y() + normal.z() * normal.z());
    normal[0] /= length;
    normal[1] /= length;
    normal[2] /= length;

    vertices[0].normal = normal;
    vertices[1].normal = normal;
    vertices[2].normal = normal;
    // = {
    //     // Vertex data for face 0
    //     {QVector3D(-0.5f, -0.5f, 0.0f), QVector4D(1.0f, 0.5f, 1.0f, 1.0f)},
    //     {QVector3D(0.5f, -0.5f, 0.0f), QVector4D(1.0f, 0.5f, 1.0f, 1.0f)},
    //     {QVector3D(0.0f,  0.5f, 0.0f), QVector4D(1.0f, 0.5f, 1.0f, 1.0f)}
    // };
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
    //glEnable(GL_LIGHTING | GL_CULL_FACE);
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


