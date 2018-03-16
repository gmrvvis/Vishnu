#ifndef __AUXILIARS__
#define __AUXILIARS__

#include <QLayoutItem>
#include <QGridLayout>
#include <string>
#include <iostream>

class Auxiliars
{
public:
  static void remove(QGridLayout *layout, int row, int column, bool deleteWidgets) {
    // We avoid usage of QGridLayout::itemAtPosition() here to improve performance.
    for (int i = layout->count() - 1; i >= 0; i--) {
      int r, c, rs, cs;
      layout->getItemPosition(i, &r, &c, &rs, &cs);
      if ((r <= row && r + rs - 1 >= row) || (c <= column && c + cs - 1 >= column)) {
        // This layout item is subject to deletion.
        QLayoutItem *item = layout->takeAt(i);
        if (deleteWidgets) {
          deleteChildWidgets(item);
        }
        delete item;
      }
    }
  }
protected:
  /**
   * Helper function. Deletes all child widgets of the given layout @a item.
   */
  static void deleteChildWidgets(QLayoutItem *item) {
    if (item->layout()) {
      // Process all child items recursively.
      for (int i = 0; i < item->layout()->count(); i++) {
        deleteChildWidgets(item->layout()->itemAt(i));
      }
    }
    delete item->widget();
  }
public:
  /**
   * Removes all layout items on the given @a row from the given grid
   * @a layout. If @a deleteWidgets is true, all concerned child widgets
   * become not only removed from the layout, but also deleted. Note that
   * this function doesn't actually remove the row itself from the grid
   * layout, as this isn't possible (i.e. the rowCount() and row indices
   * will stay the same after this function has been called).
   */
  static void removeRow(QGridLayout *layout, int row, bool deleteWidgets = true) {
    remove(layout, row, -1, deleteWidgets);
    layout->setRowMinimumHeight(row, 0);
    layout->setRowStretch(row, 0);
  }

};

#endif /* __AUXILIARS__ */
