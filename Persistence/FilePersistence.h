#ifndef FilePersistence_h
#define FilePersistence_h

#include "IPersistence.h"
#include "Common/Entry.h"
#include "Common/User.h"
#include "Common/Diary.h"
#include <QMap>
#include <QString>
#include <fstream>
#include <ostream>

/*
 * @author Chachulski, Korte, Mathea
 */

namespace Persistence{

class FilePersistence : public IPersistence{

public:
    FilePersistence();
    ~FilePersistence();

    //Diary related functions
    void writeDiaries(const QMap<int, Common::Diary>& diaryMap) override;
    QMap<int, Common::Diary> readDiaries() const override;

    //User related functions
    void writeUserMapToFile(const QMap<int, Common::User>& users) override;
    QMap<int, Common::User> readUserMapFromFile() const override;

    //Entry related functions
    QStringList loadTopicListFromFile() const override;
    void writeTopics(const QStringList topicList) override;
    void writeMoodTrackingData(QStringList& trackingDataList) override;
    const QList<QStringList> loadSavedTrackingData(const QString& diaryName) override;
    const QString saveImageFile(const QString& filePath) override;
    void deleteImageFile(const QString& filePath) override;
};
}

#endif






