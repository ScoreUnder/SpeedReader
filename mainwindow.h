/*
    Copyright 2014-2015 Vanniktech - Niklas Baudy

    This file is part of SpeedReader.

    SpeedReader is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    SpeedReader is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with SpeedReader. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QClipboard>

#include "settingswindow.h"
#include "settings.h"
#include "speedreadertext.h"
#include "i18n.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT  // Required by UI designer

public:
    explicit MainWindow(QWidget* parent = 0);
    ~MainWindow();

    void setSpeedReadingText(QString text);

public slots:
    void on_speedreadButton_clicked();

protected:
    void changeEvent(QEvent* event);
    void closeEvent(QCloseEvent *event);

protected slots:
    void slotLanguageChanged(QAction* action);

private slots:
    void changed(QString text, int speedreadTextInPercent, SpeedReaderText::SpeedReaderStatus status);
    void changedSlot(QClipboard::Mode mode);

    void speedreadText(QString text);

    void on_stopButton_clicked();
    void on_cancelButton_clicked();
    void on_continueButton_clicked();
    void on_plainTextEdit_textChanged();

    // Actions
    void on_actionReportABug_triggered();
    void on_actionSendFeedback_triggered();
    void on_actionSpeedReader_triggered();
    void on_actionAboutSpeedReader_triggered();
    void on_actionForkMeOnGitHub_triggered();

    // Shortcuts
    void speedReadFromClipBoardShortcut();
    void sShortcut();
    void cShortcut();
    void escapeShortcut();

private:
    SpeedReaderText  mSpeedReaderText;
    Settings*        mSettings;
    SettingsWindow*  mSettingsWindow;

    Ui::MainWindow* mUI;
    QRectF mOldTextBoundaries;
    void updateSpeedReadLabelText(QString value);

    QClipboard* mClipBoard;

    void updateStatusWidget();
    void updateSpeedReaderText(QString text);

    I18N* mI18N;
    void createLanguageMenu();
};

#endif // MAINWINDOW_H
