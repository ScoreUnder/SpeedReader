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

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QDesktopServices>
#include <QPainter>
#include <QShortcut>
#include <QInputDialog>

#include "lib/vntformatter.h"

#ifdef Q_OS_WIN32
const QString OS = "Windows";
#elif defined Q_OS_MAC
const QString OS = "Mac";
#elif defined Q_OS_LINUX
const QString OS = "Linux";
#endif

const QString coSite = ORGANIZATION_DOMAIN;
const QString coSiteGitHub = "https://github.com/vanniktech/SpeedReader";
const QString coSiteSpeedReader = coSite + "SpeedReader/";
const QString coSiteSpeedReaderOSTXT = coSiteSpeedReader + "latest/" + OS + ".txt";
const QString coSiteSpeedReaderOSZIP = coSiteSpeedReader + OS + "/" + APPLICATION_VERSION;

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), mSpeedReaderText(Settings::getInstance(), this), mUI(new Ui::MainWindow) {
    mUI->setupUi(this);

    mI18N = new I18N(this);

    // Some layout fixes (can't be done through Qt Designer)
    mUI->buttonLayout->setAlignment(Qt::AlignLeft);
    mUI->stopCancelButtonLayout->setAlignment(Qt::AlignLeft);
    mUI->continueButton->setVisible(false);

    QObject::connect(&mSpeedReaderText, &SpeedReaderText::changed, this, &MainWindow::changed);
    this->updateStatusWidget();

    mSettings = Settings::getInstance();
    mSettingsWindow = new SettingsWindow(this);

    mClipBoard = QApplication::clipboard();
    QObject::connect(mClipBoard, &QClipboard::changed, this, &MainWindow::changedSlot);
    this->changedSlot(QClipboard::Clipboard);

    this->createLanguageMenu();

    #ifdef QT_DEBUG
        this->setFixedWidth(1100);
    #else
        this->restoreGeometry(mSettings->getMainWindowGeometry());
    #endif

    QShortcut *speedReadFromClipBoardShortcut = new QShortcut(QKeySequence("Ctrl+R"), this);
    QObject::connect(speedReadFromClipBoardShortcut, &QShortcut::activated, this, &MainWindow::speedReadFromClipBoardShortcut);

    QShortcut *sShortcut = new QShortcut(QKeySequence("S"), this);
    QObject::connect(sShortcut, &QShortcut::activated, this, &MainWindow::sShortcut);

    QShortcut *cShortcut = new QShortcut(QKeySequence("C"), this);
    QObject::connect(cShortcut, &QShortcut::activated, this, &MainWindow::cShortcut);

    QShortcut *escapeShortcut = new QShortcut(QKeySequence("Esc"), this);
    QObject::connect(escapeShortcut, &QShortcut::activated, this, &MainWindow::escapeShortcut);
}

MainWindow::~MainWindow() {
    delete mSettings;
}

void MainWindow::setSpeedReadingText(QString text) {
    mUI->plainTextEdit->setPlainText(text);
}

void MainWindow::changedSlot(QClipboard::Mode mode) {
    if (mode == QClipboard::Clipboard && mUI->pasteWidget->isVisible() && !mSettingsWindow->isVisible()) this->setSpeedReadingText(mClipBoard->text(mode));
}

void MainWindow::updateSpeedReadLabelText(QString value) {
    QPixmap pixmap(mUI->label->width(), mUI->label->height());
    pixmap.fill(mSettings->getTextBackgroundColor());

    QPainter paint(&pixmap);

    QFont font(mSettings->getFontFamily());
    font.setPixelSize(mSettings->getFontSize());
    paint.setFont(font);
    paint.setPen(mSettings->getTextColor());

    QRectF textBoundaries;
    paint.drawText(mUI->label->rect(), Qt::AlignCenter, value, &textBoundaries);

    if (!value.isEmpty()) mOldTextBoundaries = textBoundaries;
    else textBoundaries = mOldTextBoundaries;

    const float size = 5.f;
    float x = (float) mUI->label->width() / 8.f;
    float width = (float) mUI->label->width() / 4.f * 3.f;
    paint.fillRect(x, textBoundaries.y() - size * 2, width, size, mSettings->getLinesColor());
    paint.fillRect(x, textBoundaries.y() + textBoundaries.height() + size * 2, width, size, mSettings->getLinesColor());

    mUI->label->setPixmap(pixmap);
}

