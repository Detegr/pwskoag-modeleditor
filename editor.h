#pragma once

#include <QtGui/QtGui>
#include "editor_gl.h"
#include "dataeditor.h"

class C_Editor : public QWidget
{
	friend class C_GLEditor;
	Q_OBJECT
	private:
		C_GLEditor* m_Editor;
		QFileDialog* m_Open;
		QFileDialog* m_Save;
		QMenuBar* m_Menu;
		QStandardItemModel* m_Model;
		QStandardItem* m_SelectedItem;
		QHeaderView* m_HeaderView;
		QPushButton* m_Center;
		QPushButton* m_New;
		QPushButton* m_Insert;
		QPushButton* m_Edit;
		QColorDialog* m_ColorDialog;
		C_DataEditor* m_DataEditor;
		QSplitter* m_Splitter;
		QTreeView* m_List;
		QList<C_Vertex*> m_VertsToColor;
		QShortcut* m_Delete;
		bool m_AutoUpdate;
		QDir m_EditedFile;

		void m_Init();
	private slots:
		void S_Center();
		void S_DeletePoint();
		void S_SetPos(C_Vertex&, float, float);
		QStandardItem* S_NewPolygon(const std::string& name="Object");
		void S_SetActivePoly(const QModelIndex&);
		void S_AddToList(QStandardItem*, float, float, int);
		void S_UpdateList(QStandardItem* i);
		void S_SetInsertMode();
		void S_SetEditMode();
		void S_OpenColorDialog(QList<C_Vertex*>);
		void S_ColorChanged(const QColor&);
		void S_OpenFile(const QString&);
		void S_SaveFile(const QString& path);
		void S_SaveOrSaveAs();
		void S_ModeChanged(QAction* a);
		void S_OpenDialog()
		{
			m_Open->setVisible(true);
		}
		void S_SaveDialog()
		{
			m_Save->setVisible(true);
		}
		void S_ReverseActivePolygon();
		void S_OpenDataDialog(const QModelIndex&);
		void S_SetData();
	signals:
		void S_SplitPossible(bool b);
		void S_SaveAs();
	protected:
		void keyPressEvent(QKeyEvent* e);
		void keyReleaseEvent(QKeyEvent* e);
	public slots:
		void S_Split();
	public:
		C_Editor();
		C_Editor(const QString& openwith);
};
