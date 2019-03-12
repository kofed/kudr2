#include "imageviewer.h"
#include <QScrollBar>
#include <QVBoxLayout>
#include "lib/selectiveImageLabel.h"
#include <QEvent>

ImageViewer::ImageViewer()
    :ImageViewer(new QLabel)

{
}

ImageViewer::ImageViewer(QLabel * _imageLabel)
    :imageLabel(_imageLabel)
    , scrollArea(new QScrollArea(this))
    , scaleFactor(1)
{

    imageLabel->setBackgroundRole(QPalette::Base);
  //  imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    imageLabel->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    imageLabel->setScaledContents(true);


    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(imageLabel);
    scrollArea->setVisible(false);

    imageLabel->installEventFilter(this);
    scrollArea->viewport()->installEventFilter(this);
    scrollArea->installEventFilter(this);
       //  connect(scrollArea, SIGNAL(wheelEvent(QWheelEvent*)), this, SLOT(wheelEvent(QWheelEvent*)));
  //  QHBoxLayout* scrollLayout = new QHBoxLayout;
 //   scrollLayout->addWidget(imageLabel);
 //   scrollArea->setLayout(scrollLayout);
    QVBoxLayout * layout = new QVBoxLayout();
   // layout->addWidget(imageLabel);
    layout->addWidget(scrollArea);
    setLayout(layout);

}

void ImageViewer::setImage(const QPixmap & image){
    if (image.isNull()) {
        throw new std::runtime_error("Image is null");
    }


    scaleFactor = 1.0;
    scrollArea->setVisible(true);

  //  fitToWindowAct->setEnabled(true);
    //if (!fitToWindowAct->isChecked())
    imageLabel->setPixmap(image);
    imageLabel->show();
    imageLabel->adjustSize();

    //imageLabel->resize(imageLabel->pixmap()->size());//If the Fit to Window option is turned off*/
}

void ImageViewer::updateImage(const QPixmap & image){
    if (image.isNull()) {
        throw new std::runtime_error("Image is null");
    }
    imageLabel->setPixmap(image);
    imageLabel->show();
}

void ImageViewer::zoomIn()
{
    scaleImage(1.1);
}

void ImageViewer::zoomOut()
{
    scaleImage(0.9);
}

void ImageViewer::normalSize()
{
    imageLabel->adjustSize();
    scaleFactor = 1.0;
}

void ImageViewer::fitToWindow()
{
    bool fitToWindow = true;
    scrollArea->setWidgetResizable(fitToWindow);
    if (!fitToWindow)
        normalSize();
}

void ImageViewer::scaleImage(double factor)
{
    Q_ASSERT(imageLabel->pixmap());
    scaleFactor *= factor;
    imageLabel->resize(scaleFactor * imageLabel->pixmap()->size());

    adjustScrollBar(scrollArea->horizontalScrollBar(), factor);
    adjustScrollBar(scrollArea->verticalScrollBar(), factor);

    emit scaleFactorChanged(scaleFactor);
}

void ImageViewer::adjustScrollBar(QScrollBar *scrollBar, double factor)
{
    scrollBar->setValue(int(factor * scrollBar->value()
                            + ((factor - 1) * scrollBar->pageStep()/2)));
}

void ImageViewer::wheelEvent(QWheelEvent *event){
    int i = event->angleDelta().y() / 40;

    while(i > 0){
        zoomIn();
        --i;
    }

    while(i < 0){
       zoomOut();
       ++i;
    }

    event->accept();
}
