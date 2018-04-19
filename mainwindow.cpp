#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    Steps = NULL;
    MatrixView = NULL;
    built = false;
    rowNumb = start = target = ways = 0;
    res = "";
    con.clear();
    DelayTime = 600;
    ui->setupUi(this);
    ui->weightEdit->setHidden(true);
    ui->fromEdit->setHidden(true);
    ui->toEdit->setHidden(true);
    ui->edge->setEnabled(false);
    ui->vertex->setEnabled(false);
    ui->startLabel->setHidden(true);
    ui->vertexLabel->setHidden(true);
    ui->targetLabel->setHidden(true);
    ui->countSpin->setHidden(true);
    ui->startSpin->setHidden(true);
    ui->targetSpin->setHidden(true);
    ui->TableCreate->setHidden(true);
    ui->MatrixCreate->setHidden(true);

    scene = new Paint(ui->Graphics);
    scene->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_makeButton_clicked()
{
    built = false;
    if(MatrixView)
    {
        delete MatrixView;
        MatrixView = NULL;
    }
    ui->startLabel->setHidden(false);
    ui->vertexLabel->setHidden(false);
    ui->targetLabel->setHidden(false);
    ui->countSpin->setHidden(false);
    ui->startSpin->setHidden(false);
    ui->targetSpin->setHidden(false);
    ui->TableCreate->setHidden(false);
    ui->MatrixCreate->setHidden(false);

}

void MainWindow::GetMatrix(QString Filename)
{
    Steps = NULL;
    rowNumb = 0;
    con.clear();
    res = "";
    start = target = 0;

    ui->startLabel->setHidden(true);
    ui->vertexLabel->setHidden(true);
    ui->targetLabel->setHidden(true);
    ui->countSpin->setHidden(true);
    ui->startSpin->setHidden(true);
    ui->targetSpin->setHidden(true);
    ui->TableCreate->setHidden(true);
    ui->MatrixCreate->setHidden(true);

    if(MatrixView)
    {
        delete MatrixView;
        MatrixView = NULL;
    }
    res = "";
    QFile file(Filename);
    MatrixView = new QTextBrowser(this);
    MatrixView->setGeometry(10, 125, 271, 221);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        res = "Произошла ошибка при открытии файла";
    }
    else
    {
        res = file.readAll();
    }
    MatrixView->setText(res);
    MatrixView->show();
}


void MainWindow::on_action_1_triggered()
{
    GetMatrix("example1.txt");
}

void MainWindow::on_action_2_triggered()
{
    GetMatrix("example2.txt");
}

bool updateNum(int &num, char symb)
{
    if(symb == '0')      num = num * 10;
    else if(symb == '1') num = num * 10 + 1;
    else if(symb == '2') num = num * 10 + 2;
    else if(symb == '3') num = num * 10 + 3;
    else if(symb == '4') num = num * 10 + 4;
    else if(symb == '5') num = num * 10 + 5;
    else if(symb == '6') num = num * 10 + 6;
    else if(symb == '7') num = num * 10 + 7;
    else if(symb == '8') num = num * 10 + 8;
    else if(symb == '9') num = num * 10 + 9;
    else return false;
    return true;
}

bool MainWindow::compileText()
{
    bool spacing = true, created = false, addRow = false;
    bool startset = false, intro = false;
    int number = 0;
    con.clear();
    int it1 = 0, it2 = 0;
    QVector <QPair <int, int> > ToAdd;
    int columnNumb = 0;
    for(int i = 0; i < res.length(); i++)
    {
        if(!intro)
        {
            if(res[i] == '\n')
            {
                if(startset)
                {
                    target = number;
                    number = 0;
                    intro = true;
                }
                else
                {
                    return 0;
                }
            }
            else if(res[i] == ' ')
            {
                if(startset)
                {
                    continue;
                }
                else
                {
                    start = number;
                    number = 0;
                    startset = true;
                }
            }
            else
            {
                if(!updateNum(number, res[i].toLatin1()))
                {
                    return 0;
                }
            }
        }
        else
        {
            if(res[i] == ' ' || res[i] == '\n' || res[i] == '\0')
            {
                if(spacing) continue;
                else if(number)
                {
                    created = true;
                    addRow = true;
                    QPair <int, int> temp(it2, number);
                    ToAdd.push_back(temp);
                    number = 0;
                    spacing = true;
                }
                it2++;
                if(res[i] == '\n' || res[i] == '\0')
                {
                    created = false;
                    addRow = false;
                    it1++;
                    con.push_back(ToAdd);
                    ToAdd.clear();
//                    if(it2 != columnNumb && it1 != 1)
//                    {
//                        return 0;
//                    }
                    columnNumb = it2;
                    it2 = 0;
                    spacing = true;
                }
            }
            else
            {
                if(!updateNum(number, res[i].toLatin1()))
                {
                    return 0;
                }
                spacing = false;
            }
        }
    }
    if(created)
    {
        con.push_back(ToAdd);
    }
    rowNumb = it1;
    if(addRow)
    {
        rowNumb++;
    }
    if(rowNumb != columnNumb)
    {
        //return 0;
    }
    if(start > rowNumb || target > rowNumb)
    {
        return 0;
    }
    return 1;
}



