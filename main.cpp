#include <QApplication>
#include "Presentation/loginview.h"
#include "Business/Business.h"

/**
 * Essential Functionalities Implemented
 *
 * @author Chachulski, Korte, Mathea
 */

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    Business::IBusiness* b = new Business::Business();

    LoginView login(b);
    login.show();
    return app.exec();
}
