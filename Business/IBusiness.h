#ifndef IBUSINESS_H
#define IBUSINESS_H
#include "Common/Entry.h"
#include "Common/Diary.h"
#include "Common/User.h"
#include "Common/Entry.h"

/**
 * IBusiness is a class which only contains pure virtual functions and therefore is being used as an interface.
 * The interface intermediates between the presentation layer and the implemented logic in the business layer.
 *
 * @author Chachulski, Korte, Mathea
 */

namespace Business{

class IBusiness{

public:
    virtual ~IBusiness(){}
    virtual bool login(const QString& userName, const QString& password) = 0;
    virtual bool registerUser(const QString& email, const QString& userName, const QString& password) = 0;
    virtual void deleteCurrentUser() = 0;
    virtual const Common::User& getCurrentUser() = 0;
    virtual bool createDiary(QString diaryName, int userID) = 0;
    virtual const QMap<int, Common::Diary>& getUserDiaryMap(int userID) = 0;
    virtual bool deleteDiary(QString diaryName) = 0;
    virtual const Common::Diary getDiary(const QString&) = 0;
    virtual const QString saveEntry(Common::Entry& entry) = 0;
    virtual void deleteEntry(Common::Entry entryID)=0;
    virtual int getNextPage(QString m_currentDiary,int currentUserID)=0;
    virtual const Common::Entry loadEntry(QString diaryName, int currentUser, int diaryPage)=0;
    virtual const QStringList loadTopicList()=0;
    virtual void writeTopics(QStringList topicList)=0;

    /* Methode für die Anzeige eines Eintrages.
    Für die Bearbeitung entweder A: eine zweite Methode ohne const anlegen,
    oder B: diese Methode ändern ?? */
    //virtual const Common::Entry& loadEntry(int diaryID, int entryID) = 0;

};
}

#endif //IBUSINESS_H
