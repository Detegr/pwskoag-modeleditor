#pragma once
#include <QtGui/QtGui>

class C_DataEditor : public QDialog
{
	Q_OBJECT
	friend class C_Editor;
	private:
		QString m_Data;
		QPushButton*	m_OkButton;
		QLineEdit*		m_Editor;
		QModelIndex		m_ModelIndex;
		QCheckBox*		m_BezierButton;

	public:

		C_DataEditor(QWidget* parent=NULL) : QDialog(parent)
		{
			setFixedWidth(250);
			setFixedHeight(100);
			setModal(true);
			m_OkButton = new QPushButton(tr("Ok"));
			m_OkButton->setDefault(true);
			QObject::connect(m_OkButton, SIGNAL(clicked()), this, SLOT(ok()));

			m_Editor = new QLineEdit;

			QVBoxLayout* layout = new QVBoxLayout(this);
			QHBoxLayout* bezier_layout = new QHBoxLayout(this);
			m_BezierButton = new QCheckBox(this);
			QLabel* beziertext = new QLabel(this);

			bezier_layout->addWidget(m_BezierButton);
			beziertext->setText(tr("Is bezier?"));
			bezier_layout->addWidget(beziertext);

			layout->addWidget(m_Editor);
			layout->addLayout(bezier_layout);
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

		bool getIsBezier() const
		{
			return m_BezierButton->isChecked();
		}

		void setIsBezier(bool b)
		{
			m_BezierButton->setChecked(b);
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
