#include "memoryModule.h"
#include <stdint.h>
#include "SYSCall.h"
void* malloc(size_t size) {
  void* address;
  systemCall((uint64_t)MALLOC, (uint64_t)&address, (uint64_t)size, 0, 0, 0, 0);
  return address;
}

void free(void * address)
{
  systemCall((uint64_t)FREE, (uint64_t)address, 0, 0, 0, 0, 0);
}

void printPage(void* address) {
  systemCall((uint64_t)PRINTPAGE, (uint64_t)address, 0, 0, 0, 0, 0);
}