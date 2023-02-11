#ifndef TRAVELVIEW_H
#define TRAVELVIEW_H

#include <QWidget>
#include <QCloseEvent>
#include "Business/IBusiness.h"
#include "Common/Diary.h"
#include "Common/Entry.h"
#include "moodview.h"

/**
 * @author Chachulski, Korte, Mathea
 */


namespace Ui {
class TravelView;
}

class TravelMapView;

class TravelView : public QWidget{
    Q_OBJECT

public:
    explicit TravelView(Business::IBusiness* b, QString currentDiary, QWidget *parent = nullptr);
    explicit TravelView(Business::IBusiness* b, QString currentDiary, Common::Entry entry, QWidget *parent = nullptr);
    ~TravelView();

protected:
    void closeEvent(QCloseEvent* event) override;
    void initialize();
    void deleteEntry(Common::Entry entry);
    void updateEntry(Common::Entry entry);
    void displayMap();
    void saveImage();
    void displayImage();
    void displayFullSize(const QString& filePath);

private slots:
    void onButtonDescription(const QString &description);
    void travelMapButtonPressed(const QString& message);
    void saveEntry();
    void addMood();
    void addPlace();
    void addImage();
    void addMap();
    void addDate();
    void showImage();
    void showMap();
    void addNote();

private:
    Ui::TravelView* ui;
    Business::IBusiness* m_b;
    MoodView* m_moodView;
    QString m_currentDiary;

    TravelMapView* m_travelMap;

    QString m_mood;
    QString m_place;
    QString m_pathToImg;
    QString m_pathToMap;
    QString m_date;
    QString m_note;

};

#endif // TRAVELVIEW_H