void MainWindow::FindTheWay()
{

    if(Steps)
    {
        delete Steps;
    }
    Steps = new int [rowNumb];
    bool *visited = new bool [rowNumb];
    QVector <int> Parent;
    QVector <QPair <int, int> > Go;
    for(int i = 0; i < rowNumb; i++)
    {
        if(i == start)
        {
            Steps[i] = 0;
            visited[i] = true;
        }
        else
        {
            visited[i] = false;
            Steps[i] = -1;
        }
    }
    if(con.size())
    {
        for(int i = 0; i < con.at(start).size(); i++)
        {
            Parent.push_front(start);
            Go.push_front((con.at(start)).at(i));
        }
    }
    while(!Go.empty())
    {
        QPair <int, int> cur = Go.back();

        QApplication::processEvents();
        ui->actionLabel->setText("<<Проверка ребра>>");
        ui->fromEdit->setHidden(false);
        ui->toEdit->setHidden(false);
        ui->fromEdit->setText(QString::number(Parent.back()));
        ui->toEdit->setText(QString::number(cur.first));


        ui->vertex->setEnabled(false);
        ui->edge->setEnabled(false);
        if(scene)
        {
            delete scene;
        }
        scene = new Paint(ui->Graphics);
        scene->show();

        scene->scene->build(rowNumb, con);
        scene->scene->showLine(Parent.back(), cur.first, 0);
        scene->scene->sendRes(Steps);


        this->repaint();
        Sleep(uint(DelayTime));

        scene->scene->hideLine();
        ui->fromEdit->setHidden(true);
        ui->toEdit->setHidden(true);

        if((Steps[cur.first] == -1) || (Steps[cur.first] > Steps[Parent.back()] + cur.second))
        {
            for(int i = 0; i < con.at(cur.first).size(); i++)
            {
                Parent.push_front(cur.first);
                Go.push_front((con.at(cur.first)).at(i));
            }
            Steps[cur.first] = Steps[Parent.back()] + cur.second;
            ui->actionLabel->setText("<<Обновление ребра>>");
            ui->weightEdit->setText(QString::number(Steps[cur.first]));
            ui->weightEdit->setHidden(false);

            ui->vertex->setEnabled(false);
            ui->edge->setEnabled(false);
            if(scene)
            {
                delete scene;
            }
            scene = new Paint(ui->Graphics);
            scene->show();

            scene->scene->build(rowNumb, con);
            scene->scene->showLine(Parent.back(), cur.first, 1);
            scene->scene->sendRes(Steps);



            this->repaint();
            Sleep(uint(DelayTime));

            scene->scene->hideLine();
            ui->weightEdit->setHidden(true);
        }
        visited[cur.first] = true;
        Parent.pop_back();
        Go.pop_back();
    }
    if(Steps[target] > 0)
    {
        //ui->actionLabel->setText("<<Действие>>");
        ui->resText->setText(QString::number(Steps[target]));
    }
    else
    {
        ui->actionLabel->setText("<<Действие>>");
        ui->error->setText("Путь до\nуказанной вершины\nотсутствует");
    }

}

void MainWindow::on_showDeWay_clicked()
{

    if(!compileText())
    {
        ui->error->setText("Неверный\nформат файла");
        return;
    }
    ui->actionLabel->setText("<<Действие>>");
    ui->error->setText("");
    ui->resText->setText("");
    FindTheWay();

    if(!built)
    {
        ui->vertex->setEnabled(false);
        ui->edge->setEnabled(false);
        if(scene)
        {
            delete scene;
        }
        scene = new Paint(ui->Graphics);
        scene->show();
        scene->scene->build(rowNumb, con);
    }
    on_makeTable_clicked();
    scene->scene->sendRes(Steps);
}

