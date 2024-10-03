// SPDX-License-Identifier: GPL-2.0-only
// Copyright (c) 2023 - 2024 jo-hannes <jo-hannes@dev-urandom.de>

#include "styleswindow.h"

#include "styletabledelegate.h"

#include "../common/buttons.h"
#include "../common/dialogs.h"

#include <QDialogButtonBox>
#include <QFileDialog>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QItemSelectionModel>
#include <QLabel>
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

  // List view
  stylesView = new QListView();
  stylesView->setModel(&styles);
  QObject::connect(stylesView, &QListView::pressed, this, &StylesWindow::styleSelect);
  mainLayout->addWidget(stylesView, 1, 0, 2, 1);
  // Name edit
  mainLayout->addWidget(new QLabel(tr("Name") + ":"), 1, 1, Qt::AlignLeft);
  nameEdit = new QLineEdit();
  QObject::connect(nameEdit, &QLineEdit::textEdited, this, &StylesWindow::setName);
  mainLayout->addWidget(nameEdit, 1, 2);
  // Table view
  styleTableView = new QTableView();
  styleTableView->setModel(styles.getStyle(0));
  StyleTableDelegate* delegate = new StyleTableDelegate(this);
  styleTableView->setItemDelegate(delegate);
  styleTableView->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContentsOnFirstShow);
  styleTableView->verticalHeader()->setDefaultAlignment(Qt::AlignRight);
  mainLayout->addWidget(styleTableView, 2, 1, 1, 2);

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
  QObject::connect(buttons->btnCancel, &QPushButton::clicked, this, &StylesWindow::stylesLoad);

  mainLayout->addWidget(buttons, 3, 0, 1, 3, Qt::AlignHCenter);

  mainLayout->setColumnStretch(2, 1);

  setLayout(mainLayout);

  if (styles.rowCount() >= 1) {
    styleSelect(stylesView->model()->index(0, 0));
  }
}

void StylesWindow::closeEvent(QCloseEvent* event) {
  // check for changes
  if (saveChangesDialog() == QMessageBox::Cancel) {
    event->ignore();
  } else {
    event->accept();
  }
}

void StylesWindow::setName(QString name) {
  styles.getStyle(selected)->setName(name);
  styles.getStyle(selected)->setChanged(true);
  stylesView->update(stylesView->currentIndex());
}

void StylesWindow::styleAdd() {
  // check for changes first
  if (saveChangesDialog() == QMessageBox::Cancel) {
    return;  // user cancelation => do nothing
  }
  styles.addStyle(new Style(tr("New")));
  // Select new style = last style
  styleSelect(stylesView->model()->index(styles.rowCount() - 1, 0));
}

void StylesWindow::styleCopy() {
  // check for changes first
  if (saveChangesDialog() == QMessageBox::Cancel) {
    return;  // user cancelation => do nothing
  }
  // Qt objects can't be copied, so we need to do this by our own
  Style* copy = styles.getStyle(selected)->copy();
  copy->updateCreationTime();
  copy->setName(tr("Copy of ") + copy->getName());
  styles.addStyle(copy);
  // Select copied style = last style
  styleSelect(stylesView->model()->index(styles.rowCount() - 1, 0));
}

void StylesWindow::styleDelete() {
  styles.deleteStyle(selected);
}

void StylesWindow::styleImport() {
  // check for changes first
  if (saveChangesDialog() == QMessageBox::Cancel) {
    return;  // user cancelation => do nothing
  }
  QString path = QFileDialog::getOpenFileName(this, tr("Bierstil importieren"),
                                              QStandardPaths::writableLocation(QStandardPaths::HomeLocation),
                                              tr("JSON (*.json);; Any (*.*)"));
  if (path.isEmpty()) {
    return;
  }
  if (styles.importStyle(path)) {
    // Select imported style = last style
    styleSelect(stylesView->model()->index(styles.rowCount() - 1, 0));
  } else {
    Dialogs::info(tr("Fehler beim Importieren"), tr("Konnte Bierstil nicht im JSON finden"));
  }
}

void StylesWindow::styleExport() {
  // check for changes first
  if (saveChangesDialog() == QMessageBox::Cancel) {
    return;  // user cancelation => do nothing
  }
  QString suggestedFileName = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/" +
                              styles.getStyle(selected)->getName() + ".json";
  QString path = QFileDialog::getSaveFileName(this, tr("Bierstil exportieren"), suggestedFileName, "JSON (*.json)");
  if (path.isEmpty()) {
    return;
  }
  if (!styles.exportStyle(path, selected)) {
    Dialogs::info(tr("Fehler beim Exportieren"), tr("Konnte Bierstil nicht speichern"));
  }
}

void StylesWindow::stylesLoad() {
  styles.load();
  if (styles.rowCount() >= 1) {
    selected = -1;  // force re selection of style
    styleSelect(stylesView->model()->index(0, 0));
  } else {
    selected = -1;
  }
}

void StylesWindow::styleSelect(const QModelIndex& index) {
  if (selected == index.row()) {  // same row selected again => do noting
    return;
  }
  // check for changes first
  if (saveChangesDialog() == QMessageBox::Cancel) {
    // Select previously selected item
    stylesView->setCurrentIndex(stylesView->model()->index(selected, 0));
  } else {
    // Just switch to new selection
    selected = index.row();
    stylesView->setCurrentIndex(stylesView->model()->index(selected, 0));
    styleTableView->setModel(styles.getStyle(selected));
    nameEdit->setText(styles.getStyle(selected)->getName());
  }
}

int StylesWindow::saveChangesDialog() {
  if (styles.isChanged() || styles.getStyle(selected)->isChanged()) {
    int ret = Dialogs::saveChanges(
        tr("Änderungen speichern?"),
        tr("Bierstil \"%1\" hat ungespeicherte Änderungen").arg(styles.getStyle(selected)->getName()));
    //  save or discard
    switch (ret) {
      case QMessageBox::Save:
        styles.save();
        break;
      case QMessageBox::Discard:
        styles.load();  // discard changes
        break;
    }
    return ret;
  } else {
    return 0;
  }
}
