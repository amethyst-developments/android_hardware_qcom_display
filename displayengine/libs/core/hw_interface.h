/*
* Copyright (c) 2014, The Linux Foundation. All rights reserved.
*
* Redistribution and use in source and binary forms, with or without modification, are permitted
* provided that the following conditions are met:
*    * Redistributions of source code must retain the above copyright notice, this list of
*      conditions and the following disclaimer.
*    * Redistributions in binary form must reproduce the above copyright notice, this list of
*      conditions and the following disclaimer in the documentation and/or other materials provided
*      with the distribution.
*    * Neither the name of The Linux Foundation nor the names of its contributors may be used to
*      endorse or promote products derived from this software without specific prior written
*      permission.
*
* THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
* OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef __HW_INTERFACE_H__
#define __HW_INTERFACE_H__

#include <core/device_interface.h>
#include <private/strategy_interface.h>
#include <utils/constants.h>

namespace sde {

enum HWBlockType {
  kHWPrimary,
  kHWHDMI,
  kHWWriteback0,
  kHWWriteback1,
  kHWWriteback2,
  kHWBlockMax
};

struct HWResourceInfo {
  uint32_t mdp_version;
  uint32_t hw_revision;
  uint32_t num_dma_pipe;
  uint32_t num_vig_pipe;
  uint32_t num_rgb_pipe;
  uint32_t num_rotator;
  uint32_t num_control;
  uint32_t num_mixer_to_disp;
  uint32_t smp_total;
  uint32_t smp_size;
  uint32_t num_smp_per_pipe;
  uint32_t max_scale_up;
  uint32_t max_scale_down;
  uint32_t max_bandwidth_low;
  uint32_t max_bandwidth_high;
  bool has_bwc;
  bool has_decimation;
  bool has_non_scalar_rgb;
  bool is_src_split;
  bool has_microtile;

  HWResourceInfo()
    : mdp_version(0), hw_revision(0), num_dma_pipe(0), num_vig_pipe(0),
      num_rgb_pipe(0), num_rotator(0), num_control(0), num_mixer_to_disp(0), smp_total(0),
      smp_size(0), num_smp_per_pipe(0), max_scale_up(0), max_scale_down(0), max_bandwidth_low(0),
      max_bandwidth_high(0), has_bwc(false), has_decimation(false), has_non_scalar_rgb(false),
      is_src_split(false), has_microtile(false) { }
};

struct HWPipeInfo {
  uint32_t pipe_id;
  LayerRect src_roi;
  LayerRect dst_roi;

  HWPipeInfo() : pipe_id(0) { }
};

struct HWLayerConfig {
  bool use_non_dma_pipe;
  bool is_right_pipe;
  HWPipeInfo left_pipe;
  HWPipeInfo right_pipe;

  HWLayerConfig() : use_non_dma_pipe(false), is_right_pipe(true) { }
};

struct HWLayers {
  HWLayersInfo info;
  HWLayerConfig config[kNumLayersMax];
};

struct HWDeviceAttributes : DeviceConfigVariableInfo {
  bool is_device_split;
  uint32_t split_left;

  HWDeviceAttributes() : is_device_split(false), split_left(0) { }
};

class HWInterface {
 public:
  static DisplayError Create(HWInterface **intf);
  static DisplayError Destroy(HWInterface *intf);
  virtual DisplayError GetHWCapabilities(HWResourceInfo *hw_res_info) = 0;
  virtual DisplayError Open(HWBlockType type, Handle *device) = 0;
  virtual DisplayError Close(Handle device) = 0;
  virtual DisplayError GetNumDeviceAttributes(Handle device, uint32_t *count) = 0;
  virtual DisplayError GetDeviceAttributes(Handle device, HWDeviceAttributes *device_attributes,
                                       uint32_t mode) = 0;
  virtual DisplayError PowerOn(Handle device) = 0;
  virtual DisplayError PowerOff(Handle device) = 0;
  virtual DisplayError Doze(Handle device) = 0;
  virtual DisplayError Standby(Handle device) = 0;
  virtual DisplayError Validate(Handle device, HWLayers *hw_layers) = 0;
  virtual DisplayError Commit(Handle device, HWLayers *hw_layers) = 0;

 protected:
  virtual ~HWInterface() { }
};

}  // namespace sde

#endif  // __HW_INTERFACE_H__
