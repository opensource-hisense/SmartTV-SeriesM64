enum module_check_mode {
	MODULE_CHECK_ORIG, /* strcmp without timestamp, reject if error */
	MODULE_CHECK_SILENT, /* strcmp with timestamp, silent & accept if error */
	MODULE_CHECK_PROMPT, /* strcmp with timestamp, warn & accept if error */
	MODULE_CHECK_STRICT, /* strcmp with timestamp, erro & reject if error */
};
