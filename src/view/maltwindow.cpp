#include "maltwindow.h"

#include <QDialogButtonBox>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QPushButton>
#include <QStandardPaths>
#include <QVBoxLayout>

MaltWindow::MaltWindow(MainModel* model, QWidget* parent) : QWidget{parent} {
  this->model = model;
  volatileMalts = new Malts();
  volatileMalts->setMalts(model->malts->getMalts());
  // Window tittle
  title = tr("Malze");
  this->setWindowTitle(title);
  QObject::connect(volatileMalts, &Malts::unsavedMalts, this, &MaltWindow::unsavedMalts);

  // main layout
  QVBoxLayout* mainLayout = new QVBoxLayout(this);

  // Table View
  maltsView = new QTableView();
  maltsView->setModel(volatileMalts);
  maltsView->verticalHeader()->setVisible(false);
  maltsView->hideColumn(1);  // Hide mass column, not needed here
  mainLayout->addWidget(maltsView);

  // Buttons
  QPushButton* btnAdd = new QPushButton();
  btnAdd->setToolTip(tr("Malz hinzufügen"));
  btnAdd->setIcon(QIcon(":/icons/document-plus.svg"));
  QObject::connect(btnAdd, &QPushButton::clicked, this, &MaltWindow::maltAdd);
  QPushButton* btnCopy = new QPushButton();
  btnCopy->setToolTip(tr("Malz kopieren"));
  btnCopy->setIcon(QIcon(":/icons/document-duplicate.svg"));
  QObject::connect(btnCopy, &QPushButton::clicked, this, &MaltWindow::maltCopy);
  QPushButton* btnDelete = new QPushButton();
  btnDelete->setToolTip(tr("Malz löschen"));
  btnDelete->setIcon(QIcon(":/icons/document-minus.svg"));
  QObject::connect(btnDelete, &QPushButton::clicked, this, &MaltWindow::maltDelete);
  QPushButton* btnImport = new QPushButton();
  btnImport->setToolTip(tr("Malz importieren"));
  btnImport->setIcon(QIcon(":/icons/document-arrow-down.svg"));
  QObject::connect(btnImport, &QPushButton::clicked, this, &MaltWindow::maltImport);
  QPushButton* btnExport = new QPushButton();
  btnExport->setToolTip(tr("Malz exportieren"));
  btnExport->setIcon(QIcon(":/icons/document-arrow-up.svg"));
  QObject::connect(btnExport, &QPushButton::clicked, this, &MaltWindow::maltExport);
  QPushButton* btnCancle = new QPushButton();
  btnCancle->setToolTip(tr("Abbrechen"));
  btnCancle->setIcon(QIcon(":/icons/archive-box-x-mark.svg"));
  QObject::connect(btnCancle, &QPushButton::clicked, this, &MaltWindow::cancel);
  QPushButton* btnSave = new QPushButton();
  btnSave->setToolTip(tr("Speichern"));
  btnSave->setIcon(QIcon(":/icons/archive-box.svg"));
  QObject::connect(btnSave, &QPushButton::clicked, this, &MaltWindow::save);

  QDialogButtonBox* buttons = new QDialogButtonBox();
  buttons->addButton(btnAdd, QDialogButtonBox::ActionRole);
  buttons->addButton(btnCopy, QDialogButtonBox::ActionRole);
  buttons->addButton(btnDelete, QDialogButtonBox::ActionRole);
  buttons->addButton(btnImport, QDialogButtonBox::ActionRole);
  buttons->addButton(btnExport, QDialogButtonBox::ActionRole);
  buttons->addButton(btnCancle, QDialogButtonBox::ActionRole);
  buttons->addButton(btnSave, QDialogButtonBox::ActionRole);
  mainLayout->addWidget(buttons);
}

void MaltWindow::maltAdd() {
  Malt m;
  volatileMalts->addMalt(m);
}

void MaltWindow::maltCopy() {
  QModelIndex idx = maltsView->currentIndex();
  if (!idx.isValid()) {
    return;
  }
  Malt m = volatileMalts->getMalt(idx.row());
  m.setName("Copy of " + m.getName());
  volatileMalts->addMalt(m);
}

void MaltWindow::maltDelete() {
  QModelIndex idx = maltsView->currentIndex();
  if (!idx.isValid()) {
    return;
  }
  volatileMalts->deleteMalt(idx.row());
}

void MaltWindow::maltImport() {
  QString path = QFileDialog::getOpenFileName(this, tr("Malz Importieren"),
                                              QStandardPaths::writableLocation(QStandardPaths::HomeLocation),
                                              tr("JSON (*.json);; Any (*.*)"));
  if (path.isEmpty()) {
    return;
  }
  if (!volatileMalts->importMalt(path)) {
    QMessageBox msgBox;
    msgBox.setText(tr("Konnte Malz nicht im JSON finden"));
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.exec();
  }
}

void MaltWindow::maltExport() {
  QModelIndex idx = maltsView->currentIndex();
  if (!idx.isValid()) {
    return;
  }
  QString suggestedFileName = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/" +
                              volatileMalts->getMalt(idx.row()).getName() + ".json";
  QString path = QFileDialog::getSaveFileName(this, tr("Malz Exportieren"), suggestedFileName, tr("JSON (*.json)"));
  if (path.isEmpty()) {
    return;
  }
  if (!volatileMalts->exportMalt(path, idx.row())) {
    QMessageBox msgBox;
    msgBox.setText(tr("Konnte Malz nicht speichern"));
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.exec();
  }
}

void MaltWindow::save() {
  model->malts->setMalts(volatileMalts->getMalts());
  volatileMalts->setSaved();
  model->saveMalts();
}

void MaltWindow::cancel() {
  volatileMalts->setMalts(model->malts->getMalts());
}

void MaltWindow::unsavedMalts(bool unsaved) {
  if (unsaved) {
    setWindowTitle("* " + title);
  } else {
    setWindowTitle(title);
  }
  emit maltWindowUnsavedChanges(unsaved);
}
