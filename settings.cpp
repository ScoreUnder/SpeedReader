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

#include "settings.h"

#include <QFontDatabase>
#include <QSettings>

Settings* Settings::mInstance = 0;

const QString SETTINGS_TEXT_COLOR = "textColor";
const QString SETTINGS_TEXT_BACKGROUND_COLOR = "textBackgroundColor";
const QString SETTINGS_LINES_COLOR = "linesColor";
const QString SETTINGS_FONT_FAMILY = "fontFamily";
const QString SETTINGS_FONT_SIZE = "fontSize";
const QString SETTINGS_DISPLAY_LONGER_WORDS_LONGER = "displayLongerWordsLonger";
const QString SETTINGS_WORD_LENGTH = "wordLength";
const QString SETTINGS_NUMBER_OF_WORDS = "numberOfWords";
const QString SETTINGS_WORDS_PER_MINUTE = "wordsPerMinute";
const QString SETTINGS_NUMBER_GROUPING = "numberGrouping";
const QString SETTINGS_STALL_AT_INDENTIONS = "stallAtIndentions";
const QString SETTINGS_WORDS = "words";
const QString SETTINGS_WORD = "word";
const QString SETTINGS_VALUE = "value";
const QString SETTINGS_STOP_WORD = "stopWord";
const QString SETTINGS_BREAK_WORD = "breakWord";
const QString SETTINGS_DELAY_WORD = "delayWord";
const QString SETTINGS_AUTO_UPDATE = "autoUpdate" + QString(APPLICATION_VERSION);
const QString SETTINGS_MAIN_WINDOW_GEOMETRY = "mainWindowGeometry";

const int FONT_SIZE_DEFAULT = 35;
const bool DISPLAY_LONGER_WORDS_LONGER_DEFAULT = true;
const int WORD_LENGTH_DEFAULT = 8;
const int NUMBER_OF_WORDS = 1;
const bool NUMER_GROUPING_DEFAULT = true;
const bool STALL_AT_INDENTIONS_DEFAULT = true;
const int WORDS_PER_MINUTE = 300;
const QColor TEXT_BACKGROUND_COLOR_DEFAULT = QColor(255, 255, 255);
const QColor TEXT_COLOR_DEFAULT = QColor(0, 0, 0);
const QColor LINES_COLOR_DEFAULT = QColor(28, 153, 255);

Settings* Settings::getInstance() {
    if (!mInstance) {
        mInstance = new Settings();
    }

    return mInstance;
}

Settings::Settings() {
    QSettings settings;
    this->setFontSize(settings.value(SETTINGS_FONT_SIZE, FONT_SIZE_DEFAULT).toInt());
    this->setFontFamily(settings.value(SETTINGS_FONT_FAMILY, QFontDatabase().families()[0]).toString());
    this->setShouldDisplayLongerWordsLonger(settings.value(SETTINGS_DISPLAY_LONGER_WORDS_LONGER, DISPLAY_LONGER_WORDS_LONGER_DEFAULT).toBool());
    this->setWordLength(settings.value(SETTINGS_WORD_LENGTH, WORD_LENGTH_DEFAULT).toInt());
    this->setShouldGroupNumbers(settings.value(SETTINGS_NUMBER_GROUPING, NUMER_GROUPING_DEFAULT).toBool());
    this->setShouldStallAtIndentions(settings.value(SETTINGS_STALL_AT_INDENTIONS, STALL_AT_INDENTIONS_DEFAULT).toBool());
    this->setNumberOfWords(settings.value(SETTINGS_NUMBER_OF_WORDS, NUMBER_OF_WORDS).toInt());
    this->setWordsPerMinute(settings.value(SETTINGS_WORDS_PER_MINUTE, WORDS_PER_MINUTE).toInt());
    this->setTextBackgroundColor(settings.value(SETTINGS_TEXT_BACKGROUND_COLOR, TEXT_BACKGROUND_COLOR_DEFAULT).value<QColor>());
    this->setTextColor(settings.value(SETTINGS_TEXT_COLOR, TEXT_COLOR_DEFAULT).value<QColor>());
    this->setLinesColor(settings.value(SETTINGS_LINES_COLOR, LINES_COLOR_DEFAULT).value<QColor>());
    mAutoUpdate = settings.value(SETTINGS_AUTO_UPDATE, 1).value<int>();
    mMainWindowGeometry = settings.value(SETTINGS_MAIN_WINDOW_GEOMETRY, QByteArray()).value<QByteArray>();

    int size = settings.beginReadArray(SETTINGS_WORDS);
    for (int i = 0; i < size; i++) {
        settings.setArrayIndex(i);

        Word word;
        word.word = settings.value(SETTINGS_VALUE).toString();
        word.stopWord = settings.value(SETTINGS_STOP_WORD).toBool();
        word.breakWord = settings.value(SETTINGS_BREAK_WORD).toBool();
        word.delayWord = qMax(0, settings.value(SETTINGS_DELAY_WORD, 0).toInt());
        this->appendWord(word);
    }

    settings.endArray();
}

