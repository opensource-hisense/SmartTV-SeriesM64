/*
   (c) Copyright 2001-2009  The world wide DirectFB Open Source Community (directfb.org)
   (c) Copyright 2000-2004  Convergence (integrated media) GmbH

   All rights reserved.

   Written by Denis Oliver Kropp <dok@directfb.org>,
              Andreas Hundt <andi@fischlustig.de>,
              Sven Neumann <neo@directfb.org>,
              Ville Syrjälä <syrjala@sci.fi> and
              Claudio Ciccani <klan@users.sf.net>.

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

#ifndef __CORE_H__
#define __CORE_H__

#include <fusion/types.h>
#include <fusion/lock.h>
#include <fusion/object.h>
#include <fusion/property.h>
#include <directfb.h>

#include "coretypes.h"
#include "coredefs.h"


#define DIRECTFB_CORE_ABI     45


typedef enum {
     DFCP_CLIPBOARD,
     DFCP_COLORHASH,
     DFCP_GRAPHICS,
     DFCP_INPUT,
     DFCP_LAYER,
     DFCP_SCREEN,
     DFCP_SURFACE,
     DFCP_SYSTEM,
     DFCP_WM,

     _DFCP_NUM
} DFBCorePartID;


typedef struct {
    unsigned int    maxPeakMem;           /* record maximum memory */
    unsigned int    currentMem;           /* record current memory */    
    unsigned int    layerSurfaceMem;      /* record layer surface memory */
    bool            bUse;
    pid_t           PPID;

} DFBPeakMemInfo;

#define MAX_PRCOESS_ID     10000

struct __DFB_CoreDFBShared {
     int                  magic;

     FusionSkirmish       lock;
     bool                 active;

     int                  busy_refs;

     FusionObjectPool    *layer_context_pool;
     FusionObjectPool    *layer_region_pool;
     FusionObjectPool    *palette_pool;
     FusionObjectPool    *surface_pool;
     FusionObjectPool    *window_pool;

     FusionSHMPoolShared *shmpool;
     FusionSHMPoolShared *shmpool_data; /* for raw data, e.g. surface buffers */
     bool                 bHWDecodeUsed;
     FusionSkirmish       lock_hwDecoder;
     FusionSkirmish       lock_sw_jpg_Decoder;  /* sw jpg lock */
     FusionSkirmish       lock_hw_png_gif_Decoder;  /* png gif share the same HW-IP */
     FusionSkirmish       lock_sw_png_Decoder;  /* sw png lock */
     FusionSkirmish       lock_sw_gif_Decoder;  /* sw gif lock */
     FusionSkirmish       lock_hw_gop_Decoder;  /* hw gop lock */
#if 0
     FusionSkirmish       lock_sw_sdr2hdr; /* sdr2hdr lock */
#else
     FusionProperty       lock_sw_sdr2hdr;
     bool                 enable_GLES2;
#endif
     int                  maxPeakMem;               /* record maximum memory */
     int                  currentMem;           /* record current memory */

     int dump_pid_start; /* for dump surface the function "Flip" of Surface, Window, or Layer ; default value is -1 */
     int dump_pid_end; /* for dump surface the function "Flip" of Surface, Window, or Layer ; default value is -1 */
     DFBPeakMemInfo       *memInfo;  

};

struct __DFB_CoreDFB {
     int                      magic;

     int                      refs;

     int                      fusion_id;

     FusionWorld             *world;
     FusionArena             *arena;

     CoreDFBShared           *shared;

     bool                     master;
     bool                     suspended;

     DirectLink              *cleanups;

     DirectThreadInitHandler *init_handler;

     DirectSignalHandler     *signal_handler;

     DirectCleanupHandler    *cleanup_handler;
     bool                    layers_suspended;
     bool                    input_suspended;
};



/*
 * Cleanup function, callback of a cleanup stack entry.
 */
typedef void (*CoreCleanupFunc)(void *data, int emergency);



/*
 * Core initialization and deinitialization
 */
DFBResult  dfb_core_create  ( CoreDFB       **ret_core );

DFBResult  dfb_core_destroy ( CoreDFB        *core,
                              bool            emergency );

void      *dfb_core_get_part( CoreDFB        *core,
                              DFBCorePartID   part_id );


#define DFB_CORE(core,PART)   dfb_core_get_part( core, DFCP_##PART )


/*
 * Object creation
 */
CoreLayerContext *dfb_core_create_layer_context( CoreDFB *core );
CoreLayerRegion  *dfb_core_create_layer_region ( CoreDFB *core );
CorePalette      *dfb_core_create_palette      ( CoreDFB *core );
CoreSurface      *dfb_core_create_surface      ( CoreDFB *core );
CoreWindow       *dfb_core_create_window       ( CoreDFB *core );

/*
 * Debug
 */
DirectResult dfb_core_enum_surfaces      ( CoreDFB               *core,
                                           FusionObjectCallback   callback,
                                           void                  *ctx );
DirectResult dfb_core_enum_layer_contexts( CoreDFB               *core,
                                           FusionObjectCallback   callback,
                                           void                  *ctx );
DirectResult dfb_core_enum_layer_regions ( CoreDFB               *core,
                                           FusionObjectCallback   callback,
                                           void                  *ctx );


/*
 * Returns true if the calling process is the master fusionee,
 * i.e. handles input drivers running their threads.
 */
bool         dfb_core_is_master( CoreDFB *core );

/*
 * Allows other (blocking) Fusionees to enter the DirectFB session.
 */
void         dfb_core_activate( CoreDFB *core );

/*
 * Returns the core's fusion world.
 */
FusionWorld *dfb_core_world( CoreDFB *core );

/*
 * Returns the core arena.
 */
FusionArena *dfb_core_arena( CoreDFB *core );

/*
 * Returns the shared memory pool of the core.
 */
FusionSHMPoolShared *dfb_core_shmpool( CoreDFB *core );

/*
 * Returns the shared memory pool for raw data, e.g. surface buffers.
 */
FusionSHMPoolShared *dfb_core_shmpool_data( CoreDFB *core );

/*
 * Suspends all core parts, stopping input threads, closing devices...
 */
DFBResult    dfb_core_suspend( CoreDFB *core );

/*
 * Resumes all core parts, reopening devices, starting input threads...
 */
DFBResult    dfb_core_resume( CoreDFB *core );

/*
 * Adds a function to the cleanup stack that is called during deinitialization.
 * If emergency is true, the cleanup is even called by core_deinit_emergency().
 */
CoreCleanup *dfb_core_cleanup_add( CoreDFB         *core,
                                   CoreCleanupFunc  func,
                                   void            *data,
                                   bool             emergency );

/*
 * Removes a function from the cleanup stack.
 */
void         dfb_core_cleanup_remove( CoreDFB     *core,
                                      CoreCleanup *cleanup );

/*
*only resume the core  layes
*/
DFBResult    dfb_core_resume_layers( CoreDFB *core );


/*
*only suspend the core  layes
*/
DFBResult    dfb_core_suspend_layers( CoreDFB *core );

/*
*only resume the core  input
*/
DFBResult    dfb_core_resume_input( CoreDFB *core );

/*
*only suspend the core  input
*/
DFBResult    dfb_core_suspend_input( CoreDFB *core );


#endif

