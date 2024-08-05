// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2023 - 2024 jo-hannes <jo-hannes@dev-urandom.de>

#include "styleswindow.h"

#include "styletabledelegate.h"

#include "../common/buttons.h"

#include <QDialogButtonBox>
#include <QFileDialog>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QItemSelectionModel>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QSizePolicy>
#include <QStandardPaths>
#include <QVBoxLayout>

StylesWindow::StylesWindow(Styles& model, QWidget* parent) : QWidget{parent}, styles{model} {
  selected = -1;

  // mainLayout
  QGridLayout* mainLayout = new QGridLayout();

  // Headlines
  setWindowTitle("Aqua mixtura - " + tr("Bierstile"));
  QLabel* txtStyles = new QLabel("<b>" + tr("Bierstile") + "</b>");
  mainLayout->addWidget(txtStyles, 0, 0, Qt::AlignLeft);
  QLabel* txtValues = new QLabel("<b>" + tr("Werte") + "</b>");
  mainLayout->addWidget(txtValues, 0, 1, Qt::AlignLeft);

  // List/Table views
  stylesView = new QListView();
  stylesView->setModel(&styles);
  stylesView->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
  QItemSelectionModel* selectionModel = stylesView->selectionModel();
  QObject::connect(selectionModel, &QItemSelectionModel::currentChanged, this, &StylesWindow::styleSelectionChanged);

  mainLayout->addWidget(stylesView, 1, 0);
  styleTableView = new QTableView();
  styleTableView->setModel(styles.getStyle(0));
  StyleTableDelegate* delegate = new StyleTableDelegate(this);
  styleTableView->setItemDelegate(delegate);
  styleTableView->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContentsOnFirstShow);
  styleTableView->verticalHeader()->setDefaultAlignment(Qt::AlignRight);
  mainLayout->addWidget(styleTableView, 1, 1);

  // Buttons
  Buttons* buttons =
      new Buttons(tr("Bierstil hinzufügen"), tr("Bierstil kopieren"), tr("Bierstil löschen"),
                  tr("Bierstil importieren"), tr("Bierstil exportieren"), tr("Speichern"), tr("Abbrechen"));
  QObject::connect(buttons->btnAdd, &QPushButton::clicked, this, &StylesWindow::styleAdd);
  QObject::connect(buttons->btnCopy, &QPushButton::clicked, this, &StylesWindow::styleCopy);
  QObject::connect(buttons->btnDelete, &QPushButton::clicked, this, &StylesWindow::styleDelete);
  QObject::connect(buttons->btnImport, &QPushButton::clicked, this, &StylesWindow::styleImport);
  QObject::connect(buttons->btnExport, &QPushButton::clicked, this, &StylesWindow::styleExport);
  QObject::connect(buttons->btnSave, &QPushButton::clicked, &styles, &Styles::save);
  QObject::connect(buttons->btnCancel, &QPushButton::clicked, &styles, &Styles::load);

  mainLayout->addWidget(buttons, 2, 0, 1, 2, Qt::AlignHCenter);

  mainLayout->setColumnStretch(1, 1);

  setLayout(mainLayout);
}

void StylesWindow::styleSelectionChanged(const QModelIndex& current, const QModelIndex& previous) {
  Q_UNUSED(previous);
  styleSelect(current.row());
}

void StylesWindow::styleAdd() {
  styles.addStyle(new Style(tr("New")));
  // Select new style = last style
  styleSelect(styles.rowCount() - 1);
}

void StylesWindow::styleCopy() {
  // Qt objects can't be copied, so we need to do this by our own
  Style* copy = styles.getStyle(selected)->copy();
  copy->updateCreationTime();
  copy->setName(tr("Copy of ") + copy->getName());
  styles.addStyle(copy);
  // Select copied style = last style
  styleSelect(styles.rowCount() - 1);
}

void StylesWindow::styleDelete() {
  styles.deleteStyle(selected);
}

void StylesWindow::styleImport() {
  QString path = QFileDialog::getOpenFileName(this, tr("Bierstil Importieren"),
                                              QStandardPaths::writableLocation(QStandardPaths::HomeLocation),
                                              tr("JSON (*.json);; Any (*.*)"));
  if (path.isEmpty()) {
    return;
  }
  if (styles.importStyle(path)) {
    // Select imported style = last style
    styleSelect(styles.rowCount() - 1);
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
                              styles.getStyle(selected)->getName() + ".json";
  QString path = QFileDialog::getSaveFileName(this, tr("Bierstil Exportieren"), suggestedFileName, tr("JSON (*.json)"));
  if (path.isEmpty()) {
    return;
  }
  if (!styles.exportStyle(path, selected)) {
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
  styleTableView->setModel(styles.getStyle(selected));
}
