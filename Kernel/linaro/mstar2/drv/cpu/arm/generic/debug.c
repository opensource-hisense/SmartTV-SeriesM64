#include <linux/init.h>
#include <linux/string.h>
#include <linux/printk.h>
#include "mdrv_debug.h"

#ifdef CONFIG_MP_DEBUG_TOOL_MODULE_CHECK
enum module_check_mode module_check_mode = MODULE_CHECK_ORIG;
static __init int module_check_parse(char *s)
{
	if (!s)
		return 0;

	if (strcmp(s, "strict") == 0)
		module_check_mode = MODULE_CHECK_STRICT;
	else if (strcmp(s, "prompt") == 0)
		module_check_mode = MODULE_CHECK_PROMPT;
	else if (strcmp(s, "silent") == 0)
		module_check_mode = MODULE_CHECK_SILENT;

	return 0;
}
early_param("module_check_mode", module_check_parse);
#endif