void Settings::synchronize() {
    QSettings settings;

    settings.setValue(SETTINGS_TEXT_COLOR, mTextColor);
    settings.setValue(SETTINGS_TEXT_BACKGROUND_COLOR, mTextBackgroundColor);
    settings.setValue(SETTINGS_LINES_COLOR, mLinesColor);
    settings.setValue(SETTINGS_FONT_FAMILY, mFontFamily);
    settings.setValue(SETTINGS_FONT_SIZE, mFontSize);
    settings.setValue(SETTINGS_DISPLAY_LONGER_WORDS_LONGER, mDisplayLongerWordsLonger);
    settings.setValue(SETTINGS_WORD_LENGTH, mWordLength);
    settings.setValue(SETTINGS_NUMBER_OF_WORDS, mNumberOfWords);
    settings.setValue(SETTINGS_WORDS_PER_MINUTE, mWordsPerMinute);
    settings.setValue(SETTINGS_NUMBER_GROUPING, mNumberGrouping);

    settings.beginWriteArray(SETTINGS_WORDS);
    settings.remove("");

    for (int i = 0; i < mWords.count(); i++) {
        settings.setArrayIndex(i);
        settings.setValue(SETTINGS_VALUE, mWords.at(i).word);
        settings.setValue(SETTINGS_STOP_WORD, mWords.at(i).stopWord);
        settings.setValue(SETTINGS_BREAK_WORD, mWords.at(i).breakWord);
        settings.setValue(SETTINGS_DELAY_WORD, mWords.at(i).delayWord);
    }

    settings.endArray();

    settings.sync();

    emit updatedSettings();
}

QColor Settings::getTextColor() {
    return mTextColor;
}

void Settings::setTextColor(QColor textColor) {
    mTextColor = textColor;
}

QColor Settings::getTextBackgroundColor() {
    return mTextBackgroundColor;
}

void Settings::setTextBackgroundColor(QColor textBackgroundColor) {
    mTextBackgroundColor = textBackgroundColor;
}

QColor Settings::getLinesColor() {
    return mLinesColor;
}

void Settings::setLinesColor(QColor lineColor) {
    mLinesColor = lineColor;
}

QString Settings::getFontFamily() {
    return mFontFamily;
}

void Settings::setFontFamily(QString fontFamily) {
    QStringList families = QFontDatabase().families();

    mFontFamily = families.contains(fontFamily) ? fontFamily : families[0];
}

int Settings::getFontSize() {
    return mFontSize;
}

void Settings::setFontSize(int fontSize) {
    mFontSize = this->minMaxValue(MIN_FONT_SIZE, MAX_FONT_SIZE, fontSize);
}

bool Settings::shouldDisplayLongerWordsLonger() {
    return mDisplayLongerWordsLonger;
}

