// feature_selection_dialog.cpp

// Copyright (C) 2018 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include <string.h>     // for memcmp()
#include <assert.h>
#include <stdio.h>
#include <iostream>

#include <QtWidgets/QMessageBox>
#include <QtWidgets/QRadioButton>

#include <ddcutil_types.h>

#include "base/feature_list.h"
#include "help/help_dialog.h"

#include "nongui/msgbox_queue.h"
// generated by uic from feature_selection_dialog.ui:
#include "ui_feature_selection_dialog.h"
#include "base/global_state.h"

#include "feature_selection_dialog.h"
#include "../base/core.h"


using namespace std;


void FeatureSelectionDialog::useSelectorData(FeatureSelector * fsel)
{
    bool debugFunc = false;
    debugFunc = debugFunc || debugFeatureSelection;
    if (debugFunc) {
        TRACEC("Setting dialog box widgets from FeatureSelector:");
        fsel->dbgrpt();
    }

    QRadioButton * curButton;
    // switch is exhaustive, so curButton is always assigned, however -Wmaybe-uninitialized
    // flags the curButton->setChecked() line  Assign a dummy value to quiet the warning
    curButton = _ui->known_radioButton;   // dummy value
    DDCA_Feature_Subset_Id local_fsid = fsel->_featureSubsetId;
    switch(local_fsid) {
    case DDCA_SUBSET_KNOWN:
        curButton = _ui->known_radioButton;
        _ui->custom_lineEdit->setEnabled(false);
        _ui->allCapabilities_checkbox->setEnabled(true);
        _ui->onlyCapabilities_checkbox->setEnabled(true);
        break;
    case DDCA_SUBSET_COLOR:
        curButton = _ui->color_radioButton;
        _ui->custom_lineEdit->setEnabled(false);
        _ui->allCapabilities_checkbox->setEnabled(false);
        _ui->onlyCapabilities_checkbox->setEnabled(true);
        break;
    case DDCA_SUBSET_PROFILE: // removed from dialog, case left to satisfy that all cases addressed
        // curButton = _ui->profile_radioButton;
        break;
    case DDCA_SUBSET_MFG:
        curButton = _ui->mfg_radioButton;
        _ui->custom_lineEdit->setEnabled(false);
        _ui->allCapabilities_checkbox->setEnabled(false);
        _ui->onlyCapabilities_checkbox->setEnabled(true);
        break;
    case DDCA_SUBSET_CAPABILITIES:
        curButton = _ui->capabilities_radioButton;
        _ui->custom_lineEdit->setEnabled(false);
        _ui->allCapabilities_checkbox->setEnabled(false);
        _ui->onlyCapabilities_checkbox->setEnabled(false);
        break;
    case DDCA_SUBSET_SCAN:
        curButton = _ui->scan_radioButton;
        _ui->custom_lineEdit->setEnabled(false);
        _ui->allCapabilities_checkbox->setEnabled(false);
        _ui->onlyCapabilities_checkbox->setEnabled(false);
        break;
    case DDCA_SUBSET_CUSTOM:
       curButton = _ui->custom_radioButton;
       _ui->custom_lineEdit->setText( ddca_feature_list_string(&fsel->_customFeatureList, "", " ") );
       _ui->custom_lineEdit->setEnabled(true);
       _ui->allCapabilities_checkbox->setEnabled(false);
       _ui->onlyCapabilities_checkbox->setEnabled(true);
       break;
    case DDCA_SUBSET_UNSET:
        assert(false);
        break;
    }
    _ui->showUnsupported_checkbox->setEnabled(true);   // true for all feature sets

    curButton->setChecked(true);
    // to do: enable/disable other buttons as appropriate

    if (debugFunc) {
       TRACEC("fsel before setting flag checkboxes:");
       fsel->dbgrpt();
    }
    _ui->includeTable_checkbox->setChecked(    fsel->_includeTableFeatures);
    _ui->showUnsupported_checkbox->setChecked( fsel->_showUnsupportedFeatures);
    _ui->onlyCapabilities_checkbox->setChecked(fsel->_includeOnlyCapabilities);
    _ui->allCapabilities_checkbox->setChecked( fsel->_includeAllCapabilities);
}


void FeatureSelectionDialog::useSelectorData() {
   useSelectorData(_featureSelector);
}


