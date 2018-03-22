#ifndef _RENDER_
#define _RENDER_
#include <QGLWidget>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <vector>
#include <algorithm>
#include "Matrix4.h"
#include "HalfEdge.h"
#include "readObj.h"


class Render : public QGLWidget
{
	Q_OBJECT

public:
	Render(QWidget *parent = 0);
	~Render();

	static GLint SCREEN_WIDTH;
	static GLint SCREEN_HEIGHT;
	static std::vector<Vertex> vertices;
	static std::vector<Face> faces;
	static std::vector<HalfEdge> halfEdges;

	static GLfloat PI;
	static GLboolean wireframeEnabled;
	//Camera cam = Camera(Vector4(0, 0, 10), Vector4(0, 1, 0), Vector4(100, 100, 100), 10, 6.0);

	QString status;

protected:
	void initializeGL();
	void paintGL() override;
	void resizeGL(int width, int height);

	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void wheelEvent(QWheelEvent *event);
	

private:
	QOpenGLShaderProgram *m_program;
	QMatrix4x4 model;
	QMatrix4x4 view;
	QMatrix4x4 projection;
	QVector2D mousePos;
	QImage image;

	static std::vector<Vertex> new_vertices;
	static std::vector<Face> new_faces;
	static std::vector<HalfEdge> new_halfEdges;
	static std::vector<GLint> edgeMap;

	Matrix4 sceneMatrix_A, sceneMatrix_B;
	float distance, s, angle = PI / 2;
	float angle_x = PI / 2, angle_y = PI / 2 , z_depth=0;
	Vector4 eye, up, light;

	//void loadShaders();
	//void compileAttachLinkShaderFromSource(const QString& vs, const QString& fs);
	void draw();
	void move(GLfloat angle_var, GLfloat height_var, GLfloat deep_var);
	void createSceneMatrix();
	void loop_subdivision();
	GLint nIndex(GLint ei, GLint type);
};


#endif