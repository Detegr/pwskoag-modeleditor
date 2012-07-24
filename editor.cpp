#include "editor.h"
#include <iostream>
#include <sstream>
#include "filereader.h"

C_Editor::C_Editor() : m_Editor(NULL)
{
	m_Center = new QPushButton(tr("Center"), this);
	QObject::connect(m_Center, SIGNAL(clicked()), this, SLOT(S_Center()));
	m_New = new QPushButton(tr("New polygon"), this);
	QObject::connect(m_New, SIGNAL(clicked()), this, SLOT(S_NewPolygon()));
	m_Insert = new QPushButton(tr("Insert"), this);
	m_Insert->setDown(true);
	QObject::connect(m_Insert, SIGNAL(clicked()), this, SLOT(S_SetInsertMode()));
	m_Edit = new QPushButton(tr("Edit"), this);
	QObject::connect(m_Edit, SIGNAL(clicked()), this, SLOT(S_SetEditMode()));
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
	m_Model = new QStandardItemModel(this);
	QObject::connect(m_Model, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(S_UpdateList(QStandardItem*)));
	m_Model->setColumnCount(2);
	QStandardItem* root = new QStandardItem("Object 1");
	m_Model->appendRow(root);
	vb->setAlignment(Qt::AlignTop);

	m_Editor = new C_GLEditor(this, root);
	QObject::connect(m_Editor, SIGNAL(S_MousePressed(QStandardItem*, float, float)), this, SLOT(S_AddToList(QStandardItem*, float, float)));

	m_ColorDialog = new QColorDialog(this);
	m_ColorDialog->hide();
	QObject::connect(m_Editor, SIGNAL(S_RequestColorDialog(QList<C_Vertex*>)), this, SLOT(S_OpenColorDialog(QList<C_Vertex*>)));
	QObject::connect(m_ColorDialog, SIGNAL(colorSelected(const QColor&)), this, SLOT(S_ColorChanged(const QColor&)));

	m_New->setFixedWidth(160);
	m_Center->setFixedWidth(160);

	m_List = new QTreeView(this);
	QObject::connect(m_List, SIGNAL(clicked(const QModelIndex&)), this, SLOT(S_SetActivePoly(const QModelIndex&)));
	m_Splitter->addWidget(m_List);
	m_List->setHeaderHidden(true);
	m_List->setModel(m_Model);
	m_List->setFixedWidth(160);
	m_List->setIndentation(12);
	m_List->setColumnWidth(0, 80);
	m_List->setColumnWidth(1, 20);
	m_List->setFirstColumnSpanned(0, m_Model->index(0, -1), true);
	m_List->expandAll();

	vb->addWidget(m_New);
	vb->addWidget(m_Center);
	vb->addWidget(m_Splitter);
	layout->addLayout(vb);
	layout->addWidget(m_Editor);

	setLayout(layout);
	setMinimumSize(800,600);
}

void C_Editor::S_OpenColorDialog(QList<C_Vertex*> affectedverts)
{
	m_ColorDialog->setVisible(true);
	m_VertsToColor=affectedverts; // Copying... Don't know if this could be done more cleverly.
}
void C_Editor::S_ColorChanged(const QColor& c)
{
	for(QList<C_Vertex*>::iterator it=m_VertsToColor.begin(); it!=m_VertsToColor.end(); ++it)
	{
		(*it)->M_SetColor(c);
	}
}

void C_Editor::S_Center()
{
	m_Editor->M_Center();
}

void C_Editor::S_NewPolygon()
{
	QStandardItem *newroot = new QStandardItem("Object");
	m_Model->appendRow(newroot);
	m_Editor->m_Polygons.push_back(C_Polygon(newroot));
	m_Editor->m_ActivePoly=&m_Editor->m_Polygons.back();
}

void C_Editor::S_SetActivePoly(const QModelIndex& index)
{
	QStandardItem* i=m_Model->itemFromIndex(index);
	while(i->parent()) i=(QStandardItem*)parent();
	std::cout << i->row() << std::endl;
}

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
		std::pair<float,float> oldp=m_Editor->m_ActivePoly->M_Vertex(i->row()).M_Pos();
		if(i->column()==0)
		{
			m_Editor->m_ActivePoly->M_Vertex(i->row()).M_SetPos(newdata, oldp.second);
		}
		else
		{
			m_Editor->m_ActivePoly->M_Vertex(i->row()).M_SetPos(oldp.first, newdata);
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
	m_Editor->m_ActivePoly->M_Add(x,y);
}

void C_Editor::S_SetInsertMode()
{
	m_Insert->setDown(true);
	m_Edit->setDown(false);
	m_Editor->m_Mode=C_GLEditor::Insert;
	m_Editor->setMouseTracking(false);
}
void C_Editor::S_SetEditMode()
{
	m_Insert->setDown(false);
	m_Edit->setDown(true);
	m_Editor->m_Mode=C_GLEditor::Edit;
	m_Editor->setMouseTracking(true);
}

void C_Editor::S_OpenFile(const QString& path)
{
/*
	std::vector<std::string> strs=C_FileReader::M_ReadToArray(path.toStdString());
	std::stringstream ss;
	ss.precision(3);
	m_Model->clear();
	m_Root = new QStandardItem("Object 1");
	m_Model->setColumnCount(2);
	m_Model->appendRow(m_Root);
	m_List->setFirstColumnSpanned(0, m_Model->indexFromItem(m_Root), true);
	m_Editor->m_Polygon=C_Polygon(m_Root);
	m_List->setFirstColumnSpanned(0, m_Model->index(0, -1), true);

	float x=0; float y=0;
	for(std::vector<std::string>::iterator it=strs.begin(); it!=strs.end(); ++it)
	{
		ss << *it++;
		ss >> x;
		ss.clear();
		ss << *it;
		ss >> y;
		ss.clear();
		S_AddToList(m_Root,x,y);
	}
*/
}

void C_Editor::S_SaveFile(const QString& path)
{
/*
	std::ofstream out(path.toStdString().c_str());
	for(C_Polygon::iterator it=m_Editor->m_Polygon.begin(); it!=m_Editor->m_Polygon.end(); ++it)
	{
		out << it->M_Pos().first << "\n" << it->M_Pos().second << "\n";
	}
	out.close();
*/
}
