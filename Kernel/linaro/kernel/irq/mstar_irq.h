#include <chip_int.h>

#define END_OF_TABLE 0xFFFFFFFF

int mstar_share_intr_list[]=
{
E_IRQ_DISP,
E_IRQ_GOP,
END_OF_TABLE
};

static inline int irq_shared_dispatch(int irq){
	int i = 0;
	int shared = -1;
	while(mstar_share_intr_list[i] != END_OF_TABLE){
		if(mstar_share_intr_list[i] == irq){
			shared = 1;
			break;
		}
		i++;
	}
	return shared;
}


