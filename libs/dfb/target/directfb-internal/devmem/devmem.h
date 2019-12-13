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

#ifndef __DEVMEM_DEVMEM_H__
#define __DEVMEM_DEVMEM_H__

#include <fusion/shmalloc.h>

#include <core/surface_pool.h>

#include "surfacemanager.h"


#define DEV_MEM     "/dev/mem"

extern const SurfacePoolFuncs devmemSurfacePoolFuncs;
extern const SurfacePoolFuncs devmemSurfacePoolSecondaryFuncs;
extern const SurfacePoolFuncs preallocInVidSurfacePoolFuncs;


typedef struct {
     FusionSHMPoolShared *shmpool;

     CoreSurfacePool     *pool;
     CoreSurfacePool     *pool_secondary;
     CoreSurfacePool     *preAllocInVideoSurfacePool;
     SurfaceManager      *manager;

     unsigned long pool_handle_id;
        unsigned long long offset_in_pool;

          unsigned long sec_pool_handle_id;
        unsigned long long sec_offset_in_pool;

} DevMemDataShared;

typedef struct {
     DevMemDataShared    *shared;

     void                *mem;
     void                *mem_secondary;
     volatile void       *reg;
} DevMemData;

typedef struct {
     int             magic;

     SurfaceManager *manager;

} DevMemPoolData;

typedef struct {
     int             magic;

     CoreDFB        *core;
     void           *mem;
} DevMemPoolLocalData;

typedef struct {
     int   magic;

     int   offset;
     int   pitch;
     int   size;

     Chunk *chunk;
} DevMemAllocationData;
#endif

