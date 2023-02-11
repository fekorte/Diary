#include "travelmapview.h"
#include "ui_travelmapview.h"
#include <QMessageBox>
#include <QPainter>
#include <QFont>
#include <QPoint>
#include <QImageReader>
#include <QImage>
#include <QMouseEvent>
#include <QDateTime>


TravelMapView::TravelMapView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TravelMapView)
{
    ui->setupUi(this);
    QObject::connect(ui->pushButton, &QPushButton::clicked, this, &TravelMapView::onButtonPressed);
    m_mousePressed = false;
}

TravelMapView::~TravelMapView()
{
    delete ui;
    ui = nullptr;
}

void TravelMapView::onButtonPressed()
{
    if(m_mousePressed){
        QString unique = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
        QString filePath = "Data/" + unique + ".png";
        m_newPixmap.save(filePath, "PNG"); //the newly created image is being saved directly in the Data/ folder in the build folder
        m_pathToMap = filePath;

        emit returnMap(m_pathToMap);

    } else {
        this->close();
    }
}

void TravelMapView::mousePressEvent(QMouseEvent *event) {
    if(event->buttons() == Qt::LeftButton) {
        m_mousePressed = true;

        m_newPixmap = m_pixmap; //load unpainted image in the pixmap that will become painted

        QPoint pos = ui->label->mapFromGlobal(QCursor::pos());
        QPainter p(&m_newPixmap);
        p.setRenderHint(QPainter::Antialiasing, true);
        p.setBrush(Qt::red);
        p.drawEllipse(QPoint(pos.x(), pos.y()), 8, 8);

        ui->label->setPixmap(m_newPixmap);
    }
}


bool TravelMapView::loadImage(const QString& path){
    QImageReader reader(path);
    const QImage img(reader.read());

    if(img.isNull()) {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(), "Could not open image");
        return false;
    } else {
        m_pixmap = (QPixmap::fromImage(img));
        m_pixmap = m_pixmap.scaled(800, 800, Qt::KeepAspectRatio);
        ui->label->setScaledContents(true);
        ui->label->setPixmap(m_pixmap);
        ui->label->resize(m_pixmap.size());
        this->resize(m_pixmap.size());
        ui->label->setMouseTracking(true);
        return true;
    }
}
