/*
   (c) Copyright 2001-2009  The world wide DirectFB Open Source Community (directfb.org)
   (c) Copyright 2000-2004  Convergence (integrated media) GmbH

   All rights reserved.

   Written by Denis Oliver Kropp <dok@directfb.org>,
              Andreas Hundt <andi@fischlustig.de>,
              Sven Neumann <neo@directfb.org>,
              Ville Syrjälä <syrjala@sci.fi> and
              Claudio Ciccani <klan@users.sf.net>.
    bool   mst_enable_GLES2;
   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the
   Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
*/

#ifndef __CONF_H__
#define __CONF_H__

#include <signal.h>

#include <directfb.h>
#include <fusion/types.h>
#include <fusion/vector.h>

#include <core/coredefs.h>

typedef enum {
    E_DFB_MEMMAP_DEVMEM = 0,
    E_DFB_MEMMAP_MSOS_POOL,
    E_DFB_MEMMAP_CMA_POOL,
} DFBMapMemMode;

typedef struct {
     bool                                init;

     DFBDisplayLayerConfig               config;
     DFBColor                            src_key;
     int                                 src_key_index;

     struct {
          DFBDisplayLayerBackgroundMode  mode;
          DFBColor                       color;
          int                            color_index;
          char                          *filename;
     } background;

     DFBWindowStackingClass              stacking;

     DFBColor                           *palette;
     bool                                palette_set;

     int                                 rotate;
} DFBConfigLayer;

typedef enum {
     DCWF_NONE                          = 0x00000000,

     DCWF_CREATE_SURFACE                = 0x00000001,
     DCWF_CREATE_WINDOW                 = 0x00000002,

     DCWF_ALLOCATE_BUFFER               = 0x00000010,

     DCWF_ALL                           = 0x00000013,
} DFBConfigWarnFlags;

typedef enum {
    SWRF_NONE             = 0x00000000,

    SWRF_FILLRECT         = 0x00000001,
    SWRF_DRAWRECT        = 0x00000002,
    SWRF_DRAWLINE        = 0x00000004,
    SWRF_BLIT            = 0x00000008,
    SWRF_STRETCHBLIT    = 0x00000010,
    SWRF_STRETCHBLITEX    = 0x00000020,
    SWRF_DRAWOVAL        = 0x00000040,
    SWRF_BLITTRAPEZOID     = 0x00000080,
    
    SWRF_ENABLE         = 0xffffffff,
}SWRenderFlags;

typedef enum {
    CONF_ION_HEAP_SYSTEM_MASK = 0,
    CONF_ION_HEAP_MIU0_MASK,
    CONF_ION_HEAP_MIU1_MASK,
    CONF_ION_HEAP_MIU2_MASK,
}DFBConfigIonHeapMask;

typedef DFBInputDeviceKeySymbol(*keypad_func)(int keycode);
typedef DFBInputDeviceKeySymbol2(*keypad_func2)(u32 index,int keycode);  

