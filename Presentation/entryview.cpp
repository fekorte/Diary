#include "entryview.h"
#include "ui_entryview.h"
#include "mainwindow.h"
#include "topicview.h"
#include "Business/IBusiness.h"
#include "Common/User.h"
#include "Common/Entry.h"
#include "Common/Entry.h"
#include <QString>
#include <QTextEdit>
#include <string>
#include <QMessageBox>
#include "exerciseview.h"
#include <iostream>

/**
 * The class EntryView is responsible for displaying a new entry in the diary and allowing the user to edit/update it.
 * The class has two constructors: one for creating a new entry and the other for editing an existing entry.
 *
 * The code implements a UI with a scroll area containing a text edit widget for the entry, buttons for saving,
 * deleting, editing, adding a topic, and adding a mood. The code also includes a label to display the username and diary name.
 *
 * @author Chachulski, Korte, Mathea
 */

EntryView::EntryView(Business::IBusiness* b, const QString& currentDiary, QWidget* parent) :
    QWidget(parent), ui(new Ui::EntryView), m_b(b), m_currentDiary(currentDiary) {

    ui->setupUi(this);

    // Mood: Weekly review
    loadMoodsFromLast7Days(m_currentDiary);

    ui->userNameLabel->setText(m_b->getCurrentUser().getUserName());
    ui->diaryNameLabel->setText(currentDiary);

    //Entry Functions
    QObject::connect(ui->saveEntryButton,&QPushButton::clicked,this,&EntryView::saveCurrentEntry);
    QObject::connect(ui->deleteEntryButton, &QPushButton::clicked, this, &EntryView::clearEntry);
    QObject::connect(ui->editEntryButton, &QPushButton::clicked, this, &EntryView::editEntry);
    QObject::connect(ui->addTopicButton, &QPushButton::clicked, this, &EntryView::addTopic);
    QObject::connect(ui->addMoodButton, &QPushButton::clicked, this, &EntryView::addMood);
    QObject::connect(ui->goBackButton,&QPushButton::clicked,this,&EntryView::goBack);

    currentUser = m_b->getCurrentUser();
    currentUserID = m_b->getCurrentUser().getUserId();
    ui->editEntryButton->setEnabled(false);
}

EntryView::EntryView(Business::IBusiness* b, const QString& currentDiary, const Common::Entry& entry, QWidget *parent):
    QWidget(parent),
    ui(new Ui::EntryView), m_b(b), m_currentDiary(currentDiary), m_entry(entry){

    ui->setupUi(this);

    ui->userNameLabel->setText(m_b->getCurrentUser().getUserName());
    ui->diaryNameLabel->setText(currentDiary);

    ui->entryEdit->setText(entry.getMyNote());

    //Existing Entry Function
    QObject::connect(ui->saveEntryButton,&QPushButton::clicked,[this, entry](){ this->updateEntry(entry); });
    QObject::connect(ui->deleteEntryButton, &QPushButton::clicked, [this, entry](){ this->deleteEntry(entry); });
    QObject::connect(ui->editEntryButton, &QPushButton::clicked, this, &EntryView::editEntry);
    QObject::connect(ui->goBackButton,&QPushButton::clicked,this,&EntryView::goBack);

    currentUser = m_b->getCurrentUser();
    currentUserID = m_b->getCurrentUser().getUserId();
    ui->entryEdit->setReadOnly(true);
    ui->scrollArea_2->hide();
    ui->moodTableWidget->hide();
    ui->label->hide();
    ui->addTopicButton->hide();
    ui->addMoodButton->hide();
}

EntryView::~EntryView(){

    delete ui;
    ui = nullptr;
}


