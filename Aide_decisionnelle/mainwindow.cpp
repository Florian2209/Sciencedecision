#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QObject::connect(ui->tableEleve, SIGNAL(triggered(bool)), this, SLOT(lancerRequeteEleve()));
    QObject::connect(ui->tableForum, SIGNAL(triggered(bool)), this, SLOT(lancerRequeteForum()));
    QObject::connect(ui->fichierCSV, SIGNAL(triggered(bool)), this, SLOT(lancerCreationFichierGlobal()));
    QObject::connect(ui->fichierCSV, SIGNAL(triggered(bool)), this, SLOT(lancerCreationFichierAnneeSpecifique()));
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
            if(caseList.at(4)!="")                                  //This test permits to not register a forum where no one came
            {
                query.prepare("INSERT INTO Forum(id, Titre, Etablissement, Departement, Date, NombreParticipants) VALUES(:id, :Titre, :Etablissement, :Departement, :Date, :NombreParticipants)");
                QString Titre = caseList.at(0);
                Titre.remove("\"", Qt::CaseInsensitive);
                QString Etablissement = caseList.at(1);
                Etablissement.remove("\"", Qt::CaseInsensitive);
                QString Date = caseList.at(3);
                Date.remove("\"", Qt::CaseInsensitive);
                query.bindValue(":id", identifiantBaseDeDonnees);
                query.bindValue(":Titre", Titre);
                query.bindValue(":Etablissement", Etablissement);
                query.bindValue(":Departement", caseList.at(2));
                query.bindValue(":Date", Date);
                QString champParticipants = caseList.at(4);
                int nbParticipants = champParticipants.count("#");
                if(nbParticipants > 1) // Traitements à effectuer sur le nombre d'occurences de # dûes aux conventions d'écritures dans le fichier .csv
                {
                    nbParticipants--;
                    nbParticipants = (nbParticipants/2)+1;
                }
                query.bindValue(":NombreParticipants", nbParticipants);
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
            if(caseList.at(0) != "\"ANNEE\"")
            {
                query.prepare("INSERT INTO Eleve(id, Annee, Etape, Nombre, DeptBac) VALUES(:id, :Annee, :Etape, :Nombre, :DeptBac)");
                QString Etape = caseList.at(1);
                Etape.remove("\"", Qt::CaseInsensitive);
                QString Nombre = caseList.at(3);
                Nombre.remove("\"", Qt::CaseInsensitive);
                QString DeptBac = caseList.at(4);
                DeptBac.remove("\"", Qt::CaseInsensitive);
                query.bindValue(":id", identifiantBaseDeDonnees);
                query.bindValue(":Annee", caseList.at(0));
                query.bindValue(":Etape", Etape);
                query.bindValue(":Nombre", Nombre);
                query.bindValue(":DeptBac", DeptBac);
                query.exec();
                identifiantBaseDeDonnees++;
            }
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
        query.exec("CREATE TABLE Forum(id INTEGER, Titre VARCHAR (50), Etablissement VARCHAR(50), Departement INTEGER, Date VARCHAR (50), NombreParticipants INTEGER)");
        query.exec("CREATE TABLE Eleve(id INTEGER, Annee VARCHAR(50), Etape VARCHAR(50), Nombre INTEGER, DeptBac INTEGER)");
        connexionBDD.close();
    }
    MainWindow::remplirBDDAvecFichierCSVForums();
    MainWindow::remplirBDDAvecFichierCSVEleves();

}

void MainWindow::lancerRequeteEleve()
{
    RequeteEleve * fenetre = new RequeteEleve();
    fenetre->show();
}

void MainWindow::lancerRequeteForum()
{
    RequeteForum * fenetre = new RequeteForum();
    fenetre->show();
}

