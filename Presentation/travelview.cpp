#include "travelview.h"
#include "ui_travelview.h"
#include "travelmapview.h"
#include "moodview.h"
#include "exerciseview.h"
#include "mainwindow.h"
#include <QString>
#include <QFileDialog>
#include <QFile>
#include <QPixmap>
#include <QDebug>
#include <QStringList>
#include <QInputDialog>
#include <QLayout>
#include <QLabel>
#include <QDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QDate>
#include <QDateEdit>


/**
 * View to display or create or update a travel entry.
 * @author Chachulski, Korte, Mathea
 */

/*
 * To inherit from EntryView would have decreased code duplication but also the flexibility/independence of the work process.
 * This is why TravelView does not inherit from EntryView.
 */

//constructor if create travel entry button has been clicked (new entry will be created)
TravelView::TravelView(Business::IBusiness* b, const QString& currentDiary, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TravelView), m_b(b), m_currentDiary(currentDiary){

    initialize();

    QObject::connect(ui->pushButton,&QPushButton::clicked,this,&TravelView::saveEntry);

    //the two following buttons will only be displayed if image or map have been uploaded
    ui->pushButton_7->hide();
    ui->pushButton_8->hide();
    ui->pushButton_9->hide(); //delete button will not be displayed because no entry exists yet that can be deleted
    ui->pushButton_11->hide();
    ui->pushButton_12->hide();
}


//this constructor loads an existing (travel) entry and displays its information
TravelView::TravelView(Business::IBusiness* b, const QString& currentDiary, const Common::Entry& entry, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TravelView), m_b(b), m_currentDiary(currentDiary), m_mood(entry.getMood()),
    m_place(entry.getPlace()), m_pathToImg(entry.getPathToImgFile()), m_pathToMap(entry.getPathToMapFile()), m_date(entry.getDate()), m_note(entry.getMyNote()){

    initialize();

    QObject::connect(ui->pushButton,&QPushButton::clicked,[this, entry](){ this->updateEntry(entry); }); //save button leads to updateEntry()
    QObject::connect(ui->pushButton_9, &QPushButton::clicked, [this, entry](){ this->deleteEntry(entry); });

    ui->label_4->setText("Travel place: " + entry.getPlace());
    ui->label_3->setText("Date: " + entry.getDate());
    ui->textEdit->setText(m_note);

    displayMap();
    displayImage();
}

void TravelView::initialize(){

    ui->setupUi(this);
    m_travelMap = new TravelMapView();

    ui->label->setText("Welcome " + m_b->getCurrentUser().getUserName() + " to your diary " + m_currentDiary);

    QObject::connect(m_travelMap, &TravelMapView::returnMap, this, &TravelView::travelMapButtonPressed);
    QObject::connect(ui->pushButton_2,&QPushButton::clicked,this,&TravelView::addMood);
    QObject::connect(ui->pushButton_3,&QPushButton::clicked,this,&TravelView::addPlace);
    QObject::connect(ui->pushButton_4,&QPushButton::clicked,this,&TravelView::addImage);
    QObject::connect(ui->pushButton_5,&QPushButton::clicked,this,&TravelView::addMap);
    QObject::connect(ui->pushButton_6,&QPushButton::clicked,this,&TravelView::addDate);
    QObject::connect(ui->pushButton_7,&QPushButton::clicked,[this](){ this->displayFullSize(m_pathToImg);});
    QObject::connect(ui->pushButton_8,&QPushButton::clicked,[this](){ this->displayFullSize(m_pathToMap);});
    QObject::connect(ui->pushButton_10,&QPushButton::clicked,this,&TravelView::goBack);
    QObject::connect(ui->pushButton_11,&QPushButton::clicked,this,&TravelView::removeMap);
    QObject::connect(ui->pushButton_12,&QPushButton::clicked,this,&TravelView::removeImage);
    QObject::connect(ui->textEdit,&QTextEdit::textChanged,this,&TravelView::addNote); //activated every time text changes
}

TravelView::~TravelView(){

    delete ui;
    ui = nullptr;
}


void TravelView::closeEvent(QCloseEvent *event){

    if (event->spontaneous()){
        delete m_b;
        m_b = nullptr;
    }
    event->accept();
}

