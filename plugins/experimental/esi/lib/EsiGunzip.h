/** @file

  A brief file description

  @section license License

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

#ifndef _ESI_GUNZIP_H
#define _ESI_GUNZIP_H

#include "ComponentBase.h"
#include <zlib.h>
#include <string>

class EsiGunzip : private EsiLib::ComponentBase
{

public:

  EsiGunzip(const char *debug_tag,
               EsiLib::ComponentBase::Debug debug_func, EsiLib::ComponentBase::Error error_func);

  virtual ~EsiGunzip();

  bool stream_decode(const char *data, int data_len, std::string &udata);

  inline bool stream_decode(std::string data, std::string &udata) {
    return stream_decode(data.data(), data.size(), udata);
  }

  bool stream_finish();

private:

  int _downstream_length;
  int _total_data_length;
  z_stream _zstrm;
  uLong _crc;

  bool _init;  
  bool _success;
};


#endif // _ESI_GUNZIP_H