FeatureSelectionDialog::FeatureSelectionDialog(
      QWidget *         parent,
      FeatureSelector * featureSelector
     )
   :  QDialog(parent),
      _cls(strdup(metaObject()->className())),
      _ui(new Ui::FeatureSelectionDialog),
      _featureSelector(featureSelector)
{
    // TRACE("_ui=%p, _featureSelector = %p\n", _ui, _featureSelector);
    _ui->setupUi(this);

    QObject::connect(_ui->known_radioButton, &QAbstractButton::clicked,
                      this, &FeatureSelectionDialog::on_known_radioButton_clicked);
    QObject::connect(_ui->capabilities_radioButton, &QAbstractButton::clicked,
                      this, &FeatureSelectionDialog::on_capabilities_radioButton_clicked);
    QObject::connect(_ui->mfg_radioButton, &QAbstractButton::clicked,
                      this, &FeatureSelectionDialog::on_mfg_radioButton_clicked);
#ifdef NO_PROFILE_BUTTON
    QObject::connect(_ui->profile_radioButton, &QAbstractButton::clicked,
                      this, &FeatureSelectionDialog::on_profile_radioButton_clicked);
#endif
    QObject::connect(_ui->color_radioButton, &QAbstractButton::clicked,
                      this, &FeatureSelectionDialog::on_color_radioButton_clicked);
    QObject::connect(_ui->scan_radioButton, &QAbstractButton::clicked,
                        this, &FeatureSelectionDialog::on_scan_radioButton_clicked);
    QObject::connect(_ui->custom_radioButton, &QAbstractButton::clicked,
                        this, &FeatureSelectionDialog::on_custom_radioButton_clicked);

    useSelectorData(_featureSelector);
}


FeatureSelectionDialog::~FeatureSelectionDialog()
{
    delete _ui;
}


#ifdef UNUSED
void FeatureSelectionDialog::setFeatureSet(int fsid) {

    cout << "(setFeatureSet) fsid=" << fsid << endl;
}

void FeatureSelectionDialog::on_known_radioButton_clicked(bool checked)
{
   cout << "(FeatureSelectionDialog::on_known_radioButton_clicked) checked=" << checked << endl;
}

void FeatureSelectionDialog::on_known_radioButton_clicked()
{
  setFeatureSet(DDCA_SUBSET_KNOWN);
}

void FeatureSelectionDialog::on_scan_radioButton_clicked()
{
  // setFeatureSet(DDCA_FEATURE_LIST_SCAN);
}

void FeatureSelectionDialog::on_mfg_RadioButton_clicked()
{
  setFeatureSet(DDCA_SUBSET_MFG);
}

void FeatureSelectionDialog::on_profile_RadioButton_clicked()
{
  setFeatureSet(DDCA_SUBSET_PROFILE);
}

void FeatureSelectionDialog::on_color_radioButton_clicked()
{
  setFeatureSet(DDCA_SUBSET_COLOR);
}
#endif


void FeatureSelectionDialog::on_known_radioButton_clicked(bool checked) {
    // cout << "(on_known_radioButton_clicked) arg1 = " << checked << endl;
   _ui->allCapabilities_checkbox->setEnabled(true);
   _ui->onlyCapabilities_checkbox->setEnabled(true);
   _ui->custom_lineEdit->setEnabled(false);
}


void FeatureSelectionDialog::on_scan_radioButton_clicked(bool checked) {
    // cout << "(on_scan_radioButton_clicked) arg1 = " << checked << endl;
   _ui->allCapabilities_checkbox->setEnabled(false);
   _ui->onlyCapabilities_checkbox->setEnabled(false);
   _ui->allCapabilities_checkbox->setChecked(false);
   _ui->onlyCapabilities_checkbox->setChecked(false);
   _ui->custom_lineEdit->setEnabled(false);
}


void FeatureSelectionDialog::on_capabilities_radioButton_clicked(bool checked) {
    // cout << "(on_capabilities_radioButton_clicked) arg1 = " << checked << endl;
   _ui->allCapabilities_checkbox->setEnabled(false);
   _ui->onlyCapabilities_checkbox->setEnabled(false);
   _ui->allCapabilities_checkbox->setChecked(false);
   _ui->onlyCapabilities_checkbox->setChecked(false);
   _ui->custom_lineEdit->setEnabled(false);
}


void FeatureSelectionDialog::on_mfg_radioButton_clicked(bool checked) {
    // cout << "(mfg_radioButton_clicked) arg1 = " << checked << endl;
   _ui->allCapabilities_checkbox->setEnabled(false);
   _ui->onlyCapabilities_checkbox->setEnabled(true);
   _ui->allCapabilities_checkbox->setChecked(false);
   _ui->custom_lineEdit->setEnabled(false);
}


#ifdef NO_PROFILE_BUTTON
void FeatureSelectionDialog::on_profile_radioButton_clicked(bool checked) {
    // cout << "(on_profile_radioButton_clicked) arg1 = " << checked << endl;
   _ui->allCapabilities_checkbox->setEnabled(false);
   _ui->onlyCapabilities_checkbox->setEnabled(true);
   _ui->allCapabilities_checkbox->setChecked(false);
}
#endif


