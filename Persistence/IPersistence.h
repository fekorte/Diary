#ifndef IPersistence_h
#define IPersistence_h

#include "Common/Entry.h"
#include "Common/User.h"
#include "Common/Diary.h"
#include <QMap>
#include <QString>

/**
 * General Interface for access to a storage medium (file or database)
 * Use cases include adding, deleting data of Diaries, Entries and Users.
 *
 * Classes who need a persistence interface can implement it.
 *
 * @author Chachulski, Korte, Mathea
 */

namespace Persistence{

class IPersistence{

public:
    virtual ~IPersistence(){}

    //Diary related functions
    virtual QMap<int, Common::Diary> readDiaries() const = 0;
    virtual void writeDiaries(const QMap<int, Common::Diary>& diaryMap) = 0;

    //User related functions
    virtual void writeUserMapToFile(const QMap<int, Common::User>& users)= 0;
    virtual QMap<int, Common::User> readUserMapFromFile() const = 0;

    //Entry related functions
    virtual QStringList loadTopicListFromFile() const = 0;
    virtual void writeTopics(const QStringList topicList) = 0;
    virtual void writeMoodTrackingData(QStringList& trackingDataList) = 0;
    virtual const QList<QStringList> loadSavedTrackingData(const QString& diaryName) = 0;
    virtual const QString saveImageFile(const QString& filePath) = 0;
    virtual void deleteImageFile(const QString& filePath) = 0;
    };
}

#endif



