#include "render.h"

GLint Render::SCREEN_WIDTH = 800;
GLint Render::SCREEN_HEIGHT = 800;
GLfloat Render::PI = 3.141592654;
GLboolean Render::wireframeEnabled = true;

std::vector<Vertex> Render::vertices = std::vector<Vertex>();
std::vector<Face> Render::faces = std::vector<Face>();
std::vector<HalfEdge> Render::halfEdges = std::vector<HalfEdge>();
//std::vector<Normal> Render::normals = std::vector<Normal>();

std::vector<Vertex> Render::new_vertices = std::vector<Vertex>();
std::vector<Face> Render::new_faces = std::vector<Face>();
std::vector<HalfEdge> Render::new_halfEdges = std::vector<HalfEdge>();
std::vector<GLint> Render::edgeMap = std::vector<GLint>();

Render::Render(QWidget *parent) : QGLWidget(parent)
{
	eye = Vector4(0, 0, 10);
	up = Vector4(0, 1, 0);
	//light = Vector4(10, 20, 20);
	light = Vector4(0, 0, 1);
	distance = 10;
	s = 6;
}

Render::~Render(){}

void Render::initializeGL()
{

	//glClearColor(0.0, 0.0, 0.0, 1.0);
	qglClearColor(Qt::gray);
	glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_GREATER);
	//glDepthFunc(GL_LESS);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, Render::SCREEN_WIDTH, 0, Render::SCREEN_HEIGHT, -1000.f, 1000.f);
	glMatrixMode(GL_MODELVIEW);

}

void Render::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	draw();

	//qglColor(Qt::red);

	//int xmin = 0.2*Render::SCREEN_WIDTH;
	//int xmax = 0.8*Render::SCREEN_WIDTH;
	//int ymin = 0.2*Render::SCREEN_HEIGHT;
	//int ymax = 0.8*Render::SCREEN_HEIGHT;
	//glBegin(GL_TRIANGLES);

	//glVertex3d(xmin, ymin,-100);
	//glVertex3d(xmax, ymax, -100);
	//glVertex3d(xmin, ymax, -100);

	//glEnd();

	

}

void Render::resizeGL(int width, int height)
{

	Render::SCREEN_WIDTH = width;
	Render::SCREEN_HEIGHT = height;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0,  Render::SCREEN_WIDTH, 0, Render::SCREEN_HEIGHT, -1000.f, 1000.f);
	glMatrixMode(GL_MODELVIEW);
	this->update();
}

void Render::draw()
{

	std::vector<Vertex> vertices_var;
	for (Vertex v : Render::vertices) {
		vertices_var.push_back({ new Vector4(sceneMatrix_B* sceneMatrix_A* *v.vector), v.halfEdge });
	}

	for (int f = 0; f < faces.size();f++) {

		Vector4 verts[3] = {
			*vertices_var[Render::faces[f].verts[0]].vector ,
			*vertices_var[Render::faces[f].verts[1]].vector ,
			*vertices_var[Render::faces[f].verts[2]].vector 
		};


		GLfloat lightIntensity = (Vector4::innerProduct(Vector4(light).normalize(), 
			Vector4::crossProduct(verts[1] - verts[0], verts[2] - verts[0]).normalize())+1)/2;
		glColor3f(lightIntensity, 0.9 * lightIntensity, .4 * lightIntensity);
		//glColor3f(lightIntensity, lightIntensity, lightIntensity);
		glBegin(GL_TRIANGLES);

		glVertex3d(verts[0].x / verts[0].h, verts[0].y / verts[0].h, verts[0].z / verts[0].h);
		glVertex3d(verts[1].x / verts[1].h, verts[1].y / verts[1].h, verts[1].z / verts[1].h);
		glVertex3d(verts[2].x / verts[2].h, verts[2].y / verts[2].h, verts[2].z / verts[2].h);

		glEnd();

		if (wireframeEnabled) {
			glColor3f(0, 0, 0);
			//glLineWidth(2);
			glBegin(GL_LINE_LOOP);

			glVertex3d(verts[0].x / verts[0].h, verts[0].y / verts[0].h, verts[0].z / verts[0].h);
			glVertex3d(verts[1].x / verts[1].h, verts[1].y / verts[1].h, verts[1].z / verts[1].h);
			glVertex3d(verts[2].x / verts[2].h, verts[2].y / verts[2].h, verts[2].z / verts[2].h);

			glEnd();
		}
	}

	for (Vertex v : vertices_var) {
		if (v.vector != nullptr) {
			delete v.vector;
			v.vector = nullptr;
		}
	}

}

