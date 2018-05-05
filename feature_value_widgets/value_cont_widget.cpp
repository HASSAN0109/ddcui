
#include <string.h>

#include <QtWidgets/QWidget>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

#include <QtWidgets/QHBoxLayout>
// #include <QtWidgets/QVBoxLayout>

#include "base/ddcui_globals.h"
#include "base/debug_utils.h"
#include "value_cont_widget.h"

static bool dimensionReportShown = false;

ValueContWidget::ValueContWidget(QWidget *parent):
    ValueBaseWidget(parent)
{
   _cls = strdup(metaObject()->className());
    // printf("(ValueContWidget::ValueContWidget) Starting.\n" ); fflush(stdout);

    QSizePolicy fixedSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    fixedSizePolicy.setHorizontalStretch(0);    // needed?
    fixedSizePolicy.setVerticalStretch(0);
    // sizePolicy1.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
    fixedSizePolicy.setHeightForWidth(false);

    QFont monoValueFont;
    monoValueFont.setPointSize(9);
    monoValueFont.setFamily(QString::fromUtf8("Monospace"));

    QFont nonMonoValueFont;
    nonMonoValueFont.setPointSize(8);

    QFont nonMonoFont9;
    nonMonoFont9.setPointSize(8);

    _curSlider = new QSlider(Qt::Horizontal);
    _curSlider->setFocusPolicy(Qt::StrongFocus);
    _curSlider->setTickPosition(QSlider::TicksBelow);   // alt TicksBothSides
    _curSlider->setSingleStep(1);
    _curSlider->setFixedSize(200,18);
    if (debugLayout)
    _curSlider->setStyleSheet("background-color:pink;");

    _curSpinBox = new QSpinBox();
    _curSpinBox->setSingleStep(1);
    _curSpinBox->setFixedSize(45,18);
    if (debugLayout)
    _curSpinBox->setStyleSheet("background-color:green;");

    _maxTitle = new QLabel("Max:");
    _maxTitle->setFixedSize(25,18);
    _maxTitle->setFont(nonMonoValueFont);
    if (debugLayout)
    _maxTitle->setStyleSheet("background-color:cyan;");

    _maxValue = new QLabel();
    _maxValue->setFont(monoValueFont);
    // _maxValue->setFrameStyle(QFrame::Sunken | QFrame::Panel);
    _maxValue->setFrameStyle(QFrame::Plain | QFrame::NoFrame);
    _maxValue->setFixedSize(30,20);
    if (debugLayout)
    _maxValue->setStyleSheet("background-color:blue;");

    _spinBoxTimer = new QTimer();
    _spinBoxTimer->setSingleShot(true);
    _spinBoxTimer->setInterval(1000);

    connect(_spinBoxTimer,   SIGNAL(timeout()),
            this,            SLOT(onSpinBoxTimedOut()));


    connect( _curSlider,  SIGNAL(valueChanged(int)),
             _curSpinBox, SLOT(  setValue(int)));
    connect(_curSpinBox,  SIGNAL(valueChanged(int)),
            _curSlider,   SLOT(  setValue(int)));

    connect( _curSlider,  SIGNAL(valueChanged(int)),
             this,        SLOT(  onSliderValueChanged(int)));

    connect( _curSlider,  SIGNAL(valueChanged(int)),
             this,        SLOT(  onSliderValueChanged(int)));

    connect( _curSlider,  SIGNAL(sliderReleased()),
             this,        SLOT(  onSliderReleased()));
    connect( _curSpinBox, SIGNAL(valueChanged(int)),
             this,        SLOT(  onSpinBoxValueChanged(int)));
    connect( _curSpinBox, SIGNAL(editingFinished()),
             this,        SLOT(  onSpinBoxEditingFinished()));

    if (useApplyCancel) {
        QSizePolicy* sizePolicy = new QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
       _applyButton  = new QPushButton("Apply");
       _cancelButton = new QPushButton("Cancel");
       _applyButton->setMaximumSize(55,20);
       _applyButton->setSizePolicy(*sizePolicy);
       _applyButton->setFont(nonMonoFont9);
       _cancelButton->setMaximumSize(55,20);
       _cancelButton->setSizePolicy(*sizePolicy);
       _cancelButton->setFont(nonMonoFont9);

       _applyButton->setEnabled(false);
       _cancelButton->setEnabled(false);

       connect(_applyButton,  &QPushButton::clicked,
               this,          &ValueContWidget::onApplyButtonClicked);
       connect(_cancelButton, &QPushButton::clicked,
               this,          &ValueContWidget::onCancelButtonClicked);
    }

   QLabel* spacer = new QLabel();
   spacer->setFixedSize(10,18);


    QHBoxLayout * layout = new QHBoxLayout();
    layout->addWidget(_curSlider);
    layout->addWidget(_curSpinBox);
    layout->addWidget(_maxTitle);
    layout->addWidget(_maxValue);
    // not the culprit
  // layout->addWidget(spacer);  adds space here, but also whole valuestackedwidget shifts right

    layout->setStretch(1,0);

    if (useApplyCancel) {
       layout->addSpacing(5);
       layout->addWidget(_applyButton);
       layout->addWidget(_cancelButton);
    }
    else {
       layout->addSpacing(5);
       layout->addWidget(spacer);
    }
    layout->setContentsMargins(0,0,0,0);
    setLayout(layout);

    if (debugLayout) {
       this->setStyleSheet("background-color:yellow;");

       if (!dimensionReportShown) {
          PRINTFCM("curSlider dimensions\n");
          reportWidgetDimensions(_curSlider, _cls, __func__);

           PRINTFCM("_curSpinBox dimensions\n");
           reportWidgetDimensions(_curSpinBox, _cls, __func__);

           PRINTFCM("_maxTitle dimensions\n");
           reportWidgetDimensions(_maxTitle, _cls, __func__);

           PRINTFCM("_maxValue dimensions\n");
           reportWidgetDimensions(_maxValue, _cls, __func__);

           PRINTFCM("ValueContWidget dimensions\n");
           reportWidgetDimensions(this, _cls, __func__);
           dimensionReportShown = true;
       }
    }
}


