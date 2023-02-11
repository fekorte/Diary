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
    explicit EntryView(Business::IBusiness* b, QString currentDiary, QWidget* parent = nullptr);
    explicit EntryView(Business::IBusiness* b, QString currentDiary, Common::Entry entry, QWidget* parent = nullptr);

    ~EntryView();

public slots:
    void setSelectedTopics(const QStringList &topics);
    void onButtonDescription(const QString &description);

protected:
    //void closeEvent(QCloseEvent *event) override;
    void saveCurrentEntry();
    void deleteEntry(Common::Entry entryID);
    void clearEntry();
    void editEntry();
    void updateEntry(Common::Entry entry);
    void addTopic();
    void addMood();
    void setNonClosable();
    void on_suggestionBoxExec(const QString& mood);
    void loadMoodsFromLast7Days(QString m_currentDiary);
    void showMainWindow();

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

    void showExerciseView(QString mood);

private slots:
    void goBack();
};

#endif // ENTRYVIEW_H
