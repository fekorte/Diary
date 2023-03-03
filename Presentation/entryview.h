#ifndef ENTRYVIEW_H
#define ENTRYVIEW_H

#include <QWidget>
#include <QTextEdit>
#include <QScrollArea>
#include <QTableWidget>
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

class EntryView : public QWidget{

    Q_OBJECT

public:
    explicit EntryView(Business::IBusiness* b, const QString& currentDiary, QWidget* parent = nullptr);
    explicit EntryView(Business::IBusiness* b, const QString& currentDiary, const Common::Entry& entry, QWidget* parent = nullptr);
    ~EntryView();

public slots:
    void setSelectedTopics(const QStringList& topics);
    void onButtonDescription(const QString& description);

protected:
    void saveCurrentEntry();
    void deleteEntry(const Common::Entry& entryID);
    void clearEntry();
    void editEntry();
    void updateEntry(const Common::Entry& entry);
    void addTopic();
    void addMood();
    void setNonClosable();
    void on_suggestionBoxExec(const QString& mood);
    void loadMoodsFromLast7Days(const QString& m_currentDiary);
    void showMainWindow();
    QString getMoodValue(QString mood);
    QString extractValueFromUBA(const QString &dateTime, int component);

private:
    Ui::EntryView* ui;
    Business::IBusiness* m_b;
    QString m_currentDiary;
    QTextEdit* entryEdit;
    QScrollArea* scrollArea;
    QScrollArea* scrollArea_2;
    Common::Entry m_entry;
    Common::User currentUser;
    int currentUserID;
    QStringList topicList;
    QString mood;
    TopicView *m_topicView;
    MoodView *m_moodView;
    QTableWidget* moodTableWidget;

    void showExerciseView(const QString& mood);

private slots:
    void goBack();
};

#endif // ENTRYVIEW_H
