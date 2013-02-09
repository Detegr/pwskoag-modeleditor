#include "editor.h"
#include <iostream>
#include <sstream>
#include "filereader.h"
#include <assert.h>

#define SIDEBAR_WIDTH 140

C_Editor::C_Editor() : m_Editor(NULL)
{
	m_Init();
}

void C_Editor::m_Init()
{
	m_Open=new QFileDialog(this);
	m_Open->setNameFilter("*.2dmodel");
	m_Open->setAcceptMode(QFileDialog::AcceptOpen);
	m_Open->setFileMode(QFileDialog::ExistingFile);
	m_Open->setOption(QFileDialog::DontUseNativeDialog);
	QObject::connect(m_Open, SIGNAL(fileSelected(const QString&)), this, SLOT(S_OpenFile(const QString&)));
	m_Save=new QFileDialog(this);
	m_Save->setFileMode(QFileDialog::AnyFile);
	m_Save->setAcceptMode(QFileDialog::AcceptSave);
	m_Save->setDefaultSuffix("2dmodel");
	m_Save->setNameFilter("*.2dmodel");
	m_Save->setConfirmOverwrite(true);
	m_Save->setOption(QFileDialog::DontUseNativeDialog);
	QObject::connect(m_Save, SIGNAL(fileSelected(const QString&)), this, SLOT(S_SaveFile(const QString&)));
	QObject::connect(this, SIGNAL(S_SaveAs()), this, SLOT(S_SaveDialog()));

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
	frame->setFixedWidth(SIDEBAR_WIDTH-10);
	QHBoxLayout *layout = new QHBoxLayout(this);
	QVBoxLayout *vb = new QVBoxLayout();
	vb->addWidget(frame);
	m_Splitter->setOrientation(Qt::Vertical);
	m_Model = new QStandardItemModel(this);
	QObject::connect(m_Model, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(S_UpdateList(QStandardItem*)));
	m_Model->setColumnCount(3);
	QStandardItem* root = new QStandardItem("Object");

	m_Model->appendRow(root);
	vb->setAlignment(Qt::AlignTop);

	m_Editor = new C_GLEditor(this, root);
	QObject::connect(m_Editor, SIGNAL(S_MousePressed(QStandardItem*, float, float, int, const QString&)), this, SLOT(S_AddToList(QStandardItem*, float, float, int, const QString&)));
	QObject::connect(m_Editor, SIGNAL(S_SetPos(C_Vertex&, float, float)), this, SLOT(S_SetPos(C_Vertex&, float, float)));
	QObject::connect(m_Editor, SIGNAL(S_SelectionChanged()), this, SLOT(S_UpdateEditorSelection()));
	QObject::connect(m_Editor, SIGNAL(S_ClearSelection()), this, SLOT(S_ClearSelection()));

	m_ColorDialog = new QColorDialog(this);
	m_ColorDialog->hide();
	QObject::connect(m_Editor, SIGNAL(S_RequestColorDialog(QList<C_Vertex*>)), this, SLOT(S_OpenColorDialog(QList<C_Vertex*>)));
	QObject::connect(m_ColorDialog, SIGNAL(colorSelected(const QColor&)), this, SLOT(S_ColorChanged(const QColor&)));

	m_DataEditor = new C_DataEditor(this);

	m_New->setFixedWidth(SIDEBAR_WIDTH);
	m_Center->setFixedWidth(SIDEBAR_WIDTH);

	m_List = new QTreeView(this);
	m_Selection = new QItemSelectionModel(m_Model);
	QObject::connect(m_Selection, SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)), this, SLOT(S_SelectionChanged(const QItemSelection&, const QItemSelection&)));
	QObject::connect(
			m_List,
			SIGNAL(doubleClicked(const QModelIndex&)),
			this,
			SLOT(S_OpenDataDialog(const QModelIndex&)));
	QObject::connect(m_DataEditor, SIGNAL(accepted()), this, SLOT(S_SetData()));

	m_Splitter->addWidget(m_List);
	m_List->setHeaderHidden(true);
	m_List->setModel(m_Model);
	m_List->setSelectionModel(m_Selection);
	m_List->setFixedWidth(SIDEBAR_WIDTH);
	m_List->setIndentation(10);
	m_List->setColumnWidth(0, 60);
	m_List->setColumnWidth(1, 40);
	m_List->setColumnWidth(2, 10);
	m_List->setFirstColumnSpanned(root->row(), m_Model->indexFromItem(root->parent()), true);
	m_List->expandAll();

	vb->addWidget(m_New);
	vb->addWidget(m_Center);
	vb->addWidget(m_Splitter);
	layout->addLayout(vb);
	layout->addWidget(m_Editor);

	m_Delete = new QShortcut(QKeySequence(tr("Delete")), this);
	QObject::connect(m_Delete, SIGNAL(activated()), this, SLOT(S_DeletePoint()));

	m_SelectedItem=NULL;

	setLayout(layout);
	setMinimumSize(800,600);
	m_AutoUpdate=true;

}

