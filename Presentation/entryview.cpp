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

/**
 * The class EntryView is responsible for displaying an entry in the diary and allowing the user to edit/update it.
 * It uses the UI created by the UI designer. The class has two constructors: one for creating a new entry and the other
 * for editing an existing entry.
 *
 * The class has functions to save a new entry, delete an existing entry, add a topic, and add a mood.
 * The functions are connected to the buttons in the UI through the QObject::connect function. When the "Save Entry" button
 * is clicked, the function saveCurrentEntry is triggered, which retrieves the text from the QTextEdit widget, formats it,
 * and sends it to the business logic (IBusiness class) to be saved.
 *
 * The code implements a UI with a scroll area containing a text edit widget for the entry, buttons for saving,
 * deleting, editing, adding a topic, and adding a mood. The code also includes a label to display the username and diary name.
 *
 * @author Chachulski, Korte, Mathea
 */

EntryView::EntryView(Business::IBusiness* b, QString currentDiary, QWidget* parent) :
    QWidget(parent), ui(new Ui::EntryView), m_b(b), m_currentDiary(currentDiary) {

    ui->setupUi(this);
    //this->entryEdit->setAttribute(Qt::WA_StyledBackground);

    ui->userNameLabel->setText(m_b->getCurrentUser().getUserName());
    ui->diaryNameLabel->setText(currentDiary);

    //Entry Functions
    QObject::connect(ui->saveEntryButton,&QPushButton::clicked,this,&EntryView::saveCurrentEntry);
    QObject::connect(ui->deleteEntryButton, &QPushButton::clicked, this, &EntryView::clearEntry);
    QObject::connect(ui->editEntryButton, &QPushButton::clicked, this, &EntryView::editEntry);
    QObject::connect(ui->addTopicButton, &QPushButton::clicked, this, &EntryView::addTopic);
    QObject::connect(ui->addMoodButton, &QPushButton::clicked, this, &EntryView::addMood);

    // EntryView constantly closes after save button. Until error is found, use this:
    QObject::connect(ui->saveEntryButton,&QPushButton::clicked,this,&EntryView::setNonClosable);

    currentUser = m_b->getCurrentUser();
    currentUserID = m_b->getCurrentUser().getUserId();

}

EntryView::EntryView(Business::IBusiness* b, QString currentDiary, Common::Entry entry, QWidget *parent):
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
    QObject::connect(ui->addTopicButton, &QPushButton::clicked, this, &EntryView::noNewTopicForOldTopics);
    QObject::connect(ui->addMoodButton, &QPushButton::clicked, this, &EntryView::noNewMoodsForOldEntries);

    currentUser = m_b->getCurrentUser();
    currentUserID = m_b->getCurrentUser().getUserId();
    ui->entryEdit->setReadOnly(true);
}

EntryView::~EntryView()
{
    delete ui;
    ui = nullptr;
}


void EntryView::saveCurrentEntry()
{

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

        int page = m_b->getNextPage(m_currentDiary,currentUserID);

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


        Common::Entry* newEntry = new Common::Entry(currentUserID,page,text,dateString, diaryName, topicList, mood);

        if (m_b->saveEntry(*newEntry) == "-"){
            ui->entryEdit->setReadOnly(true);
            ui->entryEdit->setFocus();
            QMessageBox::information(this, "Entry Saved!", "Another page is added to the story of you  life.");
            return;
        }else {
            ui->entryEdit->setReadOnly(true);
            ui->entryEdit->setFocus();

        //Dialog des Mental Trainers
            QMessageBox::StandardButton suggestion;
            suggestion = QMessageBox::question(nullptr, "Mental Trainer",
                                                "Are you up for an exercise that makes you "
                                                "feel better?", QMessageBox::Yes|QMessageBox::No);

            if(suggestion == QMessageBox::Yes){
                showExerciseView(mood);
            }
        }

    }
}


void EntryView::updateEntry(Common::Entry entry){

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

        Common::Entry* newEntry = new Common::Entry(entry.getUserID(),entry.getPage(),text,entry.getDate(), entry.getDiaryName(), entry.getTopics(), entry.getMood());

        m_b->saveEntry(*newEntry);
        ui->entryEdit->setReadOnly(true);
        ui->entryEdit->setFocus();

    }
}


void EntryView::deleteEntry(Common::Entry entry){
    QMessageBox::StandardButton deleteThisEntry;
    deleteThisEntry = QMessageBox::question(nullptr, "Delete This Memory?",
                                        "Are you sure you want to delete "
                                        "this memory?", QMessageBox::Yes|QMessageBox::No);

    if(deleteThisEntry == QMessageBox::Yes){
        m_b->deleteEntry(entry);
        ui->entryEdit->setText("");
    }
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


void EntryView::noNewTopicForOldTopics(){
    QMessageBox::warning(nullptr, "Warning", "You can't add new Topics to an existing entry!");
    return;
}

void EntryView::noNewMoodsForOldEntries(){
    QMessageBox::warning(nullptr, "Warning", "You can't add new Moods to an existing entry!");
    return;
}


void EntryView::showExerciseView(QString mood){
    ExerciseView* exView = new ExerciseView(mood);
    exView->setAccessibleName("Mental Trainer");
    exView->show();
}

void EntryView::setNonClosable(){
    this->setAttribute(Qt::WA_DeleteOnClose, false);
}

void EntryView::closeEvent(QCloseEvent *event){
    MainWindow *goBack = new MainWindow(m_b,m_b->getUserDiaryMap(currentUserID));
    goBack->show();

    event->accept();
}

void EntryView::addTopic(){

    // Topic Selection

    m_topicView = new TopicView(m_b,nullptr);
    m_topicView->show();

    connect(m_topicView, &TopicView::selectedTopicsChanged, this, &EntryView::setSelectedTopics);



}

void EntryView::setSelectedTopics(const QStringList &topics)
{
    topicList = topics;

    // update display
    ui->plainTextEdit->setPlainText(topicList.first());
}

void EntryView::addMood(){

    m_moodView = new MoodView(nullptr);
    m_moodView->show();

    connect(m_moodView, &MoodView::buttonDescription, this, &EntryView::onButtonDescription);
}



void EntryView::onButtonDescription(const QString &description)
{
    mood=description;
    ui->plainTextEdit->setPlainText(mood);
}

    /*
    void EntryView::on_suggestionBoxExec(const Common::Mood& mood){
         switch (ret){
                case QMessageBox::Yes:
                    if (m_b->startExercise(mood) //enthält *.mp3* ){
                        // TODO: in Fenster mit Hintergrundbild Audio mit QMediaPlayer abspielen.
                    }else {
                        // TODO: Fenster mit zurückgegebener Übungsanleitung (QString) + Bild öffnen
                    }
                    break;
                case QMessageBox::No:
                    return;
                    break;
                default:
                    return;
         }
    }*/
