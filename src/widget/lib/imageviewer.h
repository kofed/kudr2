#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QLabel>
#include <QScrollArea>
#include <QAction>
#include <QImage>
#include <QWheelEvent>

class ImageViewer : public QWidget
{
    Q_OBJECT
public:
    ImageViewer();

    ImageViewer(QLabel * _imageLabel);

    virtual ~ImageViewer(){}

    void setImage(const QPixmap & image);
    void updateImage(const QPixmap & image);

signals:
    void scaleFactorChanged(double scaleFactor);
protected:
    void wheelEvent(QWheelEvent * event);
private:
    QImage image;
    QScrollArea *scrollArea;
    QLabel *imageLabel;
    double scaleFactor;

    void createActions();
    void updateActions();
    void scaleImage(double factor);
    void adjustScrollBar(QScrollBar *scrollBar, double factor);

private slots:
    //void open();
    void zoomIn();
    void zoomOut();
    void normalSize();
    void fitToWindow();
};

#endif // IMAGEVIEWER_H
