#ifndef BUSINESS_H
#define BUSINESS_H

#include "IBusiness.h"
#include "Common/User.h"
#include "Persistence/IPersistence.h"
#include "DiaryManager.h"
#include "EntryManager.h"
#include "ExerciseManager.h"
#include "UserManager.h"
#include "MoodTrackManager.h"
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
    MoodTrackManager m_moodTrackManager;

public:

    Business();

    ~Business();

    bool login(const QString& userName, const QString& password) override;

    bool registerUser(const QString& email, const QString& userName, const QString& password) override;

    void deleteCurrentUser() override;

    const Common::User& getCurrentUser() override;

    const QString saveEntry(Common::Entry& entry) override;

    void deleteEntry(Common::Entry entryID) override;

    // const Common::Entry& loadEntry(int diaryID, int entryID);

    bool createDiary(QString diaryName, int userID) override;

    bool deleteDiary(QString diaryName) override;

    const Common::Diary getDiary(const QString&) override;

    const QMap<int, Common::Diary>& getUserDiaryMap(int userID) override;

    int getNextPage(QString m_currentDiary, int currentUserID) override;

    const Common::Entry loadEntry(QString diaryName,int m_currentUserID, int diaryPage) override;

    const QStringList loadTopicList() override;

    void writeTopics(QStringList topicList) override;

};
}

#endif //BUSINESS_h