void MainWindow::updateSpeedReaderText(QString text) {
    mSpeedReaderText.setText(text);
    this->updateStatusWidget();
}

void MainWindow::changed(QString text, int speedreadTextInPercent, SpeedReaderText::SpeedReaderStatus status) {
    if (status == SpeedReaderText::STOPPING) this->on_stopButton_clicked();
    else if (status == SpeedReaderText::FINISHED) this->on_cancelButton_clicked();
    else if (status == SpeedReaderText::SPEEDREADING) {
        this->updateSpeedReadLabelText(text);
        mUI->progressBar->setValue(speedreadTextInPercent);
    }
}

void MainWindow::createLanguageMenu() {
    QActionGroup* langGroup = new QActionGroup(mUI->menuLanguage);
    langGroup->setExclusive(true);

    QObject::connect(langGroup, &QActionGroup::triggered, this, &MainWindow::slotLanguageChanged);

    QList<Language> languages = mI18N->getLanguages();

    foreach (Language const& language, languages) {
        QAction* action = new QAction(QIcon(language.iconFilePath), language.name, langGroup);
        action->setCheckable(true);
        action->setData(language.locale);

        mUI->menuLanguage->addAction(action);

        if (language.isDefault) {
            action->setChecked(true);
            this->slotLanguageChanged(action);
        }
    }
}

void MainWindow::slotLanguageChanged(QAction* action) {
    if (0 != action) {
        QString locale = action->data().toString();
        mI18N->loadLanguage(locale);
    }
}

void MainWindow::changeEvent(QEvent* event) {
    if (0 != event) {
        switch(event->type()) {
            case QEvent::LanguageChange: // this event is send if a translator is loaded (will be in I18N class)
                mUI->retranslateUi(this);
                mSettingsWindow->retranslate();
                break;
            case QEvent::LocaleChange: // this event is send, if the system language changes
                mI18N->onSystemLocalChanged();
                break;
            default:
                break;
        }
    }

    QMainWindow::changeEvent(event);
}

void MainWindow::closeEvent(QCloseEvent *event) {
    mSettings->setMainWindowGeometry(this->saveGeometry());

    QMainWindow::closeEvent(event);
}

void MainWindow::on_actionReportABug_triggered() {
    QDesktopServices::openUrl(QUrl("mailto:niklas.baudy@vanniktech.de?subject=[Bug report] - SpeedReader (" + OS + ")"));
}

void MainWindow::on_actionSendFeedback_triggered() {
    QDesktopServices::openUrl(QUrl("mailto:niklas.baudy@vanniktech.de?subject=[Feedback] - SpeedReader (" + OS + ")"));
}

void MainWindow::on_actionSpeedReader_triggered() {
    mSettingsWindow->show();
    mSettingsWindow->raise();
    mSettingsWindow->activateWindow();
}

void MainWindow::on_actionAboutSpeedReader_triggered() {
    QMessageBox msgBox;
    msgBox.setTextFormat(Qt::RichText);
    msgBox.setText(QString("SpeedReader %1<br>%2 Vanniktech<br>Copyright 2014 - 2015 Vanniktech - Niklas Baudy<br><a href=\"%3\">%4</a>").arg(APPLICATION_VERSION, tr("Published by"), coSiteSpeedReader, tr("More information")));
    msgBox.exec();
}

void MainWindow::on_actionForkMeOnGitHub_triggered() {
    QDesktopServices::openUrl(QUrl(coSiteGitHub));
}

