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

#include "settingswindow.h"
#include "ui_settingswindow.h"

#include <QFontDatabase>
#include <QMessageBox>

SettingsWindow::SettingsWindow(QWidget* parent) : QDialog(parent), mUI(new Ui::SettingsWindow) {
    mUI->setupUi(this);

    #ifdef QT_DEBUG
        this->setFixedWidth(1100);
    #endif

    mSettings = Settings::getInstance();

    QStringList families = QFontDatabase().families();

    foreach (QString family, families) {
        mUI->fontFamilyComboBox->addItem(family);
    }

    mUI->wordsPerMinuteSpinBox->setMinimum(Settings::MIN_WORDS_PER_MINUTE);
    mUI->wordsPerMinuteSpinBox->setMaximum(Settings::MAX_WORDS_PER_MINUTE);

    mUI->numberOfWordsSpinBox->setMinimum(Settings::MIN_NUMBER_OF_WORDS);
    mUI->numberOfWordsSpinBox->setMaximum(Settings::MAX_NUMBER_OF_WORDS);

    mUI->wordLengthSpinBox->setMinimum(Settings::MIN_WORD_LENGTH);
    mUI->wordLengthSpinBox->setMaximum(Settings::MAX_WORD_LENGTH);

    mUI->fontSizeSpinBox->setMinimum(Settings::MIN_FONT_SIZE);
    mUI->fontSizeSpinBox->setMaximum(Settings::MAX_FONT_SIZE);

    mUI->numberOfWordsSpinBox->setValue(mSettings->getNumberOfWords());
    mUI->wordsPerMinuteSpinBox->setValue(mSettings->getWordsPerMinute());
    mUI->displayLongerWordsCheckBox->setChecked(mSettings->shouldDisplayLongerWordsLonger());
    this->on_displayLongerWordsCheckBox_clicked(mSettings->shouldDisplayLongerWordsLonger());
    mUI->wordLengthSpinBox->setValue(mSettings->getWordLength());
    mUI->fontSizeSpinBox->setValue(mSettings->getFontSize());
    mUI->fontFamilyComboBox->setCurrentText(mSettings->getFontFamily());
    mUI->numberGroupingCheckBox->setChecked(mSettings->shouldGroupNumbers());
    mUI->stallAtIndentionsCheckBox->setChecked(mSettings->shouldStallAtIndentions());

    this->changeBackground(mUI->textColorFrame, mSettings->getTextColor());
    this->changeBackground(mUI->textBackgroundColorFrame, mSettings->getTextBackgroundColor());
    this->changeBackground(mUI->linesColorFrame, mSettings->getLinesColor());

    QList<Word> words = mSettings->getWords();
    mUI->wordsTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    foreach (Word word, words) this->addTableRow(word);
}

SettingsWindow::~SettingsWindow() {
    delete mUI;
}

void SettingsWindow::retranslate() {
    mUI->retranslateUi(this);
}

void SettingsWindow::on_changeTextColorPushButton_clicked() {
    mSettings->setTextColor(this->changeColorButtonClicked(mSettings->getTextColor(), mUI->textColorFrame));
}

void SettingsWindow::on_changeTextBackgroundColorPushButton_clicked() {
    mSettings->setTextBackgroundColor(this->changeColorButtonClicked(mSettings->getTextBackgroundColor(), mUI->textBackgroundColorFrame));
}

void SettingsWindow::on_changeLinesColorPushButton_clicked() {
    mSettings->setLinesColor(this->changeColorButtonClicked(mSettings->getLinesColor(), mUI->linesColorFrame));
}

QColor SettingsWindow::changeColorButtonClicked(QColor initialColor, QFrame* frame) {
    QColor color = QColorDialog::getColor(initialColor, this);

    this->changeBackground(frame, color);
    this->raise();
    this->activateWindow();

    return color;
}

void SettingsWindow::on_wordAddButton_clicked() {
    QString wordValue = mUI->wordLineEdit->text();

    if (wordValue.trimmed().length() == 0) return;

    for (int i = 0; i < mUI->wordsTableWidget->rowCount(); i++) if (mUI->wordsTableWidget->item(i, 0)->text() == wordValue) return;

    Word word;
    word.word = wordValue;
    word.stopWord = false;
    word.breakWord = false;
    word.delayWord = 0;

    this->addTableRow(word);
    mUI->wordLineEdit->setText("");
}