void Render::createSceneMatrix()
{
	Vector4 eyeToCenter = -eye;

	Vector4 z_var = -eyeToCenter.normalize();

	Vector4 x_var = Vector4::crossProduct(up, z_var).normalize();

	Vector4 y_var = Vector4::crossProduct(z_var, x_var).normalize();

	//sceneMatrix_A = Matrix4({
	//	{ x_var.x, x_var.y, x_var.z, -Vector4::innerProduct(x_var, eye) },
	//	{ y_var.x, y_var.y, y_var.z, -Vector4::innerProduct(y_var, eye) },
	//	{ z_var.x, z_var.y, z_var.z, -Vector4::innerProduct(z_var, eye) },
	//	{ 0, 0, 0, 1 }
	//});
	sceneMatrix_A = Matrix4({
		{ x_var.x, x_var.y, x_var.z, 0 },
		{ y_var.x, y_var.y, y_var.z, 0 },
		{ z_var.x, z_var.y, z_var.z, 0 },
		{ 0, 0, 0, 1 }
	});

	//sceneMatrix_A = Matrix4({
	//	{ cos(angle_y), 0, sin(angle_y), 0 },
	//	{ 0 , 1 , 0 , 0 },
	//	{ -sin(angle_y), 0, cos(angle_y), 0 },
	//	{ 0, 0, 0, 1 }
	//});
	//sceneMatrix_A.destructMultiply(Matrix4({
	//	{ 1,  0 , 0 , 0 },
	//	{ 0 , cos(angle_x), -sin(angle_x), 0 },
	//	{ 0 , sin(angle_x), cos(angle_x), 0 },
	//	{ 0, 0, 0, 1 }
	//}));
	//sceneMatrix_A.destructMultiply(Matrix4({
	//	{ 1, 0, 0, 0 },
	//	{ 0, 1, 0, 0 },
	//	{ 0, 0, 1, 0 },
	//	{ 0, 0, 0, 1 }
	//}));

	//std::cout << sceneMatrix_A << std::endl;

	GLfloat wx = -.5f, wy = -.5f, ww = 1.f, wh = 1.f;
	GLfloat vx = Render::SCREEN_WIDTH / 2 , vy = Render::SCREEN_HEIGHT / 2 , vw = 100, vh = 100;

	if (vx<vy)
		sceneMatrix_B = Matrix4({
			{ vx, 0, 0, vx },
			{ 0, vx, 0, vy },
			{ 0, 0, vx, 0 },
			{ 0, 0, 0, 1 }
		});
	else
		sceneMatrix_B = Matrix4({
			{ vy, 0, 0, vx },
			{ 0, vy, 0, vy },
			{ 0, 0, vy, distance },
			{ 0, 0, 0, 1 }
	});

	//std::cout << sceneMatrix_B << std::endl;

	//sceneMatrix_B.destructMultiply(Matrix4({
	//	{ 1, 0, 0, 0 },
	//	{ 0, 1, 0, 0 },
	//	{ 0, 0, 0, 0 },
	//	{ 0, 0, -1.f / s, 1 }
	//}));


	//std::cout << sceneMatrix_B << std::endl;

	//std::cout << sceneMatrix_B * sceneMatrix_A << std::endl;
}


void Render::move(GLfloat angle_var, GLfloat height_var, GLfloat deep_var)
{
	angle_x += height_var;
	angle_y += angle_var;
	z_depth += deep_var;

	eye.x = distance * std::cos(angle += angle_var);
	eye.y += height_var*10;
	eye.z = distance * std::sin(angle += angle_var);
	distance += deep_var;

	createSceneMatrix();
}

