#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "iostream"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    contourHand(),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->loadButton->setToolTip("Choisir image...");
    connect(ui->loadButton, SIGNAL(released()), this, SLOT(onLoadButton()));
    connect(ui->contoursButton, SIGNAL(released()), this, SLOT(onContourButton()));
    connect(ui->hystAutoButton, SIGNAL(released()), this, SLOT(onAutoHyteresisButton()));
    connect(ui->affineButton, SIGNAL(released()), this, SLOT(onAffineButton()));
    connect(ui->completeButton, SIGNAL(released()), this, SLOT(onCompletionButton()));
    connect(ui->seuilGlobalButton, SIGNAL(released()), this, SLOT(onSeuilGlobalButton()));
    connect(ui->seuilLocalButton, SIGNAL(released()), this, SLOT(onSeuilLocalButton()));
    connect(ui->baseButton, SIGNAL(released()), this, SLOT(onBaseButton()));
    connect(ui->typeFilterBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onChangeType()));
    connect(ui->chainesButton, SIGNAL(released()), this, SLOT(onChainesButton()));
    connect(ui->penteButton, SIGNAL(released()), this, SLOT(onShowPente()));
    connect(ui->contoursMultiButton, SIGNAL(released()), this, SLOT(onContourMultiButton()));
    connect(ui->colorGradientButton, SIGNAL(released()), this, SLOT(onColorGradient()));
    connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(onChangeMaxSpinner()));
    connect(ui->horizontalSlider_2, SIGNAL(valueChanged(int)), this, SLOT(onChangeMinSpinner()));
    connect(ui->hysteSetButton, SIGNAL(released()), this, SLOT(onHysteParam()));

    ui->typeFilterBox->addItem(tr("Prewitt"));
    ui->typeFilterBox->addItem(tr("Sobel"));
    lastOp=-1;

    ui->hystHeadLabel->setText("Hystéresis paramétrée:");
    ui->hystHeadLabel->setAlignment(Qt::AlignCenter);

    ui->horizontalSlider->setMaximum(254);
    ui->horizontalSlider->setMinimum(11);
    ui->horizontalSlider->setValue(110);
    ui->maxlabel->setText(QString::number(110)+"(Haut)");

    ui->horizontalSlider_2->setMaximum(254);
    ui->horizontalSlider_2->setMinimum(1);
    ui->horizontalSlider_2->setValue(10);
    ui->minlabel->setText(QString::number(10)+"(Bas)");
    setHysteParam();

    imageWid = new CVImageWidget();
    ui->imageWidget = imageWid;
    ui->scrollArea->setWidget(ui->imageWidget);
    ui->scrollArea->setAlignment(Qt::AlignCenter);
}


void MainWindow::onChangeMaxSpinner(){
    ui->maxlabel->setText(QString::number(ui->horizontalSlider->value())+"(Haut)");
    if(ui->horizontalSlider->value()<= ui->horizontalSlider_2->value()){
        ui->horizontalSlider_2->setValue(ui->horizontalSlider_2->value()-1);
    }
    setHysteParam();
}

void MainWindow::onChangeMinSpinner(){
    ui->minlabel->setText(QString::number(ui->horizontalSlider_2->value())+"(Bas)");
    if(ui->horizontalSlider_2->value()>= ui->horizontalSlider->value()){
        ui->horizontalSlider->setValue(ui->horizontalSlider_2->value()+1);
    }
    setHysteParam();
}

void MainWindow::onHysteParam(){
    imageWid->showImage(contourHand.customHysteresis());
    lastOp = 10;
}

void MainWindow::setHysteParam(){
    contourHand.seuilBas = ui->horizontalSlider_2->value();
    contourHand.seuilHaut = ui->horizontalSlider->value();
}

void MainWindow::setimagewidget(QWidget *wid){
    ui->imageWidget = wid;

    ui->scrollArea->setWidget(ui->imageWidget);
    ui->scrollArea->setAlignment(Qt::AlignCenter);
    //ui->imagewidget_2->resize(300, 300);
    //ui->imageWidget->repaint();
    //qApp->processEvents();
}

void MainWindow::onLoadButton() {
    QString fileName = QFileDialog::getOpenFileName(this,
            tr("Ouvrir image"), "",
            tr("Jpeg (*.jpg);;Png (*.png);;All Files (*)"));
    if(!fileName.isEmpty()){
        contourHand.loadImage(fileName.toStdString());
        imageWid->showImage(contourHand.renderBase());
        lastOp = 0;
    }
}

void MainWindow::onBaseButton(){
    imageWid->showImage(contourHand.renderBase());
    lastOp = 1;
}

void MainWindow::onContourButton(){
    imageWid->showImage(contourHand.calculContoursMulti());
    lastOp = 8;
}

void MainWindow::onContourMultiButton(){
    imageWid->showImage(contourHand.calculContours());
    lastOp = 2;
}

void MainWindow::onAutoHyteresisButton(){
    imageWid->showImage(contourHand.autoHyste());
    lastOp = 3;
}

void MainWindow::onAffineButton(){
    imageWid->showImage(contourHand.affine());
    lastOp = 4;
}

void MainWindow::onCompletionButton(){
    imageWid->showImage(contourHand.completeContours());
    lastOp = 5;
}

void MainWindow::onSeuilGlobalButton(){
    bool ok;
    QStringList list;
    list.append("Ecart-type");
    list.append("Moyenne");
    QString item = QInputDialog::getItem(this, tr("Choix seuil"), tr("Type seuil:"), list, 0, false, &ok);

    if (ok && !item.isEmpty()) {

        if(item.compare("Ecart-type") == 0) imageWid->showImage(contourHand.seuilGlobal(0));
        else {
            imageWid->showImage(contourHand.seuilGlobal(1));
            //std::cout << "test" << std::endl;
        }
    }
}

void MainWindow::onChangeType(){
    //std::cout << "index : " << ui->typeFilterBox->currentIndex() << std::endl;
    int j = ui->typeFilterBox->currentIndex();
    Filter t = Prewitt;
    if(j == 1) t = Sobel;
    contourHand.fil = t;
    contourHand.contoursdone =false;
    contourHand.hystedone = false;
    contourHand.thindone = false;
    reloadLast();
}

void MainWindow::onSeuilLocalButton(){
    bool ok;
    int j = QInputDialog::getInt(this, tr("Seuil Local"), tr("Voisinage:"), 3, 3, 33, 1, &ok);
    if(ok) imageWid->showImage(contourHand.seuilLocal(j));
}

void MainWindow::onChainesButton(){
    imageWid->showImage(contourHand.chainageContour());
    lastOp = 6;
}

void MainWindow::onShowPente(){
    imageWid->showImage(contourHand.showPenteColor());
    lastOp = 7;
}


void MainWindow::onColorGradient(){
    imageWid->showImage(contourHand.colorGradient());
    lastOp = 9;
}

void MainWindow::reloadLast(){
    switch (lastOp) {
    case 1:
        onBaseButton();
        break;
    case 2:
        onContourButton();
        break;
    case 3:
        onAutoHyteresisButton();
        break;
    case 4:
        onAffineButton();
        break;
    case 5:
        onCompletionButton();
        break;
    case 6:
        onChainesButton();
        break;
    case 7:
        onShowPente();
        break;
    case 8:
        onContourMultiButton();
        break;
    case 9:
        onColorGradient();
        break;
    case 10:
        onHysteParam();
        break;
    default:
        break;
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
