#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->pushButton_selection_fichier_donnees, SIGNAL(clicked()), this, SLOT(creer_BDD()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::remplirBDDAvecFichierCSVForums()
{

    QFile file("forums.csv");

    file.open(QFile::ReadOnly | QFile::Text);            //Open the file with readonly mode and text mode

    QTextStream stream(&file);                       //the stream "stream" permits to access to the file "file"

    QSqlDatabase connexionBDD = QSqlDatabase::addDatabase("QSQLITE");                                                                   //Connection to the database
    connexionBDD.setDatabaseName("BDD");
    connexionBDD.open();

    if (connexionBDD.isOpen())
    {
        QSqlQuery query(connexionBDD);
        QList<QString> listeOfLine;
        while(!stream.atEnd())
        {
            listeOfLine.append(stream.readLine());
        }

        int identifiantBaseDeDonnees = 0;
        for(int i=1; i<listeOfLine.length();i++)
        {
            QStringList caseList = listeOfLine.at(i).split(";");
            if(caseList.at(6)!="")                                  //This test permits to not register a forum where no one came
            {
                query.prepare("INSERT INTO Forum(id, Titre, Etablissement, Date) VALUES(:id, :Titre, :Etablissement, :Date)");
                query.bindValue(":id", identifiantBaseDeDonnees);
                query.bindValue(":Titre", caseList.at(0));
                query.bindValue(":Etablissement", caseList.at(1));
                query.bindValue(":Date", caseList.at(2));
                query.exec();
                identifiantBaseDeDonnees++;
            }
        }

    }
    connexionBDD.close();

}

void MainWindow::remplirBDDAvecFichierCSVEleves()
{

    QFile file("eleves.csv");

    file.open(QFile::ReadOnly | QFile::Text);            //Open the file with readonly mode and text mode

    QTextStream stream(&file);                       //the stream "stream" permits to access to the file "file"

    QSqlDatabase connexionBDD = QSqlDatabase::addDatabase("QSQLITE");                                                                   //Connection to the database
    connexionBDD.setDatabaseName("BDD");
    connexionBDD.open();

    if (connexionBDD.isOpen())
    {
        QSqlQuery query(connexionBDD);
        QList<QString> listeOfLine;
        while(!stream.atEnd())
        {
            listeOfLine.append(stream.readLine());
        }
        int identifiantBaseDeDonnees = 0;
        for(int i=1; i<listeOfLine.length();i++)
        {
            QStringList caseList = listeOfLine.at(i).split(";");
            query.prepare("INSERT INTO Eleve(id, Annee, Etape, Nombre, DeptBac) VALUES(:id, :Annee, :Etape, :Nombre, :DeptBac)");
            query.bindValue(":id", identifiantBaseDeDonnees);
            query.bindValue(":Annee", caseList.at(0));
            query.bindValue(":Etape", caseList.at(1));
            query.bindValue(":Nombre", caseList.at(3));
            query.bindValue(":DeptBac", caseList.at(4));
            query.exec();
            identifiantBaseDeDonnees++;
        }

    }
    connexionBDD.close();
}

void MainWindow::creer_BDD()
{
    QSqlDatabase connexionBDD = QSqlDatabase::addDatabase("QSQLITE");
    connexionBDD.setDatabaseName("BDD");
    connexionBDD.open();
    if(connexionBDD.isOpen())
    {
        QSqlQuery query(connexionBDD);
        query.exec("CREATE TABLE Forum(id INTEGER, Titre VARCHAR (50), Etablissement VARCHAR(50), Date VARCHAR (50))");
        query.exec("CREATE TABLE Eleve(id INTEGER, Annee VARCHAR(50), Etape VARCHAR(50), Nombre INTEGER, DeptBac INTEGER)");
        connexionBDD.close();
    }
    MainWindow::remplirBDDAvecFichierCSVForums();
    MainWindow::remplirBDDAvecFichierCSVEleves();

}

