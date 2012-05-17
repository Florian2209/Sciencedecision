#include "requeteeleve.h"
#include "ui_requeteeleve.h"

RequeteEleve::RequeteEleve(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RequeteEleve)
{
    ui->setupUi(this);
    ui->departementSpinBox->setVisible(false);
    ui->resultatLabel->setVisible(false);
    connect(ui->validerButton, SIGNAL(clicked()), this, SLOT(lancerRequete()));
    connect(ui->annulerButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->departementCheckBox, SIGNAL(clicked(bool)), this, SLOT(affichageSpinBox(bool)));
}

RequeteEleve::~RequeteEleve()
{
    delete ui;
}

void RequeteEleve::changeEvent(QEvent *e)
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

void RequeteEleve::lancerRequete()
{
    QSqlDatabase connexionBDD = QSqlDatabase::addDatabase("QSQLITE");
    connexionBDD.setDatabaseName("BDD");
    connexionBDD.open();
    if(connexionBDD.isOpen())
    {
        QSqlQuery query;
        double resultat = 0;
        QString annee = ui->anneeComboBox->currentText();
        QString promotion = ui->promoComboBox->currentText();
        int departement = -1;
        if(ui->departementSpinBox->isVisible())
        {
            departement = ui->departementSpinBox->value();
        }

        if(!(annee.contains("A"))) // On est toujours sur la valeur par défaut
        {
            if(!(promotion.contains("Promo"))) // Idem
            {
                if(departement != -1)
                {
                    query.prepare("SELECT nombre FROM eleve WHERE annee = :Annee AND etape = :Promo AND DeptBac = :Dept");
                    query.bindValue(":Annee", annee);
                    query.bindValue(":Promo", promotion);
                    query.bindValue(":Dept", departement);
                    if(query.exec())
                    {
                        while(query.next())
                        {
                            resultat += query.value(0).toDouble();
                        }
                    }
                }
                else
                {
                    query.prepare("SELECT nombre FROM eleve WHERE annee = :Annee AND etape = :Promo");
                    query.bindValue(":Annee", annee);
                    query.bindValue(":Promo", promotion);
                    if(query.exec())
                    {
                        while(query.next())
                        {
                            resultat += query.value(0).toDouble();
                        }
                    }
                }
            }
            else
            {
                if(departement != -1)
                {
                    query.prepare("SELECT nombre FROM eleve WHERE annee = :Annee AND DeptBac = :Dept");
                    query.bindValue(":Annee", annee);
                    query.bindValue(":Dept", departement);
                    if(query.exec())
                    {
                        while(query.next())
                        {
                            resultat += query.value(0).toDouble();
                        }
                    }
                }
                else
                {
                    query.prepare("SELECT nombre FROM eleve WHERE annee = :Annee");
                    query.bindValue(":Annee", annee);
                    if(query.exec())
                    {
                        while(query.next())
                        {
                            resultat += query.value(0).toDouble();
                        }
                    }
                }
            }
        }
        else
        {
            if(!(promotion.contains("Promo"))) // Idem
            {
                if(departement != -1)
                {
                    query.prepare("SELECT nombre FROM eleve WHERE etape = :Promo AND DeptBac = :Dept");
                    query.bindValue(":Promo", promotion);
                    query.bindValue(":Dept", departement);
                    if(query.exec())
                    {
                        while(query.next())
                        {
                            resultat += query.value(0).toDouble();
                        }
                    }
                }
                else
                {
                    query.prepare("SELECT nombre FROM eleve WHERE etape = :Promo");
                    query.bindValue(":Promo", promotion);
                    if(query.exec())
                    {
                        while(query.next())
                        {
                            resultat += query.value(0).toDouble();
                        }
                    }
                }
            }
            else
            {
                if(departement != 1)
                {
                    query.prepare("SELECT nombre FROM eleve WHERE DeptBac = :Dept");
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
        }
        ui->resultatLabel->setText("Resultat : " + QString::number(resultat) + " etudiants concernes");
        ui->resultatLabel->setVisible(true);
    }
}

void RequeteEleve::affichageSpinBox(bool visible){
    ui->departementSpinBox->setVisible(visible);
}

