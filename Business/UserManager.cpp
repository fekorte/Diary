#include "UserManager.h"
#include "Common/User.h"
#include "Persistence/FilePersistence.h"

/**
 * Handels all actions related to User Objects.
 * Reads and writes user-related data from and to the persistence layer.
 * Saves a pointer to the currently logged in user as a member attribute (m_currentUser).
 *
 * Used by class Business for intermediation between presentation layer and implemented business logic related to User Objects.
 *
 * QMap<int, Common::User> userMap:
 * key: userID, value: User
 *
 * @author Chachulski, Korte, Mathea
 */

namespace Business{

    UserManager::UserManager(){    
        m_filePersistence = new Persistence::FilePersistence();

    }

    UserManager::~UserManager(){
        delete m_filePersistence;
        m_filePersistence = nullptr;
    }


    bool UserManager::processLogin(const QString& userName, const QString& password){
        if(getCurrentUserMap().empty()){
            throw std::invalid_argument("Login failed. Please register first.");
        }

        QMap<int, Common::User> userMap = getCurrentUserMap();
        for (auto it = userMap.begin(); it != userMap.end(); ++it){
            if(it->getUserName() == userName && it->getPassword() == password){
                m_currentUser = it.value();
                return true;
            }
        }
        throw std::invalid_argument("Login failed. User name or password wrong. Please try again.");
    }



   bool UserManager::processRegister(const QString& email, const QString& userName, const QString& password){

        if(email == "" || userName == "" || password == ""){ //check for empty input
            throw std::invalid_argument("Invalid input. Please try again.");
        }

        if(!email.contains("@")){ //check for valid email address (has to contain "@")
            throw std::invalid_argument("Please enter a valid email address.");
        }


        QMap<int, Common::User> userMap = getCurrentUserMap();
        for (auto it = userMap.begin(); it != userMap.end(); ++it){ //no registration if a user with similar email or user name already exists
            if (it->getEmail() == email){
                throw std::invalid_argument("Email address already exists.");
            }
            if (it->getUserName() == userName){
                throw std::invalid_argument("User name already exists.");
            }
        }

      int id = userMap.empty() ? 1 : userMap.last().getUserId()+1; //check if user is the first one to register. If yes, userID is one. If no, userId is the last given userID plus one
      Common::User newUser(id, email, userName, password);
      userMap.insert(newUser.getUserId(), newUser);
      m_filePersistence->writeUserMapToFile(userMap);
      return true;
    }

   void UserManager::deleteCurrentUser(){
       QMap<int, Common::User> userList = getCurrentUserMap();
       userList.remove(m_currentUser.getUserId());
       m_filePersistence->writeUserMapToFile(userList);
   }

   const Common::User& UserManager::getCurrentUser(){
       return m_currentUser;
   }

    QMap<int, Common::User> UserManager::getCurrentUserMap() const{
        return m_filePersistence->readUserMapFromFile();
    }
}
