/*
 * usbh_monitor.h
 *
 *  Created on: Jan 4, 2024
 *      Author: Balint
 */

#ifndef __USB_HOST_H__
#define __USB_HOST_H__

#ifdef __cplusplus
extern "C" {
#endif

int usb_host_init(void);
int usb_host_deinit(void);

#ifdef __cplusplus
}
#endif
#endif /* __USB_HOST_H__ */
