#ifndef REQUETEELEVE_H
#define REQUETEELEVE_H

#include <QWidget>

#include <QSqlDatabase>
#include <QSqlQuery>

#include <QtDebug>

namespace Ui {
    class RequeteEleve;
}

class RequeteEleve : public QWidget {
    Q_OBJECT
public:
    RequeteEleve(QWidget *parent = 0);
    ~RequeteEleve();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::RequeteEleve *ui;

public slots:
    void lancerRequete();
    void affichageSpinBox(bool);
};

#endif // REQUETEELEVE_H
