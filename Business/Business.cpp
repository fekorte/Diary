#include "Business.h"
#include "IBusiness.h"
#include "Common/Diary.h"
#include "Common/Entry.h"
#include "Common/User.h"
#include "Persistence/IPersistence.h"
#include <exception>
#include <QString>
#include <QMap>

/**
 * Class Business inherits from IBusiness and overrides all its functions.
 * Therefore, class IBusiness works like an interface which class Business implements.
 *
 * The class Business contains four manager attributes (DiaryManager, EntryManager, UserManager and MoodTrackManager).
 * Function calls are being passed to the responsible manager to support the single-responsiblity principle.
 *
 * @author Chachulski, Korte, Mathea
 */

namespace Business {


    Business::Business(){
    }
    Business::~Business(){}

    bool Business::login(const QString& userName,const QString& password){

         return m_userManager.processLogin(userName, password);

    }

    bool Business::registerUser(const QString& email, const QString& userName, const QString& password){

       return m_userManager.processRegister(email, userName, password);

   }

     void Business::deleteCurrentUser(){
         m_userManager.deleteCurrentUser();
     }

     const Common::User& Business::getCurrentUser(){
         return m_userManager.getCurrentUser();
     }

     bool Business::createDiary(QString diaryName, int userID) {
        return m_diaryManager.processCreateDiary(diaryName, userID);
     }

     bool Business::deleteDiary(QString diaryName) {
         return m_diaryManager.deleteDiary(diaryName);
     }

     const Common::Diary Business::getDiary(const QString& diaryName){
         return m_diaryManager.getDiary(diaryName);
     }

     const QMap<int, Common::Diary>& Business::getUserDiaryMap(int userID){
         return m_diaryManager.getUserDiaryMap(userID);
     }

    const QString Business::saveEntry(Common::Entry& currentEntry) {

        return m_entryManager.saveEntry(currentEntry);

    }

    void Business::deleteEntry(Common::Entry entry){
        m_entryManager.deleteEntry(entry);
    };

    int Business::getNextPage(QString m_currentDiary, int currentUserID){
        return m_entryManager.getNextPage(m_currentDiary, currentUserID);
    }

    const Common::Entry Business::loadEntry(QString diaryName,int m_currentUserID, int diaryPage){
        return m_entryManager.loadEntry(diaryName,m_currentUserID, diaryPage);
    }

    void Business::writeTopics(QStringList topicList){
        m_entryManager.writeTopics(topicList);
    }



    const QStringList Business::loadTopicList(){
        return m_entryManager.loadTopicList();
    }


}
