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
    void remplirBDDAvecFichierCSVForums();
    void remplirBDDAvecFichierCSVEleves();

public slots:
    void creer_BDD();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
