////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2007 MStar Semiconductor, Inc.
// All rights reserved.
//
// Unless otherwise stipulated in writing, any and all information contained
// herein regardless in any format shall remain the sole proprietary of
// MStar Semiconductor Inc. and be kept in strict confidence
// (��MStar Confidential Information��) by the recipient.
// Any unauthorized act including without limitation unauthorized disclosure,
// copying, use, reproduction, sale, distribution, modification, disassembling,
// reverse engineering and compiling of the contents of MStar Confidential
// Information is unlawful and strictly prohibited. MStar hereby reserves the
// rights to any and all damages, losses, costs and expenses resulting therefrom.
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   GMAC.h
/// @author MStar Semiconductor Inc.
/// @brief  GMAC Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////


// -----------------------------------------------------------------------------
// Linux GMAC.h define start
// -----------------------------------------------------------------------------

#ifndef __DRV_GMAC_H_
#define __DRV_GMAC_H_

#define GMAC_DBG(fmt, args...)              {printk("Mstar_gmac: "); printk(fmt, ##args);}
#define GMAC_INFO                           {printk("Line:%u\n", __LINE__);}

//-------------------------------------------------------------------------------------------------
//  Define Enable or Compiler Switches
//-------------------------------------------------------------------------------------------------
#define GMAC_USE_TASK                            1            // 1:Yes, 0:No
#define GMAC_MTU                            (1524)
#define GMAC_DRVNAME                        "mstar gmac"

#ifdef GMAC_TX_QUEUE_4
#define GMAC_TX_RING_SIZE						(8)  //effected size = TX_RING_SIZE - 1
#else
#define GMAC_TX_RING_SIZE						(2)  //effected size = TX_RING_SIZE - 1
#endif
//--------------------------------------------------------------------------------------------------
//  Constant definition
//--------------------------------------------------------------------------------------------------
#if (!GMAC_USE_TASK) // MEM_BASE_ADJUSTMENT ......................................
#define GMAC_RAM_VA_BASE                         0xA0000000UL
#define GMAC_RAM_PA_BASE                         0x00000000UL
#define GMAC_RAM_VA_PA_OFFSET                    0x00000000UL
#define GMAC_RX_BUFFER_BASE                      0x00000000UL       // ==0xA0000000 ~~ 0xA0004000 (Max: 16 KB)
#define GMAC_RBQP_BASE                           GMAC_RX_BUFFER_SIZE//0x00004000         // ==0xA0004000 ~~ 0xA0005FFF for MAX 1024 descriptors
#define GMAC_TX_BUFFER_BASE                      (GMAC_RX_BUFFER_SIZE+GMAC_RBQP_SIZE)//0x00006000         // ==0xA0006000 ~~ ????????
#define GMAC_TX_SKB_BASE                         GMAC_TX_BUFFER_BASE+0x100UL//0x00006100
#define GMAC_RX_FRAME_ADDR                       GMAC_TX_SKB_BASE+0x600UL//0x00007000         // Software COPY&STORE one RX frame. Size is not defined.
#else // The memory allocation for TASK.
//--------------------------------------------------------------------------------------------------
//  Global variable
//--------------------------------------------------------------------------------------------------
u32     GMAC_RAM_VA_BASE;                      //= 0x00000000;     // After init, RAM_ADDR_BASE = GMAC_ABSO_MEM_BASE
u32     GMAC_RAM_PA_BASE;
u32     GMAC_RAM_VA_PA_OFFSET;
u32     GMAC_RX_BUFFER_BASE;                     //= 0x00000000;     // IMPORTANT: lowest 14 bits as zero.
u32     GMAC_RBQP_BASE;                          //= RX_BUFFER_SIZE;//0x00004000;     // IMPORTANT: lowest 13 bits as zero.
u32     GMAC_TX_BUFFER_BASE;                     //= (RX_BUFFER_SIZE+RBQP_SIZE);//0x00006000;
u32     GMAC_TX_SKB_BASE;                        //= (RX_BUFFER_SIZE+RBQP_SIZE+0x600);//0x00006100;
#endif //^MEM_BASE_ADJUSTMENT ...............................................