void FeatureSelectionDialog::on_color_radioButton_clicked(bool checked) {
    // cout << "(on_color_radioButton_clicked) arg1 = " << checked << endl;
   _ui->allCapabilities_checkbox->setEnabled(false);
   _ui->onlyCapabilities_checkbox->setEnabled(true);
   _ui->allCapabilities_checkbox->setChecked(false);
   _ui->custom_lineEdit->setEnabled(false);
}

void FeatureSelectionDialog::on_custom_radioButton_clicked(bool checked) {
    // cout << "(on_color_radioButton_clicked) arg1 = " << checked << endl;
   _ui->allCapabilities_checkbox->setEnabled(false);
   _ui->allCapabilities_checkbox->setChecked(false);
   _ui->onlyCapabilities_checkbox->setEnabled(true);
   _ui->onlyCapabilities_checkbox->setChecked(false);

   _ui->custom_lineEdit->setEnabled(true);
}



void FeatureSelectionDialog::on_onlyCapabilities_checkbox_stateChanged(int arg1)
{
   // cout << "(on_onlyCapabilities_checkBox_stateChanged) arg1 = " << arg1 << endl;
   if (_ui->onlyCapabilities_checkbox->isChecked() ) {     // or != 0
      _ui->allCapabilities_checkbox->setCheckState(Qt::Unchecked);
   }
}


void FeatureSelectionDialog::on_allCapabilities_checkbox_stateChanged(int arg1)
{
   // cout << "(on_allCapabilities_checkBox_stateChanged) arg1 = " << arg1 << endl;
   if (arg1 == 2) {
      _ui->onlyCapabilities_checkbox->setCheckState(Qt::Unchecked);
   }
}

#ifdef UNUSED
void FeatureSelectionDialog::on_showUnsupported_checkbox_stateChanged(int arg1)
{
   cout << "(on_showUnsupported_checkBox_stateChanged) arg1 = " << arg1 << endl;
   // if (_ui->showUnsupported_checkbox->isChecked() ) {     // or != 0
   //    _ui->allCapabilities_checkbox->setCheckState(Qt::Unchecked);
   // }
}
#endif


#ifdef UNUSED
void FeatureSelectionDialog::on_showUsupported_checkbox_stateChanged(int arg1)
{
   cout << "(on_show_unpported_checkBox_stateChanged) arg1 = " << arg1 << endl;
}
void FeatureSelectionDialog::on_includeTable_checkbox_stateChanged(int arg1)
{
   cout << "(on_includeTable_checkbox_StateChanged) arg1 = " << arg1 << endl;
}
#endif