typedef struct
{
     bool      mouse_motion_compression;          /* use motion compression? */
     char     *mouse_protocol;                    /* mouse protocol */
     char     *mouse_source;                      /* mouse source device name */
     bool      mouse_gpm_source;                  /* mouse source is gpm? */

     int       window_policy;                     /* swapping policy for the
                                                     surface of a window */
     int       buffer_mode;                       /* default buffer mode for
                                                     primary layer */

     bool      pollvsync_after;
     bool      pollvsync_none;

     bool      software_only;                     /* disable hardware acceleration */
     bool      hardware_only;                     /* disable software fallbacks */

     bool      mmx;                               /* mmx support */

     bool      banner;                            /* startup banner */

     bool      force_windowed;                    /* prohibit exclusive modes */

     bool      deinit_check;

     bool      vt_switch;                         /* allocate a new VT */
     int       vt_num;                            /* number of TTY to use or -1
                                                     if the default */
     bool      kd_graphics;                       /* put terminal into graphics
                                                     mode */

     DFBScreenEncoderTVStandards matrox_tv_std;   /* Matrox TV standard */
     int       matrox_cable;                      /* Matrox cable type */
     bool      matrox_sgram;                      /* Use Matrox SGRAM features */
     bool      matrox_crtc2;                      /* Experimental CRTC2 stuff */

     bool      sync;                              /* Do sync() in core_init() */
     bool      vt_switching;                      /* Allow VT switching by
                                                     pressing Ctrl+Alt+<F?> */

     char     *fb_device;                         /* Used framebuffer device,
                                                     e.g. "/dev/fb0" */

     struct {
          int  bus;                               /* PCI Bus */
          int  dev;                               /* PCI Device */
          int  func;                              /* PCI Function */
     } pci;

     bool      lefty;                             /* Left handed mouse, swaps
                                                     left/right mouse buttons */
     bool      no_cursor;                         /* Never create a cursor */
     bool      translucent_windows;               /* Allow translucent
                                                     windows */

     struct {
          int                   width;            /* primary layer width */
          int                   height;           /* primary layer height */
          int                   depth;            /* primary layer depth */
          DFBSurfacePixelFormat format;           /* primary layer format */
     } mode;

     struct {
          int                   width;            /* scaled window width */
          int                   height;           /* scaled window height */
     } scaled;

     int       videoram_limit;                    /* limit amount of video
                                                     memory used by DirectFB */

     char     *screenshot_dir;                    /* dump screen content into
                                                     this directory */

     char     *system;                            /* FBDev, SDL, etc. */

     bool      capslock_meta;                     /* map CapsLock -> Meta */

     bool      block_all_signals;                 /* block all signals */

     int       session;                           /* select multi app world */

     int       primary_layer;                     /* select alternative primary
                                                     display layer */

     bool      force_desktop;                     /* Desktop background is
                                                     the primary surface. */

     bool      linux_input_ir_only;               /* Ignore non-IR devices. */

     struct {
          char *host;                             /* Remote host to connect to. */
          int   session;                          /* Remote session number. */
     } remote;

     char      *wm;                               /* Window manager to use. */

     bool       vt;                               /* Use VT stuff at all? */

     bool       decorations;                      /* Enable window decorations. */

     DFBSurfacePixelFormat font_format;           /* Preferred font format. */

     char      *h3600_device;                     /* H3600 Touchscreen Device */

     char      *mut_device;                       /* MuTouch Device */

     char      *penmount_device;                  /* PenMount Device */

     char      *zytronic_device;                  /* Zytronic Device */

     char      *elo_device;                       /* elo Device */

     int        unichrome_revision;               /* Unichrome hardware
                                                     revision number override */

     bool       dma;                              /* Enable DMA */

     int        agp;                              /* AGP mode */
     int        agpmem_limit;                     /* Limit of AGP memory
                                                     used by DirectFB */
     bool       i8xx_overlay_pipe_b;              /* video overlay output via pixel pipe B */
     bool       primary_only;                     /* tell application only about primary layer */

     bool       thrifty_surface_buffers;          /* don't keep system instance while video instance is alive */
     bool       surface_sentinel;

     DFBConfigLayer  layers[MAX_LAYERS];
     DFBConfigLayer *config_layer;

     DFBSurfaceRenderOptions  render_options;     /* default render options */

     bool       startstop;                        /* Issue StartDrawing/StopDrawing to driver */

     unsigned long video_phys_cpu;                    /* Physical base address of video memory */
     unsigned int  video_length;                  /* Size of video memory */


     unsigned long video_phys_secondary_cpu;         /* Physical base address of the secondary video memory*/
     unsigned long video_length_secondary;       /* size of the secondary video memory */

     unsigned long mmio_phys;                     /* Physical base address of MMIO area */
     unsigned int  mmio_length;                   /* Size of MMIO area */
     int           accelerator;                   /* Accelerator ID */

     bool          font_premult;                  /* Use premultiplied data in case of ARGB glyph images */

     FusionVector  linux_input_devices;
     FusionVector  tslib_devices;

     bool          thread_block_signals;          /* Call direct_signals_block_all() in direct_thread_main() startup. */

     bool          linux_input_grab;              /* Grab input devices. */

     bool          autoflip_window;               /* If primary surface is non-flipping, but windowed, flip automatically. */
     bool          software_warn;                 /* Show warnings when doing/dropping software operations. */

     int           surface_shmpool_size;          /* Set the size of the shared memory pool used for
                                                     shared system memory surfaces. */

     bool          no_cursor_updates;             /* Never show the cursor etc. */

     struct {
          DFBConfigWarnFlags  flags;              /* Warn on various actions as window/surface creation. */

          struct {
               DFBDimension   min_size;
          } create_surface;

          struct {
               DFBDimension   min_size;
          } allocate_buffer;
     } warn;

     int           keep_accumulators;             /* Free accumulators above this limit */

     bool          software_trace;

  /*mstar GFX driver specifical configurations */
     int           mst_gfx_gop_index;
     int           mst_gfx_width;
     int           mst_gfx_height;
     int           mst_gfx_v_offset;
     int           mst_gfx_h_offset;
     int           mst_lcd_width;
     int           mst_lcd_height;
     unsigned long  mst_ge_vq_phys_addr;
     unsigned long  mst_ge_vq_phys_len;
     unsigned long  mst_gop_regdmaphys_addr;
     unsigned long  mst_gop_regdmaphys_len;
     unsigned long mst_miu0_hal_offset;//physical addr
     unsigned long mst_miu0_hal_length;
     unsigned long mst_miu0_cpu_offset;//bus addr
     unsigned long mst_miu1_hal_offset;
     unsigned long mst_miu1_hal_length;
     unsigned long mst_miu1_cpu_offset;
     unsigned long long mst_miu2_hal_offset;
     unsigned long mst_miu2_hal_length;
     unsigned long mst_miu2_cpu_offset;
     int           default_layer_opacity;
     u8            mst_gop_dstPlane[6];
     u8            mst_gop_available[6];
     u8            mst_gop_available_r[6];
     DFBConfigIonHeapMask     ion_heapmask_by_layer[6];        //layer surface allocate in which ion heap: miu0 miu1, miu2
     DFBConfigIonHeapMask     ion_heapmask_by_surface;         //normal surface allocate in which ion heap: miu0 miu1, miu2
     u8            mst_gop_counts;
   /*mstar hw decode jpeg special configurations*/
    unsigned long mst_jpeg_outbuff_addr;
    unsigned long mst_jpeg_outbuff_length;
    unsigned long mst_jpeg_interbuff_addr;
    unsigned long mst_jpeg_interbuff_length;
    unsigned long mst_jpeg_readbuff_addr;
    unsigned long mst_jpeg_readbuff_length;
    bool          mst_jpeg_hwdecode;
   /*mstar sw decode jpeg special configurations*/
    bool          mst_jpeg_hwdecode_option;       

   /* mstar hw dither configurations */
    bool          mst_dither_enable;       

   /*mstar mux special configuration*/
     u8           mst_mux_counts;
     u8           mst_gop_mux[6];
   /*mstar special configuration, it is similar with mux*/
    u8            mst_goplayer_counts;
    u8            mst_goplayer[6];
    bool            mst_disable_hwclip;   /*disable hw clip  */
    bool           mst_dfb_register_app_manager;
    /*describe the gop miu settings, the most significant bit is 1 means
    *mst_gop_miu_setting will be referred to get the gop miu information.
    *BIT0------->GOP0, BIT1------>GOP1, BIT2 ----->GOP2,
    *BIT3------->GOP3, BIT31-----> the mst_gop_miu_setting is valid
    */
    u32           mst_gop_miu_setting;
    s8            mst_GOP_HMirror; // -1, no change, 0 disable, 1 enable
    s8            mst_GOP_VMirror; //-1, no change, 0 disable, 1 enable
    bool          mst_layer_bTiled[4];
    bool          mst_osd_to_ve;  //only for u4
    unsigned long mst_xc_to_ve_mux; //only for u4
    bool          mst_GOP_Set_YUV;  //only for s12

    unsigned long video_phys_secondary_hal;         /* Physical base address of the secondary video memory*/
    unsigned long video_phys_hal;                    /* Physical base address of video memory */

    keypad_func keypadCallback;        /*keypad callback for setting by users */
    keypad_func2 keypadCallback2;        /*keypad callback for setting by users */
    ulong   keyinternal;        /*keypad internal for setting by users */

    bool    do_stretchdown_patch;       //whether do stretch down patch
    bool    line_stretchblit_patch;       // GE HW bug, enable line stretchblit patch
    bool    static_stretchdown_buf;     //whether the buffer for doing the stretchdown buffer is static or not(allocate every time)
    bool          mst_png_hwdecode;
    u32     mst_forcewrite_DFB;
    bool    tvos_mode;                          //whether it is tvos mode
    bool    enable_devmem_dump;                 //whether enable devmem dump
    bool    do_yuvtorgb_sw_patch;               //whether do YUV to RGB SW patch
    u32     mst_ir_max_keycode;                 //the max ir keycode num
    bool    bUsingHWTLB;   //hwtlb exist
    bool    bGOPUsingHWTLB;  //hwtlb support gop
    bool    bPrealloc_map_tlb;   //preallocated surface map to tlb
    int     TLBAlignmentSize;    //tlb alignment size
    bool    mbootGOPIndex;   //gop index show logo in mboot

    /*
    default : enable
    goal    : whether Enable the GE VQ
    */
    bool    mst_enable_gevq;


    s8      mst_disable_layer_init; // -1, no change, 
    bool    mst_debug_layer;          //whether print layer message
    bool    mst_debug_surface;          //whether print surface message
    bool    mst_debug_input;          //whether print input message
    bool    mst_debug_ion;

    int     mst_layer_default_width;  //set layer default width size
    int     mst_layer_default_height; //set layer default width size
    u8      full_update_numerator;              //window full-update nmerator
    u8      full_update_denominator;              //window full-update denominator
    s8      mst_surface_memory_type;  //force to set sufrace memory_type. 0:default 1:system only, 2:video only, 3:video high
    bool    mst_disable_decode_small_jpeg_by_sw;  //disable decode_small_jpeg_by_sw; default is false
    bool    mst_enable_GOP_Vmirror_Patch;  //enable GOP V mirror patch for side effect; default is false  
    bool    do_GE_Vmirror;  //enable GE V mirror patch for side effect; default is false   
    u8      stretchdown_patch_ratio; //whether do stretch down patch, set the ratio rule.  default is 1. 
    bool     null_driver; // for null driver.
    unsigned int sw_render; // using sw render
    bool    mst_enable_jpeg_quality; // patch for neflix quality test.  
    bool    mst_enable_dip;//Enable the DIP to replace the Dwin capture
    unsigned long long mst_dip_mload_addr; // The DIP menuload buffer address
    unsigned long mst_dip_mload_length; // The DIP menuload buffer length
    u8      mst_dip_select; // which DIP select (0, 1, 2)
    bool    mst_disable_master_pri_high;    // this flag will be disable the feature 
                                            // about make the master process to have the most hightest priority to release memory

    bool    small_font_patch;  // if font(freetype2) height smaller than a specifical value, we set the opacity to avoid display the sawtooth
    bool    freeze_image_by_dfb;      // if use static frame for changing channel, this flag will be assigned with true, it will save bw
    bool    do_i8toargb4444_sw_patch;               //whether do I8 to ARGB4444 SW patch 
    bool    mst_enable_ve_init;   // whether enable GOP VE init step

    bool  mst_register_gpd;  //register GPDRegisterToUtopia on MIPS chip
    bool  window_double_buffer;//forece window create double buffer
    bool  do_hw_jpg_limit_patch;// when hw jpg must scale-down the quailty to decode, force use sw decode for quailty.   

    bool    mst_GE_performanceTest; // add GE performance test
    bool    mst_MIU_protect;    // add miu protect, protect dfb video buffer
    int     mst_MIU_protect_BlockID;    // use 0~3 miu protect block, default = 3
    //bool    mst_disable_msos;               //disable msos in devmem (systems)
    bool    mst_disable_dfbinfo;             //disable dfbinfo load
    //bool    mst_framebuffer_cma;             //whether framebuffer is alocated in cma
    DFBMapMemMode mst_mapmem_mode;            //mode for get mem, 0: devmem  1: msos_mpool   2: cma_pool
    unsigned long mst_cma_heap_id;           //maybe shared with more than one pools which based on this heap
    unsigned long mst_sec_cma_heap_id;           //maybe shared with more than one pools which based on this heap

    int  src_color_key_index_patch; // src color key index mode patch. 0:disable patch  1:current patch
    int  mst_ir_repeat_time; // default:250, add to set the interval for ir release evnet
    int  mst_ir_first_time_out;   // if not repeat, the time out is repeat_time + first_time_out.

    bool mst_handle_wm_key; // default disable DFB handle wm key.
    bool disable_cjk_string_break; // default enable cjk string break
    bool disable_quick_press; // default is enable quick press.
    bool mst_disable_window_scale_patch; // default is enable window scale patch.
    bool mst_force_flip_wait_tagid; // default is disable, force to wait ge tagid when flip
    bool enable_cursor_mouse_optimize; // default disable cursor mouse optimize
    bool layer_support_palette; // default disable. layer surface support I8 palette
    bool stretchblit_with_rotate; // default enable. support stretchblit with rotate
    int  mst_layer_gwin_level; /* default =1. this config can set gwin layer level (-1, 0, 1) when boot up
                                  set layer(id) level.
                                  1:Enable GOP,
                                  -1:Disable GOP only,
                                  0:Disable GOP until the next surface->Flip of a Layer/Window invoked
                               */

    bool  mst_enable_gop_gwin_partial_update;  // enable gop gwin partial update

    /* It acting on the gop blending with scaler(XC)
            0, normal alpha blend
            1, alpha blending with pre-multiplex mode
            2, hw limitation, hw not support pre-multiplex mode */
    int   mst_new_alphamode;
    /*
           withch layer the new alphamode be applying on
           bit "n" =0 : layer n not support;
           bit "n" = 1: layer n support newalphamode
           bit 0 stands for layer 0,
           bit 1 stands for layer 1, etc.
           e.g.
           mst_newalphamode_on_layer = 7(b' 111) means to new alphamode will be
           applying on layer0-layer2  */
    u8    mst_newalphamode_on_layer;

    bool  mst_layer_bfullupdate[6];

    unsigned int mst_AFBC_layer_enable; /* bit "n"= 0: disable, 1: enable; where n means layer id which is from 0 to 5 (max); that is to say,
    bit 0 stands for layer 0,
    bit 1 stands for layer 1,
    bit 2 stands for layer 2, etc.
    e.g.
    mst_AFBC_layer_enable = 0x3f (b'111111) means to enable layer 0 ~ layer 5 (default)
    mst_AFBC_layer_enable = 0 means to disable all layers
    */

    int mst_AFBC_mode; // Define GOP FB string (@apiGOP.h, enum EN_GOP_FRAMEBUFFER_STRING) :  E_GOP_FB_AFBC_NONSPLT_YUVTRS_ARGB8888=0x101 (default)

    bool mst_enhance_stretchblit_precision;

      /*describe the gop miu settings, the most significant bit is 1 means
      *mst_gop_miu_setting_extend will be referred to get the gop miu2 information.
      *BIT0------->GOP0, BIT1------>GOP1, BIT2 ----->GOP2,
      *BIT3------->GOP3, BIT31-----> the mst_gop_miu_setting is valid
      */
    u32           mst_gop_miu2_setting_extend;




    // use GLES to blit layer
    bool   mst_enable_GLES2;  // use GLES to blit layer

    /*  record how many process max peak mem usage
        ex.  mst_mem_peak_usage=10000
        will record 0~9999 process max peak mrem usage
    */
    int  mst_mem_peak_usage;

    // enable Gwin multialpha for STB platform
    bool   mst_enable_gwin_multialpha;

    /*
    default : disable
    goal    : To get PNG SW/HW decode time and render time
    */
    bool   mst_measure_png_performance;

    /* Set the color space, it will be effect on GE when converting RGB to YUV
           Including:
           0: BT601,              GFX_YUV_RGB2YUV_PC
           1: CSC to 0-255,    GFX_YUV_RGB2YUV_255
           2: BT709,              At present most of the chip is not support
           Utopia code path: mxlib/include/apiGFX.h
         default:  1, GFX_YUV_RGB2YUV_255
         goal    : To set the color space when covert RGB to YUV
    */
    int    mst_rgb2yuv_mode;

    /*
    default : disable
    goal    : whether print cma debug message
    */
    bool    mst_debug_cma;

     /* make GOP dest(x,y,w,h) can be set
         default:  0
         goal    : To modify  graphic(width height)  in TV Screen
    */
    int      mst_margine_left;
    int      mst_margine_wright;
    int      mst_margine_top;
    int      mst_margine_bottom;

    /* using second lookup map while keyboard look for symbol
         default:  false
         goal    : avoid second lookup symbol(wrong) to overwrite first lookup symbol(right)
    */
    bool    mst_CTV_linux_input_patch;
    /* check process whether use cma pool
         default:  false
         goal    : check process whether use cma pool
    */
    bool    mst_memory_use_cma;


    /*
    specify the filename of gfxdriver

    e.g.
    mst_gfxdriver=auto  // auto-detect in the folder "gfxdrivers'
    or
    mst_gfxdriver=libdirectfb_mstar_g2.so (default)

    default: libdirectfb_mstar_g2.so
    goal    : avoid using improper gfxdriver, such as libdirectfb_mstar.so

    */
    char* mst_gfxdriver;


    /*
    default : disable
    goal    : allocate video memory for font surface
    */
    bool    mst_font_use_video_mem;


    /*
    default : /config/libdfbinfo.so
    goal    : Move libdfbinfo.so to any directory.
              Choose either default or assign.
    e.g
    go to     /config/directfbrc
    add dfbinfo_dir = "/applications/config/libdfbinfo.so"
    dfbinfo_dir="/applications/config/libdfbinfo.so"(assign)
    or
    dfbinfo_dir="/config/libdfbinfo.so"(default)

    */
    char*   dfbinfo_dir;

    /*
         default:  true
         goal    : whether check bus address from utopia
    */
    bool bus_address_check;

    /*
    specify the flip mode

    default : false
    goal     : true , layer flip use blit mode.
    */
    bool  mst_layer_flip_blit;

    /*
    forece window create single buffer

    default : false
    goal     : true
    */
    bool  window_single_buffer;

    /*
    default : DISPLAYER_TRANSPCOLOR_STRCH_DUPLICATE
    goal    : set layer transparent mode
    */
    int     mst_t_stretch_mode;

    /*
    default : DISPLAYER_HSTRCH_6TAPE_LINEAR
    goal    : layer h stretch mode
    */
    int     mst_h_stretch_mode;

    /*
    default : DISPLAYER_VSTRCH_LINEAR
    goal    : set layer v stretch mode
    */
    int     mst_v_stretch_mode;
    /*
    default : DISPLAYER_VSTRCH_LINEAR
    goal    : set layer v stretch mode
    */
    int     mst_blink_frame_rate;
    /*
    default : null
    goal    : Having three functions.
             (1)Display PID info which app call window flip
                How to use: ex: mst_probe_window_flip and set env PROBE_WIN_FLIP=measure
             (2)Dump front files which app call surface_window or surface_layer flip
                How to use: ex: mst_probe_window_flip =front@/usb/sda1 means dumping front file
                to /usb/sda1, and set env PROBE_WIN_FLIP=dump
             (3)Dump front + back files which app call surface_window or surface_layer flip
                How to use: ex: mst_probe_window_flip =total@/usb/sda1,
                and set env PROBE_WIN_FLIP=measure
    */
    char    *mst_probe_window_flip;

    /*
    default : the original string break
    goal    : fix string break
    */
    bool     mst_fix_string_break;

    /*
    Some customers want to display special effects
    when the pixelformat is ARGB1555.

    default : false
    goal     : true
    */
    bool   mst_argb1555_display;
    /*
    default : disable SDR to HDR.
    goal    : enable SDR to HDR or not.
    */
    bool      mst_gles2_sdr2hdr;

    /*
    ===================watch here===================
    If you want to add new config,
    plz write the exhaustive note above the new config

    ex.
    default :
    goal    :
    ================================================
    */

} DFBConfig;

