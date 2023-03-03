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


    Business::Business(){}

    Business::~Business(){}


    //User related functions
    bool Business::login(const QString& userName,const QString& password){

         return m_userManager.processLogin(userName, password);

    }

    bool Business::registerUser(const QString& email, const QString& userName, const QString& password){

       return m_userManager.processRegister(email, userName, password);

   }

     void Business::deleteCurrentUser(){

        QMap<QString, Common::Diary> userDiaryMap = m_diaryManager.getUserDiaryMap(m_userManager.getCurrentUser().getUserId());
        for(const Common::Diary& diary : userDiaryMap){
            m_diaryManager.deleteDiary(diary.getDiaryName());
        }
        m_userManager.deleteCurrentUser();

     }

     const Common::User& Business::getCurrentUser(){

         return m_userManager.getCurrentUser();
     }



    //Entry related functions
    const QString Business::saveEntry(const Common::Entry& currentEntry) {

        return m_entryManager.saveEntry(currentEntry);

    }

    void Business::deleteEntry(const Common::Entry& entry){

        m_entryManager.deleteEntry(entry);
    }

    int Business::getNextPage(const QString& currentDiary, int currentUserID){

        return m_entryManager.getNextPage(currentDiary, currentUserID);
    }

    const Common::Entry Business::loadEntry(const QString& diaryName,int m_currentUserID, int diaryPage){

        return m_entryManager.loadEntry(diaryName,m_currentUserID, diaryPage);
    }

    void Business::writeTopics(const QStringList& topicList){

        m_entryManager.writeTopics(topicList);
    }

    const QStringList Business::loadTopicList(){

        return m_entryManager.loadTopicList();
    }

    const QString Business::travelContinentChoice(const QString& decision, const QStringList& continents){

        return m_entryManager.travelContinentChoice(decision, continents);
    }

    void Business::deleteImageFile(const QString& filePath){

        m_entryManager.deleteImageFile(filePath);
    }

    void Business::writeMoodTrackingData(QStringList& trackingDataList){

        m_moodTrackManager.writeMoodTrackingData(trackingDataList);

    }

    const QList<QStringList> Business::loadSavedTrackingData(const QString& diaryName){
        return m_moodTrackManager.loadSavedTrackingData(diaryName);
    }



    //Diary related functions
    bool Business::createDiary(const QString& diaryName, int userID){

       return m_diaryManager.processCreateDiary(diaryName, userID);
    }

    bool Business::deleteDiary(const QString& diaryName) {

        return m_diaryManager.deleteDiary(diaryName);
    }

    const Common::Diary Business::getCurrentDiary(const QString& diaryName, int userId){

        return m_diaryManager.getCurrentDiary(diaryName, userId);
    }

    const QMap<QString, Common::Diary>& Business::getUserDiaryMap(int userID){

        return m_diaryManager.getUserDiaryMap(userID);
    }
}
