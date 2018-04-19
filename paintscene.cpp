#include "paintscene.h"

paintScene::paintScene(QObject *parent) : QGraphicsScene(parent)
{
    SetEdge(false);
    SetPainting(false);
    previousPoint.setX(-100);
    distance = 25;
    weight = 0;
    entering = false;
}

paintScene::~paintScene()
{

}


void paintScene::sendRes(int *Steps)
{
    for(int i = 0; i < points.size(); i++)
    {
        QString str = QString::number(Steps[i]);
        QGraphicsItem *text = new QGraphicsTextItem(str);
        text->setX(points.at(i).first - 10);
        text->setY(points.at(i).second);
        stepsQ.push_back(text);
        addItem(text);
    }
}

QVector <QPair <int, int> > paintScene::getVertexes()
{
    return points;
}

QVector <int> paintScene::getWeights()
{
    return weights;
}

void paintScene::SetEdge(bool value)
{
    previousPoint.setX(-100);
    edge = value;
}

void paintScene::SetPainting(bool value)
{
    previousPoint.setX(-100);
    painting = value;
}

bool paintScene::approved(int x, int y, bool &put)
{
    for(QVector <QPair <int, int> >::iterator it = points.begin(); it != points.end(); it++)
    {
        if(put)                                                    // Если ищется точка конца линии
        {
            if(currentPoint == QPointF(it->first, it->second))     // При обнаружении первой точки
            {
                for(QVector <QPair <int, int> >::iterator jt = points.begin(); jt != points.end(); jt++)
                {                                                  // Происходит поиск второй
                    if((x + distance/2 > jt->first) && (x - distance/2 < jt->first) &&
                       (y + distance/2 > jt->second) && (y - distance/2 < jt->second))
                    {
                        if(it == jt)                               // Если попытались сделать граф рефлексивного отношения
                        {
                            return false;                          // Прекратить рисование
                        }
                        for(QVector <QPair <QPointF, QPointF> >::iterator kt = edges.begin(); kt != edges.end(); kt++)
                        {
                            if((kt->first == QPointF(it->first, it->second)) && (kt->second == QPointF(jt->first, jt->second)))
                            {                                      // Если такое ребро уже есть
                                return false;
                            }
                         //   qDebug() << " ";
                         //   qDebug() << "x - " << currentPoint.x() << " y -" << currentPoint.y() << " to " <<
                         //               "x - " << x << " y -" << y;
                        }
                        edges.push_back(QPair <QPointF, QPointF>(currentPoint, QPointF(jt->first, jt->second)));
                        edges.push_back(QPair <QPointF, QPointF>(QPointF(jt->first, jt->second), currentPoint));
                        return true;
                    }
                }
            }
        }
        else
        {
            if((x + distance/2 > it->first) && (x - distance/2 < it->first) && // Если координаты близки к точке по X
               (y + distance/2 > it->second) && (y - distance/2 < it->second)) // Если координаты близки к точке по Y
            {
                currentPoint = QPointF(it->first, it->second);
                return true;
            }
        }
    }
    // TODO - ADD EDGE
    return false;
}

bool paintScene::approved(QGraphicsSceneMouseEvent *event)
{
    int x = event->scenePos().x();
    int y = event->scenePos().y();
    for(QVector <QPair <int, int> >::iterator it = points.begin(); it != points.end(); it++)
    {
        if((x + distance > it->first) && (x - distance < it->first) &&
           (y + distance > it->second) && (y - distance < it->second))
        {
            return false;
        }
    }
    QPair <int, int> temp;
    temp.first = x;
    temp.second = y;
    points.push_back(temp);
    // TODO - ADD VERTEX
    return true;
}

void paintScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(entering)
    {
        return;
    }
    if(painting)
    {
        if(edge)
        {
            if(previousPoint.x() == -100)
            {
                bool put = false;
                if(approved(event->scenePos().x(), event->scenePos().y(), put))
                {
                    addEllipse(event->scenePos().x() - 3,
                    event->scenePos().y() - 3,
                    6,
                    6,
                    QPen(Qt::NoPen),
                    QBrush(Qt::red));
                    previousPoint = event->scenePos();
                }
            }
            else
            {
                bool put = true;
                if(approved(event->scenePos().x(), event->scenePos().y(), put))
                {
                    linesQ.push_back(*(items().begin()));
                    previousPoint.setX(-100);
                    entering = true;
                    QPointF temp1 = edges.back().first;
                    QPointF temp2 = edges.back().second;

                    addEllipse(temp1.x()/2 + temp2.x()/2,
                    temp1.y()/2 + temp2.y()/2, 20, 20,
                    QPen(Qt::FlatCap), QBrush(QColor(240, 190, 160)));

                }
                else
                {
                    removeItem(*(items().begin()));
                }
                currentPoint = QPointF(-100, -100);
                previousPoint.setX(-100);
            }
        }
        else
        {
            if(approved(event))
            {
                addEllipse(event->scenePos().x() - 10, event->scenePos().y(), 20, 20, QPen(Qt::FlatCap), QBrush(Qt::red));
            }
        }
    }
}

