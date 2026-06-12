#ifndef BOLUN_SERVICES_SERVICE_REGISTRY_HPP
#define BOLUN_SERVICES_SERVICE_REGISTRY_HPP

#include <array>
#include <cstddef>
#include <cstdint>
#include <string_view>

namespace bolun::services {

enum class ServiceClass : std::uint8_t {
    package,
    update,
    notification,
    power,
    storage,
    account,
};

struct ServiceDescriptor {
    std::string_view name;
    ServiceClass service_class;
    std::uint8_t start_order;
    bool critical;
};

constexpr std::array<ServiceDescriptor, 6> kCoreServices{{
    {"Package Service", ServiceClass::package, 10U, true},
    {"Power Service", ServiceClass::power, 20U, true},
    {"Storage Service", ServiceClass::storage, 30U, true},
    {"Update Service", ServiceClass::update, 40U, true},
    {"Notification Service", ServiceClass::notification, 50U, false},
    {"Account Service", ServiceClass::account, 60U, false},
}};

[[nodiscard]] constexpr const ServiceDescriptor *find_service(ServiceClass service_class)
{
    for (const auto &service : kCoreServices) {
        if (service.service_class == service_class) {
            return &service;
        }
    }
    return nullptr;
}

} // namespace bolun::services

#endif
