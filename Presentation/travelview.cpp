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
#include <QDebug>


/*
 * @author Chachulski, Korte, Mathea
 */


TravelView::TravelView(Business::IBusiness* b, QString currentDiary, QWidget *parent) :

    QWidget(parent),
    ui(new Ui::TravelView), m_b(b), m_currentDiary(currentDiary)
{
    initialize();

    QObject::connect(ui->pushButton,&QPushButton::clicked,this,&TravelView::saveEntry);

    ui->pushButton_7->hide(); //buttons will only be displayed if image or map have been uploaded
    ui->pushButton_8->hide();
}


TravelView::TravelView(Business::IBusiness* b, QString currentDiary, Common::Entry entry, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TravelView), m_b(b), m_currentDiary(currentDiary)
{
    initialize();

    QObject::connect(ui->pushButton,&QPushButton::clicked,[this, entry](){ this->updateEntry(entry); });
    QObject::connect(ui->pushButton_9, &QPushButton::clicked, [this, entry](){ this->deleteEntry(entry); });


    m_mood = entry.getMood();
    m_place = entry.getPlace();
    m_pathToImg = entry.getPathToImgFile();
    m_pathToMap = entry.getPathToMapFile();
    m_date = entry.getDate();
    m_note = entry.getMyNote();

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
    QObject::connect(ui->pushButton_7,&QPushButton::clicked,this,&TravelView::showImage);
    QObject::connect(ui->pushButton_8,&QPushButton::clicked,this,&TravelView::showMap);
    QObject::connect(ui->pushButton_10,&QPushButton::clicked,[this](){ this->close(); });
    QObject::connect(ui->textEdit, &QTextEdit::textChanged, this, &TravelView::addNote);
}

TravelView::~TravelView()
{
    delete m_travelMap;
    m_travelMap = nullptr;
    delete ui;
    ui = nullptr;
}


void TravelView::closeEvent(QCloseEvent *event){
    MainWindow *goBack = new MainWindow(m_b,m_b->getUserDiaryMap(m_b->getCurrentUser().getUserId()));
    goBack->show();
    event->accept();
}


void TravelView::travelMapButtonPressed(const QString& pathToMap) //slot that reacts to signal emitted by TravelMapView
{
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
}

void TravelView::saveEntry(){
    saveImage();

    QStringList topic;
    topic << "Travelling";

    int page = m_b->getNextPage(m_currentDiary, m_b->getCurrentUser().getUserId());

    Common::Entry* newEntry = new Common::Entry(m_b->getCurrentUser().getUserId(), page, m_note, m_date, m_currentDiary, topic, m_mood, m_pathToImg, m_pathToMap, m_place);

    m_b->saveEntry(*newEntry);

    //Todo: there should be a way to check if save entry failed or succeeded
    QMessageBox::information(this,"Success", "Your travel entry has been saved successfully.");
}

void TravelView::deleteEntry(Common::Entry entry){
    QMessageBox::StandardButton deleteThisEntry;
    deleteThisEntry = QMessageBox::question(nullptr, "Delete This Memory?",
                                        "Are you sure you want to delete "
                                        "this memory?", QMessageBox::Yes|QMessageBox::No);

    if(deleteThisEntry == QMessageBox::Yes){
        m_b->deleteEntry(entry);
        this->close();
    }
}


void TravelView::updateEntry(Common::Entry entry){
    if(entry.getPathToImgFile() != m_pathToImg){
        saveImage();
    }
    Common::Entry* newEntry = new Common::Entry(entry.getUserID(),entry.getPage(), m_note, m_date, m_currentDiary, entry.getTopics(), m_mood, m_pathToImg, m_pathToMap, m_place);
    m_b->saveEntry(*newEntry);

    //Todo: there should be a way to check if save entry failed or succeeded
    QMessageBox::information(this,"Success", "Your travel entry has been saved successfully.");
}

void TravelView::addMood(){
    if(!m_mood.isEmpty()){
        QMessageBox::StandardButton changeMood;
        changeMood = QMessageBox::question(this, "Current mood", "Your selected mood is "
                                           + m_mood + " do you want to change it?", QMessageBox::Yes|QMessageBox::No);
        if(changeMood == QMessageBox::No){
            return;
        }
    }
    m_moodView = new MoodView(nullptr);
    m_moodView->show();

    connect(m_moodView, &MoodView::buttonDescription, this, &TravelView::onButtonDescription);
}

void TravelView::onButtonDescription(const QString &description)
{
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

void TravelView::addImage(){
    QString pathToImg = QFileDialog::getOpenFileName(this, tr("Open Image"), "/Desktop", tr("Images (*.png *.xpm *.jpg *.bmp)"));

    if (!pathToImg.isEmpty()) {
        m_pathToImg = pathToImg;
        ui->pushButton_7->show();
        displayImage();
    }
}

void TravelView::saveImage(){
    //copies the selected file to the Data/ folder in the build folder and uses a unique identifier
    QFileInfo fileInfo(m_pathToImg);
    QString fileExtension = fileInfo.suffix();
    QString newFile = QDateTime::currentDateTime().toString("yyyyMMddhhmmss") + "." + fileExtension;
    bool success = QFile::copy(m_pathToImg, "Data/" + newFile);

    if(!success){
        qDebug()<<"Copy failed";
    } else {
        m_pathToImg = "Data/" + newFile;
    }
}

void TravelView::displayImage(){
    QPixmap image(m_pathToImg);
    ui->label_6->setPixmap(image);
    ui->label_6->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    ui->label_6->setScaledContents(true);
    ui->label_6->setMaximumSize(500,500);
}

void TravelView::addMap(){
    QStringList continents;
    continents << "Asia" << "Africa" << "North America" << "South America" << "Europe" << "Australia";

    bool ok;
    QString decision = QInputDialog::getItem(this, tr("Choose a map"), tr("On which continent are you travelling?:"), continents, 0, false, &ok);
    if (ok && !decision.isEmpty()){
        switch(continents.indexOf(decision))
            {
                case 0:
                    m_pathToMap = ":/Images/Images/asia.jpg";
                    break;
                case 1:
                    m_pathToMap = ":/Images/Images/africa.jpg";
                    break;
                case 2:
                    m_pathToMap = ":/Images/Images/northAmerica.jpg";
                    break;
                case 3:
                    m_pathToMap = ":/Images/Images/southAmerica.jpg";
                    break;
                case 4:
                    m_pathToMap = ":/Images/Images/europe.jpg";
                    break;
                case 5:
                    m_pathToMap = ":/Images/Images/australia.jpg";
                    break;
                default:
                    qDebug() << "Invalid continent selected";
        }

        if(m_travelMap->loadImage(m_pathToMap)){ //TravelMapView loads the selected image and displays it
            QMessageBox::information(this,"Select travel place", "Please click on the map to select a travel place");
            m_travelMap->show();
        }
    }
}

void TravelView::addDate(){
    bool ok;
    QString selectedDate = QInputDialog::getText(nullptr, "Select a date", "Enter a date (MM/dd/yyyy):", QLineEdit::Normal, QDate::currentDate().toString("MM/dd/yyyy"), &ok);
    if (ok && !selectedDate.isEmpty()) {
        m_date = selectedDate;
        ui->label_3->setText("Date: " + selectedDate);
    }
}

void TravelView::showImage(){
    displayFullSize(m_pathToImg);
}

void TravelView::showMap(){
    displayFullSize(m_pathToMap);
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

