#include "DiaryManager.h"
#include "UserManager.h"
#include "Persistence/FilePersistence.h"

/**
 * Handels all actions related to Diary Obects.
 * Intermediates between persistence layer and presentation layer via the business interface IBusiness.
 *
 * QMap<int, Common::Diary>* m_diary
 * and
 * QMap<int, Common::Diary>* m_userDiaryMap:
 *
 * key = diaryID, value = Diary
 *
 * @author Chachulski, Korte, Mathea
 *
 */

namespace Business {

    DiaryManager::DiaryManager(){  
        m_diaryMap = new QMap<int, Common::Diary>;
        m_userDiaryMap = new QMap<int, Common::Diary>;
        m_persistence = new Persistence::FilePersistence();
    }

    DiaryManager::~DiaryManager(){
        delete m_diaryMap;
        m_diaryMap = nullptr;
        delete m_persistence;
        m_persistence = nullptr;
        delete m_userDiaryMap;
        m_userDiaryMap = nullptr;
    }

    const Common::Diary DiaryManager::getDiary(const QString& diaryName) {
        updateDiaryMap();

        Common::Diary specificDiaryPtr;
        for(Common::Diary diary : m_userDiaryMap->values()){
            if (diary.getDiaryName() == diaryName)
            specificDiaryPtr = diary;
        }
        return specificDiaryPtr;
    }


    // --- Laura Chachulski: the following are needed for new entries to be written into a diary, that is  again updated in the diary map

    const Common::Diary& DiaryManager::getCurrentDiary(const QString& diaryName, int userID) {

        getUserDiaryMap(userID);

        for(Common::Diary& diary : *m_userDiaryMap){
            if (diary.getDiaryName() == diaryName)
                return diary;
        }
        throw std::out_of_range("Diary not found");
    }


    QMap<int, Common::Diary> DiaryManager::updateDiariesMap_withNewEntries(const Common::Diary& currentDiary) {

        for(Common::Diary& diary : *m_diaryMap){
            if (diary.getDiaryName() == currentDiary.getDiaryName()){
                diary = currentDiary;
            }

        }

        return *m_diaryMap;
    }


    // ------ End functions for writing/deleting entries ---------


    bool DiaryManager::processCreateDiary(const QString diaryName, int userID){

        /* check if Diary already exists*/
        for (Common::Diary diary : m_userDiaryMap->values()){
            if (diary.getDiaryName() == diaryName){
                    return 0;
            }
        }

        /* check if diary is the user's first one. If yes, diaryID is one.
        If no, diaryID is the last given userID plus one */
        int id = m_userDiaryMap->empty() ? 1 : m_userDiaryMap->last().getDiaryID()+1;
        QMap<int, Common::Entry> emptyEntryList;
        Common::Diary newDiary(emptyEntryList, id, diaryName, userID);

        m_userDiaryMap->insert(newDiary.getDiaryID(), newDiary);

        if(! m_diaryMap->isEmpty()){

            for (Common::Diary diary : m_diaryMap->values()){
                if (diary.getUserID() == userID)
                    m_diaryMap->remove(diary.getDiaryID());
            }
        }


        //outcommented line because my (Felina Korte) qt version 5 does not support it, for each loop as a temporary work-around
        //m_diaryMap->insert(*m_userDiaryMap);
        for(Common::Diary diary : *m_userDiaryMap){
            m_diaryMap->insert(diary.getDiaryID(), diary);
        }

        m_persistence->writeDiaries(*m_diaryMap);

        return 1;
    }

    bool DiaryManager::deleteDiary(QString diaryName){
        for (Common::Diary diary: m_diaryMap->values()){
            if (diary.getDiaryName() == diaryName){
                m_diaryMap->remove(diary.getDiaryID());
                m_persistence->writeDiaries(*m_diaryMap);
                return true;
            }
        }
        return false;
    }

    void DiaryManager::updateDiaryMap(){
        *m_diaryMap = m_persistence->readDiaries();
    }

    void DiaryManager::updateUserDiaryMap(int currentUserID) {
        m_userDiaryMap->clear();
        updateDiaryMap();
        for(Common::Diary diary : m_diaryMap->values()){
            if (diary.getUserID() == currentUserID){
                m_userDiaryMap->insert(diary.getDiaryID(), diary);
            }
        }
    }

    const QMap<int, Common::Diary>& DiaryManager::getUserDiaryMap(int userID){
        updateUserDiaryMap(userID);
        return *m_userDiaryMap;
    }

    const QMap<int, Common::Diary>& DiaryManager::getDiaryMap(){
        return *m_diaryMap;
    }

}
