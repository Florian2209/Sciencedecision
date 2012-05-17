#ifndef REQUETEFORUM_H
#define REQUETEFORUM_H

#include <QWidget>

#include <QSqlDatabase>
#include <QSqlQuery>

#include <QtDebug>

namespace Ui {
    class RequeteForum;
}

class RequeteForum : public QWidget {
    Q_OBJECT
public:
    RequeteForum(QWidget *parent = 0);
    ~RequeteForum();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::RequeteForum *ui;

public slots:
    void lancerRequete();
    void affichageSpinBox(bool);
};

#endif // REQUETEFORUM_H