void SettingsWindow::on_wordDeleteButton_clicked() {
    mUI->wordsTableWidget->removeRow(mUI->wordsTableWidget->currentRow());
}

void SettingsWindow::on_wordDeleteAllButton_clicked() {
    if (this->areYouSureMessageBox(tr("Delete all words"), tr("Are you sure you want to delete all words?")) == QMessageBox::Yes) mUI->wordsTableWidget->setRowCount(0);
}

void SettingsWindow::done(int r) {
    QDialog::done(r);
    if (!mSynchronized) this->synchronizeWithSettings();
}

void SettingsWindow::closeEvent(QCloseEvent* closeEvent) {
    if (!mSynchronized) this->synchronizeWithSettings();
    closeEvent->accept();
}

void SettingsWindow::synchronizeWithSettings() {
    mSynchronized = true;

    QList<Word> words;

    for (int i = 0; i < mUI->wordsTableWidget->rowCount(); i++) {
        Word word;
        word.word = mUI->wordsTableWidget->item(i, 0)->text();
        word.stopWord = mUI->wordsTableWidget->item(i, 1)->checkState() == Qt::Checked;
        word.breakWord = mUI->wordsTableWidget->item(i, 2)->checkState() == Qt::Checked;
        word.delayWord = qMax(0, mUI->wordsTableWidget->item(i, 3)->text().toInt());
        words.append(word);
    }

    mSettings->setWords(words);

    mSettings->setFontFamily(mUI->fontFamilyComboBox->currentText());
    mSettings->setFontSize(mUI->fontSizeSpinBox->value());
    mSettings->setShouldDisplayLongerWordsLonger(mUI->displayLongerWordsCheckBox->isChecked());
    mSettings->setWordLength(mUI->wordLengthSpinBox->value());
    mSettings->setNumberOfWords(mUI->numberOfWordsSpinBox->value());
    mSettings->setWordsPerMinute(mUI->wordsPerMinuteSpinBox->value());
    mSettings->setShouldGroupNumbers(mUI->numberGroupingCheckBox->isChecked());
    mSettings->setShouldStallAtIndentions(mUI->stallAtIndentionsCheckBox->isChecked());

    mSettings->synchronize();
}

void SettingsWindow::changeBackground(QFrame* frame, QColor backgroundColor) {
    frame->setStyleSheet("QFrame { background-color: " + backgroundColor.name() + "}");
}

void SettingsWindow::addTableRow(Word word) {
    int rowCount = mUI->wordsTableWidget->rowCount();
    mUI->wordsTableWidget->insertRow(rowCount);

    QTableWidgetItem* item = new QTableWidgetItem(word.word);
    mUI->wordsTableWidget->setItem(rowCount, 0, item);

    QTableWidgetItem* stopWordWidgetItem = new QTableWidgetItem();
    stopWordWidgetItem->setCheckState(word.stopWord ? Qt::Checked : Qt::Unchecked);
    stopWordWidgetItem->setFlags(item->flags() ^ Qt::ItemIsEditable);
    mUI->wordsTableWidget->setItem(rowCount, 1, stopWordWidgetItem);

    QTableWidgetItem* breakWordWidgetItem = new QTableWidgetItem();
    breakWordWidgetItem->setCheckState(word.breakWord ? Qt::Checked : Qt::Unchecked);
    breakWordWidgetItem->setFlags(item->flags() ^ Qt::ItemIsEditable);
    mUI->wordsTableWidget->setItem(rowCount, 2, breakWordWidgetItem);

    QTableWidgetItem* delayWordWidgetItem = new QTableWidgetItem(QString::number(word.delayWord));
    mUI->wordsTableWidget->setItem(rowCount, 3, delayWordWidgetItem);
}

void SettingsWindow::on_displayLongerWordsCheckBox_clicked(bool checked) {
    mUI->wordLengthSpinBox->setEnabled(checked);
}

int SettingsWindow::areYouSureMessageBox(QString title, QString message) {
    QMessageBox messageBox(title, message, QMessageBox::Question, QMessageBox::Yes | QMessageBox::Default, QMessageBox::No | QMessageBox::Escape, QMessageBox::NoButton, this);
    messageBox.setButtonText(QMessageBox::Yes, tr("Yes"));
    messageBox.setButtonText(QMessageBox::No, tr("No"));

    return messageBox.exec();
}
