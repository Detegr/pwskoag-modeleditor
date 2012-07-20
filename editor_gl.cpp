#include "editor_gl.h"
#include <iostream>

C_GLEditor::C_GLEditor(QWidget* parent, QStandardItem* root) :
	QGLWidget(QGLFormat(QGL::SampleBuffers), parent), m_Polygon(root)
{
	float f=2.0f/m_GridSize;
	for(unsigned i=0; i<m_GridSize-1; ++i, f+=(2.0f/m_GridSize))
	{
		m_Grid[i]=-1.0+f;
	}
	m_Mode=Insert;
	m_Drag=false;
	m_Move=false;
}

void C_GLEditor::initializeGL()
{
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void C_GLEditor::resizeGL(int w, int h)
{
	glViewport(0,0,w,h);
	glPointSize(5.0f);
}

void C_GLEditor::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glColor4f(0.85f, 0.85f, 0.85f, 1.0f);
	glBegin(GL_LINES);
	for(unsigned i=0; i<m_GridSize-1; ++i)
	{
		glVertex2f(-1.0f, m_Grid[i]);
		glVertex2f(1.0f, m_Grid[i]);
		glVertex2f(m_Grid[i], -1.0f);
		glVertex2f(m_Grid[i], 1.0f);
	}
	glEnd();
	glColor4f(0.4f, 0.4f, 0.6f, 0.6f);
	glBegin(GL_TRIANGLE_STRIP);
	for(std::vector<C_Vertex>::iterator it=m_Polygon.m_Verts.begin(); it!=m_Polygon.m_Verts.end(); ++it)
	{
		std::pair<float, float> pos=it->M_Pos();
		glVertex2f(pos.first, pos.second);
	}
	glEnd();
	glBegin(GL_POINTS);
	for(std::vector<C_Vertex>::iterator it=m_Polygon.m_Verts.begin(); it!=m_Polygon.m_Verts.end(); ++it)
	{
		if(it->M_Selected())
		{
			qglColor(QColor::fromRgbF(0.0f, 1.0f, 0.0f, 1.0f));
		}
		else qglColor(it->M_Color());
		std::pair<float, float> pos=it->M_Pos();
		glVertex3f(pos.first, pos.second, -1.0f);
	}
	glEnd();
	if(m_Drag)
	{
		glColor4f(0.6f, 0.8f, 1.0f, 0.4f);
		glBegin(GL_TRIANGLE_STRIP);
		glVertex3f(m_DragPoints[0], m_DragPoints[2], -1.0f);
		glVertex3f(m_DragPoints[1], m_DragPoints[2], -1.0f);
		glVertex3f(m_DragPoints[0], m_DragPoints[3], -1.0f);
		glVertex3f(m_DragPoints[1], m_DragPoints[3], -1.0f);
		glEnd();
		glColor4f(0.0f, 0.2f, 1.0f, 0.8f);
		glBegin(GL_LINE_STRIP);
		glVertex3f(m_DragPoints[0], m_DragPoints[2], -1.0f);
		glVertex3f(m_DragPoints[1], m_DragPoints[2], -1.0f);
		glVertex3f(m_DragPoints[1], m_DragPoints[3], -1.0f);
		glVertex3f(m_DragPoints[0], m_DragPoints[3], -1.0f);
		glVertex3f(m_DragPoints[0], m_DragPoints[2], -1.0f);
		glEnd();
	}
}

