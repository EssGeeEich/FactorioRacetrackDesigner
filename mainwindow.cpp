#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QImageReader>
#include <QPixmap>
#include <QMessageBox>
#include "qdragbutton.h"
#include "qfactoriobasicitem.h"
#include "qfactorioconstants.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	m_openProp(nullptr),
	m_openPropObjID(0)
{
	ui->setupUi(this);
	m_scene.initialize(ui->treeWidget);
	ui->image->setScene(&m_scene);
	ui->image->show();

	// Add tne buttons
	int btnId = 0;
	for(auto it = g_mimeEquivalence.begin(); it != g_mimeEquivalence.end();
		++it, ++btnId)
	{
		QDragButton* button = new QDragButton(ui->entities);
		button->setObjectName(QString("entityButton_%1").arg(btnId));
		button->setText(tr(qPrintable(it->m_name)));
		button->setMimeFormat(g_factorioMimeFormat);
		button->setMimeData(it->m_mime);
		ui->entityLayout->insertWidget(btnId, button);
	}

	QList<QDragButton*> entities = this->findChildren<QDragButton*>();

	for(auto it = entities.begin(); it != entities.end(); ++it)
	{
		QDragButton* p = *it;
		bool bOk = false;
		int val = p->property("FactorioType").toInt(&bOk);
		if(bOk && val >= 0 && val < QFIT_COUNT)
		{
			auto iter = g_mimeEquivalence.begin();
			std::advance(iter, val);

			p->setMimeFormat(g_factorioMimeFormat);
			p->setMimeData(iter->m_mime);
		}
	}

	m_connections.append(
		connect(ui->treeWidget, &QTreeWidget::itemSelectionChanged,
			this, &MainWindow::OnSelectedEntityChange)
	);

	emptySelection();
}

void MainWindow::emptySelection()
{
	if(ui->properties)
	{
		delete ui->properties;
		ui->properties = nullptr;
	}
}

void MainWindow::OnSelectedEntityChange()
{
	QFactorioBasicItem* lastItem = m_scene.getItem(m_openPropObjID);
	if(lastItem)
		lastItem->beforeUnpopulate();
	emptySelection();
	if(lastItem)
	{
		lastItem->afterUnpopulate();
		lastItem->unSelect();
	}

	QTreeWidgetItem* item = ui->treeWidget->currentItem();
	if(!item)
		return;

	// Get object ID
	bool ok = false;
	m_openPropObjID = item->data(0, QF_USERROLE_ITEM).toUInt(&ok);
	if(!ok)
		return;

	// Find the item with that ID
	QFactorioBasicItem* facItem = m_scene.getItem(m_openPropObjID);
	if(facItem)
	{
		// Exists? Create a properties panel.
		ui->properties = new QFrame(ui->propertyDock);
		ui->properties->setObjectName(QString::fromUtf8("properties"));
		ui->propertyDockLayout->addWidget(ui->properties);

		facItem->populatePropertiesPanel(ui->properties);
		facItem->select();
	}
}

MainWindow::~MainWindow()
{
	for(auto it = m_connections.begin(); it != m_connections.end(); ++it)
	{
		QObject::disconnect(*it);
	}
	m_connections.clear();

	delete ui;
}

void MainWindow::on_actionLoad_Image_triggered()
{
	QString filePath = QFileDialog::getOpenFileName(this,
		tr("Open a track image file..."),
		QString(),
		tr("Images (*.png *.bmp)")
	);
	if(filePath.isEmpty())
		return;
	QImageReader imgReader(filePath);
	m_scene.setBackgroundPixmap(QPixmap::fromImageReader(&imgReader));
}

void MainWindow::on_actionLoad_Project_triggered()
{
	QString filePath = QFileDialog::getOpenFileName(this,
		tr("Open a Factorio Track Project file..."),
		QString(),
		tr("Factorio Track Projects (*.fkp)")
	);
	if(filePath.isEmpty())
		return;
	QFile f(filePath);
	if(!f.open(QFile::ReadOnly))
	{
		QMessageBox::warning(this, tr("Cannot load track project file"), tr("Can't read this file."));
		return;
	}

	m_scene.clear();
	QDataStream input(&f);
	if(m_scene.load(input).status() != QDataStream::Ok)
	{
		m_scene.clear();
		QMessageBox::warning(this, tr("Cannot load track project file"), tr("This file is not a valid track project file."));
		return;
	}
}

void MainWindow::on_actionSave_Project_triggered()
{
	QString filePath = QFileDialog::getSaveFileName(this,
		tr("Save a Factorio Track Project file..."),
		QString(),
		tr("Factorio Track Projects (*.fkp)")
	);
	if(filePath.isEmpty())
		return;
	QFile f(filePath);
	if(!f.open(QFile::WriteOnly))
	{
		QMessageBox::warning(this, tr("Cannot save track project file"), tr("Can't create this file."));
		return;
	}

	QDataStream output(&f);
	if(m_scene.save(output).status() != QDataStream::Ok)
	{
		QMessageBox::warning(this, tr("Cannot save track project file"), tr("An error occurred while trying to save this file."));
		return;
	}
}

void MainWindow::on_actionQuit_triggered()
{
	qApp->quit();
}

