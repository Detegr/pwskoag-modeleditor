#include <QtGui/QtGui>
#include "editor_gl.h"

class C_Editor : public QWidget
{
	Q_OBJECT
	private:
		C_GLEditor* m_Editor;
		QMenuBar* m_Menu;
		QStandardItemModel* m_Model;
		QHeaderView* m_HeaderView;
		QPushButton* m_Center;
		QPushButton* m_New;
		QPushButton* m_Insert;
		QPushButton* m_Edit;
		QColorDialog* m_ColorDialog;
		QSplitter* m_Splitter;
		QTreeView* m_List;
		QList<C_Vertex*> m_VertsToColor;
	private slots:
		void S_Center();
		void S_NewPolygon();
		void S_AddToList(QStandardItem*, float, float);
		void S_UpdateList(QStandardItem* i);
		void S_SetInsertMode();
		void S_SetEditMode();
		void S_OpenColorDialog(QList<C_Vertex*>);
		void S_ColorChanged(const QColor&);
		void S_OpenFile(const QString&);
		void S_SaveFile(const QString& path);
	public:
		C_Editor();
};
