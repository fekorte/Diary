#ifndef TOPICVIEW_H
#define TOPICVIEW_H

#include <QWidget>
#include <QStringList>
#include <QListWidget>
#include <QScrollArea>
#include "Business/IBusiness.h"

/*
 * @author Chachulski, Korte, Mathea
 */


namespace Ui {
class TopicView;
}

class TopicView : public QWidget
{
    Q_OBJECT

public:
    explicit TopicView(Business::IBusiness* b, QWidget *parent = nullptr);
    ~TopicView();

    void writeTopics(QStringList topicList);
    QStringList loadTopicList();
    void setSelectedTopics(const QStringList &topics);
    void saveSelectedTopics();

private:
    Ui::TopicView *ui;
    QStringList topicList;
    Business::IBusiness* m_business;
    QListWidget* topicListWidget;
    QListWidget* topicListSelectedWidget;
    QScrollArea* scrollArea;
    QScrollArea* scrollArea_2;



signals:
    void selectedTopicsChanged(const QStringList &topics);

};



#endif // TOPICVIEW_H