void MainWindow::lancerCreationFichierGlobal()
{
    QFile file("tableauDonneesGlobal.csv");

    if (file.open(QFile::WriteOnly)) {

        QTextStream flux(&file);
        QSqlDatabase connexionBDD = QSqlDatabase::addDatabase("QSQLITE");                                                                   //Connection to the database
        connexionBDD.setDatabaseName("BDD");
        connexionBDD.open();

        if (connexionBDD.isOpen())
        {
            QSqlQuery query;

            int totalColonneAMG = 0;
            int totalColonneELE = 0;
            int totalColonneINF = 0;
            int totalColonneMEC = 0;

            for(int i = 1; i<=95; i++)
            {
                int nombre = 0;
                int totalLigne = 0;
                query.prepare("SELECT nombre FROM eleve WHERE DeptBac = :Dept AND etape = :Promo");
                query.bindValue(":Dept", i);
                query.bindValue(":Promo", "EI3AMG");
                if(query.exec())
                {
                    while(query.next())
                    {
                        nombre += query.value(0).toInt();
                    }
                    totalColonneAMG += nombre;
                    totalLigne += nombre;
                    flux << nombre << ";";
                }
                nombre = 0;
                query.prepare("SELECT nombre FROM eleve WHERE DeptBac = :Dept AND etape = :Promo");
                query.bindValue(":Dept", i);
                query.bindValue(":Promo", "EI3ELE");
                if(query.exec())
                {
                    while(query.next())
                    {
                         nombre += query.value(0).toInt();
                    }
                    totalColonneELE += nombre;
                    totalLigne += nombre;
                    flux << nombre << ";";
                }
                nombre = 0;
                query.prepare("SELECT nombre FROM eleve WHERE DeptBac = :Dept AND (etape = :Promo OR etape = :Promo2)");
                query.bindValue(":Dept", i);
                query.bindValue(":Promo", "EI3INF");
                query.bindValue(":Promo2", "EI3INI");
                if(query.exec())
                {
                    while(query.next())
                    {
                        nombre += query.value(0).toInt();
                    }
                    totalColonneINF += nombre;
                    totalLigne += nombre;
                    flux << nombre << ";";
                }
                nombre = 0;
                query.prepare("SELECT nombre FROM eleve WHERE DeptBac = :Dept AND etape = :Promo");
                query.bindValue(":Dept", i);
                query.bindValue(":Promo", "EI3MEC");
                if(query.exec())
                {
                    while(query.next())
                    {
                        nombre += query.value(0).toInt();
                    }
                    totalColonneMEC += nombre;
                    totalLigne += nombre;
                    flux << nombre << ";";
                }
                flux << totalLigne << ";" << endl;
            }

            flux << totalColonneAMG << ";" << totalColonneELE << ";" << totalColonneINF << ";" << totalColonneMEC << ";" <<
                    totalColonneAMG + totalColonneELE + totalColonneINF + totalColonneMEC << ";" << endl;

            connexionBDD.close();
        }
    }
    else
    {
        cout << "ERREUR : Impossible d'ouvrir le fichier" << endl;
    }
}

void MainWindow::lancerCreationFichierAnneeSpecifique()
{

    for(int annee = 2008; annee <= 2011; annee++)
    {
        QString an = QString::number(annee);
        QString nomFichier = "tableauDonnees";
        nomFichier = nomFichier.append(an).append(".csv");
        QFile file(nomFichier);

        if (file.open(QFile::WriteOnly)) {

            QTextStream flux(&file);
            QSqlDatabase connexionBDD = QSqlDatabase::addDatabase("QSQLITE");                                                                   //Connection to the database
            connexionBDD.setDatabaseName("BDD");
            connexionBDD.open();

            if (connexionBDD.isOpen())
            {
                QSqlQuery query;

                int totalColonneAMG = 0;
                int totalColonneELE = 0;
                int totalColonneINF = 0;
                int totalColonneMEC = 0;

                for(int i = 1; i<=95; i++)
                {
                    int nombre = 0;
                    int totalLigne = 0;
                    query.prepare("SELECT nombre FROM eleve WHERE DeptBac = :Dept AND etape = :Promo AND annee = :Annee");
                    query.bindValue(":Dept", i);
                    query.bindValue(":Promo", "EI3AMG");
                    query.bindValue(":Annee", annee);
                    if(query.exec())
                    {
                        while(query.next())
                        {
                            nombre += query.value(0).toInt();
                        }
                        totalColonneAMG += nombre;
                        totalLigne += nombre;
                        flux << nombre << ";";
                    }
                    nombre = 0;
                    query.prepare("SELECT nombre FROM eleve WHERE DeptBac = :Dept AND etape = :Promo AND annee = :Annee");
                    query.bindValue(":Dept", i);
                    query.bindValue(":Promo", "EI3ELE");
                    query.bindValue(":Annee", annee);
                    if(query.exec())
                    {
                        while(query.next())
                        {
                             nombre += query.value(0).toInt();
                        }
                        totalColonneELE += nombre;
                        totalLigne += nombre;
                        flux << nombre << ";";
                    }
                    nombre = 0;
                    query.prepare("SELECT nombre FROM eleve WHERE DeptBac = :Dept AND (etape = :Promo OR etape = :Promo2) AND annee = :Annee");
                    query.bindValue(":Dept", i);
                    query.bindValue(":Promo", "EI3INF");
                    query.bindValue(":Promo2", "EI3INI");
                    query.bindValue(":Annee", annee);
                    if(query.exec())
                    {
                        while(query.next())
                        {
                            nombre += query.value(0).toInt();
                        }
                        totalColonneINF += nombre;
                        totalLigne += nombre;
                        flux << nombre << ";";
                    }
                    nombre = 0;
                    query.prepare("SELECT nombre FROM eleve WHERE DeptBac = :Dept AND etape = :Promo AND annee = :Annee");
                    query.bindValue(":Dept", i);
                    query.bindValue(":Promo", "EI3MEC");
                    query.bindValue(":Annee", annee);
                    if(query.exec())
                    {
                        while(query.next())
                        {
                            nombre += query.value(0).toInt();
                        }
                        totalColonneMEC += nombre;
                        totalLigne += nombre;
                        flux << nombre << ";";
                    }
                    flux << totalLigne << ";" << endl;
                }

                flux << totalColonneAMG << ";" << totalColonneELE << ";" << totalColonneINF << ";" << totalColonneMEC << ";" <<
                        totalColonneAMG + totalColonneELE + totalColonneINF + totalColonneMEC << ";" << endl;

                connexionBDD.close();
            }
        }
        else
        {
            cout << "ERREUR : Impossible d'ouvrir le fichier" << endl;
        }
    }
}