void paintScene::showLine(int from, int to, bool sure)
{
    if(sure)
    {
        tempLine =
        addLine(points.at(from).first, points.at(from).second,
        points.at(to).first, points.at(to).second,
        QPen(Qt::green,10,Qt::SolidLine,Qt::RoundCap));
    }
    else
    {
        tempLine =
        addLine(points.at(from).first, points.at(from).second,
        points.at(to).first, points.at(to).second,
        QPen(Qt::yellow,10,Qt::SolidLine,Qt::RoundCap));
    }
}

void paintScene::hideLine()
{
    removeItem(tempLine);
}

void paintScene::build(int num, QVector<QVector<QPair<int, int> > > con)
{

    QApplication::processEvents();
    int angle = 0;
    int angleDif = 180/num;
    int rad = 100;
    int x, y;
    int x0 = 150;
    int y0 = 100;
    for(int i = 0; i < num; i++)
    {
        x = x0 + rad * cos(angle);
        y = y0 + rad * sin(angle);
        angle += angleDif;
        QPair <int, int> temp;
        temp.first = x;
        temp.second = y;
        points.push_back(temp);
        addEllipse(x - 10,
        y,
        distance,
        distance,
        QPen(Qt::NoPen),
        QBrush(Qt::red));
        pointsQ.push_back(*items().begin());
    }
    int i = 0;
    for(QVector <QVector<QPair<int, int> > >::iterator it = con.begin(); it != con.end(); it++)
    {
        for(QVector<QPair<int, int> >::iterator jt = it->begin(); jt != it->end(); jt++)
        {
            addLine(points.at(i).first, points.at(i).second,
            points.at(jt->first).first, points.at(jt->first).second,
            QPen(Qt::red,10,Qt::SolidLine,Qt::RoundCap));
            edges.push_back(QPair <QPointF, QPointF> (QPointF(points.at(i).first, points.at(i).second),
                                                      QPointF(points.at(jt->first).first, points.at(jt->first).second)));
            linesQ.push_back(*items().begin());
        }
        i++;
    }
}

QVector <QPair <QPointF, QPointF> > paintScene::getEdges()
{
    return edges;
}

void paintScene::keyPressEvent(QKeyEvent *event)
{
    if(!entering)
    {
        return;
    }
    qDebug() << event->key();
    QPointF temp1 = edges.back().first;
    QPointF temp2 = edges.back().second;
    int res = 0;
    if(((event->key() == 0x01000005) || (event->key() == 0x01000004)) && (weight))
    {
        weights.push_back(weight);
        textsQ.push_back(*items().begin());
        entering = false;
        weight = 0;
        return;
    }
    else if(event->key() == 0x01000003)
    {
        if(weight)
        {
            removeItem(*items().begin());
        }
        weight /= 10;
        if(weight == 0)
        {

        }
        else
        {
            QString str = (weight > 99) ? ("99+") : (QString::number(weight));
            QGraphicsItem *text = new QGraphicsTextItem(str);
            text->setX(temp1.x()/2 + temp2.x()/2);
            text->setY(temp1.y()/2 + temp2.y()/2);
            addItem(text);
        }
        return;
    }
    else if(event->key() == 0x31)
    {
        res = 1;
    }
    else if(event->key() == 0x32)
    {
        res = 2;
    }
    else if(event->key() == 0x33)
    {
        res = 3;
    }
    else if(event->key() == 0x34)
    {
        res = 4;
    }
    else if(event->key() == 0x35)
    {
        res = 5;
    }
    else if(event->key() == 0x36)
    {
        res = 6;
    }
    else if(event->key() == 0x37)
    {
        res = 7;
    }
    else if(event->key() == 0x38)
    {
        res = 8;
    }
    else if(event->key() == 0x39)
    {
        res = 9;
    }
    else return;
    if(weight)
    {
        removeItem(*items().begin());
    }
    weight = weight * 10 + res;
    QString str = (weight > 99) ? ("99+") : (QString::number(weight));
    QGraphicsItem *text = new QGraphicsTextItem(str);
    text->setX(temp1.x()/2 + temp2.x()/2);
    text->setY(temp1.y()/2 + temp2.y()/2);
    addItem(text);
}

void paintScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{

}

void paintScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(entering)
    {
        return;
    }
    if(painting)
    {
        if(edge)
        {
            if(previousPoint.x() != -100)
            {
                removeItem(*(items().begin()));
                addLine(previousPoint.x(),
                previousPoint.y(),
                event->scenePos().x(),
                event->scenePos().y(),
                QPen(Qt::red,10,Qt::SolidLine,Qt::RoundCap));
            }
        }
    }
}