void MainWindow::speedReadFromClipBoardShortcut() {
    this->setSpeedReadingText(QApplication::clipboard()->text());
    this->on_speedreadButton_clicked();
}

void MainWindow::sShortcut() {
    if (mUI->stackedWidget->currentIndex() == 0 && mUI->speedreadButton->isVisible()) this->on_speedreadButton_clicked();
    else if (mUI->stackedWidget->currentIndex() == 1 && mUI->stopButton->isVisible()) this->on_stopButton_clicked();
}

void MainWindow::cShortcut() {
    if (mUI->stackedWidget->currentIndex() == 0 && mUI->continueButton->isVisible()) this->on_continueButton_clicked();
    else if (mUI->stackedWidget->currentIndex() == 1 && mUI->cancelButton->isVisible()) this->on_cancelButton_clicked();
}

void MainWindow::escapeShortcut() {
    if (mUI->stackedWidget->currentIndex() == 1 && mUI->cancelButton->isVisible()) this->on_cancelButton_clicked();
}

void MainWindow::on_stopButton_clicked() {
    if (mSpeedReaderText.stopSpeedReading()) {
        mUI->continueButton->setVisible(true);
        mUI->speedreadButton->setVisible(false);
        mUI->stackedWidget->setCurrentIndex(0);
        this->updateStatusWidget();

        mUI->actionSpeedReader->setEnabled(true);
    }
}

void MainWindow::on_cancelButton_clicked() {
    mSpeedReaderText.terminate();

    mUI->stackedWidget->setCurrentIndex(0);

    mUI->actionSpeedReader->setEnabled(true);
    this->updateStatusWidget();
}

void MainWindow::on_continueButton_clicked() {
    mUI->stackedWidget->setCurrentIndex(1);
    mUI->continueButton->setVisible(false);
    mUI->speedreadButton->setVisible(true);

    mSpeedReaderText.continueSpeedReading();
}

void MainWindow::on_speedreadButton_clicked() {
    QString text = mUI->plainTextEdit->toPlainText();
    if (text.isEmpty()) text = QApplication::clipboard()->text();

    this->speedreadText(text);
}

void MainWindow::on_plainTextEdit_textChanged() {
    QString plainText = mUI->plainTextEdit->toPlainText();

    if (plainText != mSpeedReaderText.getText()) {
        this->updateSpeedReaderText(mUI->plainTextEdit->toPlainText());
        mUI->continueButton->setVisible(false);
        mUI->speedreadButton->setVisible(true);
    }
}

void MainWindow::speedreadText(QString text) {
    this->updateSpeedReaderText(text);

    mUI->actionSpeedReader->setEnabled(false);
    mUI->stackedWidget->setCurrentIndex(1);
    mUI->progressBar->setValue(0);

    mSpeedReaderText.continueSpeedReading();
}

void MainWindow::updateStatusWidget() {
    mUI->textEdit->setHtml(mSpeedReaderText.getHTMLColoredText());
    mUI->textEdit->scrollToAnchor(SpeedReaderText::HTML_ANCHOR_CURRENT_WORD);

    mUI->segmentsStatusLabel->setText(QString("%1 / %2 %3").arg(QString::number(mSpeedReaderText.getIndex()), QString::number(mSpeedReaderText.getMaxIndex()), tr("segments SpeedRead")));
    mUI->stopWordsStatusLabel->setText(QString("%1 / %2 %3").arg(QString::number(mSpeedReaderText.getStopWordsRead()), QString::number(mSpeedReaderText.getStopWordsCount()), tr("stopwords SpeedRead")));
    mUI->estimatedRemainingSpeedReadingTimeLabel->setText(QString("%1 %2").arg(tr("Estimated remaining SpeedReading time:"), VNTFormatter::formatMilliseconds(mSpeedReaderText.getEstimatedTimeInMS())));
}
