#include "feature_scrollarea/features_scrollarea_view.h"

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QMessageBox>

#include <stdio.h>

#include "../base/monitor.h"
#include "base/ddcui_globals.h"
#include "base/debug_utils.h"

#include "nongui/ddc_error.h"

#include "features_scrollarea_contents.h"


bool dimensionReportShown = false;

FeaturesScrollAreaView::FeaturesScrollAreaView(
        Monitor *          monitor,
        FeatureBaseModel * model,
        QStackedWidget *   centralStackedWidget,
        QObject *          parent)
    : QObject(parent)
{
    _cls  = metaObject()->className();

    _monitor = monitor;
    _centralStackedWidget = centralStackedWidget;
    _baseModel = model;

}


// static   // unused
QSizePolicy pageSizePolicy() {
    QSizePolicy policy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);
    policy.setHorizontalStretch(1);
    policy.setVerticalStretch(0);
    policy.setHeightForWidth(false);
    return policy;
}



void FeaturesScrollAreaView::onEndInitialLoad(void) {
   printf("(FeatuesScrollAreaView::%s) Starting\n", __func__);  fflush(stdout);
   if (_monitor->_curFeaturesView != Monitor::FEATURES_VIEW_SCROLLAREA_VIEW) {
      printf("(%s::%s) Not FEATURES_VIEW_SCROLLAREA, skipping\n", _cls, __func__);  fflush(stdout);
      return;
   }


    // QStackedWidget *   _centralStackedWidget;
    // FeatureBaseModel * _baseModel;

    QScrollArea * scrollArea = new QScrollArea();
    FeaturesScrollAreaContents * scrollAreaContents = new FeaturesScrollAreaContents();
    // no effect:
    // scrollAreaContents->setSizePolicy(pageSizePolicy());

    // screws things up, value block forced right and truncated
    // scrollAreaContents->setMinimumSize(QSize(900,0));

    scrollAreaContents->setObjectName("scrollAreaContents local to onEndInitialLoad");
    QVBoxLayout * vLayout  = new QVBoxLayout();
    vLayout->setObjectName("vLayout in onEndInitLoad");
    vLayout->setMargin(0);
    vLayout->setSpacing(0);    // <=== CONTROLS SPACING BETWEEN ROWS
    if (debugLayout) {
       scrollAreaContents->setStyleSheet("background-color:chartreuse;");
       _centralStackedWidget->setStyleSheet("background-color:chocolate:");
    }

    int ct = 0;
    for (int feature_code = 0; feature_code < 256; feature_code++) {
         FeatureValue * fv =  _baseModel->modelVcpValueFilteredFind(feature_code);
         if (fv) {
             FeatureWidgetBasic * w = new FeatureWidgetBasic();
             if (debugLayout) {
                // gets applied to the widgets inside w, not to w itself
                w->setStyleSheet("background-color:brown;");
             }
             w->setFeatureValue(*fv);
             QObject::connect(w ,   &FeatureWidgetBasic::valueChanged,
                              this, &FeaturesScrollAreaView::onUIValueChanged);
             vLayout->addWidget(w);
             _widgets[feature_code] = w;
             ct++;
         }
    }

    // printf("Calling scrollAreaContents->setLayout()\n"); fflush(stdout);
    scrollAreaContents->setLayout(vLayout);
    // printf("Calling scrollArea->setWidget()\n"); fflush(stdout);
    scrollArea->setWidget(scrollAreaContents);

    // QObject::connect(_baseModel,   &FeatureBaseModel::signalFeatureUpdated,
    //                  scrollAreaContents, &FeaturesScrollAreaContents::featureUpdated);

    QObject::connect(_baseModel, &FeatureBaseModel::signalFeatureUpdated3,
                     this,       &FeaturesScrollAreaView::onModelValueChanged);



    // printf("Calling _centralStackedWidget->addWidget()\n"); fflush(stdout);
    /* int pageno = */ _centralStackedWidget->addWidget(scrollArea);
    // _centralStackedWidget->hide();    // no effect
    // _centralStackedWidget->setCurrentIndex(pageno);
    _centralStackedWidget->setCurrentWidget(scrollArea);


    if (!dimensionReportShown && debugLayout) {
       printf("-------------------------------------------->\n"); fflush(stdout);
       reportWidgetDimensions(scrollAreaContents, _cls, __func__);
        printf("-------------------------------------------->\n"); fflush(stdout);
        reportWidgetDimensions(_centralStackedWidget, _cls, __func__);
        // dimensionReportShown = true;
    }

    _centralStackedWidget->show();
    printf("(FeatuesScrollAreaView::%s) Done.  feature count: %d\n", __func__, ct);  fflush(stdout);
}

void FeaturesScrollAreaView::onUIValueChanged(uint8_t featureCode, uint8_t sh, uint8_t sl) {
   printf("(%s::%s) feature_code = 0x%02x, sh=0x%02x, sl=0x%02x\n", _cls, __func__,
          featureCode, sh, sl); fflush(stdout);
   VcpRequest * rqst = new VcpSetRequest(featureCode, sl);   // n.b. ignoring sh
   emit signalVcpRequest(rqst);  // used to call into monitor
}


void FeaturesScrollAreaView::onModelValueChanged(
      uint8_t featureCode, uint8_t sh, uint8_t sl)
{
   printf("(%s::%s) feature_code = 0x%02x, sh=0x%02x, sl=0x%02x\n", _cls, __func__,
          featureCode, sh, sl); fflush(stdout);

   // find the entry in _widgets
   FeatureWidgetBasic * curWidget = _widgets[featureCode];

   // set value in the widget
   uint16_t newval = sh << 8 | sl;
   curWidget->setCurrentValue(newval);
}


void FeaturesScrollAreaView::onModelDdcError(DdcError erec) {
   printf("(%s::%s) erec=%s\n", _cls, __func__, erec.srepr() );    fflush(stdout);
   QMessageBox * msgBox = new QMessageBox();
   // msgBox.setText("Oy");
   QString detail = erec.repr();
   // detailed text adds details button
   // msgBox.setDetailedText(detail);
   // how to position over application?
   msgBox->setText(detail);
   msgBox->setIcon(QMessageBox::Warning);
   msgBox->setModal(true);
   msgBox->setWindowTitle("Error Setting Feature Value");
   msgBox->exec();
   // msgBox.open();
}





