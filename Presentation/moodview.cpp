#include "moodview.h"
#include "ui_moodview.h"

MoodView::MoodView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MoodView)
{
    ui->setupUi(this);
    angryMood = new QPushButton();
    anxiousMood =new QPushButton();
    tiredMood = new QPushButton();
    sadMood = new QPushButton();
    stressedMood = new QPushButton();
    balancedMood = new QPushButton();
    satisfiedMood = new QPushButton();
    joyfulMood = new QPushButton();
    excitedMood = new QPushButton();
    ecstaticMood = new QPushButton();
    wideAwakeMood = new QPushButton();

//    connect(angryMood, &QPushButton::clicked, [this] {
//        emit buttonDescription(angryMood->text());
//        this->close();
//    });

    QList<QPushButton*> buttons = this->findChildren<QPushButton*>();
    for (QPushButton* button : buttons) {
        connect(button, &QPushButton::clicked, [this, button] {
            emit buttonDescription(button->text());
            this->close();
        });
    }


}

MoodView::~MoodView()
{
    delete ui;
}

