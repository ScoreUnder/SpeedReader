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

#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QDialog>
#include <QColorDialog>
#include <QFrame>
#include <QCloseEvent>

#include "settings.h"

namespace Ui {
    class SettingsWindow;
}

class SettingsWindow : public QDialog {
    Q_OBJECT  // Required by UI designer

public:
    SettingsWindow(QWidget* parent);
    ~SettingsWindow();

    void retranslate();

protected:
    void closeEvent(QCloseEvent* closeEvent);
    void done(int r);

private slots:
    void on_changeTextColorPushButton_clicked();
    void on_changeTextBackgroundColorPushButton_clicked();
    void on_changeLinesColorPushButton_clicked();
    void on_wordAddButton_clicked();
    void on_wordDeleteButton_clicked();
    void on_wordDeleteAllButton_clicked();
    void on_displayLongerWordsCheckBox_clicked(bool checked);

private:
    Ui::SettingsWindow* mUI;
    Settings* mSettings;
    bool mSynchronized;

    int areYouSureMessageBox(QString title, QString message);
    void synchronizeWithSettings();
    void changeBackground(QFrame* frame, QColor backgroundColor);
    void addTableRow(Word word);
    QColor changeColorButtonClicked(QColor initialColor, QFrame* frame);
};

#endif // SETTINGSWINDOW_H
