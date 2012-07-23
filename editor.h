#include <QtGui/QtGui>
#include "editor_gl.h"

class C_Editor : public QWidget
{
	Q_OBJECT
	private:
		C_GLEditor* m_Editor;
		QMenuBar* m_Menu;
		QStandardItemModel* m_Model;
		QStandardItem* m_Root;
		QHeaderView* m_HeaderView;
		QPushButton* m_Center;
		QPushButton* m_Save;
		QPushButton* m_Insert;
		QPushButton* m_Edit;
		QColorDialog* m_ColorDialog;
		QSplitter* m_Splitter;
		QTreeView* m_List;
		QList<C_Vertex*> m_VertsToColor;
	private slots:
		void S_Center();
		void S_Save();
		void S_AddToList(QStandardItem*, float, float);
		void S_UpdateList(QStandardItem* i);
		void S_SetInsertMode();
		void S_SetEditMode();
		void S_OpenColorDialog(QList<C_Vertex*>);
		void S_ColorChanged(const QColor&);
	public:
		C_Editor();
};
