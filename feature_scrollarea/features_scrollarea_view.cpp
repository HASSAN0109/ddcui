#include "feature_scrollarea/features_scrollarea_view.h"

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QMessageBox>

#include <stdio.h>

#include "base/vertical_scroll_area.h"
#include "base/monitor.h"
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


void FeaturesScrollAreaView::onEndInitialLoad(void) {
   printf("(FeatuesScrollAreaView::%s) Starting\n", __func__);  fflush(stdout);
   if (_monitor->_curFeaturesView != Monitor::FEATURES_VIEW_SCROLLAREA_VIEW) {
      printf("(%s::%s) Not FEATURES_VIEW_SCROLLAREA, skipping\n", _cls, __func__);  fflush(stdout);
      return;
   }

    QScrollArea * scrollArea = new QScrollArea();
    // causes extra space between C/NC/T column and value column
    // VerticalScrollArea * scrollArea = new VerticalScrollArea();

    FeaturesScrollAreaContents * scrollAreaContents = new FeaturesScrollAreaContents();
    scrollAreaContents->setObjectName("scrollAreaContents local to onEndInitialLoad");

    // no effect, only applies if no layout is set
    // scrollAreaContents->setSizePolicy(pageSizePolicy());

    // screws things up, value block forced right and truncated
    // scrollAreaContents->setMinimumSize(QSize(900,0));

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

    scrollAreaContents->setLayout(vLayout);
    scrollArea->setWidget(scrollAreaContents);

    // QObject::connect(_baseModel,   &FeatureBaseModel::signalFeatureUpdated,
    //                  scrollAreaContents, &FeaturesScrollAreaContents::featureUpdated);

    QObject::connect(_baseModel, &FeatureBaseModel::signalFeatureUpdated3,
                     this,       &FeaturesScrollAreaView::onModelValueChanged);


    /* int pageno = */ _centralStackedWidget->addWidget(scrollArea);
    // _centralStackedWidget->hide();    // no effect
    _centralStackedWidget->setCurrentWidget(scrollArea);

    if (!dimensionReportShown && debugLayout) {
        printf("(%s::%s) ---------------------> scrollAreaContents in QScrollArea\n",    _cls, __func__);
        reportWidgetDimensions(scrollAreaContents,    _cls, __func__);
        printf("(%s::%s) ---------------------> QScrollArea in _centralStackedWidget\n", _cls, __func__);
        reportWidgetDimensions(scrollArea,            _cls, __func__);
        printf("(%s::%s) ---------------------> centralStackedWidget\n",                 _cls, __func__);
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





