#ifndef User_h
#define User_h
#include <QDataStream>
#include <QString>

/*
 * @author Chachulski, Korte, Mathea
 */

namespace Common
{
	class User
	{
        private:
        int m_userId;
        QString m_email;
        QString m_userName;
        QString m_password;

        public:
        User();
        User(int userId, QString email, QString userName, QString password);
        const int& getUserId() const;
        const QString& getEmail() const;
        const QString& getUserName() const;
        const QString& getPassword() const;
	};
    QDataStream &operator<<(QDataStream &out, const User &user);
    QDataStream &operator>>(QDataStream &in, User &user);
}

#endif
