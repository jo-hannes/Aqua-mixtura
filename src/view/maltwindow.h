#ifndef MALTWINDOW_H
#define MALTWINDOW_H

#include "../mainmodel.h"

#include <QTableView>
#include <QWidget>

class MaltWindow : public QWidget
{
  Q_OBJECT
 public:
  /**
   * @brief Create MaltWindow
   * @param model Pointer to main model
   * @param parent QWidget parent
   */
  explicit MaltWindow(MainModel* model, QWidget* parent = nullptr);

 public slots:
  void maltAdd();                  /**< @brief Slot for adding a new malt */
  void maltCopy();                 /**< @brief Slot to copy the currently selected malt */
  void maltDelete();               /**< @brief Slot to delete the currently selected malt */
  void maltImport();               /**< @brief Slot for importing a malt form JSON */
  void maltExport();               /**< @brief Slot for exporting a malt to JSON */
  void save();                     /**< @brief Slot for saving all malts */
  void cancel();                   /**< @brief Slot for discarding all unsaved changes */
  void unsavedMalts(bool unsaved); /**< @brief Slot for indicating of unsaved changes exist */

 signals:
  void maltWindowUnsavedChanges(bool changed); /**< @brief Singal to notify save state of malts */

 private:
  MainModel* model;      /**< @brief Pointer to main model for accessing data */
  Malts* volatileMalts;  /**< @brief Currently edited malts */
  QTableView* maltsView; /**< @brief Table view for malts */
  QString title;         /**< @brief Window tittle */
};

#endif // MALTWINDOW_H
