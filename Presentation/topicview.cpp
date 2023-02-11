#include "topicview.h"
#include "ui_topicview.h"
#include "Business/IBusiness.h"
#include <iostream>

TopicView::TopicView(Business::IBusiness* b,QWidget* parent) :
    QWidget(parent),
    ui(new Ui::TopicView),
    m_business(b)
{
    ui->setupUi(this);

    // Write first topics
    //topicList << "C++" <<"Dream" << "Future Plans" << "Food Recipe" << "Nature" << "Shopping List" << "Thoughts" << "Travelling";
    //writeTopics(topicList);

    QObject::connect(ui->addTopicsButton,&QPushButton::clicked,this,&TopicView::saveSelectedTopics);

    topicList = loadTopicList();

    scrollArea = ui->scrollArea;
    topicListWidget = ui->topicListWidget;

    scrollArea_2 = ui->scrollArea_2;
    topicListSelectedWidget = ui->topicListSelectedWidget;

    try{
        topicListWidget->addItems(topicList);

        QObject::connect(topicListWidget, &QListWidget::doubleClicked, [this](const QModelIndex &index){
            QListWidget *listWidget = topicListWidget;
            QListWidgetItem *item = listWidget->item(index.row());
            this->topicListSelectedWidget->addItem(item->text());
        });

    } catch (...) {
        // handle exception
        std::cerr << "Error loading topics!" << std::endl;
    }
}

TopicView::~TopicView()
{
    delete ui;
}


QStringList TopicView::loadTopicList(){
    return m_business->loadTopicList();
}

void TopicView::writeTopics(QStringList topicList){
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


void TopicView::setSelectedTopics(const QStringList &topics)
{

    emit selectedTopicsChanged(topics);
}


