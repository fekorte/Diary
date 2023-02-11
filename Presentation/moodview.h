#ifndef MOODVIEW_H
#define MOODVIEW_H

#include <QWidget>
#include <QPushButton>

/*
 * @author Chachulski, Korte, Mathea
 */


namespace Ui {

class MoodView;
}

class MoodView : public QWidget{

    Q_OBJECT

public:
    explicit MoodView(QWidget *parent = nullptr);
    ~MoodView();

private:
    Ui::MoodView *ui;
    QPushButton* angryMood;
    QPushButton* anxiousMood;
    QPushButton* tiredMood;
    QPushButton* sadMood;
    QPushButton* stressedMood;
    QPushButton* balancedMood;
    QPushButton* satisfiedMood;
    QPushButton* joyfulMood;
    QPushButton* excitedMood;
    QPushButton* ecstaticMood;
    QPushButton* wideAwakeMood;
    QPushButton* nervousMood;


signals:
    void buttonDescription(const QString &description);
};

#endif // MOODVIEW_H
