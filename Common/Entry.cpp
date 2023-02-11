#include "Entry.h"
#include <QString>
#include <QDataStream>

/**
 * @author Chachulski, Korte, Mathea
 */

namespace Common{

Entry::Entry(){}
Entry::Entry(int userId, int page, QString entryText, QString date, QString diaryName, QStringList topics, QString mood)
    : m_userId(userId), m_page(page), m_entryText(entryText), m_date(date), m_diaryName(diaryName), m_topics(topics), m_mood(mood){}

//constructor which provides the option to save a travel entry with three additional attributes
//a travel entry shall always contain the topic "Travelling" in QStringList m_topics, other entries shall never contain this topic
Entry::Entry(int userId, int page, QString entryText, QString date, QString diaryName,
             QStringList topics, QString mood, QString pathToImgFile, QString pathToMapFile, QString place)
    : m_userId(userId), m_page(page), m_entryText(entryText), m_date(date),
      m_diaryName(diaryName), m_topics(topics), m_mood(mood), m_pathToImgFile(pathToImgFile), m_pathToMapFile(pathToMapFile), m_place(place){}

const int& Entry::getUserID() const{

    return m_userId;
}

const QString& Entry::getDiaryName() const{

    return m_diaryName;
}

const QStringList& Entry::getTopics() const{

    return m_topics;
}

const QString& Entry::getMood() const{

    return m_mood;
}

const QString& Entry::getDate() const{

    return m_date;
}

const int& Entry::getPage() const{

    return m_page;
}


const QString& Entry::getMyNote() const{

    return m_entryText;
}

const QString& Entry::getPathToImgFile() const{

    return m_pathToImgFile;
}

const QString& Entry::getPathToMapFile() const{

    return m_pathToMapFile;
}

const QString& Entry::getPlace() const{

    return m_place;
}

void Entry::setPathToImgFile(const QString& pathToImgFile){
   m_pathToImgFile = pathToImgFile;
}


QDataStream &operator<<(QDataStream &out, const Entry &entry){

    QStringList topicList = entry.getTopics();
    //QByteArray byteArrayTopicList = entry.getTopics().join("|").toUtf8();

    out << entry.getUserID() << entry.getPage() << entry.getMyNote() << entry.getDate() << entry.getDiaryName() << topicList << entry.getMood();

    if(topicList.contains("Travelling")){ //check if topic contains travelling, if yes -> travel entry
        out << entry.getPathToImgFile() << entry.getPathToMapFile() << entry.getPlace();
    }
    return out;
}

QDataStream &operator>>(QDataStream &in, Entry &entry){

    int uID;
    int page;
    QString entryText;
    QString date;
    QString diaryName;
    QStringList topicList;
    QString mood;
    QString pathToImgFile;
    QString pathToMapFile;
    QString place;

    in >> uID>> page >> entryText >> date >> diaryName >> topicList >> mood;

    if(topicList.contains("Travelling")){ //check if topic contains travelling, if yes -> travel entry
        in >> pathToImgFile >> pathToMapFile >> place;
        entry = Entry(uID, page, entryText, date, diaryName, topicList, mood, pathToImgFile, pathToMapFile, place);
    } else {
        entry = Entry(uID, page, entryText, date, diaryName, topicList, mood);
    }
    return in;
}
}


