#include "editor_gl.h"
#include "editor.h"
#include <iostream>
#ifdef _WIN32
#undef min
#undef max
#endif
#include <algorithm>

const float C_GLEditor::m_MouseOverPrecision=0.02f;

C_GLEditor::C_GLEditor(QWidget* parent, QStandardItem* root) :
	QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
	m_Polygons.push_back(new C_Polygon(root));
	float f=2.0f/m_GridSize;
	for(unsigned i=0; i<m_GridSize-1; ++i, f+=(2.0f/m_GridSize))
	{
		m_Grid[i]=-1.0+f;
	}
	m_Mode=Insert;
	m_DrawMode=LineLoop;
	m_Drag=false;
	m_ActivePoly=m_Polygons.back();
	m_PointPrecision=0.01f;
	m_SplitFirst=m_SplitSecond=NULL;

	m_ViewPortX=0;
	m_ViewPortY=0;
}
C_GLEditor::~C_GLEditor()
{
	for(std::vector<C_Polygon*>::iterator it=m_Polygons.begin(); it!=m_Polygons.end(); ++it)
	{
		delete *it;
	}
}

void C_GLEditor::initializeGL()
{
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	//glEnable(GL_MULTISAMPLE);
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void C_GLEditor::resizeGL(int w, int h)
{
	glViewport(0,0,w,h);
	glPointSize(5.0f);
}

void C_GLEditor::M_PaintGrid()
{
	glColor4f(0.85f, 0.85f, 0.85f, 1.0f);
	glBegin(GL_LINES);
	for(unsigned i=0; i<m_GridSize-1; ++i)
	{
		i==(m_GridSize-1)/2?glColor4f(0.0f, 0.2f, 1.0f,1.0f):glColor4f(0.85f, 0.85f, 0.85f, 1.0f);
		glVertex2f(-1.0f, m_Grid[i]);
		glVertex2f(1.0f, m_Grid[i]);
		glVertex2f(m_Grid[i], -1.0f);
		glVertex2f(m_Grid[i], 1.0f);
	}
	glEnd();
}
void C_GLEditor::M_PaintPolygon(const C_Polygon& p)
{
	glBegin(m_DrawMode);
	for(C_Polygon::const_iterator it=p.begin(); it!=p.end(); ++it)
	{
		std::pair<float, float> pos=it->M_Pos();
		qglColor(it->M_Color());
		glVertex2f(m_ViewPortX+pos.first, m_ViewPortY+pos.second);
	}
	glEnd();
}
void C_GLEditor::M_PaintPoints(const C_Polygon& p)
{
	glBegin(GL_POINTS);
	for(C_Polygon::const_iterator it=p.begin(); it!=p.end(); ++it)
	{
		if(&p == m_ActivePoly)
		{
			if(it->M_Selected() || it->M_Hovering())
			{
				if(it->M_Selected()) qglColor(QColor::fromRgbF(0.0f, 1.0f, 0.0f, 1.0f));
				else if(it->M_Hovering()) qglColor(QColor::fromRgbF(0.0f, 0.0f, 1.0f, 1.0f));
			}
			else qglColor(QColor::fromRgbF(1.0f, 0.0f, 0.0f, 1.0f));
		}
		else qglColor(QColor::fromRgbF(0.4f, 0.4f, 0.4f, 1.0f));
		std::pair<float, float> pos=it->M_Pos();
		glVertex3f(m_ViewPortX+pos.first, m_ViewPortY+pos.second, -1.0f);
	}
	glEnd();
}
void C_GLEditor::M_PaintDrag()
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

void C_GLEditor::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	if(m_DrawMode == Polygon) M_PaintGrid();
	for(std::vector<C_Polygon*>::iterator it=m_Polygons.begin(); it!=m_Polygons.end(); ++it)
	{
		M_PaintPolygon(**it);
		M_PaintPoints(**it);
	}
	if(m_DrawMode == LineStrip || m_DrawMode == LineLoop) M_PaintGrid();
	if(m_Drag) M_PaintDrag();
}

bool C_GLEditor::M_MouseOverVertex(float x, float y, const C_Vertex& v)
{
	return (M_PointInsideBox(x,y, v.M_Pos().first-m_MouseOverPrecision, v.M_Pos().second-m_MouseOverPrecision,
							 v.M_Pos().first+m_MouseOverPrecision, v.M_Pos().second+m_MouseOverPrecision));
}

