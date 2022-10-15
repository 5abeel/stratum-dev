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

// Returns the parameter mask for a ValueCase, or zero if the
// ValueCase is not known.
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
  case ValueCase::kPipelineName:
    return DPDK_GNMI_PIPELINE_NAME;
  case ValueCase::kMempoolName:
    return DPDK_GNMI_MEMPOOL_NAME;
  case ValueCase::kMtuValue:
    return DPDK_GNMI_MTU_VALUE;

  case ValueCase::kPciBdf:
    return DPDK_GNMI_PCI_BDF_VALUE;
  case ValueCase::kPacketDir:
    return DPDK_GNMI_PACKET_DIR;
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
      cfg.port_type = config_params.type();
      LOG(INFO) << "SetPortParam::kPortType = " << cfg.port_type;
      break;

    case ValueCase::kDeviceType:
      params_set |= DPDK_GNMI_DEVICE_TYPE;
      cfg.device_type = config_params.device_type();
      LOG(INFO) << "SetPortParam::kDeviceType = " << cfg.device_type;
      break;

    case ValueCase::kQueueCount:
      params_set |= DPDK_GNMI_QUEUE_COUNT;
      cfg.queues = config_params.queues();
      LOG(INFO) << "SetPortParam::kQueueCount = " << cfg.queues;
      break;

    case ValueCase::kSockPath:
      params_set |= DPDK_GNMI_SOCKET_PATH;
      cfg.socket_path = config_params.socket();
      LOG(INFO) << "SetPortParam::kSockPath = " << cfg.socket_path;
      break;

    case ValueCase::kHostConfig:
      params_set |= DPDK_GNMI_HOST_NAME;
      cfg.host_name = config_params.host_name();
      LOG(INFO) << "SetPortParam::kHostConfig = " << cfg.host_name;
      break;

    case ValueCase::kPipelineName:
      cfg.pipeline_name = config_params.pipeline();
      params_set |= DPDK_GNMI_PIPELINE_NAME;
      LOG(INFO) << "SetPortParam::kPipelineName= " << cfg.pipeline_name;
      break;

    case ValueCase::kMempoolName:
      cfg.mempool_name = config_params.mempool();
      params_set |= DPDK_GNMI_MEMPOOL_NAME;
      LOG(INFO) << "SetPortParam::kMempoolName= " << cfg.mempool_name;
      break;

#if 0
    // TODO(dfoster) This parameter is only partially implemented.
    // It doesn't have a configuration flag, and we don't use it
    // after it's stored.
    case ValueCase::kControlPort:
      control_port = config_params.control();
      LOG(INFO) << "SetPortParam::kControlPort= " << control_port;
      break;
#endif

    case ValueCase::kPciBdf:
      params_set |= DPDK_GNMI_PCI_BDF_VALUE;
      cfg.pci_bdf = config_params.pci();
      LOG(INFO) << "SetPortParam::kPciBdf= " << cfg.pci_bdf;
      break;

    case ValueCase::kMtuValue:
#if 0
      // TODO(dfoster) Come back to this later. Input validation is
      // a good thing, but errors need to be handled systematically.
      if (config_params.mtu() > MAX_MTU) {
        params_set = 0;
        node_id_port_id_to_backend_[node_id][port_id] = params_set;
        return MAKE_ERROR(ERR_INVALID_PARAM)
             << "Unsupported MTU = " << config_params.mtu()
             << ". MTU should be less than " << MAX_MTU;
      }
#endif
      cfg.mtu = config_params.mtu();
      params_set |= DPDK_GNMI_MTU_VALUE;
      LOG(INFO) << "SetPortParam::kMtuValue= " << mtu.value();
      break;

    case ValueCase::kPacketDir:
      params_set |= DPDK_GNMI_PACKET_DIR;
      cfg.packet_dir = config_params.packet_dir();
      LOG(INFO) << "SetPortParam::kPacketDir= " << cfg.packet_dir;
      break;

    default:
      break;
  }
}

}  // namespace tdi
}  // namespace hal
}  // namespace stratum
