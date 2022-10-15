// Copyright 2021-2022 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#ifndef STRATUM_HAL_LIB_TDI_DPDK_DPDK_PORT_CONFIG_H_
#define STRATUM_HAL_LIB_TDI_DPDK_DPDK_PORT_CONFIG_H_

#include <cstdint>
#include <string>

#include "absl/types/optional.h"

#include "stratum/glue/integral_types.h"
#include "stratum/hal/lib/common/common.pb.h"
#include "stratum/hal/lib/tdi/tdi_sde_interface.h"

namespace stratum {
namespace hal {
namespace tdi {

static inline constexpr uint32_t CONFIG_BIT(int n) {
    return 1U << n;
}

constexpr uint32_t DPDK_GNMI_PORT_TYPE = CONFIG_BIT(0);
constexpr uint32_t DPDK_GNMI_DEVICE_TYPE = CONFIG_BIT(1);
constexpr uint32_t DPDK_GNMI_QUEUE_COUNT = CONFIG_BIT(2);
constexpr uint32_t DPDK_GNMI_SOCKET_PATH = CONFIG_BIT(3);
constexpr uint32_t DPDK_GNMI_HOST_NAME = CONFIG_BIT(4);
constexpr uint32_t DPDK_GNMI_PIPELINE_NAME = CONFIG_BIT(5);
constexpr uint32_t DPDK_GNMI_MEMPOOL_NAME = CONFIG_BIT(6);
constexpr uint32_t DPDK_GNMI_MTU_VALUE = CONFIG_BIT(7);

constexpr uint32_t DPDK_GNMI_PCI_BDF_VALUE = CONFIG_BIT(8);
constexpr uint32_t DPDK_GNMI_HOTPLUG_SOCKET_IP = CONFIG_BIT(9);
constexpr uint32_t DPDK_GNMI_HOTPLUG_SOCKET_PORT = CONFIG_BIT(10);
constexpr uint32_t DPDK_GNMI_HOTPLUG_CMD = CONFIG_BIT(11);
constexpr uint32_t DPDK_GNMI_HOTPLUG_VM_MAC = CONFIG_BIT(12);
constexpr uint32_t DPDK_GNMI_HOTPLUG_VM_NETDEV_ID = CONFIG_BIT(13);
constexpr uint32_t DPDK_GNMI_HOTPLUG_VM_CHARDEV_ID = CONFIG_BIT(14);
constexpr uint32_t DPDK_GNMI_NATIVE_SOCKET_PATH = CONFIG_BIT(15);

constexpr uint32_t DPDK_GNMI_HOTPLUG_VM_DEVICE_ID = CONFIG_BIT(16);
constexpr uint32_t DPDK_GNMI_PACKET_DIR = CONFIG_BIT(17);

// Parameters that must be set for a VHOST port.
constexpr uint32_t DPDK_GNMI_VHOST_REQUIRED =
    DPDK_GNMI_PORT_TYPE | DPDK_GNMI_DEVICE_TYPE |
    DPDK_GNMI_QUEUE_COUNT | DPDK_GNMI_SOCKET_PATH |
    DPDK_GNMI_HOST_NAME;

// Parameters that must not be set for a VHOST port.
constexpr uint32_t DPDK_GNMI_VHOST_UNSUPPORTED = DPDK_GNMI_PCI_BDF_VALUE;

// Parameters that must be set for a LINK port.
constexpr uint32_t DPDK_GNMI_LINK_REQUIRED =
    DPDK_GNMI_PORT_TYPE | DPDK_GNMI_PCI_BDF_VALUE;

// Parameters that must not be set for a LINK port.
constexpr uint32_t DPDK_GNMI_LINK_UNSUPPORTED =
    DPDK_GNMI_DEVICE_TYPE | DPDK_GNMI_QUEUE_COUNT |
    DPDK_GNMI_SOCKET_PATH | DPDK_GNMI_HOST_NAME;

// Parameters that must be set for a TAP port.
constexpr uint32_t DPDK_GNMI_TAP_READY = DPDK_GNMI_PORT_TYPE;

// Parameters that must not be set for a TAP port.
constexpr uint32_t DPDK_GNMI_TAP_UNSUPPORTED =
    DPDK_GNMI_DEVICE_TYPE | DPDK_GNMI_QUEUE_COUNT |
    DPDK_GNMI_SOCKET_PATH | DPDK_GNMI_HOST_NAME |
    DPDK_GNMI_PCI_BDF_VALUE;

// Parameters that must be set for a HOTPLUG port.
constexpr uint32_t DPDK_GNMI_HOTPLUG_REQUIRED =
    DPDK_GNMI_HOTPLUG_SOCKET_IP | DPDK_GNMI_HOTPLUG_SOCKET_PORT |
    DPDK_GNMI_HOTPLUG_CMD | DPDK_GNMI_HOTPLUG_VM_MAC |
    DPDK_GNMI_HOTPLUG_VM_NETDEV_ID | DPDK_GNMI_HOTPLUG_VM_CHARDEV_ID |
    DPDK_GNMI_NATIVE_SOCKET_PATH | DPDK_GNMI_HOTPLUG_VM_DEVICE_ID;

using ValueCase = SetRequest::Request::Port::ValueCase;

class DpdkPortConfig {
public:
  // ADMIN_STATE_UNKNOWN indicates that something went wrong during port
  // configuration, and the port add failed or was not attempted.
  AdminState admin_state;

  absl::optional<uint64> speed_bps;  // empty if port add failed
  absl::optional<int32> mtu;         // empty if MTU configuration failed
  absl::optional<TriState> autoneg;  // empty if Autoneg configuration failed
  absl::optional<FecMode> fec_mode;  // empty if port add failed
  absl::optional<LoopbackState> loopback_mode; // empty if config failed

  // The configuration object we pass to the SDE.
  TdiSdeInterface::PortConfigParams cfg;

  DpdkPortConfig() :
      admin_state(ADMIN_STATE_UNKNOWN),
      params_set(0) {
    memset(&cfg, 0, sizeof(cfg));
  }

  // Determines whether the specified parameter has been configured.
  bool IsParamSet(ValueCase value_case) const {
    auto param_mask = ParamMaskForCase(value_case);
    return (params_set & param_mask) != 0;
  }

  // Stores the specified parameter.
  void SetParam(ValueCase value_case, const SingletonPort& singleton_port);

  // Checks whether all the specified parameters have been configured.
  bool Contains(uint32_t param_mask) const {
      return (params_set & param_mask) == param_mask;
  }

  // Removes the specified parameters from the configuration set.
  void Reset(uint32_t param_mask) {
      params_set &= ~param_mask;
  }

private:
  // Returns the parameter mask for a SetRequest::Request::Port::ValueCase.
  static uint32_t ParamMaskForCase(ValueCase value_case);

  // Bitmask indicating which parameters have been configured.
  uint32_t params_set;
};

}  // namespace tdi
}  // namespace hal
}  // namespace stratum

#endif  // STRATUM_HAL_LIB_TDI_DPDK_DPDK_PORT_CONFIG_H_
