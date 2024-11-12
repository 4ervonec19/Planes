#ifndef CREW_MEMBERS_HPP
#define CREW_MEMBERS_HPP
#include "unit.hpp"

class Pilot : public Unit {
public:
  // При публичном наследовании конструктор отнаследуется от Unit, а "PILOT"
  // будет передан в конструктор
  Pilot() : Unit("PILOT") {}

  // Переопределяем, возвращая 0 (У пилотов по условию нет багажа)
  int getHandLuggageWeight() const override { return 0; }

  // Аналогично
  int getLuggageWeight() const override { return 0; }

  bool canRegister() const override { return true; }
};

class FlightAttendant : public Unit {
public:
  // При публичном наследовании конструктор отнаследуется от Unit, а "PILOT"
  // будет передан в конструктор
  FlightAttendant() : Unit("FLIGHT_ATTENDANT") {}

  // Переопределяем, возвращая 0 (У членов экипажа по условию нет багажа)
  int getHandLuggageWeight() const override { return 0; }

  // Аналогично
  int getLuggageWeight() const override { return 0; }

  bool canRegister() const override { return true; }
};

#endif // CREW_MEMBERS_HPP
