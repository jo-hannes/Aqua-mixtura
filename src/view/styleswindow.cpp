// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2023 jo-hannes <jo-hannes@dev-urandom.de>

#include "styleswindow.h"

#include <QDialogButtonBox>
#include <QFileDialog>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QItemSelectionModel>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QSizePolicy>
#include <QStandardPaths>
#include <QVBoxLayout>

StylesWindow::StylesWindow(MainModel* model, QWidget* parent) : QWidget{parent} {
  this->model = model;
  selected = -1;

  // mainLayout
  QGridLayout* mainLayout = new QGridLayout();

  // Headlines
  QLabel* txtStyles = new QLabel("<b>" + tr("Bierstile") + "</b>");
  mainLayout->addWidget(txtStyles, 0, 0, Qt::AlignLeft);
  QLabel* txtValues = new QLabel("<b>" + tr("Werte") + "</b>");
  mainLayout->addWidget(txtValues, 0, 1, Qt::AlignLeft);

  // List/Table views
  stylesView = new QListView();
  stylesView->setModel(model->styles);
  stylesView->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
  QItemSelectionModel* selectionModel = stylesView->selectionModel();
  QObject::connect(selectionModel, &QItemSelectionModel::currentChanged, this, &StylesWindow::styleSelectionChanged);

  mainLayout->addWidget(stylesView, 1, 0);
  styleTableView = new QTableView();
  styleTableView->setModel(model->styles->getStyle(0));
  mainLayout->addWidget(styleTableView, 1, 1);

  // Buttons
  QPushButton* btnAdd = new QPushButton();
  btnAdd->setToolTip(tr("Bierstil hinzufügen"));
  btnAdd->setIcon(QIcon(":/icons/document-plus.svg"));
  QObject::connect(btnAdd, &QPushButton::clicked, this, &StylesWindow::styleAdd);
  QPushButton* btnCopy = new QPushButton();
  btnCopy->setToolTip(tr("Bierstil kopieren"));
  btnCopy->setIcon(QIcon(":/icons/document-duplicate.svg"));
  QObject::connect(btnCopy, &QPushButton::clicked, this, &StylesWindow::styleCopy);
  QPushButton* btnDelete = new QPushButton();
  btnDelete->setToolTip(tr("Bierstil löschen"));
  btnDelete->setIcon(QIcon(":/icons/document-minus.svg"));
  QObject::connect(btnDelete, &QPushButton::clicked, this, &StylesWindow::styleDelete);
  QPushButton* btnImport = new QPushButton();
  btnImport->setToolTip(tr("Bierstil importieren"));
  btnImport->setIcon(QIcon(":/icons/document-arrow-down.svg"));
  QObject::connect(btnImport, &QPushButton::clicked, this, &StylesWindow::styleImport);
  QPushButton* btnExport = new QPushButton();
  btnExport->setToolTip(tr("Bierstil exportieren"));
  btnExport->setIcon(QIcon(":/icons/document-arrow-up.svg"));
  QObject::connect(btnExport, &QPushButton::clicked, this, &StylesWindow::styleExport);
  QPushButton* btnCancle = new QPushButton();
  btnCancle->setToolTip(tr("Abbrechen"));
  btnCancle->setIcon(QIcon(":/icons/archive-box-x-mark.svg"));
  QObject::connect(btnCancle, &QPushButton::clicked, this, &StylesWindow::styleDiscard);
  QPushButton* btnSave = new QPushButton();
  btnSave->setToolTip(tr("Speichern"));
  btnSave->setIcon(QIcon(":/icons/archive-box.svg"));
  QObject::connect(btnSave, &QPushButton::clicked, this, &StylesWindow::styleSave);

  QDialogButtonBox* buttons = new QDialogButtonBox();
  buttons->addButton(btnAdd, QDialogButtonBox::ActionRole);
  buttons->addButton(btnCopy, QDialogButtonBox::ActionRole);
  buttons->addButton(btnDelete, QDialogButtonBox::ActionRole);
  buttons->addButton(btnImport, QDialogButtonBox::ActionRole);
  buttons->addButton(btnExport, QDialogButtonBox::ActionRole);
  buttons->addButton(btnCancle, QDialogButtonBox::ActionRole);
  buttons->addButton(btnSave, QDialogButtonBox::ActionRole);
  mainLayout->addWidget(buttons, 2, 0, 1, 2, Qt::AlignHCenter);

  mainLayout->setColumnStretch(1, 1);

  setLayout(mainLayout);
}

void StylesWindow::styleSelectionChanged(const QModelIndex& current, const QModelIndex& previous) {
  Q_UNUSED(previous);
  styleSelect(current.row());
}

void StylesWindow::styleSave() {
  model->saveStyles();
}

void StylesWindow::styleDiscard() {
  model->loadStyles();
}

void StylesWindow::styleAdd() {
  model->styles->addStyle(new Style(tr("New")));
  // Select new style = last style
  styleSelect(model->styles->rowCount() - 1);
}

void StylesWindow::styleCopy() {
  // Qt objects can't be copied, so we need to do this by our own
  Style* copy = model->styles->getStyle(selected)->copy();
  copy->setName(tr("Copy of ") + copy->getName());
  model->styles->addStyle(copy);
  // Select copied style = last style
  styleSelect(model->styles->rowCount() - 1);
}

void StylesWindow::styleDelete() {
  model->styles->deleteStyle(selected);
}

void StylesWindow::styleImport() {
  QString path = QFileDialog::getOpenFileName(this, tr("Bierstil Importieren"),
                                              QStandardPaths::writableLocation(QStandardPaths::HomeLocation),
                                              tr("JSON (*.json);; Any (*.*)"));
  if (path.isEmpty()) {
    return;
  }
  if (model->styles->importStyle(path)) {
    // Select imported style = last style
    styleSelect(model->styles->rowCount() - 1);
  } else {
    QMessageBox msgBox;
    msgBox.setText(tr("Konnte Bierstil nicht im JSON finden"));
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.exec();
  }
}

void StylesWindow::styleExport() {
  QString suggestedFileName = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/" +
                              model->styles->getStyle(selected)->getName() + ".json";
  QString path = QFileDialog::getSaveFileName(this, tr("Bierstil Exportieren"), suggestedFileName, tr("JSON (*.json)"));
  if (path.isEmpty()) {
    return;
  }
  if (!model->styles->exportStyle(path, selected)) {
    QMessageBox msgBox;
    msgBox.setText(tr("Konnte Bierstil nicht exportieren"));
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.exec();
  }
}

void StylesWindow::styleSelect(const qsizetype index) {
  if (selected == index) {  // same row selected again => do noting
    return;
  }
  selected = index;
  stylesView->setCurrentIndex(stylesView->model()->index(selected, 0));
  styleTableView->setModel(model->styles->getStyle(selected));
}