void MainWindow::on_exitButton_clicked()
{
    close();
}

void MainWindow::setColor(int num)
{
    int i = target;
    for(QVector <int>::iterator it = col.begin(); it != col.end(); it++)
    {
        int sector = *it;

        scene->scene->showLine(i, *it, 1);
        i = *it;
//        int row = sector / ui->graph->rowCount();
//        int column = sector - (row * ui->graph->columnCount());
//        QTableWidgetItem *temp = ui->graph->takeItem(row, column);
//        temp->setBackgroundColor(QColor(50, 150, 50));
    }
}

void MainWindow::colorite(int sector)
{
    for(QVector <QPair <int, int> >::const_iterator it = (con.at(sector)).begin(); it != (con.at(sector)).end(); it++)
    {
        int a = Steps[it->first];
        int b = it->second;
        int c = Steps[sector];
        if(a + b == c)
        {
            sector = it->first;
            col.push_back(sector);
            colorite(sector);

            return;
        }
    }
}

void MainWindow::on_makeTable_clicked()
{
    col.clear();
    if(Steps[target] != -1)
    {
        col.push_back(target);
        colorite(target);
        setColor(0);
    }
    else
    {
//        int row = target / ui->graph->rowCount();
//        int column = target - (row * ui->graph->columnCount());
//        QTableWidgetItem *temp = ui->graph->takeItem(row, column);
//        temp->setBackgroundColor(QColor(50, 150, 50));
//        ui->graph->setItem(row, column, temp);
    }
}

void MainWindow::on_getButton_clicked()
{
    QString file = QFileDialog::getOpenFileName();
    if(file != "")
    {
        GetMatrix(file);
    }
}


void MainWindow::on_countSpin_valueChanged(int arg1)
{
    int res = ui->countSpin->value();
    ui->startSpin->setMaximum(res - 1);
    ui->targetSpin->setMaximum(res - 1);

    ui->TableCreate->setColumnCount(res);
    ui->TableCreate->setRowCount(res);
    for(int i = 0; i < res; i++)
    {
        for(int j = 0; j < res; j++)
        {
            ui->TableCreate->setItem(i, j, new QTableWidgetItem(QString::number(0)));
        }
    }
    for(int i = 0; i < res; i++)
    {
        ui->TableCreate->setColumnWidth(i, (ui->TableCreate->width() - ui->TableCreate->verticalHeader()->width()) / ((res > 7 ? 7 : res) + 0.5));
        ui->TableCreate->setRowHeight(i, (ui->TableCreate->height() - ui->TableCreate->horizontalHeader()->height()) / ((res > 7 ? 7 : res) + 0.5));
    }
}

void MainWindow::on_MatrixCreate_clicked()
{
    res = "";
    res += QString::number(ui->startSpin->value()) + " ";
    res += QString::number(ui->targetSpin->value());
    res += "\n";
    rowNumb = ui->countSpin->value();
    for(int i = 0; i < ui->countSpin->value(); i++)
    {
//        QVector <QPair <int, int> > line;
        for(int j = 0; j < ui->countSpin->value(); j++)
        {
            QString takenData = ui->TableCreate->takeItem(i, j)->text();
            if((i == j) && (takenData.toInt() > 0))
            {
                res = "";
                ui->error->setText("Ошибка обработки");
                return;
            }
            res += takenData;

            if(j != ui->countSpin->value() - 1)
            {
                res += " ";
            }
            else
            {
                res += "\n";
            }

//            if(takenData.toInt() > 0)
//            {
//                line.push_back(QPair <int, int> (j, takenData.toInt()));
//            }
        }
//        con.push_back(line);
    }



    ui->startLabel->setHidden(true);
    ui->vertexLabel->setHidden(true);
    ui->targetLabel->setHidden(true);
    ui->countSpin->setHidden(true);
    ui->startSpin->setHidden(true);
    ui->targetSpin->setHidden(true);
    ui->TableCreate->setHidden(true);
    ui->MatrixCreate->setHidden(true);

    MatrixView = new QTextBrowser(this);
    MatrixView->setGeometry(10, 125, 271, 221);
    MatrixView->setText(res);
    MatrixView->show();
}

