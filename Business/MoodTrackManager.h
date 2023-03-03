#ifndef MOODTRACKMANAGER_H
#define MOODTRACKMANAGER_H

#include "Persistence/IPersistence.h"
#include <QStringList>

/*
 * @author Chachulski, Korte, Mathea
 */

namespace Business{

class MoodTrackManager{

private:
    Persistence::IPersistence* m_filePersistence;

public:
    MoodTrackManager();
    ~MoodTrackManager();

    void writeMoodTrackingData(QStringList& trackingDataList);
    const QList<QStringList> loadSavedTrackingData(const QString& diaryName);

};
}

#endif // MOODTRACKMANAGER_H