void Settings::setShouldDisplayLongerWordsLonger(bool displayLongerWordsLonger) {
    mDisplayLongerWordsLonger = displayLongerWordsLonger;
}

int Settings::getWordLength() {
    return mWordLength;
}

void Settings::setWordLength(int wordLength) {
    mWordLength = this->minMaxValue(MIN_WORD_LENGTH, MAX_WORD_LENGTH, wordLength);
}

int Settings::getNumberOfWords() {
    return mNumberOfWords;
}

void Settings::setNumberOfWords(int numberOfWords) {
    mNumberOfWords = this->minMaxValue(MIN_NUMBER_OF_WORDS, MAX_NUMBER_OF_WORDS, numberOfWords);
}

int Settings::getWordsPerMinute() {
    return mWordsPerMinute;
}

void Settings::setWordsPerMinute(int wordsPerMinute) {
    mWordsPerMinute = this->minMaxValue(MIN_WORDS_PER_MINUTE, MAX_WORDS_PER_MINUTE, wordsPerMinute);
}

bool Settings::shouldGroupNumbers() {
    return mNumberGrouping;
}

void Settings::setShouldGroupNumbers(bool shouldGroupNumbers) {
    mNumberGrouping = shouldGroupNumbers;
}

bool Settings::shouldStallAtIndentions() {
    return mStallAtIndentions;
}

void Settings::setShouldStallAtIndentions(bool stallAtIndentions) {
    mStallAtIndentions = stallAtIndentions;
}

QList<Word> Settings::getWords() {
    return mWords;
}

QList<QString> Settings::getBreakWords() {
    return mBreakWords;
}

QList<QString> Settings::getStopWords() {
    return mStopWords;
}

QMap<QString, int> Settings::getDelayWords() {
    return mDelayWords;
}

void Settings::setWords(QList<Word> words) {
    mWords = words;

    mStopWords = QList<QString>();
    mBreakWords = QList<QString>();
    mDelayWords = QMap<QString, int>();

    for (int i = 0; i < mWords.count(); i++) {
        Word word = mWords.at(i);

        if (word.stopWord) mStopWords.append(word.word);
        if (word.breakWord) mBreakWords.append(word.word);
        if (word.delayWord > 0) mDelayWords.insert(word.word, word.delayWord);
    }
}


void Settings::appendWord(Word word) {
    if (word.word.isEmpty()) return;

    foreach (Word mWord, mWords) if (mWord.word == word.word) return;

    mWords.append(word);

    if (word.stopWord) mStopWords.append(word.word);
    if (word.breakWord) mBreakWords.append(word.word);
    if (word.delayWord > 0) mDelayWords.insert(word.word, word.delayWord);
}


bool Settings::autoUpdate() {
    return mAutoUpdate != 0 && QDateTime::currentMSecsSinceEpoch() / 1000L >= mAutoUpdate;
}

void Settings::setAutoUpdateTomorrow() {
    mAutoUpdate = QDateTime::currentDateTime().addDays(1).toTime_t();
    this->syncAutoUpdate();
}

void Settings::setAutoUpdateNeverEver() {
    mAutoUpdate = 0;
    this->syncAutoUpdate();
}

void Settings::syncAutoUpdate() {
    QSettings settings;
    settings.setValue(SETTINGS_AUTO_UPDATE, mAutoUpdate);
    settings.sync();
}

void Settings::setMainWindowGeometry(const QByteArray& geometry) {
    mMainWindowGeometry = geometry;

    QSettings settings;
    settings.setValue(SETTINGS_MAIN_WINDOW_GEOMETRY, mMainWindowGeometry);
    settings.sync();
}

QByteArray Settings::getMainWindowGeometry() const {
    return mMainWindowGeometry;
}

int Settings::minMaxValue(int min, int max, int value) {
    return value < min ? min : (value > max ? max : value);
}
