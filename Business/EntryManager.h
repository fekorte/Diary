#ifndef ENTRYMANAGER_H
#define ENTRYMANAGER_H
#include "Common/Entry.h"
#include "Persistence/IPersistence.h"
#include "IBusiness.h"
#include "DiaryManager.h"
#include "UserManager.h"
#include "ExerciseManager.h"

/*
 * @author Chachulski, Korte, Mathea
 */

namespace Business{

class EntryManager{

protected:

    QMap<int, Common::Entry> entriesCurrentDiary;
    Persistence::IPersistence* m_filePersistence;
    Business::IBusiness* m_business;
    DiaryManager* m_diaryManager;
    UserManager* m_userManager;
    ExerciseManager* m_exerciseManager;

public:
    EntryManager();
    ~EntryManager();

    const QString saveEntry(Common::Entry& entry);
    void deleteEntry(Common::Entry entry);
    int getNextPage(QString m_currentDiary, int m_currentUserID);

    QMap<int,Common::Entry> getCurrentDiaryEntries();
    void setCurrentDiaryEntries(QList<Common::Entry> entries);


    const Common::Entry loadEntry(QString diaryName,int m_currentUserID, int diaryPage);

    void writeTopics(QStringList topicList);
    const QStringList loadTopicList();

};
}

#endif // ENTRYMANAGER_H
