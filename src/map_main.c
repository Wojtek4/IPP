#include "map.h"

#include <stdlib.h>
#include <string.h>

#undef NDEBUG

#include <assert.h>

int main() {
  char const* str;

  Map* m = newMap();
  assert(m);

  assert(addRoad(m, "Alin�w", "B�r", 1, 2020));
  assert(addRoad(m, "B�r", "Cieli�sk-Niek�a�sk", 2, 2020));
  assert(addRoad(m, "B�r", "D�b Stary", 3, 2020));
  assert(addRoad(m, "Cieli�sk-Niek�a�sk", "Emiliew", 4, 2020));
  assert(addRoad(m, "D�b Stary", "Emiliew", 5, 2020));
  assert(addRoad(m, "Emiliew", "B�r", 8, 2020));
  assert(addRoad(m, "Emiliew", "Fra�nik Nowy", 3, 2020));
  assert(!repairRoad(m, "Emiliew", "Cieli�sk-Niek�a�sk", 2019));
  assert(repairRoad(m, "Emiliew", "Cieli�sk-Niek�a�sk", 2021));
  assert(!repairRoad(m, "Emiliew", "Alin�w", 2020));
  assert(addRoad(m, "Fra�nik Nowy", "Grzegrzewo", 4, 2020));
  assert(addRoad(m, "Alin�w", "Grzegrzewo", 10, 2020));
  assert(addRoad(m, "Homilcin", "Grzegrzewo", 5, 2020));
  assert(addRoad(m, "Fra�nik Nowy", "Cieli�sk-Niek�a�sk", 2, 2020));
  assert(!addRoad(m, "Fra�nik Nowy", "Cieli�sk-Niek�a�sk", 2, 2020));
  assert(!addRoad(m, "Cieli�sk-Niek�a�sk", "Fra�nik Nowy", 2, 2020));
  assert(!repairRoad(m, "Emiliew", "B�r", 2018));
  assert(repairRoad(m, "Emiliew", "Cieli�sk-Niek�a�sk", 2021));
  assert(repairRoad(m, "Emiliew", "Fra�nik Nowy", 2023));
  assert(addRoad(m, "Homilcin", "Cieli�sk-Niek�a�sk", 3, 2020));
  assert(newRoute(m, 10, "Alin�w", "Emiliew"));

  str = getRouteDescription(m, 10);
  assert(strcmp(str, "10;Alin�w;1;2020;B�r;2;2020;Cieli�sk-Niek�a�sk;4;2021;Emiliew") == 0);
  free((void *)str);

  assert(extendRoute(m, 10, "Homilcin"));

  str = getRouteDescription(m, 10);
  assert(strcmp(str, "10;Alin�w;1;2020;B�r;2;2020;Cieli�sk-Niek�a�sk;4;2021;Emiliew"
                     ";3;2023;Fra�nik Nowy;4;2020;Grzegrzewo;5;2020;Homilcin") == 0);
  free((void *)str);

  deleteMap(m);

  return 0;
}
