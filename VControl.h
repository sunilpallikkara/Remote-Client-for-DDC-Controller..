#ifndef _VCONTROL_H_
#define _VCONTROL_H_

#include <SinricProDevice.h>
#include <Capabilities/PercentageController.h>

class VControl 
: public SinricProDevice
, public PercentageController<VControl> {
  friend class PercentageController<VControl>;
public:
  VControl(const String &deviceId) : SinricProDevice(deviceId, "VControl") {};
};

#endif
