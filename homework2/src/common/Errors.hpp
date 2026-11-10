#pragma once

#include <stdexcept>
#include <string>

class FleetError : public std::runtime_error {
 public:
  explicit FleetError(const std::string& message)
      : std::runtime_error(message) {}
};

class InvalidArgumentError : public FleetError {
 public:
  explicit InvalidArgumentError(const std::string& message)
      : FleetError(message) {}
};

class EnergyCalculationError : public FleetError {
 public:
  explicit EnergyCalculationError(const std::string& message)
      : FleetError(message) {}
};

class InspectionRejectedError : public FleetError {
 public:
  explicit InspectionRejectedError(const std::string& message)
      : FleetError(message) {}
};

class EmptyRegistryError : public FleetError {
 public:
  explicit EmptyRegistryError(const std::string& message)
      : FleetError(message) {}
};

class NoEnergyConsumers : public FleetError {
 public:
  explicit NoEnergyConsumers(const std::string& message)
      : FleetError(message) {}
};

class NoNoviceTransport : public FleetError {
 public:
  explicit NoNoviceTransport(const std::string& message)
      : FleetError(message) {}
};