#define GMAC_ETHERNET_TEST_NO_LINK               0x00000000UL
#define GMAC_ETHERNET_TEST_AUTO_NEGOTIATION      0x00000001UL
#define GMAC_ETHERNET_TEST_LINK_SUCCESS          0x00000002UL
#define GMAC_ETHERNET_TEST_RESET_STATE           0x00000003UL
#define GMAC_ETHERNET_TEST_SPEED_100M            0x00000004UL
#define GMAC_ETHERNET_TEST_DUPLEX_FULL           0x00000008UL
#define GMAC_ETHERNET_TEST_INIT_FAIL             0x00000010UL


u8 GMAC_MY_DEV[16] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
u8 GMAC_MY_MAC[6] = { 0x00, 0x30, 0x1B, 0xBA, 0x02, 0xDB };
u8 GMAC_PC_MAC[6] = { 0x00, 0x1A, 0x4B, 0x5C, 0x39, 0xDF };

#ifdef GMAC_INT_JULIAN_D
   u32 GmacxoffsetValue, GmacxReceiveNum;
#endif
//-------------------------------------------------------------------------------------------------
//  Data structure
//-------------------------------------------------------------------------------------------------
struct gmac_rbf_t
{
  unsigned int  addr;
  unsigned long low_tag;
  unsigned long high_tag;
  unsigned long dummy;
};

struct gmac_recv_desc_bufs
{
    struct gmac_rbf_t descriptors[GMAC_MAX_RX_DESCR];       /* must be on sizeof (rbf_t) boundary */
    char recv_buf[GMAC_RX_BUFFER_SIZE];                /* must be on MAX_RBUFF_SZ boundary */
};

struct gmac_tx_ring
{
	u8 used;
    struct sk_buff *skb;                /* holds skb until xmit interrupt completes */
    dma_addr_t skb_physaddr;            /* phys addr from pci_map_single */
};

#define GMAC_EP_FLAG_OPEND   0X00000001UL
#define GMAC_EP_FLAG_SUSPENDING   0X00000002UL


struct GMAC_private
{
    struct net_device_stats stats;
    struct mii_if_info mii;             /* ethtool support */

    /* PHY */
    unsigned long phy_type;             /* type of PHY (PHY_ID) */
    spinlock_t *lock;                    /* lock for MDI interface */
    short phy_media;                    /* media interface type */

    /* Transmit */
    struct gmac_tx_ring tx_fifo[GMAC_TX_RING_SIZE];
    unsigned char tx_rdidx;       		/* FIFO read index */
    unsigned char tx_wridx;       		/* FIFO write index */
    struct sk_buff *skb;                /* holds skb until xmit interrupt completes */
    dma_addr_t skb_physaddr;            /* phys addr from pci_map_single */
    int skb_length;                     /* saved skb length for pci_unmap_single */
    unsigned char retx_count;       	/* resend count of tx */
    unsigned int txpkt;                 /* previous tx packet pointer */
    /* Receive */
    int rxBuffIndex;                    /* index into receive descriptor list */
    struct gmac_recv_desc_bufs *dlist;       /* descriptor list address */
    struct gmac_recv_desc_bufs *dlist_phys;  /* descriptor list physical address */

    /* suspend/resume */
    unsigned long ep_flag;

	struct net_device *dev;
	struct napi_struct napi_str;
    unsigned int xReceiveFlag;
};

#define GMAC_ROUND_SUP_4(x) (((x)+3)&~3)

struct gmac_eth_drv_sgX
{
    u32 buf;
    u32 len;
};

