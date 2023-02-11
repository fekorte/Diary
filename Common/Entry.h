#ifndef Entry_h
#define Entry_h

#include <QDate>

/*
 * @author Chachulski, Korte, Mathea
 */

namespace Common{
    class Entry{

    private:
        int m_userId;
        int m_page; // = entryID
        QString m_entryText;
        QString m_date;
        QString m_diaryName;
        QStringList m_topics;
        QString m_mood;
        QString m_pathToImgFile;
        QString m_pathToMapFile;
        QString m_place;

    public:
        Entry();
        Entry(int userId, int page, QString entryText, QString date, QString diaryName, QStringList topic, QString mood);
        Entry(int userId, int page, QString entryText, QString date, QString diaryName, QStringList topic, QString mood, QString pathToImgFile, QString pathToMapFile, QString place);

        const QStringList& getTopics() const;
        const QString& getDate() const;
        const int& getPage() const;
        const QString& getMood() const;
        const QString& getMyNote()const;
        const int& getUserID() const;
        const QString& getDiaryName() const;
        const QString& getPathToImgFile() const;
        const QString& getPathToMapFile() const;
        const QString& getPlace() const;
        void setPathToImgFile(const QString& pathToImgFile);
    };

    QDataStream &operator<<(QDataStream &out, const Entry &entry);
    QDataStream &operator>>(QDataStream &in, Entry &entry);
}

#endif
