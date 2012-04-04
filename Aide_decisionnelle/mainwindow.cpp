#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    creer_BDD();

    connect(ui->pushButton_selection_fichier_donnees, SIGNAL(clicked()), this, SLOT(remplirBDD()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::remplirBDD()
{
    QString nom_BDD = QFileDialog::getOpenFileName();
}

void MainWindow::creer_BDD()
{
    QSqlDatabase connexionBDD = QSqlDatabase::addDatabase("QSQLITE");
    connexionBDD.setDatabaseName("bdd");
    connexionBDD.open();
    if(connexionBDD.isOpen())
    {
        QSqlQuery query(connexionBDD);
        query.exec("CREATE TABLE Forum(id INTEGER, Titre VARCHAR (50), Etablissement VARCHAR(50), contact VARCHAR (50), date DATE");
     }
    connexionBDD.close();
}
