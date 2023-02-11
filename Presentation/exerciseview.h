#ifndef EXERCISEVIEW_H
#define EXERCISEVIEW_H

#include <QMainWindow>

/*
 * @author Chachulski, Korte, Mathea
 */

namespace Ui {
class ExerciseView;
}

class ExerciseView : public QMainWindow
{
    Q_OBJECT

public:
    explicit ExerciseView(const QString& mood, QWidget *parent = nullptr);
    ~ExerciseView();

private:
    Ui::ExerciseView* ui;
};

#endif // EXERCISEVIEW_H
