#include <QtGui/QtGui>
#include "editor_gl.h"

class C_Editor : public QWidget
{
	Q_OBJECT
	private:
		QStandardItemModel* m_Model;
		QStandardItem* m_Root;
		QHeaderView* m_HeaderView;
		QPushButton* m_Center;
		QPushButton* m_Save;
		QSplitter* m_Splitter;
		QTreeView* m_List;
		C_GLEditor* m_Editor;
	private slots:
		void S_Center();
		void S_Save();
		void S_AddToList(float, float);
		void S_UpdateList(QStandardItem* i);
	public:
		C_Editor();
};