void ValueContWidget::setFeatureValue(const FeatureValue &fv) {
    PRINTFCMF(debugValueWidgetSignals, "Starting. feature code: 0x%02x", fv._feature_code);
    ValueBaseWidget::setFeatureValue(fv);

    int maxval = _mh << 8 | _ml;
    int curval = _sh << 8 | _sl;

    PRINTFCMF(debugValueWidgetSignals,
              "feature=0x%02x, curval=%d, maxval=%d", _featureCode, curval, maxval);

    _guiChange = false;

    _curSlider->setTickInterval(maxval/10);
    // _curSlider->setRange(0, maxVal);
    _curSlider->setMaximum(maxval);
    _curSlider->setValue(curval);

    _curSpinBox->setMaximum(maxval);
    _curSpinBox->setValue(curval);
    //_curSpinBox->setRange(0,maxval);

    QString s = QString::number(maxval);
    _maxValue->setText(s);

    if (useApplyCancel) {
        _applyButton->setEnabled(false);
        _cancelButton->setEnabled(false);
    }

    _guiChange = true;
}


void ValueContWidget::setCurrentValue(uint16_t newval) {
    ValueBaseWidget::setCurrentValue(newval);
    _guiChange = false;

     int curval = _sh << 8 | _sl;
     PRINTFCMF(debugValueWidgetSignals, "feature=0x%02x, curval=%d", _featureCode , curval);

    _curSpinBox->setValue(curval);
    _curSlider->setValue(curval);

    if (useApplyCancel) {
        _applyButton->setEnabled(false);
        _cancelButton->setEnabled(false);
    }

    _guiChange = true;
}


uint16_t ValueContWidget::getCurrentValue() {
    int curval = _curSpinBox->value();
    _sh = (curval >> 8) & 0xff;
    _sl = curval & 0xff;
    uint16_t result = (_sh << 8) | _sl;
    return result;
}


void ValueContWidget::onSliderValueChanged(int value) {
   PRINTFCMF(debugValueWidgetSignals, "NO ACTION feature=0x%02x, value=%d",  _featureCode, value);
}


