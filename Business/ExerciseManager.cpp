#include "ExerciseManager.h"

/*
 * @author Chachulski, Korte, Mathea
 */



namespace Business{

    ExerciseManager::ExerciseManager(){}
    ExerciseManager::~ExerciseManager(){
        delete m_business;
        m_business = nullptr;
        delete m_persistence;
        m_persistence = nullptr;
    }

    const QString ExerciseManager::startTraining(QString mood){

        return m_persistence->readExercise(mood);
    }
}


