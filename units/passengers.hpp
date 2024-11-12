#ifndef PASSENGERS_HPP
#define PASSENGERS_HPP
#include "unit.hpp"
#include <iostream>
#include <numeric>
#include <vector>

class Passenger : public Unit {
protected:
  std::vector<int> hand_luggage_weights;
  std::vector<int> luggage_weights;

public:
  Passenger(const std::string &input_type, const std::vector<int> &handLuggage,
            const std::vector<int> &luggage)
      : Unit(input_type), hand_luggage_weights(handLuggage),
        luggage_weights(luggage) {}

  int getHandLuggageWeight() const override {
    int total = std::accumulate(hand_luggage_weights.begin(),
                                hand_luggage_weights.end(), 0);
    return total;
  }

  int getLuggageWeight() const override {
    int total =
        std::accumulate(luggage_weights.begin(), luggage_weights.end(), 0);
    return total;
  }

  std::vector<int> getHandLuggage() const { return hand_luggage_weights; }

  std::vector<int> getLuggage() const { return luggage_weights; }

  bool canRegister() const override {
    return true; // Базовая реализация, можно будет переопределить в подклассах
  }
};

class EconomyPassenger : public Passenger {
public:
  EconomyPassenger(const std::vector<int> &handLuggage,
                   const std::vector<int> &luggage)
      : Passenger("ECONOMY", handLuggage, luggage) {}

  bool canRegister() const override {
    if (hand_luggage_weights.size() > 1) {
      return false; // Максимум 1 позиция ручной клади
    }

    int totalHandLuggageWeight = getHandLuggageWeight();
    if (totalHandLuggageWeight > 10) {
      return false; // Суммарный вес ручной клади не должен превышать 10 кг
    }

    if (luggage_weights.size() > 1) {
      return false; // Максимум 1 позиция багажа
    }

    int totalLuggageWeight = getLuggageWeight();
    if (totalLuggageWeight > 24) {
      return false; // Суммарный вес багажа не должен превышать 24 кг
    }

    return true;
  }
};

class BusinessPassenger : public Passenger {
public:
  BusinessPassenger(const std::vector<int> &handLuggage,
                    const std::vector<int> &luggage)
      : Passenger("BUSINESS", handLuggage, luggage) {}

  bool canRegister() const override {
    // Проверка ручной клади
    if (hand_luggage_weights.size() > 2) {
      return false; // Максимум 2 позиции
    }

    int totalHandLuggageWeight = getHandLuggageWeight();
    if (totalHandLuggageWeight > 24) {
      return false; // Суммарный вес ручной клади не должен превышать 24 кг
    }

    // Проверка багажа
    if (luggage_weights.size() > 2) {
      return false; // Максимум 2 позиции
    }

    int totalLuggageWeight = getLuggageWeight();
    if (totalLuggageWeight > 40) {
      return false; // Суммарный вес багажа не должен превышать 40 кг
    }

    return true;
  }
};

class FirstClassPassenger : public Passenger {
public:
  FirstClassPassenger(const std::vector<int> &handLuggage,
                      const std::vector<int> &luggage)
      : Passenger("FIRST_CLASS", handLuggage, luggage) {}

  bool canRegister() const override {
    // Проверка ручной клади
    if (hand_luggage_weights.size() > 2) {
      return false; // Максимум 2 позиции ручной клади
    }

    int totalHandLuggageWeight = getHandLuggageWeight();
    if (totalHandLuggageWeight > 60) {
      return false; // Суммарный вес ручной клади не должен превышать 60 кг
    }

    // Проверка багажа
    if (luggage_weights.size() > 2) {
      return false; // Максимум 2 позиции
    }

    return true; // Нет ограничения по суммарному весу багажа для первого класса
  }
};

#endif