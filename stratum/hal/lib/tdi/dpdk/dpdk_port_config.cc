// Copyright 2021-2022 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#include "stratum/hal/lib/tdi/dpdk/dpdk_port_config.h"

#include <cstdint>
#include <ostream>

#include "glog/logging.h"
#include "stratum/hal/lib/common/common.pb.h"

namespace stratum {
namespace hal {
namespace tdi {

// Returns the parameter mask for a SetRequest::Request::Port::ValueCase,
// or zero if the ValueCase is not known.
uint32_t DpdkPortConfig::ParamMaskForCase(ValueCase value_case) {
  switch (value_case) {
  case ValueCase::kPortType:
    return DPDK_GNMI_PORT_TYPE;
  case ValueCase::kDeviceType:
    return DPDK_GNMI_DEVICE_TYPE;
  case ValueCase::kQueueCount:
    return DPDK_GNMI_QUEUE_COUNT;
  case ValueCase::kSockPath:
    return DPDK_GNMI_SOCKET_PATH;
  case ValueCase::kHostConfig:
    return DPDK_GNMI_HOST_NAME;
  default:
    return 0;
  }
}


void DpdkPortConfig::SetParam(
    ValueCase value_case,
    const SingletonPort& singleton_port) {

  const auto& config_params = singleton_port.config_params();

  switch (value_case) {
    case ValueCase::kPortType:
      params_set |= DPDK_GNMI_PORT_TYPE;
      port_type = config_params.type();
      LOG(INFO) << "SetPortParam::kPortType = " << port_type;
      break;

    case ValueCase::kDeviceType:
      params_set |= DPDK_GNMI_DEVICE_TYPE;
      device_type = config_params.device_type();
      LOG(INFO) << "SetPortParam::kDeviceType = " << device_type;
      break;

    case ValueCase::kQueueCount:
      params_set |= DPDK_GNMI_QUEUE_COUNT;
      queues = config_params.queues();
      LOG(INFO) << "SetPortParam::kQueueCount = " << queues;
      break;

    case ValueCase::kSockPath:
      params_set |= DPDK_GNMI_SOCKET_PATH;
      socket_path = config_params.socket();
      LOG(INFO) << "SetPortParam::kSockPath = " << socket_path;
      break;

    case ValueCase::kHostConfig:
      params_set |= DPDK_GNMI_HOST_NAME;
      host_name = config_params.host_name();
      LOG(INFO) << "SetPortParam::kHostConfig = " << host_name;
      break;

    default:
      break;
  }
}

}  // namespace tdi
}  // namespace hal
}  // namespace stratum