void FeatureSelectionDialog::on_buttonBox_accepted()
{
    bool debugFunc = false;
    debugFunc == debugFunc || debugFeatureSelection;
    TRACECF(debugFunc, "Executing");

    DDCA_Feature_List flist;
    DDCA_Feature_Subset_Id fsid;
    // which feature selection button is currently checked?
    if (_ui->color_radioButton->isChecked())
        fsid = DDCA_SUBSET_COLOR;
    else if (_ui->known_radioButton->isChecked())
        fsid = DDCA_SUBSET_KNOWN;
    else if (_ui->mfg_radioButton->isChecked())
        fsid = DDCA_SUBSET_MFG;
#ifdef NO_PROFILE_BUTTON
    else if (_ui->profile_radioButton->isChecked())
        fsid = DDCA_SUBSET_PROFILE;
#endif
    else if (_ui->capabilities_radioButton->isChecked())
        fsid = DDCA_SUBSET_CAPABILITIES;
    else if (_ui->scan_radioButton->isChecked())
        fsid = DDCA_SUBSET_SCAN;
    else if (_ui->custom_radioButton->isChecked()) {
        fsid = DDCA_SUBSET_CUSTOM;
        char * y = QS2S( _ui->custom_lineEdit->text());
        TRACECF(debugFunc, "text: %d - |%s|", strlen(y), y);
        char ** error_msgs = NULL;
        bool ok = true;
        flist = parse_custom_feature_list(y, &error_msgs);
        if (ddca_feature_list_count(&flist) == 0) {
           QString qstitle = "Feature Code Error";
           QMessageBox::Icon icon = QMessageBox::Critical;
           MsgBoxQueue * msgboxQueue = GlobalState::instance()._mainWindow->_msgboxQueue;
           if (error_msgs) {
              // fprintf(stderr, "Errors in custom features\n");
              int ndx = 0;
              while (error_msgs[ndx]) {
                 // TRACECF(true,"Invalid custom feature:   %s", error_msgs[ndx]);
                 QString qsexpl = QString::asprintf("%s", error_msgs[ndx]);
                 MsgBoxQueueEntry * qe = new MsgBoxQueueEntry(qstitle,qsexpl,icon);
                 TRACECF(debugFunc, "Calling _msgboxQueue.put() for qe: %s", QS2S(qe->repr()));
                 msgboxQueue->put(qe);
                 free(error_msgs[ndx]);
                 ndx++;
             }
           }
           else {
              QString qsexpl("No custom features specified");
              MsgBoxQueueEntry * qe = new MsgBoxQueueEntry(qstitle,qsexpl,icon);
              TRACECF(debugFunc, "Calling _msgboxQueue.put() for qe: %s", QS2S(qe->repr()));
              msgboxQueue->put(qe);
           }
           return;   // there's a custom feature error, don't exit dialog
        }   // feature_list_count == 0
    }   // custom_radioButton

    else
        fsid = DDCA_SUBSET_KNOWN;    // should never occur

    TRACECF(debugFunc, "Checking for any changes...");
    bool changed = false;
    if (fsid != _featureSelector->_featureSubsetId) {
       _featureSelector->_featureSubsetId = fsid;
       changed = true;
    }
    else if (fsid == DDCA_SUBSET_CUSTOM) {
       DDCA_Feature_List old_flist = _featureSelector->_customFeatureList;
       // need an equality function in the interface
       // if (memcmp(&flist, &old_flist, sizeof(DDCA_Feature_List)) != 0) {
       if (!ddca_feature_list_eq(&flist, &old_flist)) {
          _featureSelector->_customFeatureList = flist;
          changed = true;
       }
    }
    if (_ui->includeTable_checkbox->isChecked() !=  _featureSelector->_includeTableFeatures)
    {
       _featureSelector->_includeTableFeatures = _ui->includeTable_checkbox->isChecked();
       changed = true;
    }
    if (_featureSelector->_showUnsupportedFeatures !=_ui->showUnsupported_checkbox->isChecked())
    {
        _featureSelector->_showUnsupportedFeatures = _ui->showUnsupported_checkbox->isChecked();
       changed = true;
    }
    if (_featureSelector->_includeOnlyCapabilities != _ui->onlyCapabilities_checkbox->isChecked() )
    {
        _featureSelector->_includeOnlyCapabilities = _ui->onlyCapabilities_checkbox->isChecked();
       changed = true;
    }
    if (_featureSelector->_includeAllCapabilities != _ui->allCapabilities_checkbox->isChecked() )
     {
         _featureSelector->_includeAllCapabilities = _ui->allCapabilities_checkbox->isChecked();
        changed = true;
     }
    if (_featureSelector->_showUnsupportedFeatures != _ui->showUnsupported_checkbox->isChecked() )
     {
         _featureSelector->_showUnsupportedFeatures = _ui->showUnsupported_checkbox->isChecked();
        changed = true;
     }
    if (_featureSelector->_includeTableFeatures != _ui->includeTable_checkbox->isChecked() )
     {
         _featureSelector->_includeTableFeatures = _ui->includeTable_checkbox->isChecked();
        changed = true;
     }

    if (debugFunc) {
        TRACEC("_feature_selector:");
        _featureSelector->dbgrpt();
    }

    if (changed) {
       TRACECF(debugFunc, "Signaling featureSelectionChanged()");
       emit featureSelectionChanged();
       // emit featureSelectionAccepted(fsid);
    }
    else {
       TRACECF(debugFunc, "NOT Signaling featureSelectionChanged()");
    }
    accept();
}


void FeatureSelectionDialog::on_buttonBox_helpRequested()
{
    // TRACE();
    QString fn(":/docs/feature_selection.html");
    QFile f(fn);
    f.open(QFile::ReadOnly | QFile::Text);
    QTextStream in(&f);

    QString htmlText = in.readAll();

    // qDebug() << htmlText;

    // HelpDialog2(htmlText, this);
    HelpDialog2* hd = new HelpDialog2(this);
    hd->setText(htmlText);
    // hd->_textBrowser->setSource(fn);
    hd->setWindowTitle("ddcui Help - Feature Selection");
    hd->show();
}


// Reset
void FeatureSelectionDialog::on_buttonBox_clicked(QAbstractButton* button)
{
   if(button== (QAbstractButton*) _ui->buttonBox->button(QDialogButtonBox::Reset) ){
      // TRACE("Reset");
      FeatureSelector * defaultSelector = new FeatureSelector;
      useSelectorData(defaultSelector);
      delete defaultSelector;
   }
}

