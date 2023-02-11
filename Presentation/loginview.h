#ifndef LOGINVIEW_H
#define LOGINVIEW_H

#include "registerview.h"
#include "mainwindow.h"
#include "Business/IBusiness.h"
#include <QFrame>
#include <QCloseEvent>

/*
 * @author Chachulski, Korte, Mathea
 */


namespace Ui {
class LoginView;
}

class LoginView : public QFrame {
    Q_OBJECT

public:
    explicit LoginView(Business::IBusiness* b, QWidget* parent = nullptr);
    ~LoginView();

protected:
    void closeEvent(QCloseEvent* event) override;

private slots:
    void login();
    void showRegisterView();

private:
    Ui::LoginView* ui;
    Business::IBusiness* m_b;
};

#endif // LOGINVIEW_H
