// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2023 jo-hannes <jo-hannes@dev-urandom.de>

#include "mainwindow.h"

#include <QApplication>
#include <QIcon>
#include <QLibraryInfo>
#include <QLocale>
#include <QTranslator>

int main(int argc, char* argv[]) {
  const QApplication a(argc, argv);

  a.setOrganizationName("jo-hannes");
  a.setApplicationName("Aqua-mixtura");
  a.setWindowIcon(QIcon(":/icons/logo_512x512.png"));

  QTranslator trMyApp;
  QTranslator trQt;
  const QStringList uiLanguages = QLocale::system().uiLanguages();
  for (const QString& locale : uiLanguages) {
    const QString languageCode = QLocale::languageToCode(QLocale(locale).language());
    if (trMyApp.load(":/i18n/Aqua-mixtura_" + languageCode)) {
      a.installTranslator(&trMyApp);
      // Also load qt translator
      if (trQt.load("qt_" + languageCode, QLibraryInfo::path(QLibraryInfo::TranslationsPath))) {
        a.installTranslator(&trQt);
      }
      break;
    }
  }
  MainWindow w;
  w.show();
  return a.exec();
}
