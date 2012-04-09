#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    creer_BDD();

    connect(ui->pushButton_selection_fichier_donnees, SIGNAL(clicked()), this, SLOT(remplirBDDAvecFichierCSV()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::remplirBDDAvecFichierCSV()
{
    QString fileName = QFileDialog::getOpenFileName();
    QFile file(fileName);
    file.open(QFile::ReadOnly | QFile::Text);            //Open the file with readonly mode and text mode

    QTextStream stream(&file);                       //the stream "stream" permits to access to the file "file"

    QSqlDatabase connexionBDD = QSqlDatabase::addDatabase("QSQLITE");                                                                   //Connection to the database
    connexionBDD.setDatabaseName("bdd2");
    connexionBDD.open();

    if (connexionBDD.isOpen())
    {
        QSqlQuery query(connexionBDD);
        QList<QString> listeOfLine;
        while(!stream.atEnd())
        {
            listeOfLine.append(stream.readLine());
        }
        int j;
        for(int i=0; i<listeOfLine.length();i++)
        {
            QStringList caseList = listeOfLine.at(i).split(";");
            if(caseList.at(3)!="")                                  //This test permits to not register a forum where no one came
            {
                query.prepare("INSERT INTO Forum(id, Titre, Etablissement, Date) VALUES(:id, :Titre, :Etablissement, :Date)");
                query.bindValue(":id", i);
                query.bindValue(":Titre", caseList.at(0));
                query.bindValue(":Etablissement", caseList.at(1));
                query.bindValue(":Date", caseList.at(2));
                query.exec();
            }
        }

    }
    connexionBDD.close();

}

void MainWindow::creer_BDD()
{
    QSqlDatabase connexionBDD = QSqlDatabase::addDatabase("QSQLITE");
    connexionBDD.setDatabaseName("bdd2");
    connexionBDD.open();
    if(connexionBDD.isOpen())
    {
        QSqlQuery query(connexionBDD);
        query.exec("CREATE TABLE Forum(id INTEGER, Titre VARCHAR (50), Etablissement VARCHAR(50), Date VARCHAR (50))");
        qDebug()<<"create";
        connexionBDD.close();
     }

}