struct _BasicConfigGMAC
{
    u8 connected;          // 0:No, 1:Yes    <== (20070515) Wait for Julian's reply
    u8 speed;               // 10:10Mbps, 100:100Mbps
    // ETH_CTL Register:
    u8 wes;             // 0:Disable, 1:Enable (WR_ENABLE_STATISTICS_REGS)
    // ETH_CFG Register:
    u8 duplex;              // 1:Half-duplex, 2:Full-duplex
    u8 cam;                // 0:No CAM, 1:Yes
    u8 rcv_bcast;       // 0:No, 1:Yes
    u8 rlf;                // 0:No, 1:Yes receive long frame(1522)
    // MAC Address:
    u8 sa1[6];              // Specific Addr 1 (MAC Address)
    u8 sa2[6];              // Specific Addr 2
    u8 sa3[6];              // Specific Addr 3
    u8 sa4[6];              // Specific Addr 4
};
typedef struct _BasicConfigGMAC BasicConfigGMAC;

struct _UtilityVarsGMAC
{
    u32 cntChkINTCounter;
    u32 readIdxRBQP;        // Reset = 0x00000000
    u32 rxOneFrameAddr;     // Reset = 0x00000000 (Store the Addr of "ReadONE_RX_Frame")
    // Statistics Counters : (accumulated)
    u32 cntREG_ETH_FRA;
    u32 cntREG_ETH_SCOL;
    u32 cntREG_ETH_MCOL;
    u32 cntREG_ETH_OK;
    u32 cntREG_ETH_SEQE;
    u32 cntREG_ETH_ALE;
    u32 cntREG_ETH_DTE;
    u32 cntREG_ETH_LCOL;
    u32 cntREG_ETH_ECOL;
    u32 cntREG_ETH_TUE;
    u32 cntREG_ETH_CSE;
    u32 cntREG_ETH_RE;
    u32 cntREG_ETH_ROVR;
    u32 cntREG_ETH_SE;
    u32 cntREG_ETH_ELR;
    u32 cntREG_ETH_RJB;
    u32 cntREG_ETH_USF;
    u32 cntREG_ETH_SQEE;
    // Interrupt Counter :
    u32 cntHRESP;           // Reset = 0x0000
    u32 cntROVR;            // Reset = 0x0000
    u32 cntLINK;            // Reset = 0x0000
    u32 cntTIDLE;           // Reset = 0x0000
    u32 cntTCOM;            // Reset = 0x0000
    u32 cntTBRE;            // Reset = 0x0000
    u32 cntRTRY;            // Reset = 0x0000
    u32 cntTUND;            // Reset = 0x0000
    u32 cntTOVR;            // Reset = 0x0000
    u32 cntRBNA;            // Reset = 0x0000
    u32 cntRCOM;            // Reset = 0x0000
    u32 cntDONE;            // Reset = 0x0000
    // Flags:
    u8 flagMacTxPermit;    // 0:No,1:Permitted.  Initialize as "permitted"
    u8 flagISR_INT_RCOM;
    u8 flagISR_INT_RBNA;
    u8 flagISR_INT_DONE;
    u8 flagPowerOn;        // 0:Poweroff, 1:Poweron
    u8 initedGMAC;         // 0:Not initialized, 1:Initialized.
    u8 flagRBNA;
    // Misc Counter:
    u32 cntRxFrames;        // Reset = 0x00000000 (Counter of RX frames,no matter it's me or not)
    u32 cntReadONE_RX;      // Counter for ReadONE_RX_Frame
    u32 cntCase20070806;
    u32 cntChkToTransmit;
    // Misc Variables:
    u32 mainThreadTasks;    // (20071029_CHARLES) b0=Poweroff,b1=Poweron
};
typedef struct _UtilityVarsGMAC UtilityVarsGMAC;

BasicConfigGMAC GmacThisBCE;
UtilityVarsGMAC GmacThisUVE;

typedef volatile unsigned int GMAC_REG;

struct sk_buff *Gmac_Tx_SkbAddr;


#ifdef GMAC_TESTING
    extern void GMAC_TEST_All(void);
#endif

struct sk_buff * gmac_rx_skb[GMAC_MAX_RX_DESCR];
u32 gmac_rx_abso_addr[GMAC_MAX_RX_DESCR];
struct sk_buff * gmac_rx_skb_dummy;
u32 gmac_rx_abso_addr_dummy;
#endif
// -----------------------------------------------------------------------------
// Linux GMAC.h End
// -----------------------------------------------------------------------------


