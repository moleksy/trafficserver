/**
  @section license License

  Internal traffic_ctl request/responses definitions.

  Licensed to the Apache Software Foundation (ASF) under one
  or more contributor license agreements.  See the NOTICE file
  distributed with this work for additional information
  regarding copyright ownership.  The ASF licenses this file
  to you under the Apache License, Version 2.0 (the
  "License"); you may not use this file except in compliance
  with the License.  You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
*/
#pragma once

// We base on the common client types.
#include "shared/rpc/RPCRequests.h"

/// This file defines all the traffic_ctl API client request and responses objects needed to model the jsonrpc messages used in the
/// TS JSONRPC Node API.

///
/// @brief Models the record request message to fetch all records by type.
///
struct GetAllRecordsRequest : shared::rpc::RecordLookupRequest {
  using super = shared::rpc::RecordLookupRequest;
  GetAllRecordsRequest(bool const configs) : super()
  {
    super::emplace_rec(".*", shared::rpc::REGEX, (configs ? shared::rpc::CONFIG_REC_TYPES : shared::rpc::METRIC_REC_TYPES));
  }
};
//------------------------------------------------------------------------------------------------------------------------------------
///
/// @brief Models the config reload request. No params are needed.
///
struct ConfigReloadRequest : shared::rpc::ClientRequest {
  std::string
  get_method() const override
  {
    return "admin_config_reload";
  }
};
//------------------------------------------------------------------------------------------------------------------------------------
///
/// @brief To fetch config file registry from the RPC node.
///
struct ConfigShowFileRegistryRequest : shared::rpc::ClientRequest {
  std::string
  get_method() const override
  {
    return "filemanager.get_files_registry";
  }
};
//------------------------------------------------------------------------------------------------------------------------------------
///
/// @brief Models the clear 'all' metrics request.
///
struct ClearAllMetricRequest : shared::rpc::ClientRequest {
  std::string
  get_method() const override
  {
    return "admin_clear_all_metrics_records";
  }
};
//------------------------------------------------------------------------------------------------------------------------------------
///
/// @brief Models the clear metrics request.
///
struct ClearMetricRequest : shared::rpc::ClientRequest {
  using super = shared::rpc::ClientRequest;
  struct Params {
    std::vector<std::string> names; //!< client expects a list of record names.
  };
  ClearMetricRequest(Params p) { super::params = p; }
  std::string
  get_method() const override
  {
    return "admin_clear_metrics_records";
  }
};
//------------------------------------------------------------------------------------------------------------------------------------
struct ConfigSetRecordRequest : shared::rpc::ClientRequest {
  struct Params {
    std::string recName;
    std::string recValue;
  };
  using super = shared::rpc::ClientRequest;
  ConfigSetRecordRequest(Params d) { super::params.push_back(d); }
  std::string
  get_method() const override
  {
    return "admin_config_set_records";
  }
};
struct ConfigSetRecordResponse {
  struct UpdatedRec {
    std::string recName;
    std::string updateType;
  };
  std::vector<UpdatedRec> data;
};
//------------------------------------------------------------------------------------------------------------------------------------
struct HostStatusLookUpResponse {
  struct HostStatusInfo {
    std::string hostName;
    std::string status;
  };

  std::vector<HostStatusInfo> statusList;
  std::vector<std::string> errorList;
};
//------------------------------------------------------------------------------------------------------------------------------------
struct HostSetStatusRequest : shared::rpc::ClientRequest {
  using super = shared::rpc::ClientRequest;
  struct Params {
    enum class Op : short {
      UP = 1,
      DOWN,
    };
    Op op;
    std::vector<std::string> hosts;
    std::string reason;
    std::string time{"0"};
  };

  HostSetStatusRequest(Params p) { super::params = p; }
  std::string
  get_method() const override
  {
    return "admin_host_set_status";
  }
};

struct HostGetStatusRequest : shared::rpc::ClientRequest {
  using super  = shared::rpc::ClientRequest;
  using Params = std::vector<std::string>;
  HostGetStatusRequest(Params p) { super::params = std::move(p); }

  std::string
  get_method() const override
  {
    return "admin_host_get_status";
  }
};
//------------------------------------------------------------------------------------------------------------------------------------
struct BasicPluginMessageRequest : shared::rpc::ClientRequest {
  using super = BasicPluginMessageRequest;
  struct Params {
    std::string tag;
    std::string str;
  };
  BasicPluginMessageRequest(Params p) { super::params = p; }
  std::string
  get_method() const override
  {
    return "admin_plugin_send_basic_msg";
  }
};
//------------------------------------------------------------------------------------------------------------------------------------
struct ServerStartDrainRequest : shared::rpc::ClientRequest {
  using super = shared::rpc::ClientRequest;
  struct Params {
    bool waitForNewConnections{false};
  };
  ServerStartDrainRequest(Params p) { super::params = p; }
  std::string
  get_method() const override
  {
    return "admin_server_start_drain";
  }
};
//------------------------------------------------------------------------------------------------------------------------------------
struct ServerStopDrainRequest : shared::rpc::ClientRequest {
  using super = ServerStopDrainRequest;
  std::string
  get_method() const override
  {
    return "admin_server_stop_drain";
  }
};
//------------------------------------------------------------------------------------------------------------------------------------
struct SetStorageDeviceOfflineRequest : shared::rpc::ClientRequest {
  using super = shared::rpc::ClientRequest;
  struct Params {
    std::vector<std::string> names;
  };
  SetStorageDeviceOfflineRequest(Params p) { super::params = p; }
  std::string
  get_method() const override
  {
    return "admin_storage_set_device_offline";
  }
};

//------------------------------------------------------------------------------------------------------------------------------------
struct GetStorageDeviceStatusRequest : shared::rpc::ClientRequest {
  using super = shared::rpc::ClientRequest;
  struct Params {
    std::vector<std::string> names;
  };
  GetStorageDeviceStatusRequest(Params p) { super::params = p; }
  std::string
  get_method() const override
  {
    return "admin_storage_get_device_status";
  }
};

struct DeviceStatusInfoResponse {
  struct CacheDisk {
    CacheDisk(std::string p, std::string s, int e) : path(std::move(p)), status(std::move(s)), errorCount(e) {}
    std::string path;
    std::string status;
    int errorCount;
  };
  std::vector<CacheDisk> data;
};
//------------------------------------------------------------------------------------------------------------------------------------
struct ShowRegisterHandlersRequest : shared::rpc::ClientRequest {
  using super = shared::rpc::ClientRequest;
  std::string
  get_method() const override
  {
    return "show_registered_handlers";
  }
};
//------------------------------------------------------------------------------------------------------------------------------------
///
/// @brief Config status request mapping class.
///
/// There is no interaction between the traffic_ctl and this class so all the variables are defined in this
/// class.
///
struct ConfigStatusRequest : shared::rpc::RecordLookupRequest {
  using super = shared::rpc::RecordLookupRequest;
  ConfigStatusRequest() : super()
  {
    static const std::array<std::string, 6> statusFieldsNames = {
      "proxy.process.version.server.long",        "proxy.node.restarts.proxy.start_time",
      "proxy.node.config.reconfigure_time",       "proxy.node.config.reconfigure_required",
      "proxy.node.config.restart_required.proxy", "proxy.node.config.restart_required.manager"};
    for (auto &&recordName : statusFieldsNames) {
      super::emplace_rec(recordName, shared::rpc::NOT_REGEX, shared::rpc::METRIC_REC_TYPES);
    }
  }
};
