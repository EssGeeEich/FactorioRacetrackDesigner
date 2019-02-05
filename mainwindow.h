#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qfactoriotrackeditor.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = nullptr);
	~MainWindow();

private:
	void emptySelection();

private slots:
	void on_actionLoad_Image_triggered();
	void on_actionLoad_Project_triggered();
	void on_actionSave_Project_triggered();
	void on_actionQuit_triggered();

	void OnSelectedEntityChange();

	void on_actionExport_EntityMap_triggered();

private:

	Ui::MainWindow *ui;
	QList<QMetaObject::Connection> m_connections;
	QFactorioTrackEditor m_scene;
	QObject* m_openProp;
	quint32 m_openPropObjID;
};

#endif // MAINWINDOW_H
