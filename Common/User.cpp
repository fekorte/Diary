#include "User.h"
#include <QString>

/**
 * The User class represents a user of the system.
 * Attribute userId is being generated in class UserManager.
 * Class provides stream operator overload for serialization.
 *
 * @author Chachulski, Korte, Mathea
 */

namespace Common {

User::User(){}

User::User(int userId, QString email, QString userName, QString password) //constructor for persistence
    : m_userId(userId), m_email(email), m_userName(userName), m_password(password){
}

const int& User::getUserId() const{
    return m_userId;
}

const QString& User::getEmail() const{
    return m_email;
}

const QString& User::getUserName() const{
    return m_userName;
}

const QString& User::getPassword() const{
    return m_password;
}

QDataStream &operator<<(QDataStream &out, const User &user){
    out << user.getUserId() << user.getEmail() << user.getUserName() << user.getPassword();
    return out;
}

QDataStream &operator>>(QDataStream &in, User &user){
    int id;
    QString email;
    QString userName;
    QString password;
    in >> id >> email >> userName >> password;
    user = User(id, email, userName, password);
    return in;
}
}
