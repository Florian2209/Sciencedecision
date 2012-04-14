#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QFileDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QtDebug>
#include <QDomDocument>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void creer_BDD(QString aFileName);

public slots:
    void remplirBDDAvecFichierCSV();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
