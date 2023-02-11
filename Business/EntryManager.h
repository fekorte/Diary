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

    const QString saveEntry(Common::Entry entry);
    void deleteEntry(const Common::Entry& entry);
    int getNextPage(const QString& currentDiary, int m_currentUserID);

    QMap<int,Common::Entry> getCurrentDiaryEntries();
    void setCurrentDiaryEntries(const QList<Common::Entry>& entries);


    const Common::Entry loadEntry(const QString& diaryName,int m_currentUserID, int diaryPage);

    void writeTopics(const QStringList& topicList);
    const QStringList loadTopicList();

    const QString travelContinentChoice(const QString& decision, const QStringList& continents);
    void deleteImageFile(const QString& filePath);

};
}

#endif // ENTRYMANAGER_H
