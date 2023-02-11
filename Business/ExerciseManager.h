#ifndef EXERCISEMANAGER_H
#define EXERCISEMANAGER_H
#include <QString>
#include "Persistence/IPersistence.h"
#include "Business/IBusiness.h"

/* Handels all actions related to the exercises of the MentalTrainer funcionality.
 * Intermediates between persistence layer and presentation layer via the business interface IBusiness.
 *
 * @author Chachulski, Korte, Mathea
 */

namespace Business
{
class ExerciseManager{

private:
    Business::IBusiness* m_business;
    Persistence::IPersistence* m_persistence;
    QString m_exercise;
public:
    ExerciseManager();
    ~ExerciseManager();

    const QString startTraining(QString mood);
};
}

#endif // EXERCISEMANAGER_H
