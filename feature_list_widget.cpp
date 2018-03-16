/* feature_list_widget.cpp */

#include <assert.h>
#include <iostream>

#include "nongui/feature_base_model.h"
#include "nongui/feature_value.h"
#include "feature_widget.h"

#include "feature_list_widget.h"

using namespace std;


FeatureListWidget::FeatureListWidget(QWidget * parent):
    QListWidget(parent)
{
    _cls = metaObject()->className();
}


void FeatureListWidget::setModel(FeatureBaseModel * baseModel) {
    printf("(FeatureListWidget::%s)\n", __func__); fflush(stdout);
    _baseModel = baseModel;
}


int FeatureListWidget::findFeatureItem(uint8_t feature_code) {
    int result = -1;

    for (int ndx = 0; ndx < count(); ndx++) {
        FeatureWidget * curItem = (FeatureWidget*) item(ndx);  // ???
        if (curItem->_feature_code == feature_code) {
            result = ndx;
            break;
        }
    }

    return result;
}


FeatureWidget * FeatureListWidget::getFeatureItem(uint8_t feature_code) {
    FeatureWidget * result = NULL;

    for (int ndx = 0; ndx < count(); ndx++) {
        FeatureWidget * curItem = (FeatureWidget *) item(ndx);
        if (curItem->_feature_code == feature_code) {
            result = curItem;
            break;
        }
    }

    return result;
}


void FeatureListWidget::addFeature(FeatureValue * fv) {
    printf("(%s::%s)\n", _cls, __func__); fflush(stdout);
    FeatureWidget * itemWidget = new FeatureWidget();
    itemWidget->setFeatureValue(*fv);
    // printf("(%s::%s) Calling addItem()...\n", _cls, __func__); fflush(stdout);
    addItem(itemWidget);
    // printf("(%s::%s) After addItem()\n", _cls, __func__); fflush(stdout);
}


void FeatureListWidget::updateFeature(FeatureValue * fv) {
    FeatureWidget * itemWidget = getFeatureItem(fv->_feature_code);
    assert(itemWidget);

    uint16_t newval = (fv->_sh << 8) | fv->_sl;
    itemWidget->setCurrentValue(newval);
    // signal paint event?
}


// void modelMccsVersionSet(DDCA_MCCS_Version_Spec    vspec);
// DDCA_MCCS_Version_Spec mccsVersionSpec();

void  FeatureListWidget::startInitialLoad(void) {
    printf("FeatureListWidget::%s)\n", __func__); fflush(stdout);

    // setUpdatesEnabled(true); // no effect
    // show();                  // no effect

}

void   FeatureListWidget::endInitialLoad(void) {
    printf("(FeatureListWidget::%s)\n", __func__); fflush(stdout);
    // printf("   count: %d\n", count()); fflush(stdout);
    dbgrpt();

    setUpdatesEnabled(true);
    show();
    update();
#ifdef NO
    for (int ndx = 0; ndx < count(); ndx++) {
        FeatureWidget * curItem = (FeatureWidget*) item(ndx);  // ???
        curItem->show();
    }
#endif

}

// slot
void  FeatureListWidget::featureAdded(FeatureValue fv) {
    printf("FeatureListWidget::%s) feature_code=0x%02x\n", __func__, fv._feature_code); fflush(stdout);
    // a QListWidget is not a QWidget - but is is a QObject
    QString objname = QListWidget::objectName();
    std::cout << "        objectname: " << objname.toStdString() << std::endl;
    // printf("(FeatureListWidget::%s) _baseModel=%p\n", __func__, _baseModel);
    FeatureValue * fv2 = _baseModel->modelVcpValueFind(fv._feature_code);
    assert(fv2);
    addFeature(&fv);

}

void   FeatureListWidget::featureUpdated(char feature_code) {
    printf("FeatureListWidget::%s) feature_code=0x%02x\n", __func__, feature_code); fflush(stdout);

    FeatureValue * fv = _baseModel->modelVcpValueFind(feature_code);
    assert(fv);
    updateFeature(fv);
}


#ifdef OLD
void    FeatureListWidget::featureChangedObserver(uint8_t feature_code) {
     printf("FeatureListWidget::%s) feature_code=0x%02x\n", __func__, feature_code); fflush(stdout);
     FeatureValue * fv = _baseModel->modelVcpValueFind(feature_code);
     assert(fv);
     addFeature(fv);

}
#endif

// #ifdef NO_FEATURE_CHANGED
void    FeatureListWidget::featureChanged(uint8_t feature_code)  {
     printf("FeatureListWidget::%s) feature_code=0x%02x\n", __func__, feature_code); fflush(stdout);
     FeatureValue * fv = _baseModel->modelVcpValueFind(feature_code);
     assert(fv);
     addFeature(fv);

}
// #endif

void FeatureListWidget::paintEvent(QPaintEvent *event) {
     printf("(%s::%s)\n", _cls, __func__); fflush(stdout);
     // QListWidget::paintEvent(event);
     printf("(%s::%s) Calling update() for %d FeatureWidget instances\n", _cls, __func__, count()); fflush(stdout);

     for (int ndx = 0; ndx < count(); ndx++) {
         FeatureWidget * curItem = (FeatureWidget*) item(ndx);  // ???
         // printf("(%s::%s) Calling update() for FeatureWidget %d\n", _cls, __func__, ndx); fflush(stdout);
         curItem->setUpdatesEnabled(true);  // no effect
         // curItem->setVisible(true);   // fills screen with empty window for each feature
         // fills scroll area, but items are empty
         curItem->update();    // not the culprit
         // curItem->repaint();    // syncronously for developemnt
     }
}


void  FeatureListWidget::dbgrpt() const {
    std::string on1 = objectName().toStdString();
    const char * objname = on1.c_str();
    printf("FeatureListWidget: %-20s contains %d ListWidgets\n", objname, count());
    for (int ndx = 0; ndx < count(); ndx++) {
        FeatureWidget * curItem = (FeatureWidget*) item(ndx);
        curItem->dbgrpt();
    }
    fflush(stdout);
}
