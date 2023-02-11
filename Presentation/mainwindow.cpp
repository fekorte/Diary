#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "loginview.h"
#include "entryview.h"
#include "travelview.h"
#include "Business/IBusiness.h"
#include "Common/User.h"
#include <QString>
#include <QMessageBox>
#include <iostream>
#include <QLineEdit>
#include <QInputDialog>
#include <QStringListModel>
#include <QDir>
#include <QDebug>
#include <iostream>

/**
 * @author Chachulski, Korte, Mathea
 */

MainWindow::MainWindow(Business::IBusiness* b, const QMap<QString, Common::Diary>& myDiaryMap, QWidget* parent) :
    QMainWindow(parent), ui(new Ui::MainWindow), m_business(b), m_myDiaryMap(myDiaryMap){

    ui->setupUi(this);

    filterInput = new QLineEdit();

    if (m_myDiaryMap.empty()){
                createDiary();
                ui->menuMy_diary->menuAction()->setVisible(false);
    }else {
        ui->comboBox_ChangeDiary->addItems(getDiaryNameList());
        m_currentDiary = ui->comboBox_ChangeDiary->currentText();
    }

    ui->menuLog_out->setTitle("Welcome " + m_business->getCurrentUser().getUserName());
    ui->menuMy_diary->setTitle("Current diary: " + m_currentDiary);

    QObject::connect(ui->actionLog_out_2,&QAction::triggered,this,&MainWindow::showLoginView);

    QObject::connect(ui->actionDelete_account_2,&QAction::triggered,this,&MainWindow::deleteUser);

    QObject::connect(ui->ChangeDiaryButton,&QPushButton::clicked,this,&MainWindow::changeDiary);

    QObject::connect(ui->createNewEntryButton,&QPushButton::clicked,this,&MainWindow::showEntryView);

    QObject::connect(ui->pushButton,&QPushButton::clicked,this,&MainWindow::showTravelEntryView);

    QObject::connect(ui->createDiaryButton,&QPushButton::clicked,this,&MainWindow::createDiary);

    QObject::connect(ui->DeleteDiaryButton,&QPushButton::clicked,this,&MainWindow::deleteDiary);

    QObject::connect(ui->pastEntriesListWidget, &QListWidget::itemDoubleClicked, this, &MainWindow::openEntryView); // when double clicked on an entry in past entries

    QObject::connect(ui->filterButton, &QPushButton::clicked, this, &MainWindow::displayFilteredEntries);

    displayEntries();
}

MainWindow::~MainWindow(){

    delete ui;
    ui = nullptr;
}


void MainWindow::closeEvent(QCloseEvent *event){
    if (event->spontaneous()){
        delete m_business;
        m_business = nullptr;
    }
    event->accept();
}

void MainWindow::displayEntries(){
    ui->pastEntriesListWidget->clear();
    try{

        if(m_myDiaryMap.isEmpty()){
            return;
        }

        QMap<int, Common::Entry> currentDiaryEntries = m_business->getCurrentDiary(m_currentDiary, m_business->getCurrentUser().getUserId()).getEntryMap();
        QStringList entriesString;

        if(filterInput->text().isEmpty()){
            for(auto i:currentDiaryEntries)
            {
                Common::Entry entry = i;

                int page = entry.getPage();
                QString date = entry.getDate();

                QString displayEntry;
                if(entry.getTopics().contains("Travelling")){
                    displayEntry = "Page " + QString::number(page) + "  " + date + "  " + entry.getPlace();
                } else {
                    displayEntry = "Page " + QString::number(page) + " ... " + date;
                }
                entriesString.append(displayEntry);
            }

            ui->pastEntriesListWidget->addItems(entriesString);
        }



    } catch (...) {
        // handle exception
        std::cerr << "Error loading diary entries" << std::endl;
    }
    
}

void MainWindow::displayFilteredEntries(){
    ui->pastEntriesListWidget->clear();
    try{

        QMap<int, Common::Entry> currentDiaryEntries = m_business->getCurrentDiary(m_currentDiary, m_business->getCurrentUser().getUserId()).getEntryMap();
        QStringList entriesString;
        QString topicString = ui->filterInput->text();

            for(auto i:currentDiaryEntries)
            {
                Common::Entry entry = i;

                if(entry.getTopics().contains(topicString)){
                    int page = entry.getPage();
                    QString date = entry.getDate();

                    QString displayEntry;
                    if(entry.getTopics().contains("Travelling")){
                        displayEntry = "Page " + QString::number(page) + "  " + date + "  " + entry.getPlace();
                    } else {
                        displayEntry = "Page " + QString::number(page) + " ... " + date;
                    }
                    entriesString.append(displayEntry);
                }
            }

            ui->pastEntriesListWidget->addItems(entriesString);


    } catch (...) {
        // handle exception
        std::cerr << "Error loading diary entries" << std::endl;
    }

}