extern DFBConfig *dfb_config;

/*
 * Allocate Config struct, fill with defaults and parse command line options
 * for overrides. Options identified as DirectFB options are stripped out
 * of the array.
 */
DFBResult dfb_config_init( int *argc, char *(*argv[]) );

/*
 * Read configuration options from file. Called by config_init().
 *
 * Returns DFB_IO if config file could not be opened.
 * Returns DFB_UNSUPPORTED if file contains an invalid option.
 * Returns DFB_INVARG if an invalid option assignment is done,
 * e.g. "--desktop-buffer-mode=somethingwrong".
 */
DFBResult dfb_config_read( const char *filename );


/*
 * Set indiviual option. Used by config_init(), config_read() and
 * DirectFBSetOption()
 */
DFBResult dfb_config_set( const char *name, const char *value );

const char *dfb_config_usage( void );

DFBSurfacePixelFormat dfb_config_parse_pixelformat( const char *format );

int query_miu(const unsigned long addr, unsigned long *offset);

u32 _mstarGFXAddr(u32 cpuPhysicalAddr);

u32 _mstarCPUPhyAddr(u32 halPhysicalAddr);

bool check_bus_address_error(unsigned long long u32BA0, unsigned long long u32BA1,  unsigned long long u32BA2);

#endif

