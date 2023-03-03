#include "MoodTrackManager.h"
#include "Persistence/FilePersistence.h"

/**
 * This code manages the Mood Tracking with Air Quality data (UBAA) from a Bremen Sensor
 *
 * The class has two member functions, "writeMoodTrackingData" and "loadSavedTrackingData",
 * which call the corresponding functions in the "FilePersistence" class to write and load mood tracking data.
 *
 * @author Chachulski, Korte, Mathea
 */

namespace Business{

    MoodTrackManager::MoodTrackManager(){

        m_filePersistence = new Persistence::FilePersistence();
    }

    MoodTrackManager::~MoodTrackManager(){
        delete m_filePersistence;
        m_filePersistence = nullptr;
    }


    void MoodTrackManager::writeMoodTrackingData(QStringList& trackingDataList){

        m_filePersistence->writeMoodTrackingData(trackingDataList);

    }

    const QList<QStringList> MoodTrackManager::loadSavedTrackingData(const QString& diaryName){
        return m_filePersistence->loadSavedTrackingData(diaryName);
    }

}
