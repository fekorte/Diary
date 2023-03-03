#ifndef MOODTRACKINGVIEW_H
#define MOODTRACKINGVIEW_H


#include <QWidget>
#include <QGraphicsView>
#include "Common/Diary.h"
#include "Common/Entry.h"
#include "Business/IBusiness.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

/*
 * @author Chachulski, Korte, Mathea
 */

namespace Ui {
class MoodTrackingView;
}

class MoodTrackingView : public QWidget
{
    Q_OBJECT

public:
    explicit MoodTrackingView(Business::IBusiness* b, const QString& currentDiary, QWidget *parent = nullptr);
    ~MoodTrackingView();

    void setUp(QGraphicsView& graphicsView,QMap<QString,QStringList>& data, int column);

private:
    Ui::MoodTrackingView *ui;

    Business::IBusiness* m_business;
    QString m_currentDiary;

    QGraphicsView* graphicsView_PM10;
    QGraphicsView* graphicsView_CO;
    QGraphicsView* graphicsView_SO2;
    QGraphicsView* graphicsView_Mood;
};

#endif // MOODTRACKINGVIEW_H
