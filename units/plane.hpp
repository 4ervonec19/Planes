#ifndef PLANE_HPP
#define PLANE_HPP

#include "passengers.hpp"
#include "segments.hpp"
#include <algorithm>
#include <functional>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

class Plane {
private:
  std::vector<Segment> segments;

public:
  Plane(int economy_weight, int business_weight, int first_class_weight) {
    segments.push_back(Segment("ECONOMY", economy_weight));
    segments.push_back(Segment("BUSINESS", business_weight));
    segments.push_back(Segment("FIRST", first_class_weight));
  }

  int getWeightInfo() const {
    int total = 0;
    for (const auto &segment : segments) {
      total += segment.getCurrentWeight();
    }
    return total;
  }

  int getWeightEconomyInfo() const { return segments[0].getCurrentWeight(); }
  int getWeightBusinessInfo() const { return segments[1].getCurrentWeight(); }
  int getWeightFirstClassInfo() const { return segments[2].getCurrentWeight(); }
  int getMaxWeightEconomyInfo() const { return segments[0].getMaxWeight(); }
  int getMaxWeightBusinessInfo() const { return segments[1].getMaxWeight(); }
  int getMaxWeightFirstClassInfo() const { return segments[2].getMaxWeight(); }

  bool canAddToSegment(int segmentIndex, int weight) const {
    return segments[segmentIndex].canAddLuggage(weight);
  }
  void addWeightToSegment(int segmentIndex, int weight) {
    segments[segmentIndex].updCurrentWeight(weight);
  }
  void relocateLuggageToEconomy(int requiredWeight, int passengerID) {

    int total_weight = 0;
    auto economyLuggage = segments[0].getLuggageWeights();
    for (auto p : economyLuggage) {
      total_weight += p.second;
    }
    if (total_weight <
        requiredWeight) { // Если даже убирая пассажиров эконома не сможем
                          // разместить, то и удалять не надо
      // std::cout << "!!CANNOT ADD LUGGAGE OF PASSENGER ID = " << passengerID
      //           << "!!" << std::endl;
      return;
    }

    std::sort(economyLuggage.begin(), economyLuggage.end(),
              [](const std::pair<int, int> &a, const std::pair<int, int> &b) {
                return a.second > b.second;
              });

    int currentSum = 0;
    for (auto it = economyLuggage.begin(); it != economyLuggage.end();) {
      int id = it->first;
      int weight = it->second;

      currentSum += weight;
      std::cout << "!!PASSENGER'S LUGGAGE REMOVED FROM ECONOMY, ID = " << id
                << "!!" << std::endl;

      segments[0].updCurrentWeight(-weight);
      economyLuggage.erase(it);

      if (currentSum >= requiredWeight) {
        segments[0].updCurrentWeight(requiredWeight);
        segments[0].SetLuggage(economyLuggage);
        return;
      }
    }
  }

  void registerUnit(std::unique_ptr<Unit> unit) {
    Passenger *passenger = dynamic_cast<Passenger *>(unit.get());
    if (!passenger) {
      return;
    }

    std::vector<int> handLuggage = passenger->getHandLuggage();
    std::vector<int> luggage = passenger->getLuggage();
    int handWeight = std::accumulate(handLuggage.begin(), handLuggage.end(), 0);
    int luggageWeight = std::accumulate(luggage.begin(), luggage.end(), 0);
    int totalWeight = handWeight + luggageWeight;
    bool canRegister = passenger->canRegister();

    if (!canRegister) {
      std::cout << "!!CANT REGISTER " << passenger->getType()
                << " PASSENGER, ID = " << passenger->getID() << "!!"
                << std::endl;
      return;
    }

    int segmentIndex = (passenger->getType() == "ECONOMY")       ? 0
                       : (passenger->getType() == "BUSINESS")    ? 1
                       : (passenger->getType() == "FIRST_CLASS") ? 2
                                                                 : -1;

    if (canAddToSegment(segmentIndex, totalWeight)) {
      addWeightToSegment(segmentIndex, totalWeight);
      if (segmentIndex == 0) { // Только для эконом-класса
        for (const auto &luggageItem : luggage) {
          segments[0].addLuggage(luggageItem, passenger->getID());
        }
      }
    } else if (segmentIndex == 1) { // Обработка багажа для бизнес-класса
      if (canAddToSegment(1, handWeight)) {
        addWeightToSegment(1, handWeight);

        int remainingWeight = luggageWeight;
        if (luggage.size() == 2) {
          int luggage1 = luggage[0], luggage2 = luggage[1];
          if (canAddToSegment(1, luggage1) && canAddToSegment(1, luggage2)) {

            int max = std::max(luggage1, luggage2);
            addWeightToSegment(1, max);
            remainingWeight = std::min(luggage1, luggage2);
          } else if (canAddToSegment(1, luggage1)) {
            addWeightToSegment(1, luggage1);
            remainingWeight = luggage2;
          } else if (canAddToSegment(1, luggage2)) {
            addWeightToSegment(1, luggage2);
            remainingWeight = luggage1;
          }
        }
        if (canAddToSegment(0, remainingWeight)) {
          segments[0].updCurrentWeight(remainingWeight);
          return;
        }

        if (!canAddToSegment(0, remainingWeight)) {
          relocateLuggageToEconomy(remainingWeight, passenger->getID());
        }
      } else {
        std::cout << "!!CANT REGISTER BUSINESS PASSENGER, ID = "
                  << passenger->getID() << "!!" << std::endl;
      }
    } else if (segmentIndex == 2) { // Обработка багажа для первого класса
      if (canAddToSegment(2, handWeight)) {
        addWeightToSegment(2, handWeight);

        int remainingWeight = luggageWeight;
        if (luggage.size() == 2) {
          int luggage1 = luggage[0], luggage2 = luggage[1];
          if (canAddToSegment(2, luggage1) && canAddToSegment(2, luggage2)) {

            int max = std::max(luggage1, luggage2);
            addWeightToSegment(2, max);
            remainingWeight = std::min(luggage1, luggage2);
          } else if (canAddToSegment(2, luggage1)) {
            addWeightToSegment(2, luggage1);
            remainingWeight = luggage2;
          } else if (canAddToSegment(2, luggage2)) {
            addWeightToSegment(2, luggage2);
            remainingWeight = luggage1;
          }
        }

        if (canAddToSegment(0, remainingWeight)) {
          segments[0].updCurrentWeight(remainingWeight);
          return;
        }

        if (!canAddToSegment(0, remainingWeight)) {
          relocateLuggageToEconomy(remainingWeight, passenger->getID());
        }
      } else {
        std::cout << "!!CANT REGISTER FIRST_CLASS PASSENGER, ID = "
                  << passenger->getID() << "!!" << std::endl;
      }
    } else {
      if (segmentIndex == 0) {
        std::cout << "!!CANT REGISTER " << passenger->getType()
                  << " PASSENGER, ID = " << passenger->getID() << "!!"
                  << std::endl;
        return;
      }
    }
  }
};

#endif // PLANE_HPP
