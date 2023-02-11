#ifndef REGISTERVIEW_H
#define REGISTERVIEW_H

#include <QFrame>
#include <QCloseEvent>
#include "Business/IBusiness.h"

/*
 * @author Chachulski, Korte, Mathea
 */

namespace Ui {
class RegisterView;
}

class RegisterView : public QFrame{
    Q_OBJECT

public:
    explicit RegisterView(Business::IBusiness* b, QWidget *parent = nullptr);
    ~RegisterView();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void registerNewUser();
    void showLoginView();

private:
    Ui::RegisterView* ui;
    Business::IBusiness* m_b;
};

#endif // REGISTERVIEW_H