C_Editor::C_Editor(const QString& openwith)
{
	m_Init();
	S_OpenFile(openwith);
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

void C_Editor::S_SetPos(C_Vertex& v, float x, float y)
{	
	std::pair<QStandardItem*, QStandardItem*> items=m_Editor->m_ActivePoly->M_GetItems(v);
	QStandardItem* l=items.first;
	QStandardItem* r=items.second;
	QString rs,ls;
	ls.setNum(x);
	rs.setNum(y);
	m_AutoUpdate=false;
	l->setText(ls);
	r->setText(rs);
	v.M_SetPos(x,y);
	m_AutoUpdate=true;
}

void C_Editor::S_DeletePoint()
{
	if(m_SelectedItem)
	{
		int row=m_SelectedItem->row();
		if(!m_SelectedItem->parent())
		{
			QMessageBox msg;
			msg.setText("This will delete the whole polygon. Are you sure?");
			msg.setStandardButtons(QMessageBox::Ok|QMessageBox::Cancel);
			msg.setDefaultButton(QMessageBox::Ok);
			int ret=msg.exec();
			if(ret==QMessageBox::Ok)
			{
				C_Polygon* poly=m_Editor->m_Polygons [m_SelectedItem->row()];
				m_Editor->m_Polygons.erase(m_Editor->m_Polygons.begin()+m_SelectedItem->row());
				delete poly;
				m_Editor->m_ActivePoly=m_Editor->m_Polygons.back();
				m_Model->removeRows(m_SelectedItem->row(), 1, m_Model->indexFromItem(m_SelectedItem->parent()));
				QStandardItem* newchild;
				do newchild=m_Model->invisibleRootItem()->child(row--); while(!newchild && row>=0);
				m_SelectedItem=newchild;
			}
			else return;
		}
		else
		{
			m_Editor->m_Polygons[m_SelectedItem->parent()->row()]->M_Delete(m_SelectedItem->row());
			QStandardItem* parent=m_SelectedItem->parent();
			m_Model->removeRows(m_SelectedItem->row(), 1, m_Model->indexFromItem(m_SelectedItem->parent()));
			QStandardItem* newchild;
			do newchild=parent->child(row--); while(!newchild && row>=0);
			if(!newchild) newchild=parent;
			m_SelectedItem=newchild;
		}
		if(!m_Model->hasChildren()) S_NewPolygon();
		m_Editor->updateGL();
	}
}

QStandardItem* C_Editor::S_NewPolygon(const std::string& name)
{
	QStandardItem *newroot = new QStandardItem(name.c_str());
	m_Model->appendRow(newroot);
	m_List->expand(m_Model->indexFromItem(newroot));
	m_List->setFirstColumnSpanned(newroot->row(), m_Model->indexFromItem(newroot->parent()), true);
	C_Polygon* newpoly=new C_Polygon(newroot);
	m_Editor->m_Polygons.push_back(newpoly);
	m_Editor->m_ActivePoly=newpoly;
	return newroot;
}

void C_Editor::S_SelectionChanged(const QItemSelection& s, const QItemSelection& ds)
{
	for(int i=0; i<ds.indexes().size(); ++i)
	{
		QStandardItem* item=m_Model->itemFromIndex(ds.indexes()[i]);
		int j=0;
		for(C_Polygon::iterator it=m_Editor->m_ActivePoly->begin(); it!=m_Editor->m_ActivePoly->end(); ++it, ++j)
		{
			if(j==item->row()) it->M_SetSelection(false);
		}
	}
	for(int i=0; i<s.indexes().size(); ++i)
	{
		QStandardItem* item=m_Model->itemFromIndex(s.indexes()[i]);
		m_SelectedItem=item;
		m_Editor->m_ActivePoly=m_Editor->m_Polygons[item->parent()?item->parent()->row():item->row()]; // TODO: Useless to do every time
		int j=0;
		for(C_Polygon::iterator it=m_Editor->m_ActivePoly->begin(); it!=m_Editor->m_ActivePoly->end(); ++it, ++j)
		{
			if(j==item->row()) it->M_SetSelection(true);
		}
	}
	m_Editor->updateGL();
}

void C_Editor::S_UpdateList(QStandardItem* i)
{
	if(i->parent() && m_AutoUpdate) // Allow renaming of polygon objects
	{
		const std::string& newdatastr=i->text().toStdString();
		float olddata=i->data().toFloat();
		std::stringstream ss;
		ss << newdatastr;
		float newdata;
		ss >> newdata;
		if(newdatastr.find(',') == std::string::npos)
		{
			i->setData(newdata);
			std::pair<float,float> oldp=m_Editor->m_ActivePoly->M_Vertex(i->row()).M_Pos();
			if(i->column()==0)
			{
				m_Editor->m_ActivePoly->M_Vertex(i->row()).M_SetPos(newdata, oldp.second);
			}
			else if(i->column()==1)
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
}

void C_Editor::S_AddToList(QStandardItem* obj, float x, float y, int pos, const QString& data)
{
	QString f,s;
	f.setNum(x);
	s.setNum(y);
	QStandardItem* ix = new QStandardItem(f);
	ix->setData(QVariant(x));
	QStandardItem* iy = new QStandardItem(s);
	iy->setData(QVariant(y));
	QStandardItem* id;
	if(data.length())
	{
		id=new QStandardItem("*");
	}
	else id=new QStandardItem("");
	obj->appendRow(QList<QStandardItem*>() << ix << iy << id);
	m_Editor->m_ActivePoly->M_Add(x,y,pos);
	if(data.length())
	{
		m_Editor->m_ActivePoly->M_Last().M_SetData(data);
	}
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
	std::vector<std::string> strs=C_FileReader::M_ReadToArray(path.toStdString());
	std::stringstream ss;
	ss.precision(3);
	m_Model->clear();
	m_Model->setColumnCount(3);
	m_List->setColumnWidth(0, 60);
	m_List->setColumnWidth(1, 40);
	m_List->setColumnWidth(2, 10);
	for(std::vector<C_Polygon*>::iterator it=m_Editor->m_Polygons.begin(); it!=m_Editor->m_Polygons.end(); ++it)
	{
		delete *it;
	}
	m_Editor->m_Polygons=std::vector<C_Polygon*>();

	float x=0; float y=0;
	unsigned i=0;
	QStandardItem* newroot=NULL;
	QString data;
	for(std::vector<std::string>::iterator it=strs.begin(); it!=strs.end(); ++it, ++i)
	{
		data=QString();
		if(it->find('>')==0)
		{
			newroot=S_NewPolygon(it->substr(8, std::string::npos));
			i=0;
			continue;
		}
		ss << *it++;
		ss >> x;
		ss.clear();
		ss << *it;
		ss >> y;
		ss.clear();
		if((it+1) != strs.end())
		{
			++it;
			if(it->find('<')==0)
			{
				data = QString(it->substr(8, std::string::npos).c_str());
			} else it--;
		}
		assert(newroot!=NULL);
		S_AddToList(newroot,x,y,-1,data);
	}
	m_Editor->updateGL();
	m_EditedFile=QDir(path);
	m_EditedFile.makeAbsolute();
}

void C_Editor::S_SaveFile(const QString& path)
{
	std::ofstream out(path.toStdString().c_str());
	unsigned i=0;
	for(std::vector<C_Polygon*>::const_iterator it=m_Editor->m_Polygons.begin(); it!=m_Editor->m_Polygons.end(); ++it, ++i)
	{
		QString polyname = m_Model->item(i)->text();
		out << "> POLY: " << polyname.toStdString() << "\n";
		for(C_Polygon::const_iterator pi=(*it)->begin(); pi!=(*it)->end(); ++pi)
		{
			out << pi->M_Pos().first << "\n" << pi->M_Pos().second << "\n";
			if(pi->M_GetData().length())
			{
				out << "< DATA: " << pi->M_GetData().toStdString() << "\n";
			}
		}
	}
	out.close();
}

void C_Editor::S_SaveOrSaveAs()
{
	if(m_EditedFile.path().length() > 1) S_SaveFile(m_EditedFile.path());
	else emit S_SaveAs();
}

void C_Editor::S_ModeChanged(QAction* a)
{
	if(a->text() == tr("&Polygon")) m_Editor->m_DrawMode=C_GLEditor::Polygon;
	else if(a->text() == tr("Line &strip"))	m_Editor->m_DrawMode=C_GLEditor::LineStrip;
	else if(a->text() == tr("Line &loop"))	m_Editor->m_DrawMode=C_GLEditor::LineLoop;
	m_Editor->updateGL();
}


void C_Editor::keyPressEvent(QKeyEvent* e)
{
	if(e->key() == Qt::Key_Control)
	{
		m_Editor->M_SetPrecision(0.05f);
	}
}
void C_Editor::keyReleaseEvent(QKeyEvent* e)
{
	if(e->key() == Qt::Key_Control)
	{
		m_Editor->M_SetPrecision(0.01f);
	}
}
void C_Editor::S_Split()
{
	m_Editor->M_Split();
}

void C_Editor::S_ReverseActivePolygon()
{
	m_Editor->m_ActivePoly->M_Reverse();
	for(C_Polygon::iterator it=m_Editor->m_ActivePoly->begin(); it!=m_Editor->m_ActivePoly->end(); ++it)
	{
		std::pair<float, float> pos=it->M_Pos();
		emit S_SetPos(*it, pos.first, pos.second);
	}
}

void C_Editor::S_OpenDataDialog(const QModelIndex& i)
{
	if(i.column() == 2) // Data column
	{
		m_List->setEditTriggers(QAbstractItemView::NoEditTriggers);
		m_DataEditor->setData(i, m_Editor->m_ActivePoly->M_Vertex(i.row()).M_GetData());
		m_DataEditor->setVisible(true);
	}
}

void C_Editor::S_SetData()
{
	QString data=m_DataEditor->getData();
	m_Editor->m_ActivePoly->M_Vertex(m_DataEditor->getModelIndex().row()).M_SetData(data);
	QStandardItem* item=m_Model->itemFromIndex(m_DataEditor->getModelIndex());
	if(data.length()) item->setText("*");
	else item->setText("");
	m_List->setEditTriggers(QAbstractItemView::DoubleClicked);
}

void C_Editor::S_ClearSelection()
{
	m_Selection->clearSelection();
}

void C_Editor::S_UpdateEditorSelection()
{
	int i=0;
	for(C_Polygon::iterator it=m_Editor->m_ActivePoly->begin(); it!=m_Editor->m_ActivePoly->end(); ++it, ++i)
	{
		if(it->M_Selected())
		{
			m_Selection->select(m_Model->index(i, 0, m_Model->indexFromItem(m_Editor->m_ActivePoly->M_Root())), QItemSelectionModel::Select|QItemSelectionModel::Rows);
		}
		else
		{
			m_Selection->select(m_Model->index(i, 0, m_Model->indexFromItem(m_Editor->m_ActivePoly->M_Root())), QItemSelectionModel::Deselect|QItemSelectionModel::Rows);
		}
	}
}
