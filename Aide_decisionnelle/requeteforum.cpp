#include "requeteforum.h"
#include "ui_requeteforum.h"

RequeteForum::RequeteForum(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RequeteForum)
{
    ui->setupUi(this);
    ui->departementSpinBox->setVisible(false);
    ui->resultatLabel->setVisible(false);
    connect(ui->validerButton, SIGNAL(clicked()), this, SLOT(lancerRequete()));
    connect(ui->annulerButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->departementCheckBox, SIGNAL(clicked(bool)), this, SLOT(affichageSpinBox(bool)));
}

RequeteForum::~RequeteForum()
{
    delete ui;
}

void RequeteForum::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void RequeteForum::lancerRequete()
{
    QSqlDatabase connexionBDD = QSqlDatabase::addDatabase("QSQLITE");
    connexionBDD.setDatabaseName("BDD");
    connexionBDD.open();
    if(connexionBDD.isOpen())
    {
        QSqlQuery query;
        double resultat = 0;
        QString annee = ui->anneeComboBox->currentText();
        int departement = -1;
        if(ui->departementSpinBox->isVisible())
        {
            departement = ui->departementSpinBox->value();
        }

        if(!(annee.contains("A"))) // On est toujours sur la valeur par défaut
        {
            if(departement != -1)
            {
                query.prepare("SELECT nombreParticipants, date FROM forum WHERE Departement = :Dept");
                query.bindValue(":Dept", departement);
                if(query.exec())
                {
                    while(query.next())
                    {
                        QString resultatAnnee = query.value(1).toString();
                        resultatAnnee.remove(QRegExp("[0-9][0-9]/[0-9][0-9]/"));
                        if(resultatAnnee.compare(annee) == 0)
                        {
                            resultat += query.value(0).toDouble();
                        }
                   }
                }
            }
            else
            {
                query.prepare("SELECT nombreParticipants, date FROM forum");
                if(query.exec())
                {
                    while(query.next())
                    {
                        QString resultatAnnee = query.value(1).toString();
                        resultatAnnee.remove(QRegExp("[0-9][0-9]/[0-9][0-9]/"));
                        if(resultatAnnee.compare(annee) == 0)
                        {
                            resultat += query.value(0).toDouble();
                        }
                   }
                }
            }
        }
        else
        {
            if(departement != -1)
            {
                query.prepare("SELECT nombreParticipants FROM forum WHERE Departement = :Dept");
                query.bindValue(":Dept", departement);
                if(query.exec())
                {
                    while(query.next())
                    {
                        resultat += query.value(0).toDouble();
                    }
                }
            }
        }

        ui->resultatLabel->setText("Resultat : " + QString::number(resultat) + " personnes ont participe a ce(s) forum(s)");
        ui->resultatLabel->setVisible(true);
    }
}


void RequeteForum::affichageSpinBox(bool visible){
    ui->departementSpinBox->setVisible(visible);
}
