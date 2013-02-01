#pragma once
#include <QtGui/QtGui>

class C_DataEditor : public QDialog
{
	Q_OBJECT
	private:
		QString m_Data;
		QPushButton* m_OkButton;
		QLineEdit* m_Editor;
		QModelIndex m_ModelIndex;

	public:

		C_DataEditor(QWidget* parent=NULL) : QDialog(parent)
		{
			setFixedWidth(250);
			setFixedHeight(80);
			setModal(true);
			m_OkButton = new QPushButton(tr("Ok"));
			m_OkButton->setDefault(true);
			QObject::connect(m_OkButton, SIGNAL(clicked()), this, SLOT(ok()));

			m_Editor = new QLineEdit;

			QVBoxLayout* layout = new QVBoxLayout;
			layout->addWidget(m_Editor);
			layout->addWidget(m_OkButton);
			setLayout(layout);
			hide();
		}

		void setData(const QModelIndex& i, const QString& data)
		{
			m_ModelIndex=i;
			m_Data=data;
			m_Editor->setText(m_Data);
		}

		QString getData() const
		{
			return m_Data;
		}

		const QModelIndex& getModelIndex() const
		{
			return m_ModelIndex;
		}

	public slots:
		void ok()
		{
			m_Data=m_Editor->displayText();
			accept();
		}
};