void TravelView::showMainWindow(){

    MainWindow *goBack = new MainWindow(m_b,m_b->getUserDiaryMap(m_b->getCurrentUser().getUserId()));
    this->close();
    goBack->show();
}


void TravelView::goBack(){

    QMessageBox::StandardButton leave;
    leave = QMessageBox::question(nullptr, "Go Back",
                                        "Unsaved changes might be deleted. Do you want to continue?", QMessageBox::Yes|QMessageBox::No);

    if(leave == QMessageBox::Yes){
        showMainWindow();   
    }
}

void TravelView::saveEntry(){

    QStringList topic;
    topic << "Travelling"; //user can't select a topic, it is always "Travelling"

    int page = m_b->getNextPage(m_currentDiary, m_b->getCurrentUser().getUserId());


    //it is intentional that an entry which contains no other information than the userID, diaryID and the topic can still be saved, to enable the user to create an entry without input
    Common::Entry newEntry(m_b->getCurrentUser().getUserId(), page, m_note, m_date, m_currentDiary, topic, m_mood, m_pathToImg, m_pathToMap, m_place);

    QString exercise = m_b->saveEntry(newEntry);

    QMessageBox::information(this,"Success", "Your travel entry has been saved successfully.");
    showMainWindow();


    if(exercise != "-"){ //activate personal trainer function if negative mood was selected
        showExerciseView(m_mood);
    }
}

void TravelView::showExerciseView(const QString& mood){

    QMessageBox::StandardButton suggestion;
    suggestion = QMessageBox::question(nullptr, "Mental Trainer",
                                        "Are you up for an exercise that makes you "
                                        "feel better?", QMessageBox::Yes|QMessageBox::No);

    if(suggestion == QMessageBox::Yes){
        ExerciseView* exView = new ExerciseView(mood);
        exView->setAccessibleName("Mental Trainer");
        exView->show();
     }
}

void TravelView::deleteEntry(const Common::Entry& entry){

    QMessageBox::StandardButton deleteThisEntry;
    deleteThisEntry = QMessageBox::question(nullptr, "Delete This Memory?",
                                        "Are you sure you want to delete "
                                        "this memory?", QMessageBox::Yes|QMessageBox::No);

    if(deleteThisEntry == QMessageBox::Yes){
        m_b->deleteEntry(entry);
        showMainWindow();
    }
}

void TravelView::updateEntry(const Common::Entry& entry){

    if(entry.getPathToImgFile() != m_pathToImg){
        m_b->deleteImageFile(entry.getPathToImgFile());
    }

    if(entry.getPathToMapFile() != m_pathToMap && !m_pathToMap.isEmpty()){
        m_b->deleteImageFile(entry.getPathToMapFile());
    }

    Common::Entry newEntry(entry.getUserID(),entry.getPage(), m_note, m_date, m_currentDiary, entry.getTopics(), m_mood, m_pathToImg, m_pathToMap, m_place);

    QString exercise = m_b->saveEntry(newEntry);

    QMessageBox::information(this,"Success", "Your travel entry has been saved successfully.");
    showMainWindow();


    if(exercise != "-"){ //activate personal trainer function if negative mood was selected
        showExerciseView(m_mood);
    }
}


void TravelView::addImage(){ //if user clicks "Upload image" button

    QString pathToImg = QFileDialog::getOpenFileName(this, tr("Open Image"), "/Desktop", tr("Images (*.png *.xpm *.jpg *.bmp)"));

    if (!pathToImg.isEmpty()) {
        m_pathToImg = pathToImg;
        ui->pushButton_7->show(); //display button "show image fullsize"
        ui->pushButton_12->show();
        displayImage();
    }
}

void TravelView::removeImage(){

    if(m_pathToImg.isEmpty()){
        QMessageBox::warning(this,"Error", "Please upload first.");
        return;
    }
    m_pathToImg.clear();
    ui->label_6->setPixmap(QPixmap());
    ui->label_6->update();
}

void TravelView::displayImage(){

    QPixmap image(m_pathToImg);
    ui->label_6->setPixmap(image);
    ui->label_6->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    ui->label_6->setScaledContents(true);
    ui->label_6->setMaximumSize(500,500);
}

