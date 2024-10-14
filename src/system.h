// system.h

#ifndef SYSTEM_H
#define SYSTEM_H



String get_free_heap() {
  return String(ESP.getFreeHeap());
}


#endif

