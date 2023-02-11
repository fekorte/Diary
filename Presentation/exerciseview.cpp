#include "exerciseview.h"
#include "ui_exerciseview.h"
#include <QTextStream>
#include <QFile>

/**
 * A simple display widget for an exercise.
 * Reads a .txt file into a QTextEdit.
 *
 * @author Chachulski, Korte, Mathea
 *
 */

ExerciseView::ExerciseView(QString mood, QWidget* parent) :
    QMainWindow(parent), ui(new Ui::ExerciseView){

    ui->setupUi(this);

    QFile file("Data/" + mood + ".txt");
    if (! file.open(QIODevice::ReadOnly | QIODevice::Text))
            return;
    ui->textEdit->setText(file.readAll());
    file.close();

}

ExerciseView::~ExerciseView()
{
    delete ui;
    ui = nullptr;
}