void MainWindow::on_vertex_clicked()
{
    scene->setEdge(false);
}

void MainWindow::on_edge_clicked()
{
    scene->setEdge();
}

void MainWindow::matrixCreate(QVector <QPair <QPointF, QPointF> > vec, QVector <int> wei, QVector <QPair <int, int> > points)
{
    if((vec.empty()) || (vec.size() % 2))
    {
        return;
    }
    else
    {
        con.clear();
        res = "";
        QVector <QPair < int, int> > vec2;
        for(QVector <QPair <QPointF, QPointF> >::iterator it = vec.begin(); it != vec.end(); it++)
        {//set all the points into numbers
            QPair <int , int> temp;
            int i = 0;
            for(QVector <QPair <int, int> >::iterator jt = points.begin(); jt != points.end(); jt++)
            {
                if((jt->first == it->first.x()) && (jt->second == it->first.y()))
                {
                    temp.first = i;
                }
                else if((jt->first == it->second.x()) && (jt->second == it->second.y()))
                {
                    temp.second = i;
                }
                i++;
            }
            vec2.push_back(temp);
        }

        //Construct an empty table
        if(MatrixView)
        {
            delete MatrixView;
            MatrixView = NULL;
        }
        ui->startLabel->setHidden(false);
        ui->vertexLabel->setHidden(false);
        ui->targetLabel->setHidden(false);
        ui->countSpin->setHidden(false);
        ui->startSpin->setHidden(false);
        ui->targetSpin->setHidden(false);
        ui->TableCreate->setHidden(false);
        ui->MatrixCreate->setHidden(false);

        int num = points.size();
        ui->startSpin->setMaximum(num - 1);
        ui->targetSpin->setMaximum(num - 1);

        ui->TableCreate->setColumnCount(num);
        ui->TableCreate->setRowCount(num);


        for(int i = 0; i < num; i++)
        {
            ui->TableCreate->setColumnWidth(i, (ui->TableCreate->width() - ui->TableCreate->verticalHeader()->width()) / ((num > 7 ? 7 : num) + 0.5));
            ui->TableCreate->setRowHeight(i, (ui->TableCreate->height() - ui->TableCreate->horizontalHeader()->height()) / ((num > 7 ? 7 : num) + 0.5));
        }
        for(int i = 0; i < num; i++)
        {
            for(int j = 0; j < num; j++)
            {
                ui->TableCreate->setItem(i, j, new QTableWidgetItem(QString::number(0)));
            }
        }
        //End of construction

        for(int i = 0; i < vec2.size(); i++)
        {
            con.push_back(QVector <QPair <int, int > > ());
        }

        ui->countSpin->setValue(num);
        int i = 0;
        for(QVector <QPair <int, int > >::iterator it = vec2.begin(); it != vec2.end(); it++)
        {
            int j = 0;
            QVector <QPair <int, int> > *found;
            for(QVector <QVector <QPair <int, int> > > ::iterator con_It = con.begin();
                con_It != con.end(); con_It++)
            {
                if(j == it->first)
                {
                    found = con_It;
                    break;
                }
                j++;
            }
            ui->TableCreate->setItem(j, it->second, new
                                     QTableWidgetItem(QString::number(wei.at(i / 2))));
            found->push_back(QPair <int, int> (it->second, wei.at(i / 2)));
            i++;
        }
    }
}

void MainWindow::on_change_clicked()
{
    if(ui->edge->isEnabled())
    {
        ui->edge->setEnabled(false);
        ui->vertex->setEnabled(false);
        scene->setPainting(false);
        matrixCreate(scene->scene->getEdges(), scene->scene->getWeights(), scene->scene->getVertexes());
        built = true;
    }
    else
    {
        scene->setPainting(true);
        ui->edge->setEnabled(true);
        ui->vertex->setEnabled(true);
    }
}

void MainWindow::on_change_2_clicked()
{
    built = false;
    ui->vertex->setEnabled(false);
    ui->edge->setEnabled(false);
    delete scene;
    scene = new Paint(ui->Graphics);
    scene->show();
}

void MainWindow::on_delayEdit_editingFinished()
{
    DelayTime = ui->delayEdit->text().toInt();
}
