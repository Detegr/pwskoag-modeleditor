#include "editor.h"

C_Editor::C_Editor() : m_Editor(NULL)
{
	m_Center = new QPushButton(tr("Center"), this);
	QObject::connect(m_Center, SIGNAL(clicked()), this, SLOT(S_Center()));
	m_Save = new QPushButton(tr("Save"), this);
	QObject::connect(m_Save, SIGNAL(clicked()), this, SLOT(S_Save()));
	m_Btn3 = new QPushButton(tr("3"), this);
	m_Editor = new C_GLEditor(this);

	QHBoxLayout *layout = new QHBoxLayout(this);
	QVBoxLayout *vb = new QVBoxLayout();
	vb->setAlignment(Qt::AlignTop);

	m_Save->setFixedWidth(80);
	m_Center->setFixedWidth(80);
	m_Btn3->setFixedWidth(80);
	vb->addWidget(m_Save);
	vb->addWidget(m_Center);
	vb->addWidget(m_Btn3);
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
