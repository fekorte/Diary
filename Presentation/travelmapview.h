#ifndef TRAVELMAPVIEW_H
#define TRAVELMAPVIEW_H

#include <QWidget>

/*
 * @author Chachulski, Korte, Mathea
 */


namespace Ui {
class TravelMapView;
}


class TravelMapView : public QWidget
{
    Q_OBJECT

public:
    explicit TravelMapView(QWidget *parent = nullptr);
    ~TravelMapView();
    bool loadImage(const QString& path);
    void mousePressEvent(QMouseEvent *event) override;

signals:
    void returnMap(const QString& pathToMap);

private slots:
    void onButtonPressed();


private:
    QPixmap m_pixmap;
    QPixmap m_newPixmap;
    QString m_pathToMap;
    bool m_mousePressed;

    Ui::TravelMapView *ui;

};

#endif // TRAVELMAPVIEW_H
