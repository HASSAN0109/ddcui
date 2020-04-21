/* mainwindow.h */

// Copyright (C) 2018-2019 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QComboBox>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QtWidgets>

#include <ddcutil_types.h>

#include "../base/core.h"
#include "base/user_interface_options_state.h"
#include "base/other_options_state.h"
#include "nongui/msgbox_queue.h"

#include "cmdline/parsed_cmd.h"

class FeatureBaseModel;
class FeatureSelectionDialog;
class FeatureSelector;
class Monitor;
class MsgBoxQueue;
class OtherOptionsDialog;
class OtherOptionsState;
class UserInterfaceOptionsDialog;
class UserInterfaceOptionsState;
class DebugActionsDialog;
class QMessageBox;
class VcpThread;
class WaitingSpinnerWidget;
class MsgBoxThread;

// namespace Ui {
// class MainWindow;
// }
class Ui_MainWindow;


class MainWindow : public QMainWindow
// , public PageChangeObserver
{
    Q_OBJECT

public:
    enum View {
        NoView,
        MonitorView,
        CapabilitiesView,
        FeaturesView
    };

    // explicit
    MainWindow(Parsed_Cmd * parsed_cmd, QWidget *parent = 0);
    ~MainWindow();

    void initSerialMsgbox();

    void reportDdcApiError(QString funcname, int rc) const;
    // void pageChanged(int pageno) override;
    // void pageChangedByWidget(QWidget * widget) override;

#ifdef UNUSED
    DDCA_Feature_Subset_Id feature_list_id() const;
    void set_feature_list_id(DDCA_Feature_Subset_Id feature_list_id);
#endif
    WaitingSpinnerWidget*    _spinner;

public:
    // n. MainWindow is also a singleton, combine with GlobalState?
    // to do, make private, use accessor function
    // GlobalState & _globalState;



signals:
    void featureSelectionChanged();
    void signalMonitorSummaryView();
    void signalCapabilitiesView();
    void signalFeaturesView();
    void userInterfaceOptionsChanged();
    void showStats();     // needs parm for which
    void resetStats();
    void reportApplicationEventLoopStarted();

public slots:
    void longRunningTaskStart();
    void longRunningTaskEnd();
    void setStatusMsg(QString msg);
    void showSerialMsgBox(QString title, QString text, QMessageBox::Icon icon);

private slots:
    void start_msgBoxThread();
    // View Menu
    void on_actionMonitorSummary_triggered();
    void on_actionCapabilities_triggered();

    void on_actionFeaturesScrollArea_triggered();

    // Options Menu
    void on_actionFeatureSelectionDialog_triggered();
    void for_actionFeatureSelectionDialog_accepted();
#ifdef UNUSED
    void actionFeatureSelectionDialog_destroyed(QObject * obj);
    void on_actionFeatureSelection_triggered();
    void featureSelectionAccepted(DDCA_Feature_Subset_Id feature_list);
#endif

    DDCA_Feature_Subset_Id feature_list_id() const;

    void on_actionOtherOptionsDialog_triggered();
    void for_actionOtherOptionsDialog_ncValuesSourceChanged(NcValuesSource valuesSource);
#ifdef UNUSED
    void on_actionOtherOptionsDialog_accepted();
#endif

    void on_actionUserInterfaceOptionsDialog_triggered();
    void for_actionUserInterfaceOptionsDialog_accept();

    // Actions Menu
    void on_actionRescan_triggered();
    void on_actionRedetect_triggered();
    void on_actionDebugActionsDialog_triggered();
    void for_resetStats_triggered();
    void for_reportStats_triggered(DDCA_Stats_Type stats_type, bool showTheadData);

    void capture_stats(DDCA_Stats_Type stats_type, bool show_thread_data);


    // Help Menu
    void on_actionAbout_triggered();
    void on_actionAbout_Qt_triggered();
    void on_actionContentsHelp_triggered();

    // Tool Bar
    void displaySelectorCombobox_currentIndexChanged(int index);
    // void displaySelectorCombobox_activated(int index);

   // Other
#ifdef UNUSED
    void on_vcpTableView_clicked(const QModelIndex &index);
    void on_vcpTableView_doubleClicked(const QModelIndex &index);
#endif

    void showCentralWidgetPage(int pageno);
    void showCentralWidgetByWidget(QWidget * pageWidget);

public:
    UserInterfaceOptionsDialog * _uid = NULL;
    FeatureSelector *        _feature_selector = NULL;

private:
    void initMonitors(Parsed_Cmd * parsed_cmd);
    void loadMonitorFeatures(Monitor * monitor);

    const char *             _cls;
    // Ui::MainWindow*          _ui;
    Ui_MainWindow *          _ui;
    DDCA_Display_Info_List * _dlist ;
    int                      _curDisplayIndex = -1;
    View                     _curView = NoView;
    QComboBox *              _toolbarDisplayCB;

    OtherOptionsState *      _otherOptionsState = NULL;
    UserInterfaceOptionsState* _uiOptionsState = NULL;
    QVector<Monitor*>        _monitors;
    DDCA_Feature_Subset_Id   _feature_list_id = DDCA_SUBSET_KNOWN;
    QVector<VcpThread*>      _vcp_threads;
    // WaitingSpinnerWidget*    _spinner;
    QMessageBox*             _serialMsgBox = nullptr;
    QMessageBox*             _loadingMsgBox;
    MsgBoxQueue*             _msgboxQueue = nullptr;
    MsgBoxThread *           _msgBoxThread = nullptr;
    QList<MsgBoxQueueEntry*> _deferredMsgs;

    FeatureSelectionDialog*  _fsd = NULL;
    OtherOptionsDialog*      _ood = NULL;       // for future use

};

#endif // MAINWINDOW_H
