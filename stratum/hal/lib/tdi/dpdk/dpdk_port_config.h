// Copyright 2021-2022 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#ifndef STRATUM_HAL_LIB_TDI_DPDK_DPDK_PORT_CONFIG_H_
#define STRATUM_HAL_LIB_TDI_DPDK_DPDK_PORT_CONFIG_H_

#include <string>

#include "absl/types/optional.h"
#include "stratum/glue/integral_types.h"
#include "stratum/hal/lib/common/common.pb.h"

#define GNMI_CONFIG_PORT_TYPE 0x01
#define GNMI_CONFIG_DEVICE_TYPE 0x02
#define GNMI_CONFIG_QUEUE_COUNT 0x04
#define GNMI_CONFIG_SOCKET_PATH 0x08
#define GNMI_CONFIG_HOST_NAME 0x10

#define GNMI_CONFIG_TDI (GNMI_CONFIG_PORT_TYPE | GNMI_CONFIG_DEVICE_TYPE | \
                         GNMI_CONFIG_QUEUE_COUNT | GNMI_CONFIG_SOCKET_PATH | \
                         GNMI_CONFIG_HOST_NAME)

namespace stratum {
namespace hal {
namespace tdi {

struct DpdkPortConfig {
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
      queues(0) {
  }
};

}  // namespace tdi
}  // namespace hal
}  // namespace stratum

#endif  // STRATUM_HAL_LIB_TDI_DPDK_DPDK_PORT_CONFIG_H_
