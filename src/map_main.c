#include "map.h"

#include <stdlib.h>
#include <string.h>

#undef NDEBUG

#include <assert.h>

int main() {
  char const* str;

  Map* m = newMap();
  assert(m);

  assert(addRoad(m, "Alinów", "Bór", 1, 2020));
  assert(addRoad(m, "Bór", "Cieliñsk-Niek³añsk", 2, 2020));
  assert(addRoad(m, "Bór", "D¹b Stary", 3, 2020));
  assert(addRoad(m, "Cieliñsk-Niek³añsk", "Emiliew", 4, 2020));
  assert(addRoad(m, "D¹b Stary", "Emiliew", 5, 2020));
  assert(addRoad(m, "Emiliew", "Bór", 8, 2020));
  assert(addRoad(m, "Emiliew", "FraŸnik Nowy", 3, 2020));
  assert(!repairRoad(m, "Emiliew", "Cieliñsk-Niek³añsk", 2019));
  assert(repairRoad(m, "Emiliew", "Cieliñsk-Niek³añsk", 2021));
  assert(!repairRoad(m, "Emiliew", "Alinów", 2020));
  assert(addRoad(m, "FraŸnik Nowy", "Grzegrzewo", 4, 2020));
  assert(addRoad(m, "Alinów", "Grzegrzewo", 10, 2020));
  assert(addRoad(m, "Homilcin", "Grzegrzewo", 5, 2020));
  assert(addRoad(m, "FraŸnik Nowy", "Cieliñsk-Niek³añsk", 2, 2020));
  assert(!addRoad(m, "FraŸnik Nowy", "Cieliñsk-Niek³añsk", 2, 2020));
  assert(!addRoad(m, "Cieliñsk-Niek³añsk", "FraŸnik Nowy", 2, 2020));
  assert(!repairRoad(m, "Emiliew", "Bór", 2018));
  assert(repairRoad(m, "Emiliew", "Cieliñsk-Niek³añsk", 2021));
  assert(repairRoad(m, "Emiliew", "FraŸnik Nowy", 2023));
  assert(addRoad(m, "Homilcin", "Cieliñsk-Niek³añsk", 3, 2020));
  assert(newRoute(m, 10, "Alinów", "Emiliew"));

  str = getRouteDescription(m, 10);
  assert(strcmp(str, "10;Alinów;1;2020;Bór;2;2020;Cieliñsk-Niek³añsk;4;2021;Emiliew") == 0);
  free((void *)str);

  assert(extendRoute(m, 10, "Homilcin"));

  str = getRouteDescription(m, 10);
  assert(strcmp(str, "10;Alinów;1;2020;Bór;2;2020;Cieliñsk-Niek³añsk;4;2021;Emiliew"
                     ";3;2023;FraŸnik Nowy;4;2020;Grzegrzewo;5;2020;Homilcin") == 0);
  free((void *)str);

  deleteMap(m);

  return 0;
}
