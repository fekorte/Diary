#ifndef BUSINESS_H
#define BUSINESS_H

#include "IBusiness.h"
#include "Common/User.h"
#include "Persistence/IPersistence.h"
#include "DiaryManager.h"
#include "EntryManager.h"
#include "ExerciseManager.h"
#include "UserManager.h"
#include <QMap>
#include <QString>

/*
 * @author Chachulski, Korte, Mathea
 */

namespace Business{

class Business : public IBusiness{
private:

    DiaryManager m_diaryManager;
    EntryManager m_entryManager;
    UserManager m_userManager;

public:

    Business();

    ~Business();

    bool login(const QString& userName, const QString& password) override;

    bool registerUser(const QString& email, const QString& userName, const QString& password) override;

    void deleteCurrentUser() override;

    const Common::User& getCurrentUser() override;

    const QString saveEntry(const Common::Entry& entry) override;

    void deleteEntry(const Common::Entry& entryID) override;

    bool createDiary(const QString& diaryName, int userID) override;

    bool deleteDiary(const QString& diaryName) override;

    const Common::Diary getDiary(const QString&) override;

    const QMap<int, Common::Diary>& getUserDiaryMap(int userID) override;

    int getNextPage(const QString& m_currentDiary, int currentUserID) override;

    const Common::Entry loadEntry(const QString& diaryName, int currentUserID, int diaryPage) override;

    const QStringList loadTopicList() override;

    void writeTopics(const QStringList& topicList) override;

    const QString travelContinentChoice(const QString& decision, const QStringList& continents) override;

    void deleteImageFile(const QString& filePath) override;

};
}

#endif //BUSINESS_h