void C_GLEditor::mousePressEvent(QMouseEvent* e)
{
	float xraw=(float)e->pos().x()/(this->width()/2)-1.0f;
	float yraw=-((float)e->pos().y()/(this->height()/2)-1.0f);
	float x=M_RoundToPrecision(xraw, m_PointPrecision);
	float y=M_RoundToPrecision(yraw, m_PointPrecision);
	if(e->buttons() & Qt::LeftButton)
	{
		if(m_Mode==Insert)
		{
			emit S_MousePressed(m_ActivePoly->M_Root(),x-m_ViewPortX,y-m_ViewPortY,-1,QString(""));
			m_Drag=false;
			for(C_Polygon::iterator it=m_ActivePoly->begin(); it!=m_ActivePoly->end(); ++it)
			{
				it->M_SetSelection(false);
			}
			m_ActivePoly->M_Last().M_SetSelection(true);
		}
		else
		{
			m_Drag=true;
			for(C_Polygon::iterator it=m_ActivePoly->begin(); it!=m_ActivePoly->end(); ++it)
			{
				if(M_MouseOverVertex(xraw-m_ViewPortX,yraw-m_ViewPortY,*it))
				{
					m_Drag=false;
					if(!it->M_Selected())
					{
						for(C_Polygon::iterator itt=m_ActivePoly->begin(); itt!=m_ActivePoly->end(); ++itt)
						{
							itt->M_SetSelection(false);
						}
						it->M_SetSelection(true);
						break;
					}
				}
			}
			if(m_Drag)
			{
				for(C_Polygon::iterator itt=m_ActivePoly->begin(); itt!=m_ActivePoly->end(); ++itt)
				{
					itt->M_SetSelection(false);
				}
			}
		}
	}
	m_SplitFirst=m_SplitSecond=NULL;
	m_LastClick=std::make_pair(xraw,yraw);
	m_LastMousePos=m_LastClick;
	updateGL();
}

void C_GLEditor::M_Split()
{
	if(m_SplitFirst && m_SplitSecond)
	{
		std::pair<float,float> fpos=m_SplitFirst->M_Pos();
		std::pair<float,float> spos=m_SplitSecond->M_Pos();

		float avgx = (fpos.first+spos.first) / 2;
		float avgy = (fpos.second+spos.second) / 2;

		int pos=0;
		int i=0;
		for(C_Polygon::iterator itt=m_ActivePoly->begin(); itt!=m_ActivePoly->end(); ++itt, ++i)
		{
			itt->M_SetSelection(false);
			if(&*itt == m_SplitFirst)
			{
				pos=i+1;
			}
		}

		Mode tmp=m_Mode;
		m_Mode=Insert;
		emit S_MousePressed(m_ActivePoly->M_Root(),avgx,avgy,pos,QString(""));
		m_Mode=tmp;
		updateGL();
	}
	m_SplitFirst=m_SplitSecond=NULL;
}

void C_GLEditor::mouseMoveEvent(QMouseEvent* e)
{
	float xraw=(float)e->pos().x()/(this->width()/2)-1.0f;
	float yraw=-((float)e->pos().y()/(this->height()/2)-1.0f);
	float x=M_RoundToPrecision(xraw, m_PointPrecision);
	float y=M_RoundToPrecision(yraw, m_PointPrecision);
	if(e->buttons() & Qt::LeftButton)
	{
		if(m_Mode==Insert)
		{
			m_Drag=false;
			emit S_SetPos(m_ActivePoly->M_Last(),x-m_ViewPortX,y-m_ViewPortY);
			//m_ActivePoly->M_Last().M_SetPos(x,y);
		}
		else
		{
			float lmx=m_LastMousePos.first;
			float lmy=m_LastMousePos.second;

			if(m_Drag)
			{
				m_DragPoints[0]=std::min(m_LastClick.first, xraw);
				m_DragPoints[1]=std::max(m_LastClick.first, xraw);
				m_DragPoints[2]=std::min(m_LastClick.second, yraw);
				m_DragPoints[3]=std::max(m_LastClick.second, yraw);

				int selected=0;
				int selectedsum=0;
				int vertexcount=0;
				for(C_Polygon::iterator it=m_ActivePoly->begin(); it!=m_ActivePoly->end(); ++it, ++vertexcount)
				{
					std::pair<float, float> pos=it->M_Pos();
					if(M_PointInsideBox(pos.first, pos.second, m_DragPoints[0]-m_ViewPortX, m_DragPoints[2]-m_ViewPortY, m_DragPoints[1]-m_ViewPortX, m_DragPoints[3]-m_ViewPortY))
					{
						++selectedsum;
						it->M_SetSelection(true);
						selected++;
						if(selected==1) m_SplitFirst=&(*it);
						else if(selected==2) m_SplitSecond=&(*it);
						else if(selected>2) m_SplitFirst=m_SplitSecond=NULL;
					}
					else
					{
						selected=0;
						it->M_SetSelection(false);
					}
				}

				if(m_SplitFirst == &*(m_ActivePoly->end()-2) && m_SplitSecond == &*(m_ActivePoly->end()-1) && m_ActivePoly->begin()->M_Selected()
					|| selectedsum == vertexcount)
				{
					m_SplitFirst=m_SplitSecond=NULL;
				}
				else if(!(m_SplitFirst && m_SplitSecond) && (*m_ActivePoly->begin()).M_Selected() && (m_ActivePoly->M_Last().M_Selected()))
				{
					m_SplitFirst = &m_ActivePoly->M_Last();
					m_SplitSecond = &*m_ActivePoly->begin();
				}
			}
			else
			{
				for(C_Polygon::iterator it=m_ActivePoly->begin(); it!=m_ActivePoly->end(); ++it)
				{
					if(it->M_Selected())
					{
						std::pair<float, float> prevPos=it->M_Pos();
						emit S_SetPos(
							*it,
							prevPos.first+(x-M_RoundToPrecision(lmx, m_PointPrecision)),
							prevPos.second+(y-M_RoundToPrecision(lmy, m_PointPrecision)));
					}
				}
			}
		}
	}
	else
	{
		if(e->buttons() & Qt::MiddleButton)
		{
			m_ViewPortX-=m_LastMousePos.first - xraw;
			m_ViewPortY-=m_LastMousePos.second - yraw;
		}
		if(m_Mode==Edit)
		{
			for(C_Polygon::iterator it=m_ActivePoly->begin(); it!=m_ActivePoly->end(); ++it)
			{
				if(M_MouseOverVertex(xraw,yraw,*it)) it->M_SetHovering(true);
				else it->M_SetHovering(false);
			}
		}
	}
	updateGL();
	m_LastMousePos=std::make_pair(xraw,yraw);
}