void C_GLEditor::mousePressEvent(QMouseEvent* e)
{
	float x=M_RoundToPrecision((float)e->pos().x()/(this->width()/2)-1.0f);
	float y=-M_RoundToPrecision((float)e->pos().y()/(this->height()/2)-1.0f);
	if(m_Mode==Insert)
	{
		emit S_MousePressed(m_Polygon.m_Root,x,y);
		m_Drag=false;
	}
	else
	{
		m_Move=false;
		for(std::vector<C_Vertex>::iterator it=m_Polygon.m_Verts.begin(); it!=m_Polygon.m_Verts.end(); ++it)
		{
			if(M_PointInsideBox(x,y, it->M_Pos().first-0.01f, it->M_Pos().second-0.01f,
									 it->M_Pos().first+0.01f, it->M_Pos().second+0.01f))
			{
				it->M_SetSelection(true);
				m_Move=true;
			}
			else if(!it->M_Selected()) it->M_SetSelection(false);
		}
		m_Drag=true;
	}
	m_LastClick=std::make_pair(x,y);
	m_LastMousePos=m_LastClick;
	updateGL();
}
void C_GLEditor::mouseMoveEvent(QMouseEvent* e)
{
	float x=M_RoundToPrecision((float)e->pos().x()/(this->width()/2)-1.0f);
	float y=-M_RoundToPrecision((float)e->pos().y()/(this->height()/2)-1.0f);
	if(e->buttons() & Qt::LeftButton)
	{
		if(m_Mode==Insert)
		{
			m_Drag=false;
			m_Polygon.m_Verts.back().M_SetPos(x,y);
		}
		else
		{
			float lx=m_LastClick.first;
			float ly=m_LastClick.second;
			for(std::vector<C_Vertex>::iterator it=m_Polygon.m_Verts.begin(); it!=m_Polygon.m_Verts.end(); ++it)
			{
				if(M_PointInsideBox(it->M_Pos().first,it->M_Pos().second, lx-0.01f, ly-0.01f, lx+0.01f, ly+0.01f))
				{
					m_Drag=false;
					m_Move=true;
					break;
				}
			}
			float lmx=m_LastMousePos.first;
			float lmy=m_LastMousePos.second;
			if(m_Move)
			{
				for(std::vector<C_Vertex>::iterator it=m_Polygon.m_Verts.begin(); it!=m_Polygon.m_Verts.end(); ++it)
				{
					if(it->M_Selected())
					{
						std::pair<float, float> prevPos=it->M_Pos();
						it->M_SetPos(prevPos.first+(x-lmx),prevPos.second+(y-lmy));
					}
				}
			}
			else
			{
				m_Drag=true;
				m_Move=false;
				m_DragPoints[0]=std::min(m_LastClick.first, x);
				m_DragPoints[1]=std::max(m_LastClick.first, x);
				m_DragPoints[2]=std::min(m_LastClick.second, y);
				m_DragPoints[3]=std::max(m_LastClick.second, y);
				for(std::vector<C_Vertex>::iterator it=m_Polygon.m_Verts.begin(); it!=m_Polygon.m_Verts.end(); ++it)
				{
					std::pair<float, float> pos=it->M_Pos();
					if(M_PointInsideBox(pos.first, pos.second, m_DragPoints[0], m_DragPoints[2], m_DragPoints[1], m_DragPoints[3])) it->M_SetSelection(true);
					else it->M_SetSelection(false);
				}
			}
		}
		updateGL();
	}
	m_LastMousePos=std::make_pair(x,y);
}

void C_GLEditor::mouseReleaseEvent(QMouseEvent* e)
{
	m_Move=false;
	if(m_Drag)
	{
		for(unsigned i=0; i<4; ++i) m_DragPoints[i]=0.0f;
		updateGL();
	}
	if(m_LastMousePos==m_LastClick)
	{
		for(std::vector<C_Vertex>::iterator it=m_Polygon.m_Verts.begin(); it!=m_Polygon.m_Verts.end(); ++it)
		{
			it->M_SetSelection(false);
		}
	}
}

bool C_GLEditor::M_PointInsideBox(float px, float py, float x1, float y1, float x2, float y2)
{
	return  (px >= std::min(x1,x2)) && (px <= std::max(x1,x2)) &&
			(py >= std::min(y1,y2)) && (py <= std::max(y1,y2));
}

void C_GLEditor::M_Center()
{
	float minx=1.0f, miny=1.0f;
	float maxx=-1.0f, maxy=-1.0f;
	for(std::vector<C_Vertex>::iterator it=m_Polygon.m_Verts.begin(); it!=m_Polygon.m_Verts.end(); ++it)
	{
		std::pair<float, float> pos=it->M_Pos();
		if(pos.first > maxx) maxx=pos.first;
		if(pos.first < minx) minx=pos.first;
		if(pos.second > maxy) maxy=pos.second;
		if(pos.second < miny) miny=pos.second;
	}
	float diffx=maxx-((maxx-minx)/2);
	float diffy=maxy-((maxy-miny)/2);
	for(std::vector<C_Vertex>::iterator it=m_Polygon.m_Verts.begin(); it!=m_Polygon.m_Verts.end(); ++it)
	{
		std::pair<float, float> pos=it->M_Pos();
		it->M_SetPos(pos.first-diffx, pos.second-diffy);
	}
	updateGL();
}

void C_GLEditor::M_Dump() const
{
	for(std::vector<C_Vertex>::const_iterator it=m_Polygon.m_Verts.begin(); it!=m_Polygon.m_Verts.end(); ++it)
	{
		std::cout << it->M_Pos().first << "\n" << it->M_Pos().second << std::endl;
	}
}

float C_GLEditor::M_RoundToPrecision(float num, unsigned precision)
{
	unsigned long long m = pow(10, precision);
	return floor((num * m)+0.5)/m;
}
