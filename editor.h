#include <QtGui/QtGui>
#include "editor_gl.h"

class C_Editor : public QWidget
{
	Q_OBJECT
	private:
		C_GLEditor* m_Editor;
		QMenuBar* m_Menu;
		QStandardItemModel* m_Model;
		QStandardItem* m_SelectedItem;
		QHeaderView* m_HeaderView;
		QPushButton* m_Center;
		QPushButton* m_New;
		QPushButton* m_Insert;
		QPushButton* m_Edit;
		QColorDialog* m_ColorDialog;
		QSplitter* m_Splitter;
		QTreeView* m_List;
		QList<C_Vertex*> m_VertsToColor;
		QShortcut* m_Delete;
		bool m_AutoUpdate;
	private slots:
		void S_Center();
		void S_DeletePoint();
		void S_SetPos(C_Vertex&, float, float);
		QStandardItem* S_NewPolygon(const std::string& name="Object");
		void S_SetActivePoly(const QModelIndex&);
		void S_AddToList(QStandardItem*, float, float);
		void S_UpdateList(QStandardItem* i);
		void S_SetInsertMode();
		void S_SetEditMode();
		void S_OpenColorDialog(QList<C_Vertex*>);
		void S_ColorChanged(const QColor&);
		void S_OpenFile(const QString&);
		void S_SaveFile(const QString& path);
		void S_ModeChanged(QAction* a);
	protected:
		void keyPressEvent(QKeyEvent* e);
		void keyReleaseEvent(QKeyEvent* e);
	public:
		C_Editor();
};
