#ifndef Diary_h
#define Diary_h

#include "Entry.h"
#include <QMap>
#include <QDataStream>

/*
 * @author Chachulski, Korte, Mathea
 */

namespace Common{

class Diary{

private:
    QMap<int, Entry> m_entries; // key (int) is page number in the Diary (value)
    int m_diaryID;
    QString m_diaryName;
    int m_userID;

public:
    Diary();
    Diary(QMap<int, Entry> entries, int diaryID, QString diaryName, int userID);
    ~Diary();

    const QMap<int, Entry>& getEntryMap() const;

    const int& getDiaryID() const;

    const QString& getDiaryName() const;

    const int& getUserID() const;

    void setEntryMap(QMap<int, Common::Entry> entriesCurrentDiary);

};

QDataStream &operator<<(QDataStream &out, const Diary &diary);
QDataStream &operator>>(QDataStream &in, Diary &diary);

}

#endif