void ValueContWidget::onSliderReleased() {
   PRINTFCMF(debugValueWidgetSignals, "feature=0x%02x",  _featureCode);

   int newval = _curSpinBox->value();

   uint8_t new_sh = (newval >> 8) & 0xff;
   uint8_t new_sl = newval & 0xff;
   // if (debugValueWidgetSignals)
   //     printf("(%s::%s) sh=0x%02x, sl=0x%02x \n", _cls, __func__, new_sh, new_sl); fflush(stdout);

   _newval = newval;
   if (useApplyCancel) {
      _applyButton->setEnabled(true);
      _cancelButton->setEnabled(true);
   }
   else {
      if (_guiChange)
         emit featureValueChanged(_featureCode, new_sh, new_sl);
   }
}


void ValueContWidget::onSpinBoxValueChanged(int value) {
   PRINTFCMF(debugValueWidgetSignals,
             "feature=0x%02x, value=%d, _guiChange=%d", _featureCode, value, _guiChange);

   int newval = _curSpinBox->value();
   // uint8_t new_sh = (newval >> 8) & 0xff;
   // uint8_t new_sl = newval & 0xff;
   // if (debugValueWidgetSignals)
   //    printf("(%s::%s) sh=0x%02x, sl=0x%02x \n", _cls, __func__, new_sh, new_sl); fflush(stdout);
   // emit featureValueChanged(_feature_code, new_sh, new_sl);
   _newval = newval;


   if (useApplyCancel) {
      _applyButton->setEnabled(true);
      _cancelButton->setEnabled(true);
   }
   else {
      // QTimer::singleShot(1000, this, SLOT(onSpinBoxTimedOut()));
      if (_guiChange)
         _spinBoxTimer->start();
   }

}

void ValueContWidget::onSpinBoxTimedOut() {
   PRINTFCM("feature 0x%02x, _newval=%d, emitting featureValueChanged()", _featureCode, _newval);

   uint8_t new_sh = (_newval >> 8) & 0xff;
   uint8_t new_sl = _newval & 0xff;
   emit featureValueChanged(_featureCode, new_sh, new_sl);
}


void ValueContWidget::onSpinBoxEditingFinished() {
   if (debugValueWidgetSignals)
       printf("(%s::%s) \n", _cls, __func__); fflush(stdout);
}


void  ValueContWidget::onApplyButtonClicked(bool checked) {
   PRINTFCMF(debugValueWidgetSignals, "Executing");

   uint16_t oldval = _sh << 8 | _sl;
   if (_newval != oldval) {
      emit featureValueChanged(_featureCode, _newval >> 8, _newval & 0xff);
   }

   // what to do while we're waiting for the update to apply or fail?
   _curSpinBox->setValue(oldval);

   _applyButton->setEnabled(false);
   _cancelButton->setEnabled(false);
}


void  ValueContWidget::onCancelButtonClicked(bool checked) {
    PRINTFCMF(debugValueWidgetSignals, "Executing");

   _applyButton->setEnabled(false);
   _cancelButton->setEnabled(false);
   uint16_t oldval = _sh<<8 | _sl;
   _curSpinBox->setValue(oldval);
   _newval = oldval;
}


// never triggered
void ValueContWidget::focusOutEvent(QFocusEvent * event) {
    PRINTFCMF(debugValueWidgetSignals, "Executing"  );

    ValueBaseWidget::focusOutEvent(event);
}

void ValueContWidget::focusInEvent(QFocusEvent * event) {
    PRINTFCMF(debugValueWidgetSignals, "Executing" );

    ValueBaseWidget::focusInEvent(event);
}


#ifdef USELESS

// works for mouse
// called when mouse leaves the widget's screen space.
// n. not when focus is lost
void ValueContWidget::leaveEvent(QEvent * event) {
   if (debugValueWidgetSignals)
       printf("(%s::%s) \n", _cls, __func__); fflush(stdout);
   ValueBaseWidget::leaveEvent(event);
   uint16_t oldval = _sh << 8 | _sl;
   if (oldval != _newval) {
      if (debugValueWidgetSignals)
          printf("(%s::%s) Discarding changed value\n", _cls, __func__); fflush(stdout);
      _curSpinBox->setValue(oldval);
      _newval = oldval;
   }
}

#endif


// void QWidget::mousePressEvent(QMouseEvent *event)
// void QWidget::keyPressEvent(QKeyEvent *event)




