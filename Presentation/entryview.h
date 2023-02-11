#ifndef ENTRYVIEW_H
#define ENTRYVIEW_H

#include <QWidget>
#include <QTextEdit>
#include <QScrollArea>
#include "Business/IBusiness.h"
#include "Common/Entry.h"
#include "topicview.h"
#include "moodview.h"

/*
 * @author Chachulski, Korte, Mathea
 */


namespace Ui {
class EntryView;
}

class EntryView : public QWidget
{
    Q_OBJECT

public:

    explicit EntryView(Business::IBusiness* b, QString currentDiary, QWidget* parent = nullptr);
    explicit EntryView(Business::IBusiness* b, QString currentDiary, Common::Entry entry, QWidget* parent = nullptr);

    ~EntryView();

public slots:
    void setSelectedTopics(const QStringList &topics);
    void onButtonDescription(const QString &description);

protected:
    void closeEvent(QCloseEvent *event) override;
    void saveCurrentEntry();
    void deleteEntry(Common::Entry entryID);
    void clearEntry();
    void editEntry();
    void updateEntry(Common::Entry entry);
    void addTopic();
    void addMood();
    //void setSelectedTopic(const QStringList &topic);
    void noNewTopicForOldTopics();
    void noNewMoodsForOldEntries();
    void setNonClosable();
    void on_suggestionBoxExec(const QString& mood);


private:
    Ui::EntryView* ui;
    Business::IBusiness* m_b;
    QString m_currentDiary;
    QTextEdit* entryEdit;
    QScrollArea* scrollArea;
    Common::Entry m_entry;
    Common::User currentUser;
    int currentUserID;
    QStringList topicList;
    QString mood;
    TopicView *m_topicView;
    MoodView *m_moodView;


    void showExerciseView(QString mood);

};



#endif // ENTRYVIEW_H
