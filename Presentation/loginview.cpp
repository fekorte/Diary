#include "loginview.h"
#include "ui_loginview.h"
#include "Business/IBusiness.h"
#include <exception>
#include <QMessageBox>
#include <QString>

/**
 * LoginView is a class that represents the login view for the application.
 * It inherits from QFrame, to enable the possibility to display a background image.
 *
 * This class takes a pointer to an IBusiness class in its constructor and saves it as an attribute.
 * It uses its pointer to IBusiness to interact with the business logic.
 *
 * Furthermore, the class overrides closeEvent to ensure that the pointer to IBusiness is deleted properly and to avoid memory leaks.
 *
 * @author Chachulski, Korte, Mathea
 */

LoginView::LoginView(Business::IBusiness* b, QWidget* parent) :
    QFrame(parent), ui(new Ui::LoginView), m_b(b){

    ui->setupUi(this);

    QObject::connect(ui->commandLinkButton_2,&QCommandLinkButton::clicked,this,&LoginView::login);
    QObject::connect(ui->commandLinkButton,&QCommandLinkButton::clicked,this,&LoginView::showRegisterView);
}

LoginView::~LoginView(){
    delete ui;
    ui = nullptr;
}

void LoginView::closeEvent(QCloseEvent *event){
    if (event->spontaneous()){
        delete m_b;
        m_b = nullptr;
    }
    event->accept();
}

void LoginView::login(){
    QString userName = ui->lineEdit->text();
    QString password = ui->lineEdit_2->text();

    try{
        if(m_b->login(userName, password)){
            QMap<QString, Common::Diary> myDiaryMap = m_b->getUserDiaryMap(m_b->getCurrentUser().getUserId());
            MainWindow* mView = new MainWindow(m_b, myDiaryMap);
            this->close();
            mView->show();
        }
    } catch(std::exception& e){ //show messagebox, catch exception if input is wrong
        QMessageBox::warning(nullptr,"Login failed", e.what());
    }
}

void LoginView::showRegisterView(){
    this->close();
    RegisterView* rView = new RegisterView(m_b);
    rView->show();
}