void MainWindow::showEntryView(){


    if(m_myDiaryMap.isEmpty()){
        QMessageBox::information(nullptr,"DiaryApp", "Please create a diary before you start to write.");
        return;
    }
    this->close();
    EntryView* eview = new EntryView(m_business, m_currentDiary);
    eview->show();
}

void MainWindow::showTravelEntryView(){

    if(m_myDiaryMap.isEmpty()){
        QMessageBox::information(nullptr,"DiaryApp", "Please create a diary before you start to write.");
        return;
    }
    this->close();
    TravelView* eview = new TravelView(m_business, m_currentDiary);
    eview->show();
}

void MainWindow::showLoginView(){
    this->close();
    LoginView* lView = new LoginView(m_business);
    lView->show();
}

void MainWindow::deleteUser(){
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(nullptr, "Delete account", "Are you sure that you want to delete your account?", QMessageBox::Yes|QMessageBox::No);
    if(reply == QMessageBox::Yes){
        m_business->deleteCurrentUser();
        showLoginView();
    }
}

const QList<QString> MainWindow::getDiaryNameList(){
    QList<QString> diaryNameList;
    for (Common::Diary& diary : m_myDiaryMap){
        diaryNameList.append(diary.getDiaryName());
    }
    return diaryNameList;
}

void MainWindow::changeDiary(){
    //get diary name from comboBox
    m_currentDiary = ui->comboBox_ChangeDiary->currentText();

    //name of new current Diary is used as title for filtermenu
    ui->menuMy_diary->setTitle("Current Diary: " + m_currentDiary);
    displayEntries();
}

void MainWindow::createDiary(){

    bool  ok;
    std::string output;
    m_myDiaryMap.empty() ? (output = "Name your first diary") : (output = "Diary Name: ");
    const char* charPtr = output.c_str();
    QString text = QInputDialog::getText(nullptr, tr("Time to be creative!"),
                                          tr(charPtr), QLineEdit::Normal,
                                         "my new Diary", &ok);
    if (ok && !text.isEmpty()){
        try {
            if (! m_business->createDiary(text, m_business->getCurrentUser().getUserId())){
                QMessageBox msgBox;
                msgBox.setText("This diary already exists. Choose a different name.");
                msgBox.exec();
                return;
            }

        }  catch(std::exception& e){
            QMessageBox msg;
            msg.setText(e.what());
            msg.setWindowTitle("Failed to create a new Diary!");
            msg.exec();
            this->show();
        }

        ui->menuMy_diary->menuAction()->setVisible(true);

        //name of new current Diary is used as title for filtermenu
        ui->menuMy_diary->setTitle("Current diary: " + text);

        //add the new diary to the selectBox
        ui->comboBox_ChangeDiary->addItem(text);
        ui->comboBox_ChangeDiary->setCurrentText(text);
        m_currentDiary = text;

        m_myDiaryMap = m_business->getUserDiaryMap(m_business->getCurrentUser().getUserId());
        displayEntries();

        QMessageBox newDiaryMessage;
        newDiaryMessage.setText("Success! Your diary is waiting for your input.");
        newDiaryMessage.exec();
    }
}

void MainWindow::deleteDiary(){

    if (m_currentDiary != ui->comboBox_ChangeDiary->currentText()){

        QMessageBox::StandardButton decision;
        decision = QMessageBox::warning(nullptr, "Caution!", "You are deleting " + m_currentDiary + " not " + ui->comboBox_ChangeDiary->currentText() +
                                        ". Do you want to continue?", QMessageBox::Yes|QMessageBox::No);
        if(decision == QMessageBox::No){
            return;
        }
    }
    m_business->deleteDiary(m_currentDiary);

    m_myDiaryMap = m_business->getUserDiaryMap(m_business->getCurrentUser().getUserId());

    ui->comboBox_ChangeDiary->clear();
    ui->comboBox_ChangeDiary->addItems(getDiaryNameList());

    QMessageBox newDiaryMessage;
    newDiaryMessage.setText(m_currentDiary + " has been removed. Time for something new!");
    newDiaryMessage.exec();

    if(!m_myDiaryMap.isEmpty()){
        m_currentDiary = ui->comboBox_ChangeDiary->currentText();
    } else {
        m_currentDiary.clear();
    }

    displayEntries();
}

void MainWindow::openEntryView() {

    //get the current entry from the list widget
    QListWidgetItem* item = ui->pastEntriesListWidget->currentItem();
    QString entryName = item->text();

    QStringList parts = entryName.split(" ");
    int pageID = parts[1].toInt();

    //retrive the entry data from the business logic
    Common::Entry entry = m_business->loadEntry(m_currentDiary, m_business->getCurrentUser().getUserId(),pageID);

    std::cout << entry.getMyNote().toStdString() << std::endl;


    this->close();

    //open the entry view and pass the entry data and current diary
    if(entry.getTopics().contains("Travelling")){
        TravelView* tview = new TravelView(m_business, m_currentDiary, entry);
        tview->show();
    } else {
        EntryView* eview = new EntryView(m_business, m_currentDiary, entry);
        eview->show();
    }
}
