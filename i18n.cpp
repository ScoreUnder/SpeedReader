#include "i18n.h"

#include <QDir>
#include <QLocale>
#include <QApplication>

#include "initializableqmap.h"

const QString INT_LANG_DIR = ":/languages/";

I18N::I18N(QObject *parent) : QObject(parent)
{
}

QList<Language> I18N::getLanguages() {
    QList<Language> languages;

    QString defaultLocale = this->getDefaultLocale();

    QDir dir(INT_LANG_DIR);
    QStringList fileNames = dir.entryList(QStringList("*.qm"));

    for (int i = 0; i < fileNames.size(); ++i) {
        QString locale = fileNames[i];
        locale.truncate(locale.lastIndexOf('.'));

        QString lang = QLocale::languageToString(QLocale(locale).language());

        Language language;
        language.iconFilePath = QString("%1%2.png").arg(INT_LANG_DIR).arg(locale);
        language.locale = locale;
        language.isDefault = defaultLocale == locale;
        language.name = lang;
        languages.append(language);
    }

    return languages;
}

QString I18N::getDefaultLocale() {
    QString defaultLocale = QLocale::system().name();
    defaultLocale.truncate(defaultLocale.lastIndexOf('_'));
    return defaultLocale;
}

void I18N::loadLanguage(const QString& language) {
    if (mCurrentLanguage != language) {
        mCurrentLanguage = language;
        QLocale locale = QLocale(mCurrentLanguage);
        QLocale::setDefault(locale);
        switchTranslator(mTranslator, QString("%1.qm").arg(language));
    }
}

void I18N::switchTranslator(QTranslator& translator, const QString& filename) {
    qApp->removeTranslator(&translator);

    if (translator.load(filename, INT_LANG_DIR)) qApp->installTranslator(&translator);
}

void I18N::onSystemLocalChanged() {
    this->loadLanguage(this->getDefaultLocale());
}
