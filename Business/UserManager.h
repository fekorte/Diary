#ifndef USERMANAGER_H
#define USERMANAGER_H

#include "Common/User.h"
#include "Persistence/IPersistence.h"
#include <QString>
#include <QMap>


/*
 * @author Chachulski, Korte, Mathea
 */

namespace Business{

class UserManager{

private:
    Persistence::IPersistence* m_filePersistence;
    Common::User* m_currentUser;

public:

    UserManager();
    ~UserManager();

    bool processLogin(const QString& userName, const QString& password) const;

    bool processRegister(const QString& email, const QString& userName, const QString& password);

    void deleteCurrentUser();

    const Common::User& getCurrentUser();

    QMap<int, Common::User> getCurrentUserMap() const;
};
}
#endif // USERMANAGER_H
