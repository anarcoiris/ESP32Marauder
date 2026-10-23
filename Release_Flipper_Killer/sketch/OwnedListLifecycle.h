#line 1 "C:\\Users\\soyko\\Documents\\esp32_marauder\\ESP32Marauder\\esp32_marauder\\OwnedListLifecycle.h"
#pragma once

template <typename ListType>
ListType* resetOwnedList(ListType*& list) {
  if (list != nullptr) {
    list->clear();
    delete list;
  }

  list = new ListType();
  return list;
}
