#ifndef PAINTSCENE_H
#define PAINTSCENE_H

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QPainter>
#include <cmath>
#include <QDebug>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QTimer>
#include <QWidget>
#include <QVector>
#include <QPair>
#include <QApplication>

class paintScene : public QGraphicsScene
{
    Q_OBJECT
public:
    void showLine(int, int, bool);
    void hideLine();
    void SetEdge(bool = true);
    void SetPainting(bool = true);
    explicit paintScene(QObject *parent = 0);
    ~paintScene();
    QVector <QPair <QPointF, QPointF> > getEdges();
    QVector <int> getWeights();
    QVector <QPair <int, int> > getVertexes();
    void sendRes(int *);
    void build(int, QVector <QVector <QPair <int, int> > >);
    bool edge;
    bool entering;
    bool painting;
private:
    QGraphicsItem *tempLine;
    int distance;
    int weight;
    //VertexData
    QPointF currentPoint; //Point of first vertex of the line
    QPointF previousPoint;
    QVector <QPair <int, int> > points;//Vector of vertexes
    //EdgesData
    QVector <int> weights;//Vector of weights
    QVector <QPair <QPointF, QPointF> > edges;//Vector of edges
    //Items
    QVector <QGraphicsItem *> pointsQ;
    QVector <QGraphicsItem *> linesQ;
    QVector <QGraphicsItem *> stepsQ;
    QVector <QGraphicsItem *> textsQ;
    QVector <QGraphicsItem *> textbgQ;
    //Methods
    bool approved(QGraphicsSceneMouseEvent *event);
    bool approved(int x, int y, bool &put);
    //Slots
    void mousePressEvent(QGraphicsSceneMouseEvent * event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
};


#endif // PAINTSCENE_H