void Render::mousePressEvent(QMouseEvent *event)
{
	mousePos = QVector2D(event->pos());
	if (event->buttons() == Qt::RightButton)
	{
		loop_subdivision();
		this->update();
	}
	event->accept();
	
}

void Render::mouseMoveEvent(QMouseEvent *event)
{
	if (event->buttons() == Qt::LeftButton)
	{
		QVector2D newPos = (QVector2D)event->pos();
		QVector2D diff = newPos - mousePos;
		move(diff.x()/100, diff.y()/100, 0);
		//qreal angle = (diff.length()) / 3.6;
		//// Rotation axis is perpendicular to the mouse position difference
		//// vector
		//QVector3D rotationAxis = QVector3D(diff.y(), diff.x(), 0.0).normalized();
		//rotation = QQuaternion::fromAxisAndAngle(rotationAxis, angle) * rotation;
		mousePos = newPos;
	}
	
	this->update();
	event->accept();
}

void Render::wheelEvent(QWheelEvent *event)
{
	QPoint numDegrees = event->angleDelta() / 8;

	if (numDegrees.y() > 0) {
		move(0, 0, -0.25);
	}
	else if (numDegrees.y() < 0) {
		move(0, 0, 0.25);
	}

	this->update();
	event->accept();
}


//*****************subvision*******************//
void Render::loop_subdivision()
{
	//***************update vertex point*****************//
	new_vertices.clear();
	for (int vertex_id = 0; vertex_id < vertices.size(); ++vertex_id) {
		Vector4 sum;

		int n = 0;
		int start_edge = vertices[vertex_id].halfEdge;
		int current_edge = start_edge;

		if (current_edge == -1)
		{
			new_vertices.push_back(vertices[vertex_id]);
			continue;
		}

		bool flag = false;
		do {
			current_edge = halfEdges[current_edge].pair;
			if (current_edge == -1)
			{
				flag = true;
				break;
			}

			sum += *(vertices[faces[current_edge / 3].verts[(current_edge + 1) % 3]].vector);
			n++;

			current_edge = 3 * (current_edge / 3) + ((current_edge + 2) % 3);
		} while (current_edge != start_edge);

		//****************test**********************//
		//bool r_flag = false;
		//bool l_flag = false;
		//int l_current_edge = start_edge;
		//int r_current_edge = l_current_edge;
		//if (halfEdges[current_edge].pair == -1)
		//	r_flag = true;
		//else
		//	r_current_edge = halfEdges[current_edge].pair;

		//sum += *(vertices[faces[current_edge / 3].verts[(current_edge + 1) % 3]].vector);
		//n++;
		//while (((r_flag&&l_flag) == false) && halfEdges[l_current_edge].pair != r_current_edge)
		//{
		//	if (!l_flag)
		//	{
		//		l_current_edge = 3 * (l_current_edge / 3) + ((l_current_edge + 2) % 3);
		//		sum += *(vertices[faces[l_current_edge / 3].verts[l_current_edge % 3]].vector);
		//		n++;
		//		//l_current_edge = halfEdges[l_current_edge].pair;
		//		if (halfEdges[l_current_edge].pair == -1)
		//			l_flag = true;
		//		else
		//			l_current_edge = halfEdges[l_current_edge].pair;
		//	}
		//	if (!r_flag)
		//	{
		//		r_current_edge = 3 * (r_current_edge / 3) + ((r_current_edge + 2) % 3);
		//		sum += *(vertices[faces[r_current_edge / 3].verts[(r_current_edge + 1) % 3]].vector);
		//		n++;
		//		//r_current_edge = halfEdges[r_current_edge].pair;
		//		if (halfEdges[r_current_edge].pair == -1)
		//			r_flag = true;
		//		else
		//			r_current_edge = halfEdges[r_current_edge].pair;
		//	}
		//}
		//Vector4 nov;
		//if ((r_flag&&l_flag) == true)
		//{
		//	sum = 0;
		//	sum += *(vertices[faces[l_current_edge / 3].verts[l_current_edge % 3]].vector);
		//	sum += *(vertices[faces[r_current_edge / 3].verts[(r_current_edge + 1) % 3]].vector);
		//	nov = *vertices[vertex_id].vector * (3.0/4.0 ) + sum * (1.0 / 8.0);
		//}
		//else
		//{
		//	sum -= *(vertices[faces[l_current_edge / 3].verts[(l_current_edge + 1) % 3]].vector);
		//	n--;
		//	float b = (1.0 / 64.0) * (40 - std::pow(3 + 2 * std::cos((2 * Render::PI) / n), 2));
		//	nov = *vertices[vertex_id].vector * (1 - b) + sum * (b / n);
		//}

		//*******************************************//
		Vector4 nov;
		if (!flag)
		{
			float b = (1.0 / 64.0) * (40 - std::pow(3 + 2 * std::cos((2 * Render::PI) / n), 2));
			nov = *vertices[vertex_id].vector * (1 - b) + sum * (b / n);
		}
		else
		{
			bool r_flag = false;
			bool l_flag = false;
			int l_current_edge = start_edge;
			int r_current_edge = l_current_edge;
			if (halfEdges[l_current_edge].pair == -1)
				r_flag = true;
			else
				r_current_edge = halfEdges[l_current_edge].pair;

			while ( (r_flag&&l_flag) == false)
			{
				if (!l_flag)
				{
					l_current_edge = 3 * (l_current_edge / 3) + ((l_current_edge + 1) % 3);
					if (halfEdges[l_current_edge].pair == -1)
						l_flag = true;
					else
						l_current_edge = halfEdges[l_current_edge].pair;
				}
				if (!r_flag)
				{
					r_current_edge = 3 * (r_current_edge / 3) + ((r_current_edge + 2) % 3);
					if (halfEdges[r_current_edge].pair == -1)
						r_flag = true;
					else
						r_current_edge = halfEdges[r_current_edge].pair;
				}
			}
			cout << l_current_edge << ":" << faces[l_current_edge / 3].verts[l_current_edge % 3] << "*****"
				<< r_current_edge << ":" << faces[r_current_edge / 3].verts[(r_current_edge + 1) % 3] << endl;
			sum = 0;
			sum += *(vertices[faces[l_current_edge / 3].verts[(l_current_edge+1) % 3]].vector);
			sum += *(vertices[faces[r_current_edge / 3].verts[(r_current_edge ) % 3]].vector);
			nov = *vertices[vertex_id].vector * (3.0 / 4.0) + sum * (1.0 / 8.0);
		}
		//float b = (1.0 / 64.0) * (40 - std::pow(3 + 2 * std::cos((2 * Render::PI) / n), 2));

		////where beta=1/n ( 1/64 * ( 40 - ( 3 + 2 * cos( 2pi / n ) )^2 ) )
		//Vector4 nov = *vertices[vertex_id].vector * (1 - b) + sum * (b / n);

		Vertex tmp_v;
		tmp_v.vector = new Vector4(nov);
		tmp_v.halfEdge = -1;
		new_vertices.push_back(tmp_v);
	}

	//****************create new edge point**********************//
	GLint old_vert_num = vertices.size();
	GLint old_halfedge_num = halfEdges.size();

	edgeMap.clear();
	edgeMap = std::vector<GLint>(old_halfedge_num, -1);

	GLint unused_slot = 0;
	for (GLint h = 0; h < old_halfedge_num; ++h) {
		if (edgeMap[h] == -1) {
			if (halfEdges[h].pair != -1)
			{
				int s_v = faces[h / 3].verts[h % 3];
				int e_v = faces[h / 3].verts[(h + 1) % 3];
				int neighbor_v0 = faces[h / 3].verts[(h + 2) % 3];
				int neighbor_v1 = faces[halfEdges[h].pair / 3].verts[(halfEdges[h].pair + 2) % 3];
				Vector4 vec = (*vertices[s_v].vector + *vertices[e_v].vector) * 3.0 / 8.0
					+ (*vertices[neighbor_v0].vector + *vertices[neighbor_v1].vector) * 1.0 / 8.0;
				new_vertices.push_back({ new Vector4(vec), -1 });

				edgeMap[h] = old_vert_num + unused_slot;
				edgeMap[halfEdges[h].pair] = old_vert_num + unused_slot;
			}
			else
			{
				int s_v = faces[h / 3].verts[h % 3];
				int e_v = faces[h / 3].verts[(h + 1) % 3];
				Vector4 vec = (*vertices[s_v].vector + *vertices[e_v].vector) / 2.0;
				new_vertices.push_back({ new Vector4(vec), -1 });

				edgeMap[h] = old_vert_num + unused_slot;
			}
			unused_slot++;
		}
	}

	//****************split face into 4 new faces**********************//
	new_faces.clear();
	new_faces.reserve(4 * faces.size());

	for (GLint i = 0; i < new_faces.capacity(); ++i) {
		new_faces.push_back({});
	}

	for (GLint f = 0; f < faces.size(); ++f) {
		new_faces[4 * f] = { { edgeMap[f * 3 + 2], faces[f].verts[0], edgeMap[f * 3] } };

		new_faces[4 * f + 1] = { { edgeMap[f * 3], faces[f].verts[1], edgeMap[f * 3 + 1] } };

		new_faces[4 * f + 2] = { { edgeMap[f * 3 + 1], faces[f].verts[2], edgeMap[f * 3 + 2] } };

		new_faces[4 * f + 3] = { { edgeMap[f * 3 + 2], edgeMap[f * 3], edgeMap[f * 3 + 1] } };


		new_vertices[new_faces[4 * f].verts[1]].halfEdge = f * 12;
		new_vertices[new_faces[4 * f + 1].verts[1]].halfEdge = f * 12 + 3;
		new_vertices[new_faces[4 * f + 2].verts[1]].halfEdge = f * 12 + 6;

		new_vertices[edgeMap[f * 3]].halfEdge = f * 12 + 1;
		new_vertices[edgeMap[f * 3 + 1]].halfEdge = f * 12 + 4;
		new_vertices[edgeMap[f * 3 + 2]].halfEdge = f * 12 + 2;
	}

	//*****************update halfedge list info*********************//
	new_halfEdges.clear();
	new_halfEdges.reserve(12 * faces.size());

	for (GLint i = 0; i < new_halfEdges.capacity(); ++i) {
		new_halfEdges.push_back({});
	}

	for (GLint i = 0; i < faces.size(); ++i) {

		GLint b = 12 * i;

		new_halfEdges[b] = { nIndex((i * 3) + 2, 1) };
		new_halfEdges[b + 1] = { nIndex((i * 3), 0) };
		new_halfEdges[b + 2] = { b + 9 };

		new_halfEdges[b + 3] = { nIndex((i * 3), 1) };
		new_halfEdges[b + 4] = { nIndex((i * 3) + 1, 0) };
		new_halfEdges[b + 5] = { b + 10 };

		new_halfEdges[b + 6] = { nIndex((i * 3) + 1, 1) };
		new_halfEdges[b + 7] = { nIndex((i * 3) + 2, 0) };
		new_halfEdges[b + 8] = { b + 11 };

		new_halfEdges[b + 9] = { b + 2 };
		new_halfEdges[b + 10] = { b + 5 };
		new_halfEdges[b + 11] = { b + 8 };

	}

	//**************copy new data to override original data****************//
	for (Vertex v : vertices) {
		delete v.vector;
	}

	vertices.clear();
	vertices = std::move(new_vertices);

	faces.clear();
	faces = std::move(new_faces);

	halfEdges.clear();
	halfEdges = std::move(new_halfEdges);
}

int Render::nIndex(int ei, int type)
{
	int offsets[] = { 3, 1, 6, 4, 0, 7 };

	int op = halfEdges[ei].pair;
	if (op == -1)
		return -1;

	int bp = 12 * (op / 3);

	return bp + offsets[2 * (op % 3) + type];

}