void TravelView::addMood(){

    if(!m_mood.isEmpty()){
        QMessageBox::StandardButton changeMood;
        changeMood = QMessageBox::question(this, "Current mood", "Your selected mood is: "
                                           + m_mood + ". Do you want to change it?", QMessageBox::Yes|QMessageBox::No);
        if(changeMood == QMessageBox::No){
            return;
        }
    }
    m_moodView = new MoodView(nullptr);
    m_moodView->show();

    connect(m_moodView, &MoodView::buttonDescription, this, &TravelView::onButtonDescription);
}

void TravelView::onButtonDescription(const QString &description){ //receive selected mood from MoodView

    m_mood = description;
    QMessageBox::information(this,"Success", "Mood was added successfully to your entry.");

}

void TravelView::addPlace(){

    bool ok;
    QString place = QInputDialog::getText(this, "Select travel place", "What's your current travel place?", QLineEdit::Normal, QString(), &ok);
    if (ok && !place.isEmpty()) {
        ui->label_4->setText("Travel place: " + place);
        m_place = place;
    }
}


void TravelView::addMap(){

    QStringList continents;
    continents << "Asia" << "Africa" << "North America" << "South America" << "Europe" << "Australia";

    bool ok;
    QString decision = QInputDialog::getItem(this, tr("Choose a map"), tr("On which continent are you travelling?:"), continents, 0, false, &ok);
    if (ok && !decision.isEmpty()){
        QString selectedMap = m_b->travelContinentChoice(decision, continents);

        if(m_travelMap->loadImage(selectedMap)){ //TravelMapView loads the selected image and displays it
            QMessageBox::information(this,"Select travel place", "Please click on the map to select a travel place");
            m_travelMap->show();
        }
    }
}

//slot that reacts to signal emitted by TravelMapView to receive the path of the altered map
void TravelView::travelMapButtonPressed(const QString& pathToMap){

    if(!m_pathToMap.isEmpty()){ //delete previous map in the persistence
        m_b->deleteImageFile(m_pathToMap);
    }
    m_pathToMap = pathToMap;
    m_travelMap->close();
    displayMap();
}


void TravelView::displayMap(){

    QPixmap image(m_pathToMap);
    ui->label_5->setPixmap(image);
    ui->label_5->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    ui->label_5->setScaledContents(true);
    ui->label_5->setMaximumSize(600,600);

    ui->pushButton_8->show();
    ui->pushButton_11->show();
}

void TravelView::removeMap(){

    if(m_pathToMap.isEmpty()){
        QMessageBox::warning(this,"Error", "Please upload first.");
        return;
    }

    m_b->deleteImageFile(m_pathToMap);
    m_pathToMap.clear();
    ui->label_5->setPixmap(QPixmap());
    ui->label_5->update();
}


void TravelView::addDate(){

    bool ok;
    QString selectedDate = QInputDialog::getText(nullptr, "Select a date", "Enter a date (MM/dd/yyyy):", QLineEdit::Normal, QDate::currentDate().toString("MM/dd/yyyy"), &ok);
    QDate checkDate = QDate::fromString(selectedDate, "MM/dd/yyyy");
    if (ok && !selectedDate.isEmpty() && checkDate.isValid()) {
        m_date = selectedDate;
        ui->label_3->setText("Date: " + selectedDate);
    } else {
        QMessageBox::warning(this,"Error", "Please enter a valid date.");
    }
}

void TravelView::addNote(){

    m_note = ui->textEdit->toPlainText();
}

void TravelView::displayFullSize(const QString& filePath){

    if(filePath.isEmpty()){
        QMessageBox::warning(this,"Error", "Please upload first.");
    } else {
        QDialog* imageDialog = new QDialog(this);
        imageDialog->setModal(true);
        QLabel* imageLabel = new QLabel(imageDialog);
        QPixmap image = QPixmap(filePath);
        QPixmap scaledImage = image.scaled(800, 800, Qt::KeepAspectRatio);
        imageLabel->setPixmap(scaledImage);
        imageLabel->setScaledContents(true);
        QVBoxLayout *layout = new QVBoxLayout(imageDialog);
        layout->addWidget(imageLabel);
        imageDialog->exec();
    }
}

