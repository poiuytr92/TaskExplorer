#include "stdafx.h"
#include "../Common/Settings.h"
#include "MultiErrorDialog.h"
#ifdef WIN32
#include "../API/Windows/ProcessHacker.h"
#endif


CMultiErrorDialog::CMultiErrorDialog(const QString& Message, QList<STATUS> Errors, QWidget* parent)
	: QDialog(parent)
{
	this->setWindowTitle(tr("TaskExplorer - Error"));
	m_pMainLayout = new QGridLayout(this);

	int Row = 0;
	m_pMainLayout->addWidget(new QLabel(Message), Row++, 0, 1, 4);

	m_pErrors = new CPanelWidgetEx();
	
	m_pErrors->GetTree()->setHeaderLabels(tr("Message|Status|Error").split("|"));

	m_pErrors->GetView()->setSelectionMode(QAbstractItemView::ExtendedSelection);
	m_pErrors->GetView()->setSortingEnabled(false);

	m_pMainLayout->addWidget(m_pErrors, Row++, 0, 1, 4);

	m_pButtonBox = new QDialogButtonBox();
	m_pButtonBox->setOrientation(Qt::Horizontal);
	m_pButtonBox->setStandardButtons(QDialogButtonBox::Ok|QDialogButtonBox::Cancel);
	m_pMainLayout->addWidget(m_pButtonBox, Row++, 0, 1, 4);
 
	connect(m_pButtonBox,SIGNAL(accepted()),this,SLOT(accept()));
	connect(m_pButtonBox,SIGNAL(rejected()),this,SLOT(reject()));

	restoreGeometry(theConf->GetBlob("ErrorWindow/Window_Geometry"));
	

	foreach(const STATUS& Error, Errors)
	{
		QTreeWidgetItem* pItem = new QTreeWidgetItem();
		pItem->setText(eMessage, Error.GetText());
		pItem->setText(eErrorCode, tr("0x%1").arg((quint32)Error.GetStatus(), 8, 16, QChar('0')));
#ifdef WIN32
		pItem->setText(eErrorText, CastPhString(PhGetNtMessage(Error.GetStatus())));
#else
		// linux-todo:
#endif
		m_pErrors->GetTree()->addTopLevelItem(pItem);
	}

	for(int i = 0; i < m_pErrors->GetTree()->columnCount(); i++)
        m_pErrors->GetTree()->resizeColumnToContents(i);
}
 
CMultiErrorDialog::~CMultiErrorDialog()
{
	theConf->SetBlob("ErrorWindow/Window_Geometry", saveGeometry());
}
