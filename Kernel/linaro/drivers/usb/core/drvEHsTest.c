//<MStar Software>
//******************************************************************************
// MStar Software
// Copyright (c) 2010 - 2012 MStar Semiconductor, Inc. All rights reserved.
// All software, firmware and related documentation herein ("MStar Software") are
// intellectual property of MStar Semiconductor, Inc. ("MStar") and protected by
// law, including, but not limited to, copyright law and international treaties.
// Any use, modification, reproduction, retransmission, or republication of all
// or part of MStar Software is expressly prohibited, unless prior written
// permission has been granted by MStar.
//
// By accessing, browsing and/or using MStar Software, you acknowledge that you
// have read, understood, and agree, to be bound by below terms ("Terms") and to
// comply with all applicable laws and regulations:
//
// 1. MStar shall retain any and all right, ownership and interest to MStar
//    Software and any modification/derivatives thereof.
//    No right, ownership, or interest to MStar Software and any
//    modification/derivatives thereof is transferred to you under Terms.
//
// 2. You understand that MStar Software might include, incorporate or be
//    supplied together with third party`s software and the use of MStar
//    Software may require additional licenses from third parties.
//    Therefore, you hereby agree it is your sole responsibility to separately
//    obtain any and all third party right and license necessary for your use of
//    such third party`s software.
//
// 3. MStar Software and any modification/derivatives thereof shall be deemed as
//    MStar`s confidential information and you agree to keep MStar`s
//    confidential information in strictest confidence and not disclose to any
//    third party.
//
// 4. MStar Software is provided on an "AS IS" basis without warranties of any
//    kind. Any warranties are hereby expressly disclaimed by MStar, including
//    without limitation, any warranties of merchantability, non-infringement of
//    intellectual property rights, fitness for a particular purpose, error free
//    and in conformity with any international standard.  You agree to waive any
//    claim against MStar for any loss, damage, cost or expense that you may
//    incur related to your use of MStar Software.
//    In no event shall MStar be liable for any direct, indirect, incidental or
//    consequential damages, including without limitation, lost of profit or
//    revenues, lost or damage of data, and unauthorized system use.
//    You agree that this Section 4 shall still apply without being affected
//    even if MStar Software has been modified by MStar in accordance with your
//    request or instruction for your use, except otherwise agreed by both
//    parties in writing.
//
// 5. If requested, MStar may from time to time provide technical supports or
//    services in relation with MStar Software to you for your use of
//    MStar Software in conjunction with your or your customer`s product
//    ("Services").
//    You understand and agree that, except otherwise agreed by both parties in
//    writing, Services are provided on an "AS IS" basis and the warranty
//    disclaimer set forth in Section 4 above shall apply.
//
// 6. Nothing contained herein shall be construed as by implication, estoppels
//    or otherwise:
//    (a) conferring any license or right to use MStar name, trademark, service
//        mark, symbol or any other identification;
//    (b) obligating MStar or any of its affiliates to furnish any person,
//        including without limitation, you and your customers, any assistance
//        of any kind whatsoever, or any information; or
//    (c) conferring any license or right under any intellectual property right.
//
// 7. These terms shall be governed by and construed in accordance with the laws
//    of Taiwan, R.O.C., excluding its conflict of law rules.
//    Any and all dispute arising out hereof or related hereto shall be finally
//    settled by arbitration referred to the Chinese Arbitration Association,
//    Taipei in accordance with the ROC Arbitration Law and the Arbitration
//    Rules of the Association by three (3) arbitrators appointed in accordance
//    with the said Rules.
//    The place of arbitration shall be in Taipei, Taiwan and the language shall
//    be English.
//    The arbitration award shall be final and binding to both parties.
//
//******************************************************************************
//<MStar Software>
/*
 * (C) Copyright MStarSemi 2012
 *
 * Embedded high-speed host electric test procedure
 *
 */

#include <linux/list.h>
#include <linux/usb.h>

// USB related header files
#include <linux/usb/hcd.h>

extern int usb_get_device_descriptor(struct usb_device *dev, unsigned int size);

static void issueSE0(struct usb_hcd *hcd)
{
    u32 regUTMI = hcd->utmi_base;

    printk("SE0 on port %d\n",hcd->port_index);
    writew(0x0e00, (void*) (regUTMI+0x10*2));
}