void EntryView::saveCurrentEntry(){

     QString text;
     scrollArea = new QScrollArea;

    if (scrollArea) {
        entryEdit = ui->scrollArea->findChild<QTextEdit*>("entryEdit");
        if (entryEdit) {
            QTextDocument *document = entryEdit->document();
            text = document->toPlainText();
        }
    }

    if (!text.isEmpty()){

        QString diaryName = m_currentDiary;

        int page = m_b->getNextPage(m_currentDiary, currentUserID);

        QDateTime currentDateTime = QDateTime::currentDateTime();
        QString dateString = currentDateTime.toString("yyyy-MM-dd hh:mm:ss");


        if(topicList.isEmpty()){
            topicList << "General";
        }


        if (topicList.contains("Travelling")) {
            QMessageBox::warning(nullptr, "Warning", "Travelling topic detected! - The only travelling you are doing now is to the kitchen! Choose a Travel Entry if you really travel!");
            topicList.clear();
            return;
        }

        if (mood.isEmpty()) {
            mood = "Neutral";
        }


        Common::Entry newEntry(currentUserID,page,text,dateString, diaryName, topicList, mood);

        if (m_b->saveEntry(newEntry) == "-"){
            ui->entryEdit->setReadOnly(true);
            ui->entryEdit->setFocus();
            QMessageBox::information(this, "Entry Saved!", "Another page is added to the story of your  life.");
            ui->editEntryButton->setEnabled(true);
            showMainWindow();
        } else {
            ui->entryEdit->setReadOnly(true);
            ui->entryEdit->setFocus();
            QMessageBox::information(this, "Entry Saved!", "Another page is added to the story of your life.");
            ui->editEntryButton->setEnabled(true);

            //Dialog des Mental Trainers
            QMessageBox::StandardButton suggestion;
            suggestion = QMessageBox::question(nullptr, "Mental Trainer",
                                                "Are you up for an exercise that makes you "
                                                "feel better?", QMessageBox::Yes|QMessageBox::No);

            if(suggestion == QMessageBox::Yes){
                showMainWindow();
                showExerciseView(mood);
            }
        }
    }
}


void EntryView::updateEntry(const Common::Entry& entry){

     QString text;
     scrollArea = new QScrollArea;

    if (scrollArea) {
        entryEdit = ui->scrollArea->findChild<QTextEdit*>("entryEdit");
        if (entryEdit) {
            QTextDocument *document = entryEdit->document();
            text = document->toPlainText();
        }
    }

    if (!text.isEmpty()){

        Common::Entry newEntry(entry.getUserID(),entry.getPage(),text,entry.getDate(), entry.getDiaryName(), entry.getTopics(), entry.getMood());
        m_b->saveEntry(newEntry);
        QMessageBox::information(this, "Entry Updated!", "Your diary entry has been updated.");
        ui->entryEdit->setReadOnly(true);
        ui->entryEdit->setFocus();

    }
}


void EntryView::deleteEntry(const Common::Entry& entry){

    QMessageBox::StandardButton deleteThisEntry;
    deleteThisEntry = QMessageBox::question(nullptr, "Delete This Memory?",
                                        "Are you sure you want to delete "
                                        "this memory?", QMessageBox::Yes|QMessageBox::No);

    if(deleteThisEntry == QMessageBox::Yes){
        m_b->deleteEntry(entry);
        ui->entryEdit->setText("");
    }

    showMainWindow();
}


void EntryView::clearEntry(){

    QMessageBox::StandardButton deleteThisEntry;
    deleteThisEntry = QMessageBox::question(nullptr, "Delete This Memory?",
                                        "Are you sure you want to delete "
                                        "this memory?", QMessageBox::Yes|QMessageBox::No);

    if(deleteThisEntry == QMessageBox::Yes){
        ui->entryEdit->setText("");
    }
}


void EntryView::editEntry(){

    ui->entryEdit->setReadOnly(false);
}


void EntryView::showExerciseView(const QString& mood){

    ExerciseView* exView = new ExerciseView(mood);
    exView->setAccessibleName("Mental Trainer");
    exView->show();
}

void EntryView::setNonClosable(){

    this->setAttribute(Qt::WA_DeleteOnClose, false);
}


void EntryView::addTopic(){

    // Topic Selection
    m_topicView = new TopicView(m_b,nullptr);
    m_topicView->show();

    connect(m_topicView, &TopicView::selectedTopicsChanged, this, &EntryView::setSelectedTopics);
}

void EntryView::setSelectedTopics(const QStringList &topics){
    topicList = topics;
}

