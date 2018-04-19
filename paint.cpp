#include "paint.h"
#include "ui_paint.h"

Paint::Paint(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Paint)
{
    ui->setupUi(this);
    scene = new paintScene();
    ui->graphicsView->viewport()->setMouseTracking(true);
    ui->graphicsView->setScene(scene);
    timer = new QTimer();
    scene->setSceneRect(0,0, ui->graphicsView->width() - 20, ui->graphicsView->height() - 20);
}

void Paint::setEdge(bool value)
{
    scene->SetEdge(value);
}

void Paint::setPainting(bool value)
{
    scene->SetPainting(value);
}

Paint::~Paint()
{
    delete ui;
}