void C_GLEditor::mouseReleaseEvent(QMouseEvent* e)
{
	if(e->buttons() & Qt::LeftButton)
	{
		float x=M_RoundToPrecision((float)e->pos().x()/(this->width()/2)-1.0f, m_PointPrecision);
		float y=-M_RoundToPrecision((float)e->pos().y()/(this->height()/2)-1.0f, m_PointPrecision);
		if(m_Mode==Insert)
		{
			emit S_SetPos(m_ActivePoly->M_Last(),x-m_ViewPortX,y-m_ViewPortY);
			m_ActivePoly->M_Last().M_SetSelection(false);
		}
		else
		{
			// Can't remember what this was for...caused some strange problems after m_Drag.
			/*
			for(C_Polygon::iterator it=m_ActivePoly->begin(); it!=m_ActivePoly->end(); ++it)
			{
				float lmx=m_LastMousePos.first;
				float lmy=m_LastMousePos.second;
				if(it->M_Selected())
				{
					std::pair<float, float> prevPos=it->M_Pos();
					emit S_SetPos(*it,prevPos.first+(x-lmx),prevPos.second+(y-lmy));
				}
			}
			*/
		}
	}
	if(m_Drag)
	{
		for(unsigned i=0; i<4; ++i) m_DragPoints[i]=0.0f;
		updateGL();
	}
	emit ((C_Editor*)parentWidget())->S_SplitPossible((m_SplitFirst&&m_SplitSecond));
}

void C_GLEditor::mouseDoubleClickEvent(QMouseEvent*)
{
	QList<C_Vertex*> ql;
	for(C_Polygon::iterator it=m_ActivePoly->begin(); it!=m_ActivePoly->end(); ++it)
	{
		if(it->M_Selected()) ql << &(*it);
	}
	if(ql.size()) emit(S_RequestColorDialog(ql));
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
	for(C_Polygon::iterator it=m_ActivePoly->begin(); it!=m_ActivePoly->end(); ++it)
	{
		std::pair<float, float> pos=it->M_Pos();
		if(pos.first > maxx) maxx=pos.first;
		if(pos.first < minx) minx=pos.first;
		if(pos.second > maxy) maxy=pos.second;
		if(pos.second < miny) miny=pos.second;
	}
	float diffx=maxx-((maxx-minx)/2);
	float diffy=maxy-((maxy-miny)/2);
	for(C_Polygon::iterator it=m_ActivePoly->begin(); it!=m_ActivePoly->end(); ++it)
	{
		std::pair<float, float> pos=it->M_Pos();
		emit S_SetPos(*it, pos.first-diffx, pos.second-diffy);
	}
	updateGL();
}

float C_GLEditor::M_RoundToPrecision(float num, float precision)
{
	if(num>0.5f)
	{
		return ((int)floor(num / precision))*precision;
	}
	else return ((int)ceil(num / precision))*precision;
}
