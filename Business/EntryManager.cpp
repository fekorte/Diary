#include "EntryManager.h"
#include "Common/User.h"
#include "Business.h"
#include "Persistence/FilePersistence.h"
#include <QDebug>

/**
 *
 * This code defines the EntryManager class in the Business namespace. The EntryManager class has
 * the responsibility of managing Common::Entry objects. The class contains functions for saving,
 * deleting, and retrieving entries, as well as a function for obtaining the next page number for the diary.
 *
 * The class also has a member variable entriesCurrentDiary which is a QMap of Common::Entry objects,
 * representing the entries of the current diary.
 *
 * The class has pointers to DiaryManager, UserManager, and FilePersistence objects, which are used
 * to manage diaries, users, and file persistence respectively. The class's constructor initializes
 * these objects and the destructor deletes them.
 *
 *
 * @author Chachulski, Korte, Mathea
 */

namespace Business{

    EntryManager::EntryManager(){

        m_diaryManager = new DiaryManager();
        m_userManager = new UserManager();
        m_filePersistence = new Persistence::FilePersistence();
    }

    EntryManager::~EntryManager(){

        delete m_diaryManager;
        m_diaryManager = nullptr;

        delete m_filePersistence;
        m_filePersistence = nullptr;
    }

    const QString EntryManager::saveEntry(Common::Entry entry){

        // update current diary with the new entry
        if (m_diaryManager != nullptr) {


            if(entry.getTopics().contains("Travelling") && !entry.getPathToImgFile().isEmpty()){

                QString newImagePath = m_filePersistence->saveImageFile(entry.getPathToImgFile());
                entry.setPathToImgFile(newImagePath);
            }

            // set current diary
            Common::Diary currentDiary = m_diaryManager->getCurrentDiary(entry.getDiaryName(),entry.getUserID());


            // retrieve all current diary entries
            entriesCurrentDiary = currentDiary.getEntryMap();

            // add entry to current Entry list from current diary
            entriesCurrentDiary.insert(entry.getPage(),entry);


            currentDiary.setEntryMap(entriesCurrentDiary);
            m_diaryManager->updateDiariesMap_withNewEntries(currentDiary);

        } else {
            throw std::runtime_error("m_diaryManager pointer is null!");
        }

        // write all diaries, with the info  for the current diary, to persistence
        m_filePersistence->writeDiaries(m_diaryManager->getDiaryMap());

        //start mentalTrainer if applicable
        QList<QString> moodList;
        moodList.append("stressed");
        moodList.append("nervous");
        moodList.append("sad");
        moodList.append("tired");
        moodList.append("angry");
        moodList.append("anxious");

        QString currentMood = entry.getMood();

        if (entry.getTopics().contains("Mood Tracker") && moodList.contains(currentMood)){
            return currentMood;
        }else {
            return "-";
        }
    }


    void EntryManager::deleteEntry(const Common::Entry& entry){

        // update current diary by deleting the selected entry
        if (m_diaryManager != nullptr) {

            if(entry.getTopics().contains("Travelling")){
                deleteImageFile(entry.getPathToImgFile());
                deleteImageFile(entry.getPathToMapFile());
            }

            // set current diary
            Common::Diary currentDiary = m_diaryManager->getCurrentDiary(entry.getDiaryName(),entry.getUserID());


            // retrieve all current diary entries
            entriesCurrentDiary = currentDiary.getEntryMap();

            // add entry to current Entry list from current diary
            entriesCurrentDiary.remove(entry.getPage());


            currentDiary.setEntryMap(entriesCurrentDiary);
            m_diaryManager->updateDiariesMap_withNewEntries(currentDiary);

        } else {
            throw std::runtime_error("m_diaryManager pointer is null!");
        }

        // write all diaries, with the info  for the current diary, to persistence
        m_filePersistence->writeDiaries(m_diaryManager->getDiaryMap());
    }


    int EntryManager::getNextPage(const QString& m_currentDiary, int m_currentUserID){
        // set current diary
        Common::Diary currentDiary = m_diaryManager->getCurrentDiary(m_currentDiary, m_currentUserID);


        // retrieve all current diary entries
        entriesCurrentDiary = currentDiary.getEntryMap();

        int newNumber = entriesCurrentDiary.isEmpty() ? 0 : entriesCurrentDiary.lastKey() + 1;

        return newNumber;

    }



    QMap<int,Common::Entry> EntryManager::getCurrentDiaryEntries(){

        return EntryManager::entriesCurrentDiary;

    }


    const Common::Entry EntryManager::loadEntry(const QString& diaryName,int m_currentUserID, int diaryPage){

        Common::Entry returnEntry;

        Common::Diary currentDiary = m_diaryManager->getCurrentDiary(diaryName, m_currentUserID);


        for (const auto& entry : currentDiary.getEntryMap()) {
            if (entry.getUserID() == m_currentUserID && entry.getPage() == diaryPage) {
                returnEntry = entry;
                return returnEntry;
            }
        }

        return returnEntry;

    }

    void EntryManager::writeTopics(const QStringList& topicList){
        m_filePersistence->writeTopics(topicList);
    }

    const QStringList EntryManager::loadTopicList(){
        return m_filePersistence->loadTopicListFromFile();
    }


    const QString EntryManager::travelContinentChoice(const QString& decision, const QStringList& continents){
        QString selectedMap;
        switch(continents.indexOf(decision))
            {
                case 0:
                    return selectedMap = ":/Images/Images/asia.jpg";
                case 1:
                    return selectedMap = ":/Images/Images/africa.jpg";
                case 2:
                    return selectedMap = ":/Images/Images/northAmerica.jpg";
                case 3:
                    return selectedMap = ":/Images/Images/southAmerica.jpg";
                case 4:
                    return selectedMap = ":/Images/Images/europe.jpg";
                case 5:
                    return selectedMap = ":/Images/Images/australia.jpg";
                default:
                    qDebug() << "Invalid map selected.";
                    return selectedMap;
        }
    }

    void EntryManager::deleteImageFile(const QString& filePath){
        m_filePersistence->deleteImageFile(filePath);
    }

}
