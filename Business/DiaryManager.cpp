#include "DiaryManager.h"
#include "UserManager.h"
#include "Persistence/FilePersistence.h"

/**
 * Handels all actions related to Diary Obects.
 * Intermediates between persistence layer and presentation layer via the business interface IBusiness.
 *
 * QMap<int, Common::Diary>* m_diary
 * key = diaryID, value = Diary
 *
 * QMap<int, Common::Diary>* m_userDiaryMap:
 * key = diary name, value = Diary
 *
 *
 * @author Chachulski, Korte, Mathea
 *
 */

namespace Business {

    DiaryManager::DiaryManager(){  
        m_diaryMap = new QMap<int, Common::Diary>;
        m_userDiaryMap = new QMap<QString, Common::Diary>;
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

    const Common::Diary DiaryManager::getCurrentDiary(const QString& diaryName, int userId) {

        updateUserDiaryMap(userId);
        return m_userDiaryMap->value(diaryName);
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


    bool DiaryManager::processCreateDiary(const QString& diaryName, int userID){

        if(m_userDiaryMap->contains(diaryName)){ /* check if Diary already exists*/
            return 0;
        }

        int id = m_diaryMap->empty() ? 1 : m_diaryMap->last().getDiaryID()+1; //generate diaryID according to m_diaryMap (map with all diaries)
        QMap<int, Common::Entry> emptyEntryList;
        Common::Diary newDiary(emptyEntryList, id, diaryName, userID);

        m_userDiaryMap->insert(newDiary.getDiaryName(), newDiary);

        if(!m_diaryMap->isEmpty()){
            for (Common::Diary diary : m_diaryMap->values()){
                if (diary.getUserID() == userID)
                    m_diaryMap->remove(diary.getDiaryID());
            }
        }

        for(Common::Diary diary : *m_userDiaryMap){
            m_diaryMap->insert(diary.getDiaryID(), diary);
        }

        m_persistence->writeDiaries(*m_diaryMap);

        return 1;
    }

    int DiaryManager::getDiaryId(const QString& diaryName){

        if(m_userDiaryMap->contains(diaryName)){
            return m_userDiaryMap->value(diaryName).getDiaryID();
        }
        return 0; //zero is no diary id and can therefore be used to signal that diary is non existent
    }

    bool DiaryManager::deleteDiary(const QString& diaryName){

        if(getDiaryId(diaryName) != 0){
            m_diaryMap->remove(getDiaryId(diaryName));
            return true;
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
                m_userDiaryMap->insert(diary.getDiaryName(), diary);
            }
        }
    }

    const QMap<QString, Common::Diary>& DiaryManager::getUserDiaryMap(int userID){
        updateUserDiaryMap(userID);
        return *m_userDiaryMap;
    }

    const QMap<int, Common::Diary>& DiaryManager::getDiaryMap(){
        return *m_diaryMap;
    }

}
