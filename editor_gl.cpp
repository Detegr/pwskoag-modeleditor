#include "editor_gl.h"
#include <iostream>

C_GLEditor::C_GLEditor(QWidget* parent) : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
	float f=2.0f/m_GridSize;
	for(unsigned i=0; i<m_GridSize-1; ++i, f+=(2.0f/m_GridSize))
	{
		m_Grid[i]=-1.0+f;
	}
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
	for(std::vector<std::pair<float,float> >::iterator it=m_Points.begin(); it!=m_Points.end(); ++it)
	{
		glVertex2f(it->first, it->second);
	}
	glEnd();
	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	glBegin(GL_POINTS);
	for(std::vector<std::pair<float,float> >::iterator it=m_Points.begin(); it!=m_Points.end(); ++it)
	{
		glVertex3f(it->first, it->second, -1.0f);
	}
	glEnd();
}

void C_GLEditor::mousePressEvent(QMouseEvent* e)
{
	float x=M_RoundToPrecision((float)e->pos().x()/(this->width()/2)-1.0f);
	float y=-M_RoundToPrecision((float)e->pos().y()/(this->height()/2)-1.0f);
	emit S_MousePressed(x,y);
	updateGL();
}
void C_GLEditor::mouseMoveEvent(QMouseEvent* e)
{
	if(e->buttons() & Qt::LeftButton)
	{
		float x=M_RoundToPrecision((float)e->pos().x()/(this->width()/2)-1.0f);
		float y=-M_RoundToPrecision((float)e->pos().y()/(this->height()/2)-1.0f);
		m_Points.back()=std::make_pair(x,y);
		updateGL();
	}
}

void C_GLEditor::M_Center()
{
	float minx=1.0f, miny=1.0f;
	float maxx=-1.0f, maxy=-1.0f;
	for(std::vector<std::pair<float,float> >::iterator it=m_Points.begin(); it!=m_Points.end(); ++it)
	{
		if(it->first > maxx) maxx=it->first;
		if(it->first < minx) minx=it->first;
		if(it->second > maxy) maxy=it->second;
		if(it->second < miny) miny=it->second;
	}
	float diffx=maxx-((maxx-minx)/2);
	float diffy=maxy-((maxy-miny)/2);
	for(std::vector<std::pair<float,float> >::iterator it=m_Points.begin(); it!=m_Points.end(); ++it)
	{
		*it=std::make_pair(it->first-diffx, it->second-diffy);
	}
	updateGL();
}

void C_GLEditor::M_Dump() const
{
	for(std::vector<std::pair<float,float> >::const_iterator it=m_Points.begin(); it!=m_Points.end(); ++it)
	{
		std::cout << it->first << "\n" << it->second << std::endl;
	}
}

float C_GLEditor::M_RoundToPrecision(float num, unsigned precision)
{
	unsigned long long m = pow(10, precision);
	return floor((num * m)+0.5)/m;
}
