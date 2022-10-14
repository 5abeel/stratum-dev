// Copyright 2021-2022 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#ifndef STRATUM_HAL_LIB_TDI_DPDK_DPDK_PORT_CONFIG_H_
#define STRATUM_HAL_LIB_TDI_DPDK_DPDK_PORT_CONFIG_H_

#include <cstdint>
#include <string>

#include "absl/types/optional.h"

#include "stratum/glue/integral_types.h"
#include "stratum/hal/lib/common/common.pb.h"

namespace stratum {
namespace hal {
namespace tdi {

constexpr uint32_t DPDK_GNMI_PORT_TYPE = 0x01;
constexpr uint32_t DPDK_GNMI_DEVICE_TYPE = 0x02;
constexpr uint32_t DPDK_GNMI_QUEUE_COUNT = 0x04;
constexpr uint32_t DPDK_GNMI_SOCKET_PATH = 0x08;
constexpr uint32_t DPDK_GNMI_HOST_NAME = 0x10;

constexpr uint32_t DPDK_GNMI_VHOST_REQUIRED =
    DPDK_GNMI_PORT_TYPE | DPDK_GNMI_DEVICE_TYPE |
    DPDK_GNMI_QUEUE_COUNT | DPDK_GNMI_SOCKET_PATH |
    DPDK_GNMI_HOST_NAME;

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
  // empty if loopback mode configuration failed
  absl::optional<LoopbackState> loopback_mode;
  SWBackendPortType port_type;
  SWBackendDeviceType device_type;
  int32 queues;
  std::string socket_path;
  std::string host_name;

  DpdkPortConfig() :
      admin_state(ADMIN_STATE_UNKNOWN),
      port_type(PORT_TYPE_NONE),
      device_type(DEVICE_TYPE_NONE),
      queues(0),
      params_set(0) {
  }

  bool IsParamSet(ValueCase value_case) const {
    auto param_mask = ParamMaskForCase(value_case);
    return (params_set & param_mask) != 0;
  }

  void SetParam(ValueCase value_case, const SingletonPort& singleton_port);

  bool Contains(uint32_t param_mask) const {
      return (params_set & param_mask) == param_mask;
  }

  void Reset(uint32_t param_mask) {
      params_set &= ~param_mask;
  }

private:
  // Returns the parameter mask for a SetRequest::Request::Port::ValueCase.
  static uint32_t ParamMaskForCase(ValueCase value_case);

  // Bitmask used to keep track of which parameters have been configured.
  uint32_t params_set;
};

}  // namespace tdi
}  // namespace hal
}  // namespace stratum

#endif  // STRATUM_HAL_LIB_TDI_DPDK_DPDK_PORT_CONFIG_H_
