#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QScrollArea>
#include <cvimagewidget.h>
#include <QFileDialog>
#include "contourhandler.h"
#include <QInputDialog>
//interface graphique
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setimagewidget(QWidget *wid);

private:
    Ui::MainWindow *ui;
    CVImageWidget* imageWid;
    contourHandler contourHand;
    int lastOp;

public slots:
    void onLoadButton();
    void onContourButton();
    void onContourMultiButton();
    void onAutoHyteresisButton();
    void onAffineButton();
    void onCompletionButton();
    void onSeuilGlobalButton();
    void onSeuilLocalButton();
    void onBaseButton();
    void onChainesButton();
    void onChangeType();
    void onShowPente();
    void onColorGradient();
    void reloadLast();

    void onChangeMaxSpinner();
    void onChangeMinSpinner();
    void setHysteParam();
    void onHysteParam();
};

#endif // MAINWINDOW_H
