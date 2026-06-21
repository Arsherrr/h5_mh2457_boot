#ifndef __USBTARGET_H__
#define __USBTARGET_H__
// Dependent header, other headers are not needed

extern void USBTargetConstractor(USBHALStruct* hal);

extern void USBTargetDelayUs(uint32_t us);

extern void USBTargetEnablePhy(USBHALStruct* hal, bool isEnable);

extern void USBTargetEnableInterrupt(USBHALStruct* hal, bool isEnable);

extern void USBTargetEnableModule(USBHALStruct* hal, bool isEnable);

extern void USBTargetResetModule(USBHALStruct* hal);

extern uint8_t USBTargetGetSN(uint8_t* sn, uint8_t length);

#endif
