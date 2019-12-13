extern void __flush_dcache_page(struct page *page);
extern void __init bootmem_init(void);
extern void __init arm64_swiotlb_init(void);

#ifdef CONFIG_ZONE_DMA
extern phys_addr_t arm_dma_limit;
#else
#define arm_dma_limit ((phys_addr_t)~0)
#endif

extern phys_addr_t arm_lowmem_limit;