void EntryView::addMood(){

    m_moodView = new MoodView(nullptr);
    m_moodView->show();

    connect(m_moodView, &MoodView::buttonDescription, this, &EntryView::onButtonDescription);
}



void EntryView::onButtonDescription(const QString &description){

    mood = description;
}


void EntryView::loadMoodsFromLast7Days(const QString& m_currentDiary)
{
    Common::Diary currentDiary = m_b->getCurrentDiary(m_currentDiary, m_b->getCurrentUser().getUserId());

    QDateTime currentDateTime = QDateTime::currentDateTime();
    QDateTime sevenDaysAgo = currentDateTime.addDays(-7);

    QMap<QDateTime,QString> moodByDate;

    scrollArea_2 = new QScrollArea;
    QTableWidget *table = ui->scrollArea_2->findChild<QTableWidget*>("moodTableWidget");
    if (!table) {
        table = new QTableWidget;
        table->setObjectName("moodTableWidget");
        ui->scrollArea_2->setWidget(table);
    }

    QMap<int, Common::Entry> diaryEntries = currentDiary.getEntryMap();
    for (auto i: diaryEntries) {

        Common::Entry entry = i;

        QDateTime date = QDateTime::fromString(entry.getDate(), "yyyy-MM-dd hh:mm:ss");

        if (date >= sevenDaysAgo && date <= currentDateTime) {
            moodByDate.insert(date, entry.getMood());
        }
    }

    table->clear();
    table->setRowCount(0);
    table->setColumnCount(2);
    table->setHorizontalHeaderLabels(QStringList() << "Date" << "Mood");

    try {
        if (!moodByDate.empty()) {
            table->setRowCount(moodByDate.size());
            int row = 0;
            for (auto i = moodByDate.begin(); i != moodByDate.end(); ++i) {
                QTableWidgetItem *dateItem = new QTableWidgetItem(i.key().toString("dd.MM.yyyy"));
                table->setItem(row, 0, dateItem);

                QTableWidgetItem *moodItem = new QTableWidgetItem(i.value());
                table->setItem(row, 1, moodItem);

                if (i.value() == "Angry") {
                    moodItem->setBackground(QBrush(QColor("#FF0000")));
                }
                if (i.value() == "Anxious") {
                    moodItem->setBackground(QBrush(QColor("#FF0000")));
                }
                else if (i.value() == "Tired") {
                    moodItem->setBackground(QBrush(QColor("#E6B0AA")));
                }
                else if (i.value() == "Nervous") {
                    moodItem->setBackground(QBrush(QColor("#F3CE7F")));
                }
                else if (i.value() == "Stressed") {
                    moodItem->setBackground(QBrush(QColor("#2471A3")));
                }
                else if (i.value() == "Sad") {
                    moodItem->setBackground(QBrush(QColor("#BEE3E3")));
                }
                else if (i.value() == "Balanced") {
                    moodItem->setBackground(QBrush(QColor("#3498DB")));
                }
                else if (i.value() == "Satisfied") {
                    moodItem->setBackground(QBrush(QColor("#A3E4D7")));
                }
                else if (i.value() == "Joyful") {
                    moodItem->setBackground(QBrush(QColor("#82E0AA")));
                }
                else if (i.value() == "Excited") {
                    moodItem->setBackground(QBrush(QColor("#1ABC9C")));
                }
                else if (i.value() == "Ecstatic") {
                    moodItem->setBackground(QBrush(QColor("#117A65")));
                }
                else if (i.value() == "Wide Awake") {
                    moodItem->setBackground(QBrush(QColor("#FFFFFF")));
                }

                row++;
            }
        }
    } catch (const std::exception& e) {
        // handle the exception here
    }
}

void EntryView::showMainWindow(){

    MainWindow *goBack = new MainWindow(m_b, m_b->getUserDiaryMap(m_b->getCurrentUser().getUserId()));
    this->close();
    goBack->show();
}

void EntryView::goBack(){

    QMessageBox::StandardButton leave;
    leave = QMessageBox::question(nullptr, "Go Back",
                                        "Unsaved changes might be deleted. Do you want to continue?", QMessageBox::Yes|QMessageBox::No);

    if(leave == QMessageBox::Yes){
        showMainWindow();
    }
}

