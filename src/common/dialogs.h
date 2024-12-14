// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2024 jo-hannes <jo-hannes@dev-urandom.de>

#ifndef DIALOGS_H
#define DIALOGS_H

#include <QMessageBox>
#include <QString>
#include <QWidget>

namespace Dialogs {

/**
 * @brief Ask user what to do with unsaved changes
 * @param text Main text displayed bold
 * @param infoText Additional informative text
 * @return QMessageBox::Save    User clicked "Save"
 * @return QMessageBox::Discard User clicked "Don't Save"
 * @return QMessageBox::Cancel  User clicked "Cancel"
 */
int saveChanges(const QString& text, const QString& infoText, QWidget* parent = nullptr);

/**
 * @brief Show information box with OK button only
 * @param text Main text displayed bold
 * @param infoText Additional informative text
 */
void info(const QString& text, const QString& infoText, QWidget* parent = nullptr);

/**
 * @brief Ask user question with yes and no as possible answer
 * @param text Main text displayed bold
 * @param infoText Additional informative text
 * @return QMessageBox::Yes User clicked "Yes"
 * @return QMessageBox::No  User clicked "No"
 */
int yesNo(const QString& text, const QString& infoText, QWidget* parent = nullptr);

static const int imgSize = 128; /**< @brief Size of image in dialogs */

};  // namespace Dialogs

#endif  // DIALOGS_H
