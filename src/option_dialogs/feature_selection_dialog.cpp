// feature_selection_dialog.cpp

// Copyright (C) 2018 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include <assert.h>
#include <stdio.h>
#include <iostream>

#include <QtWidgets/QRadioButton>

#include "base/ddcui_globals.h"
#include "help/help_dialog.h"

// generated by uic from feature_selection_dialog.ui:
#include "ui_feature_selection_dialog.h"

#include "feature_selection_dialog.h"


using namespace std;


void FeatureSelectionDialog::useSelectorData(FeatureSelector * fsel)
{
    if (debugFeatureSelection) {
        PRINTFTCM("_feature_selector:");
        fsel->dbgrpt();
    }

    QRadioButton * curButton;
    // switch is exhaustive, so curButton is always assigned, however -Wmaybe-uninitialized
    // flags the curButton->setChecked() line  Assign a dummy value to quiet the warning
    curButton = _ui->known_radioButton;   // dummy value
    DDCA_Feature_Subset_Id local_fsid = fsel->_featureListId;
    switch(local_fsid) {
    case DDCA_SUBSET_KNOWN:
        curButton = _ui->known_radioButton;
        break;
    case DDCA_SUBSET_COLOR:
        curButton = _ui->color_radioButton;
        break;
    case DDCA_SUBSET_PROFILE:
        curButton = _ui->profile_RadioButton;
        break;
    case DDCA_SUBSET_MFG:
        curButton = _ui->mfg_RadioButton;
        break;
    case DDCA_SUBSET_UNSET:
        assert(false);
        break;
    }
    curButton->setChecked(true);

    _ui->include_table_checkBox->setChecked(   fsel->_includeTableFeatures);
    _ui->show_unsupported_checkBox->setChecked(fsel->_showUnsupportedFeatures);
    _ui->capabilities_checkbox->setChecked(    fsel->_respectCapabilities);
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
    // PRINTFCM("_ui=%p, _featureSelector = %p\n", _ui, _featureSelector);
    _ui->setupUi(this);
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

void FeatureSelectionDialog::on_capabilities_checkbox_stateChanged(int arg1)
{
   cout << "(on_capabilities_checkBox_stateChanged) arg1 = " << arg1 << endl;
}

void FeatureSelectionDialog::on_show_unsupported_checkBox_stateChanged(int arg1)
{
   cout << "(on_show_unpported_checkBox_stateChanged) arg1 = " << arg1 << endl;
}

void FeatureSelectionDialog::on_include_table_checkBox_stateChanged(int arg1)
{
   cout << "(on_include_table_checkBox_StateChanged) arg1 = " << arg1 << endl;
}
#endif


void FeatureSelectionDialog::on_buttonBox_accepted()
{
    bool debugFunc = debugFeatureSelection;
    PRINTFTCMF(debugFunc, "Executing");
    // which button is currently clicked?

    DDCA_Feature_Subset_Id feature_list;
    if (_ui->color_radioButton->isChecked())
        feature_list = DDCA_SUBSET_COLOR;
    else if (_ui->known_radioButton->isChecked())
        feature_list = DDCA_SUBSET_KNOWN;
    else if (_ui->mfg_RadioButton->isChecked())
        feature_list = DDCA_SUBSET_MFG;
    else if (_ui->profile_RadioButton->isChecked())
        feature_list = DDCA_SUBSET_PROFILE;
    else if (_ui->scan_radioButton->isChecked())
        feature_list = DDCA_SUBSET_KNOWN;    // hack for now
    else
        feature_list = DDCA_SUBSET_KNOWN;    // should never occur

    bool changed = false;
    if (feature_list != _featureSelector->_featureListId) {
       _featureSelector->_featureListId = feature_list;
       changed = true;
    }
    if (_ui->include_table_checkBox->isChecked() !=  _featureSelector->_includeTableFeatures)
    {
       _featureSelector->_includeTableFeatures = _ui->include_table_checkBox->isChecked();
       changed = true;
    }
    if (_featureSelector->_showUnsupportedFeatures !=_ui->show_unsupported_checkBox->isChecked())
    {
        _featureSelector->_showUnsupportedFeatures = _ui->show_unsupported_checkBox->isChecked();
       changed = true;
    }
    if (_featureSelector->_respectCapabilities != _ui->capabilities_checkbox->isChecked() )
    {
        _featureSelector->_respectCapabilities = _ui->capabilities_checkbox->isChecked();
       changed = true;
    }
    // TODO: show_unsupported and show-table check boxes

    if (debugFeatureSelection) {
        PRINTFTCM("_feature_selector:");
        _featureSelector->dbgrpt();
        if (changed)
           PRINTFTCM("Signaling featureSelectionChanged()");
        else
           PRINTFTCM("NOT Signaling featureSelectionChanged()");
    }

    if (changed)
       emit featureSelectionChanged();
    // emit featureSelectionAccepted(feature_list);
}


void FeatureSelectionDialog::on_buttonBox_helpRequested()
{
    // PRINTFCM();
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
      // PRINTFCM("Reset");
      FeatureSelector * defaultSelector = new FeatureSelector;
      useSelectorData(defaultSelector);
      delete defaultSelector;
   }
}

