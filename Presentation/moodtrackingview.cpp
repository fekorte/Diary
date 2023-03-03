#include "moodtrackingview.h"
#include "ui_moodtrackingview.h"
#include "Business/IBusiness.h"
#include "Common/Diary.h"
#include "Common/Entry.h"
#include "QGraphicsView"
#include <QGraphicsLineItem>
#include <QGraphicsTextItem>
#include <QDateTime>
#include <algorithm>
#include <QtNetwork>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>


/**
 * The code represents the GUI for a mood tracking application.
 *
 * Three little graphs display the collected air quality daata (UBA api) and
 * a larger graph the assigned mood values.
 *
 * @author Chachulski, Korte, Mathea
 */


MoodTrackingView::MoodTrackingView(Business::IBusiness* b, const QString& currentDiary, QWidget *parent) :
QWidget(parent),
ui(new Ui::MoodTrackingView),
m_business(b),
m_currentDiary(currentDiary)

{
    ui->setupUi(this);

    // Get the data: mood_PM10_CO2_SO2
    QMap<QString, QStringList> data;
    QList<QStringList> savedTrackingData;

    savedTrackingData = m_business->loadSavedTrackingData(currentDiary);

    for (QStringList d : savedTrackingData){

        QStringList dataMoodEnvironment;

        dataMoodEnvironment.append(d.at(2)); //mood value
        dataMoodEnvironment.append(d.at(3)); //PM 100 value
        dataMoodEnvironment.append(d.at(4)); //CO value
        dataMoodEnvironment.append(d.at(5)); //SO2 value

        data.insert(d.at(1),dataMoodEnvironment);

    }


        // SetUp PM10 Graph
        setUp(*(ui->graphicsView_PM10), data, 1);

        // SetUp CO2 Graph
        setUp(*(ui->graphicsView_CO), data, 2);

        // SetUp SO2 Graph
        setUp(*(ui->graphicsView_SO2), data, 3);

        // SetUp Mood Graph
        setUp(*(ui->graphicsView_Mood), data, 0);

}

MoodTrackingView::~MoodTrackingView()
{
delete ui;
}




void MoodTrackingView::setUp(QGraphicsView &graphicsView, QMap<QString, QStringList> &displayData, int column)
{
    QGraphicsScene *scene = new QGraphicsScene();
    graphicsView.setScene(scene);

    QRectF sceneRect = graphicsView.rect();
    scene->setSceneRect(sceneRect);

    QMap<int, double> data;
    QMapIterator<QString, QStringList> it(displayData);
    while (it.hasNext()) {
        it.next();
        int x = it.key().toInt();
        double y = it.value().at(column).toDouble();
        if (!data.contains(x)) {
            data[x] = y;
        }
    }

    // Ensure the data is sorted by diary page
    QMap<int, double> sortedData = data;
    QList<int> xValues = sortedData.keys();
    std::sort(xValues.begin(), xValues.end());

    QList<double> yValues;
    for (int x : xValues) {
        yValues.append(sortedData[x]);
    }

    double xMin = xValues.first();
    double xMax = xValues.last();
    double xScale = sceneRect.width() / (xMax - xMin);
    double yMin = *std::min_element(yValues.begin(), yValues.end())-10;
    double yMax = *std::max_element(yValues.begin(), yValues.end())+10;
    double yScale = sceneRect.height() / (yMax - yMin);

    // Create the line for a  line graph
    QPainterPath path;
    path.moveTo((xValues.first() - xMin) * xScale, sceneRect.height() - (yValues.first() - yMin) * yScale);
    for (int i = 1; i < xValues.size(); i++) {
        path.lineTo((xValues.at(i) - xMin) * xScale, sceneRect.height() - (yValues.at(i) - yMin) * yScale);

        // Add dots at each data point
        QGraphicsEllipseItem *dot = new QGraphicsEllipseItem((xValues.at(i) - xMin) * xScale - 2, sceneRect.height() - (yValues.at(i) - yMin) * yScale - 2, 4, 4);
        dot->setPen(QPen(Qt::black, 2));
        scene->addItem(dot);

    }

    QGraphicsPathItem *line = new QGraphicsPathItem(path);
    line->setPen(QPen(Qt::blue, 3));
    scene->addItem(line);


    // Draw the x and y axis

    QGraphicsLineItem *xAxis = new QGraphicsLineItem();
    xAxis->setPen(QPen(Qt::black, 2));
    xAxis->setLine(0, sceneRect.height(), sceneRect.width(), sceneRect.height());
    scene->addItem(xAxis);

    QGraphicsLineItem *yAxis = new QGraphicsLineItem();
    yAxis->setPen(QPen(Qt::black, 2));
    yAxis->setLine(0, 0, 0, sceneRect.height());
    scene->addItem(yAxis);

    QGraphicsTextItem *xLabel = new QGraphicsTextItem("X Axis");
    xLabel->setPos(sceneRect.width() - xLabel->boundingRect().width(), sceneRect.height());
    scene->addItem(xLabel);

        // Y-Axis Labels
        // Set the number of markers
        int numMarkers = 5;

        // Calculate the increment between each marker
        double yIncrement = (yMax - yMin) / (numMarkers - 1);
        yIncrement = round(yIncrement * 10) / 10;

        // Add the y-markers labels
        for (int i = 0; i < numMarkers; i++) {
            double yValue = yMin + i * yIncrement;
            QGraphicsTextItem *marker = new QGraphicsTextItem(QString::number(yValue));
            marker->setPos(1, sceneRect.height() - (yValue - yMin) * yScale - marker->boundingRect().height() / 2);
            scene->addItem(marker);
        }

        // Add the x-axis labels indicating the page
        for (int i = 0; i < xValues.size(); i++) {
            QGraphicsTextItem *xMarker = new QGraphicsTextItem(QString::number(xValues.at(i)));
            xMarker->setPos((xValues.at(i) - xMin) * xScale - xMarker->boundingRect().width() / 2, sceneRect.height());
            scene->addItem(xMarker);

    }

}

