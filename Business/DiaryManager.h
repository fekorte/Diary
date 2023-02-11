#ifndef DIARYMANAGER_H
#define DIARYMANAGER_H

#include "Common/Diary.h"
#include "Persistence/IPersistence.h"
#include <QMap>
#include <QString>
#include <iostream>

/*
 * @author Chachulski, Korte, Mathea
 *
 */

namespace Business{

class DiaryManager{

private:

    QMap<int, Common::Diary>* m_diaryMap;

    QMap<int, Common::Diary>* m_userDiaryMap;

    Persistence::IPersistence* m_persistence;

public:
   DiaryManager();
   ~DiaryManager();

    bool processCreateDiary(const QString& diaryName, int userID);

    bool deleteDiary(const QString& diaryName);

    void updateDiaryMap();

    const Common::Diary getDiary(const QString& diaryName);

    const Common::Diary& getCurrentDiary(const QString& diaryName, int userID);

    QMap<int, Common::Diary> updateDiariesMap_withNewEntries(const Common::Diary& currentDiary);

    void updateUserDiaryMap(int userID);

    const QMap<int, Common::Diary>& getUserDiaryMap(int userID);

    const QMap<int, Common::Diary>& getDiaryMap();
};
}

#endif // DIARYMANAGER_H
