// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2024 jo-hannes <jo-hannes@dev-urandom.de>

#include "dialogs.h"

int Dialogs::saveChanges(const QString& text, const QString& infoText) {
  QMessageBox msgBox;
  msgBox.setText(text);
  msgBox.setInformativeText(infoText);
  msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
  msgBox.setDefaultButton(QMessageBox::Cancel);
  msgBox.setIconPixmap(QPixmap(":/icons/logo_512x512.png"));
  return msgBox.exec();
}

void Dialogs::info(const QString& text, const QString& infoText) {
  QMessageBox msgBox;
  msgBox.setText(text);
  msgBox.setInformativeText(infoText);
  msgBox.setStandardButtons(QMessageBox::Ok);
  msgBox.setDefaultButton(QMessageBox::Ok);
  msgBox.setIconPixmap(QPixmap(":/icons/logo_512x512.png"));
  msgBox.exec();
}

int Dialogs::yesNo(const QString& text, const QString& infoText) {
  QMessageBox msgBox;
  msgBox.setText(text);
  msgBox.setInformativeText(infoText);
  msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
  msgBox.setDefaultButton(QMessageBox::No);
  msgBox.setIconPixmap(QPixmap(":/icons/logo_512x512.png"));
  return msgBox.exec();
}
