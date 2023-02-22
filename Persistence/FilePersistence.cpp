#include "Common/User.h"
#include "Common/Entry.h"
#include "Common/Diary.h"
#include "IPersistence.h"
#include "FilePersistence.h"
#include <QString>
#include <QDataStream>
#include <QFile>
#include <QFileInfo>
#include <QDebug>
#include <iostream>
#include <fstream>
#include <sstream>
#include <QMap>

/**
 * Handels read and write access to a filesystem.
 *
 * @author Chachulski, Korte, Mathea
 */

namespace Persistence {

FilePersistence::FilePersistence(){}
FilePersistence::~FilePersistence(){}


void FilePersistence::writeDiaries(const QMap<int, Common::Diary>& diaryMap) {

    QString fileName = "Data/Diaries.bin";
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly))
    {
        qDebug() << "Could not open " << fileName;
    }
    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_5_1);
    out << diaryMap;
    file.flush();
    file.close();
}

QMap<int, Common::Diary> FilePersistence::readDiaries() const{

    QString fileName = "Data/Diaries.bin";
    QFile file(fileName);
    QMap<int, Common::Diary> diaryMap;

    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Could not open " << fileName;
        return diaryMap;
    }

    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_5_1);
    in >> diaryMap;
    file.close();
    return diaryMap;
}

QStringList FilePersistence::loadTopicListFromFile() const{

    QString fileName = "Data/Topics.bin";
    QFile file(fileName);
    QStringList topicList;

    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Could not open " << fileName;
        return topicList;
    }

    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_5_1);
    in >> topicList;
    file.close();
    return topicList;
}

void FilePersistence::writeTopics(const QStringList topicList) {

    QString fileName = "Data/Topics.bin";
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly))
    {
        qDebug() << "Could not open " << fileName;
    }
    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_5_1);
    out << topicList;
    file.flush();
    file.close();
}


void FilePersistence::writeMoodTrackingData(QStringList& trackingDataList) {


    QFile file("Data/MoodTracking.txt");
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream stream(&file);
        int count = 0;
        for (const QString &line : trackingDataList) {
            stream << line;
            if (++count % 6 == 0) {
                //outcommented line because my (Felina Korte) QTVersion 5.12.8 does not support it
                //stream << Qt::endl;
                stream << "\n";
            } else {
                stream << ", ";
            }
        }
        file.close();
    }
}


const QList<QStringList> FilePersistence::loadSavedTrackingData(const QString& diaryName){

    QList<QStringList> result;

    QFile file("Data/MoodTracking.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        while (!stream.atEnd()) {
            QString line = stream.readLine();
            QStringList entries = line.split(", ");
            QStringList filteredEntries;
            for (const QString &entry : entries) {
                if (entry.startsWith(diaryName)) {
                    filteredEntries << entries;
                }
            }
            if (!filteredEntries.isEmpty()) {
                result << filteredEntries;
            }
        }
        file.close();
    }
    return result;

}


void FilePersistence::writeUserMapToFile(const QMap<int, Common::User>& users){

    QString fileName = "Data/User.bin";
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly))
    {
        qDebug() << "Could not open " << fileName;
        return;
    }
    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_5_1);
    out << users;
    file.flush();
    file.close();
}

QMap<int, Common::User> FilePersistence::readUserMapFromFile() const{

    QString fileName = "Data/User.bin";
    QFile file(fileName);
    QMap<int, Common::User> userMap;

    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Could not open " << fileName;
        return userMap;
    }

    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_5_1);
    in >> userMap;
    file.close();
    return userMap;
}

const QString FilePersistence::saveImageFile(const QString& filePath){

    if (QFile::exists(filePath) && filePath.contains("Data/")){
        return filePath;
    }

    QFileInfo fileInfo(filePath);
    QString fileExtension = fileInfo.suffix();
    QString newFile = QDateTime::currentDateTime().toString("yyyyMMddhhmmss") + "." + fileExtension;

    bool success = QFile::copy(filePath, "Data/" + newFile);

    if(!success){
        qDebug()<<"Copy failed";
    }

    return "Data/" + newFile;
}

void FilePersistence::deleteImageFile(const QString& filePath){

    if(!filePath.isEmpty() && QFile::exists(filePath) && filePath.contains("Data/")){
        QFile toDelete(filePath);
        if (!toDelete.remove()){
            qDebug() << "Error removing file.";
        }
    }
}
}
