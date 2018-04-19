#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QVector>
#include <QTextBrowser>
#include <QPair>
#include <QFileDialog>
#include <paint.h>
#include <stdio.h>
#include <qfiledialog>
#include <windows.h> // for Sleep

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    void GetMatrix(QString Filename);

    bool compileText();

    void FindTheWay();

    void colorite(int sector);

    void setColor(int num);

    void matrixCreate(QVector <QPair <QPointF, QPointF> >, QVector <int>, QVector <QPair <int, int> >);

    explicit MainWindow(QWidget *parent = 0);


    ~MainWindow();

private slots:
    void on_action_1_triggered();

    void on_showDeWay_clicked();

    void on_exitButton_clicked();

    void on_makeTable_clicked();

    void on_action_2_triggered();

    void on_getButton_clicked();

    void on_makeButton_clicked();

    void on_countSpin_valueChanged(int arg1);

    void on_MatrixCreate_clicked();

    void on_vertex_clicked();

    void on_edge_clicked();

    void on_change_clicked();

    void on_change_2_clicked();

    void on_delayEdit_editingFinished();

private:
    bool built;
    QVector <int> col;
    int ways;
    int DelayTime;
    QVector <QVector <QPair <int, int> > > con;
    int start;
    int target;
    QString res;
    int *Steps;
    int rowNumb;
    QTextBrowser *MatrixView;
    Ui::MainWindow *ui;
    Paint *scene;
};

#endif // MAINWINDOW_H
