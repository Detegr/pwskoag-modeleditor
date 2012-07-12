#include "editor.h"

C_Editor::C_Editor() : m_Editor(NULL)
{
	m_Center = new QPushButton(tr("Center"), this);
	QObject::connect(m_Center, SIGNAL(clicked()), this, SLOT(S_Center()));
	m_Save = new QPushButton(tr("Save"), this);
	QObject::connect(m_Save, SIGNAL(clicked()), this, SLOT(S_Save()));
	m_List = new QTreeView(this);
	m_Splitter = new QSplitter(this);
	m_Editor = new C_GLEditor(this);
	QObject::connect(m_Editor, SIGNAL(S_MousePressed(float, float)), this, SLOT(S_AddToList(float, float)));

	QHBoxLayout *layout = new QHBoxLayout(this);
	QVBoxLayout *vb = new QVBoxLayout();
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
	setMinimumSize(640,480);
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
void C_Editor::S_UpdateList(QStandardItem* i)
{
	std::cout << i->parent()->row() << " " << i->row() << " " << i->column() << std::endl;
	std::cout << i->text().toStdString() << std::endl;
	std::cout << i->data().toFloat() << std::endl;
}

void C_Editor::S_AddToList(float x, float y)
{
	QString f,s;
	f.setNum(x);
	s.setNum(y);
	QStandardItem* ix = new QStandardItem(f);
	ix->setData(QVariant(x));
	QStandardItem* iy = new QStandardItem(s);
	iy->setData(QVariant(y));
	m_Root->appendRow(QList<QStandardItem*>() << ix << iy);
	m_Editor->m_Points.push_back(std::make_pair(x,y));
}
