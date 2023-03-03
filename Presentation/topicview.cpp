#include "topicview.h"
#include "ui_topicview.h"
#include "Business/IBusiness.h"
#include <iostream>
#include <QMessageBox>

/**
 * This class provides a graphical user interface for the user to interact with topics.
 * The class displays a list of topics, allows the user to add a new topic, search for
 * a specific topic, and select topics to be saved.
 *
 * @author Chachulski, Korte, Mathea
 */


TopicView::TopicView(Business::IBusiness* b,QWidget* parent) :
    QWidget(parent),
    ui(new Ui::TopicView),
    m_business(b){

    ui->setupUi(this);

    QObject::connect(ui->addTopicsButton,&QPushButton::clicked,this,&TopicView::saveSelectedTopics);
    QObject::connect(ui->clearSelectedTopicsButton,&QPushButton::clicked,this,&TopicView::clearSelectedTopics);
    QObject::connect(ui->addNewTopic,&QPushButton::clicked,this,&TopicView::addANewTopic);
    QObject::connect(ui->searchTopicButton,&QPushButton::clicked,this,&TopicView::filterTopics);

    topicList = loadTopicList();



    scrollArea = ui->scrollArea;
    topicListWidget = ui->topicListWidget;

    scrollArea_2 = ui->scrollArea_2;
    topicListSelectedWidget = ui->topicListSelectedWidget;


    updateTopicList();

    QObject::connect(topicListWidget, &QListWidget::doubleClicked, [this](const QModelIndex &index){
        QListWidget *listWidget = topicListWidget;
        QListWidgetItem *item = listWidget->item(index.row());
        this->topicListSelectedWidget->addItem(item->text());
    });


}

TopicView::~TopicView(){

    delete ui;
}


QStringList TopicView::loadTopicList(){

    return m_business->loadTopicList();
}

void TopicView::writeTopics(const QStringList& topicList){

    m_business->writeTopics(topicList);
}

void TopicView::saveSelectedTopics(){

    QStringList selectedTopicList;
    for (int i = 0; i < topicListSelectedWidget->count(); ++i) {
      QListWidgetItem *item = topicListSelectedWidget->item(i);
      selectedTopicList << item->text();
    }

    if (!selectedTopicList.isEmpty()){
        setSelectedTopics(selectedTopicList);
        this->close();

    }

}


void TopicView::updateTopicList(){

    try{
        topicListWidget->addItems(topicList);

    } catch (...) {
        // handle exception
        std::cerr << "Error loading topics!" << std::endl;
    }

}

void TopicView::addANewTopic(){

    ui->topicListWidget->clear();
    QString newTopic = ui->addTopicText->toPlainText();

    if(!this->topicList.contains(newTopic)){

        this->topicList.append(newTopic);
    }else{
        QMessageBox::information(this, "Duplicate Topic","This topic already exists!");
    }

    TopicView::writeTopics(this->topicList);
    TopicView::updateTopicList();
    ui->addTopicText->clear();
}

void TopicView::setSelectedTopics(const QStringList& topics){

    emit selectedTopicsChanged(topics);
}

void TopicView::clearSelectedTopics(){
    ui->topicListSelectedWidget->clear();
}

void TopicView::filterTopics(){

    QString searchedTopic = ui->searchTopicText->toPlainText();

    if(!searchedTopic.isEmpty()){

        if(this->topicList.contains(searchedTopic)){
            ui->topicListWidget->clear();
            this->topicListWidget->addItem(searchedTopic);
        }

    }else{
        TopicView::updateTopicList();
    }
}
