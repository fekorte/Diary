#include "Diary.h"
#include "Entry.h"
#include <QMap>
#include <QDataStream>

/**
 * @author Chachulski, Korte, Mathea
 */

namespace Common{
    Diary::Diary(){}

    Diary::Diary(QMap<int, Entry> entries, int diaryID, QString diaryName, int userID): m_entries(entries), m_diaryID(diaryID), m_diaryName(diaryName), m_userID(userID){}


    Diary::~Diary(){

    }
    const QMap<int, Entry>& Diary::getEntryMap() const {
        return m_entries;
    }

    const QString& Diary::getDiaryName() const{
        return m_diaryName;
    }

    const int& Diary::getDiaryID() const{
        return m_diaryID;
    }

    const int& Diary::getUserID() const {
        return m_userID;
    }


    void Diary::setEntryMap(QMap<int, Common::Entry> entriesCurrentDiary){
        m_entries = entriesCurrentDiary;
    }

    QDataStream &operator<<(QDataStream &out, const Diary &diary){
        out << diary.getEntryMap() << diary.getDiaryID() << diary.getDiaryName() << diary.getUserID();
        return out;
    }

    QDataStream &operator>>(QDataStream &in, Diary &diary){
        QMap<int, Common::Entry> entries;
        int id;
        QString diaryName;
        int userID;

        in >> entries >> id >> diaryName >> userID;
        diary = Diary(entries, id, diaryName, userID);
        return in;
    }

}
