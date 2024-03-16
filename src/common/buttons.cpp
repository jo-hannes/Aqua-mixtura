// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2024 jo-hannes <jo-hannes@dev-urandom.de>

#include "buttons.h"

Buttons::Buttons(const QString& save, const QString& cancle) {
  genBtnsSaveCancle(save, cancle);
}

Buttons::Buttons(const QString& add, const QString& copy, const QString& del, const QString& import,
                 const QString& exprt) {
  genBtnsListEdit(add, copy, del, import, exprt);
}

Buttons::Buttons(const QString& add, const QString& copy, const QString& del, const QString& import,
                 const QString& exprt, const QString& save, const QString& cancle) {
  // Only create buttons with text
  if (!add.isEmpty()) {
    btnAdd = new QPushButton();
    btnAdd->setToolTip(add);
    btnAdd->setIcon(QIcon(":/icons/document-plus.svg"));
    addButton(btnAdd, QDialogButtonBox::ActionRole);
  }
  if (!copy.isEmpty()) {
    btnCopy = new QPushButton();
    btnCopy->setToolTip(copy);
    btnCopy->setIcon(QIcon(":/icons/document-duplicate.svg"));
    addButton(btnCopy, QDialogButtonBox::ActionRole);
  }
  if (!del.isEmpty()) {
    btnDelete = new QPushButton();
    btnDelete->setToolTip(del);
    btnDelete->setIcon(QIcon(":/icons/document-minus.svg"));
    addButton(btnDelete, QDialogButtonBox::ActionRole);
  }
  if (!import.isEmpty()) {
    btnImport = new QPushButton();
    btnImport->setToolTip(import);
    btnImport->setIcon(QIcon(":/icons/arrow-down-on-square.svg"));
    addButton(btnImport, QDialogButtonBox::ActionRole);
  }
  if (!exprt.isEmpty()) {
    btnExport = new QPushButton();
    btnExport->setToolTip(exprt);
    btnExport->setIcon(QIcon(":/icons/arrow-up-on-square.svg"));
    addButton(btnExport, QDialogButtonBox::ActionRole);
  }
  if (!save.isEmpty()) {
    btnSave = new QPushButton();
    btnSave->setToolTip(save);
    btnSave->setIcon(QIcon(":/icons/check.svg"));
    addButton(btnSave, QDialogButtonBox::ActionRole);
  }
  if (!cancle.isEmpty()) {
    btnCancel = new QPushButton();
    btnCancel->setToolTip(cancle);
    btnCancel->setIcon(QIcon(":/icons/x-mark.svg"));
    addButton(btnCancel, QDialogButtonBox::ActionRole);
  }
}

void Buttons::genBtnsSaveCancle(const QString& save, const QString& cancle) {
  btnSave = new QPushButton();
  btnSave->setToolTip(save);
  btnSave->setIcon(QIcon(":/icons/check.svg"));
  btnCancel = new QPushButton();
  btnCancel->setToolTip(cancle);
  btnCancel->setIcon(QIcon(":/icons/x-mark.svg"));
  addButton(btnSave, QDialogButtonBox::ActionRole);
  addButton(btnCancel, QDialogButtonBox::ActionRole);
}

void Buttons::genBtnsListEdit(const QString& add, const QString& copy, const QString& del, const QString& import,
                              const QString& exprt) {
  btnAdd = new QPushButton();
  btnAdd->setToolTip(add);
  btnAdd->setIcon(QIcon(":/icons/document-plus.svg"));
  btnCopy = new QPushButton();
  btnCopy->setToolTip(copy);
  btnCopy->setIcon(QIcon(":/icons/document-duplicate.svg"));
  btnDelete = new QPushButton();
  btnDelete->setToolTip(del);
  btnDelete->setIcon(QIcon(":/icons/document-minus.svg"));
  btnImport = new QPushButton();
  btnImport->setToolTip(import);
  btnImport->setIcon(QIcon(":/icons/arrow-down-on-square.svg"));
  btnExport = new QPushButton();
  btnExport->setToolTip(exprt);
  btnExport->setIcon(QIcon(":/icons/arrow-up-on-square.svg"));
  addButton(btnAdd, QDialogButtonBox::ActionRole);
  addButton(btnCopy, QDialogButtonBox::ActionRole);
  addButton(btnDelete, QDialogButtonBox::ActionRole);
  addButton(btnImport, QDialogButtonBox::ActionRole);
  addButton(btnExport, QDialogButtonBox::ActionRole);
}
