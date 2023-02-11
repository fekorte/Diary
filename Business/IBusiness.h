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

    //User related functions
    virtual bool login(const QString& userName, const QString& password) = 0;
    virtual bool registerUser(const QString& email, const QString& userName, const QString& password) = 0;
    virtual void deleteCurrentUser() = 0;
    virtual const Common::User& getCurrentUser() = 0;


    //Entry related functions
    virtual const QString saveEntry(const Common::Entry& entry) = 0;
    virtual void deleteEntry(const Common::Entry& entryID)=0;
    virtual int getNextPage(const QString& currentDiary,int currentUserID)=0;
    virtual const Common::Entry loadEntry(const QString& diaryName, int currentUser, int diaryPage)=0;
    virtual const QStringList loadTopicList()=0;
    virtual void writeTopics(const QStringList& topicList)=0;
    virtual const QString travelContinentChoice(const QString& decision, const QStringList& continents) = 0;
    virtual void deleteImageFile(const QString& filePath) = 0;


    //Diary related functions
    virtual bool createDiary(const QString& diaryName, int userID) = 0;
    virtual const QMap<QString, Common::Diary>& getUserDiaryMap(int userID) = 0;
    virtual bool deleteDiary(const QString& diaryName) = 0;
    virtual const Common::Diary getCurrentDiary(const QString& diaryName, int userId) = 0;



};
}

#endif //IBUSINESS_H