static void issueTestJ(struct usb_hcd *hcd)
{
    u32 regUHC = hcd->ehc_base;
    u32 regUTMI = hcd->utmi_base;

    printk("TEST_J on port %d\n",hcd->port_index);
    writew(0x0, (void*) (regUTMI+0x2c*2));
    writew(0x0, (void*) (regUTMI+0x2e*2));
    writeb(readb((void*)(regUHC+0x50*2)) | 0x01, (void*) (regUHC+0x50*2)); //enable test J
}

static void issueTestK(struct usb_hcd *hcd)
{
    u32 regUHC = hcd->ehc_base;
    u32 regUTMI = hcd->utmi_base;

    printk("TEST_K on port %d\n",hcd->port_index);
    writew(0x0, (void*) (regUTMI+0x2c*2));
    writew(0x0, (void*) (regUTMI+0x2e*2));
    writeb(readb((void*)(regUHC+0x50*2)) | 0x02, (void*) (regUHC+0x50*2)); //enable test K
}

static void issueTestPacket(struct usb_hcd *hcd)
{
    u32 regUTMI = hcd->utmi_base;

    printk("Test packet on port %d\n", hcd->port_index);
    writew(0x0600, (void*) (regUTMI+0x14*2));
    writew(0x0038, (void*) (regUTMI+0x10*2));
    writew(0x0BFE, (void*) (regUTMI+0x32*2));
}

extern int ehci_urb_enqueue_EHSET (
  struct usb_hcd  *hcd,
  struct urb  *urb,
  gfp_t    mem_flags
);

#define TMODE_VID                                       0x1A0A
#define TMODE_TEST_SE0_NAK                              0x0101
#define TMODE_TEST_J                                    0x0102
#define TMODE_TEST_K                                    0x0103
#define TMODE_TEST_PACKET                               0x0104
#define TMODE_PORT_SUSPEND_RESUME                       0x0106
#define TMODE_SINGLE_STEP_GET_DEV_DESC                  0x0107
#define TMODE_SINGLE_STEP_SET_FEATURE                   0x0108
#define TMODE_NO_SESSION_TRAN_TEST          		0x0200
#define TMODE_UNSUPPORT_DEVICE_TEST         		0x0201
int usb_test_proc_vid(u16 vid, u16 pid)
{
    if (vid == TMODE_VID)
    {
        if ((pid == TMODE_TEST_SE0_NAK) || (pid == TMODE_TEST_J) || (pid == TMODE_TEST_K) || (pid == TMODE_TEST_PACKET))
            return 1;
        else if ((pid == TMODE_NO_SESSION_TRAN_TEST) || (pid == TMODE_UNSUPPORT_DEVICE_TEST))
            return 3; // through normal enumeration flow
        else
            return 2;
    }
    else
        return 0;
}

void usb_high_speed_test_proc(struct usb_device *udev, int flag)
{
    struct usb_hcd *hcd = bus_to_hcd(udev->bus);
    u32 i;
    spinlock_t ehst_lock;

    if (usb_test_proc_vid(udev->descriptor.idVendor, udev->descriptor.idProduct) == 1)
        spin_lock_irq(&ehst_lock);
    switch (udev->descriptor.idProduct)
    {
        case TMODE_TEST_SE0_NAK:
            issueSE0(hcd);
            break;
        case TMODE_TEST_J:
            issueTestJ(hcd);
            break;
        case TMODE_TEST_K:
            issueTestK(hcd);
            break;
        case TMODE_TEST_PACKET:
            issueTestPacket(hcd);
            break;
        case TMODE_SINGLE_STEP_GET_DEV_DESC:
            printk("Single step get device descriptor, delay 15 second\t");
            for (i=0; i<14; i++)
            {
                mdelay(1000);
                printk("+");
            }
            printk("\n");
            usb_get_device_descriptor(udev, USB_DT_DEVICE_SIZE);
            break;
        case TMODE_SINGLE_STEP_SET_FEATURE:
            printk("Single step set feature\n");
            hcd->ms_flag |= MS_FLAG_USB_IF_EHSET; // re-assign urb_enqueue
            usb_get_device_descriptor(udev, USB_DT_DEVICE_SIZE);
            break;
        default:
            printk("Not supported!!!\n");
    }

    if (usb_test_proc_vid(udev->descriptor.idVendor, udev->descriptor.idProduct) == 2)
        spin_lock_irq(&ehst_lock);
    printk("Embedded host test procedure end\n");
    while(1); // pause here
}
