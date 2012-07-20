#include "editor.h"

C_Editor::C_Editor() : m_Editor(NULL)
{
	m_Center = new QPushButton(tr("Center"), this);
	QObject::connect(m_Center, SIGNAL(clicked()), this, SLOT(S_Center()));
	m_Save = new QPushButton(tr("Save"), this);
	QObject::connect(m_Save, SIGNAL(clicked()), this, SLOT(S_Save()));
	m_Insert = new QPushButton(tr("Insert"), this);
	m_Insert->setDown(true);
	QObject::connect(m_Insert, SIGNAL(clicked()), this, SLOT(S_SetInsertMode()));
	m_Edit = new QPushButton(tr("Edit"), this);
	QObject::connect(m_Edit, SIGNAL(clicked()), this, SLOT(S_SetEditMode()));
	m_List = new QTreeView(this);
	m_Splitter = new QSplitter(this);

	QFrame* frame = new QFrame(this);
	QHBoxLayout *editmode = new QHBoxLayout(this);
	editmode->addWidget(m_Insert);
	editmode->addWidget(m_Edit);
	frame->setLayout(editmode);
	frame->setFixedWidth(150);
	QHBoxLayout *layout = new QHBoxLayout(this);
	QVBoxLayout *vb = new QVBoxLayout();
	vb->addWidget(frame);
	m_Splitter->setOrientation(Qt::Vertical);
	m_Splitter->addWidget(m_List);
	m_List->setHeaderHidden(true);
	m_Model = new QStandardItemModel(this);
	QObject::connect(m_Model, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(S_UpdateList(QStandardItem*)));
	m_Model->setColumnCount(2);
	m_Root = new QStandardItem("Object 1");
	m_Model->appendRow(m_Root);
	m_List->setModel(m_Model);
	vb->setAlignment(Qt::AlignTop);

	m_Editor = new C_GLEditor(this, m_Root);
	QObject::connect(m_Editor, SIGNAL(S_MousePressed(QStandardItem*, float, float)), this, SLOT(S_AddToList(QStandardItem*, float, float)));

	m_Save->setFixedWidth(160);
	m_Center->setFixedWidth(160);
	m_List->setFixedWidth(160);
	m_List->setIndentation(12);
	m_List->setColumnWidth(0, 80);
	m_List->setColumnWidth(1, 20);
	m_List->setFirstColumnSpanned(0, m_Model->index(0, -1), true);
	m_List->expandAll();
	vb->addWidget(m_Save);
	vb->addWidget(m_Center);
	vb->addWidget(m_Splitter);
	layout->addLayout(vb);
	layout->addWidget(m_Editor);

	setLayout(layout);
	setMinimumSize(800,600);
}

void C_Editor::S_Center()
{
	m_Editor->M_Center();
}

void C_Editor::S_Save()
{
	m_Editor->M_Dump();
}

#include <iostream>
#include <sstream>
void C_Editor::S_UpdateList(QStandardItem* i)
{
	const std::string& newdatastr=i->text().toStdString();
	float olddata=i->data().toFloat();
	std::stringstream ss;
	ss << newdatastr;
	float newdata;
	ss >> newdata;
	if(newdata>=-1.0f && newdata<=1.0f && newdatastr.find(',') == std::string::npos)
	{
		i->setData(newdata);
		std::pair<float,float> oldp=m_Editor->m_Polygon.m_Verts[i->row()].M_Pos();
		if(i->column()==0)
		{
			m_Editor->m_Polygon.m_Verts[i->row()].M_SetPos(newdata, oldp.second);
		}
		else
		{
			m_Editor->m_Polygon.m_Verts[i->row()].M_SetPos(oldp.first, newdata);
		}
		m_Editor->updateGL();
	}
	else
	{
		std::stringstream ss;
		ss << olddata;
		std::string oldstr;
		ss >> oldstr;
		QString oldqstr(oldstr.c_str());
		i->setText(oldqstr);
	}
}

void C_Editor::S_AddToList(QStandardItem* obj, float x, float y)
{
	QString f,s;
	f.setNum(x);
	s.setNum(y);
	QStandardItem* ix = new QStandardItem(f);
	ix->setData(QVariant(x));
	QStandardItem* iy = new QStandardItem(s);
	iy->setData(QVariant(y));
	obj->appendRow(QList<QStandardItem*>() << ix << iy);
	m_Editor->m_Polygon.M_Add(C_Vertex(x,y, QColor::fromRgbF(1.0f, 0.0f, 0.0f, 1.0f)));
}

void C_Editor::S_SetInsertMode()
{
	m_Insert->setDown(true);
	m_Edit->setDown(false);
	m_Editor->m_Mode=C_GLEditor::Insert;
}
void C_Editor::S_SetEditMode()
{
	m_Insert->setDown(false);
	m_Edit->setDown(true);
	m_Editor->m_Mode=C_GLEditor::Edit;
}
