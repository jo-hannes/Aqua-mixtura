#ifndef BUTTONS_H
#define BUTTONS_H

#include "QtWidgets/qpushbutton.h"

#include <QDialogButtonBox>
#include <QObject>

/**
 * @brief Clas for creating common button groups
 */
class Buttons : public QDialogButtonBox {
 public:
  /**
   * @brief Create save and cancle buttons
   * @param save Tooltip for save button
   * @param cancle Tooltip for cancle button
   */
  Buttons(const QString& save, const QString& cancle);

  /**
   * @brief Create list edit buttons (add, copy, delete, import, export)
   * @param add Tooltip for add button
   * @param copy Tooltip for copy button
   * @param del Tooltip for delete button
   * @param import Tooltip for import button
   * @param exprt Tooltip for export button
   */
  Buttons(const QString& add, const QString& copy, const QString& del, const QString& import, const QString& exprt);

  /**
   * @brief Create list edit buttons (add, copy, delete, import, export) with save and cancle
   * @param add Tooltip for add button
   * @param copy Tooltip for copy button
   * @param del Tooltip for delete button
   * @param import Tooltip for import button
   * @param exprt Tooltip for export button
   * @param save Tooltip for save button
   * @param cancle Tooltip for cancle button
   */
  Buttons(const QString& add, const QString& copy, const QString& del, const QString& import, const QString& exprt,
          const QString& save, const QString& cancle);

  QPushButton* btnAdd;
  QPushButton* btnCopy;
  QPushButton* btnDelete;
  QPushButton* btnImport;
  QPushButton* btnExport;
  QPushButton* btnCancel;
  QPushButton* btnSave;

 private:
  void genBtnsSaveCancle(const QString& save, const QString& cancle);
  void genBtnsListEdit(const QString& add, const QString& copy, const QString& del, const QString& import,
                       const QString& exprt);
};

#endif  // BUTTONS_H
