#include "registerview.h"
#include "ui_registerview.h"
#include "Business/IBusiness.h"
#include "loginview.h"
#include <exception>
#include <QMessageBox>
#include <QString>

/**
 * @author Chachulski, Korte, Mathea
 */


RegisterView::RegisterView(Business::IBusiness* b, QWidget* parent) :
    QFrame(parent),
    ui(new Ui::RegisterView), m_b(b){
    ui->setupUi(this);

    QObject::connect(ui->commandLinkButton,&QCommandLinkButton::clicked,this,&RegisterView::registerNewUser);
    QObject::connect(ui->commandLinkButton_2,&QCommandLinkButton::clicked,this,&RegisterView::showLoginView);
}

RegisterView::~RegisterView(){
    delete ui;
    ui = nullptr;
}

void RegisterView::closeEvent(QCloseEvent *event){
    if (event->spontaneous()){
        delete m_b;
        m_b = nullptr;
    }
    event->accept();
}

void RegisterView::registerNewUser(){
    QString email = ui->lineEdit_5->text();
    QString userName = ui->lineEdit->text();
    QString password = ui->lineEdit_2->text();

    QMessageBox msg;

    try{
        if(m_b->registerUser(email,userName,password)){
            QMessageBox::information(nullptr,"Registration", "Successful registration");
            this->close();
            LoginView* lView = new LoginView(m_b);
            lView->show();
        }
    } catch(std::exception& e) {
        QMessageBox::warning(nullptr,"Registration failed", e.what());
    }
}

void RegisterView::showLoginView(){
    this->close();
    LoginView* lView = new LoginView(m_b);
    lView->show();
}
