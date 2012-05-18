#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QFileDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDomDocument>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

#include "requeteeleve.h"
#include "requeteforum.h"

#include <QtDebug>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void remplirBDDAvecFichierCSVForums();
    void remplirBDDAvecFichierCSVEleves();

public slots:
    void creer_BDD();
    void lancerRequeteEleve();
    void lancerRequeteForum();
    void lancerCreationFichierGlobal();
    void lancerCreationFichierAnneeSpecifique();
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
