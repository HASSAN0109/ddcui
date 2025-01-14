// debug_actions_dialog.h

// Copyright (C) 2020-2024 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef DEBUG_ACTIONS_DIALOG_H
#define DEBUG_ACTIONS_DIALOG_H

#include <QtCore/QObject>       // Becasue MOC
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QDialog>

#include "ddcutil_types.h"

#include "ui_debug_actions_dialog.h"    // generated by UIC from debug_actions_dialog.ui

// namespace Ui {
// class DebugActionsDialog;
// }

class DebugActionsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DebugActionsDialog(QWidget *parent = 0);
    ~DebugActionsDialog();

    signals:
    void resetStats_triggered();
    void reportStats_triggered(DDCA_Stats_Type stats_type, bool extended);

private slots:
    void on_actionButtonBox_accepted();
    void on_actionButtonBox_helpRequested();

    void for_actionAllStatsButton_clicked(bool onoff);
    void for_actionCallsStatsButton_clicked(bool onoff);
    void for_actionErrorsStatsButton_clicked(bool onoff);
    void for_actionTriesStatsButton_clicked(bool onoff);
    void for_actionElapsedStatsButton_clicked(bool onoff);
    void for_actionApiStatsButton_clicked(bool onoff);

    void for_actionExtendedStatsCheckBox_stateChanged(int newState);

    void for_actionResetStatsButton_clicked(bool onoff);

private:
    const char * _cls;
    Ui::DebugActionsDialog *_ui;
    // void setUiSource(NcValuesSource source);

};

#endif // DEBUG_ACTIONS_DIALOG_H
