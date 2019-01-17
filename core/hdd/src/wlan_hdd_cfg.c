/*
 * Copyright (c) 2012-2019 The Linux Foundation. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software for
 * any purpose with or without fee is hereby granted, provided that the
 * above copyright notice and this permission notice appear in all
 * copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 * WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
 * AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 * PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */

/**
 * DOC:  wlan_hdd_cfg.c
 *
 * WLAN Host Device Driver configuration interface implementation
 */

/* Include Files */

#include <linux/firmware.h>
#include <linux/string.h>
#include <wlan_hdd_includes.h>
#include <wlan_hdd_main.h>
#include <wlan_hdd_assoc.h>
#include <wlan_hdd_cfg.h>
#include <linux/string.h>
#include <qdf_types.h>
#include <csr_api.h>
#include <wlan_hdd_misc.h>
#include <wlan_hdd_napi.h>
#include <cds_api.h>
#include "wlan_hdd_he.h"
#include <wlan_policy_mgr_api.h>
#include "wifi_pos_api.h"
#include "wlan_hdd_green_ap.h"
#include "wlan_hdd_twt.h"
#include "wlan_policy_mgr_ucfg.h"
#include "wlan_mlme_ucfg_api.h"
#include "wlan_mlme_public_struct.h"
#include "wlan_fwol_ucfg_api.h"
#include "cfg_ucfg_api.h"
#include "hdd_dp_cfg.h"

struct reg_table_entry g_registry_table[] = {
	REG_VARIABLE(CFG_ENABLE_CONNECTED_SCAN_NAME, WLAN_PARAM_Integer,
		     struct hdd_config, enable_connected_scan,
		     VAR_FLAGS_OPTIONAL | VAR_FLAGS_RANGE_CHECK_ASSUME_DEFAULT,
		     CFG_ENABLE_CONNECTED_SCAN_DEFAULT,
		     CFG_ENABLE_CONNECTED_SCAN_MIN,
		     CFG_ENABLE_CONNECTED_SCAN_MAX),

	REG_VARIABLE(CFG_DOT11_MODE_NAME, WLAN_PARAM_Integer,
		     struct hdd_config, dot11Mode,
		     VAR_FLAGS_OPTIONAL | VAR_FLAGS_RANGE_CHECK,
		     CFG_DOT11_MODE_DEFAULT,
		     CFG_DOT11_MODE_MIN,
		     CFG_DOT11_MODE_MAX),

	REG_VARIABLE(CFG_AP_ENABLE_PROTECTION_MODE_NAME, WLAN_PARAM_Integer,
		     struct hdd_config, apProtEnabled,
		     VAR_FLAGS_OPTIONAL | VAR_FLAGS_RANGE_CHECK_ASSUME_DEFAULT,
		     CFG_AP_ENABLE_PROTECTION_MODE_DEFAULT,
		     CFG_AP_ENABLE_PROTECTION_MODE_MIN,
		     CFG_AP_ENABLE_PROTECTION_MODE_MAX),

	REG_VARIABLE(CFG_TL_DELAYED_TRGR_FRM_INT_NAME, WLAN_PARAM_Integer,
		     struct hdd_config, DelayedTriggerFrmInt,
		     VAR_FLAGS_OPTIONAL | VAR_FLAGS_RANGE_CHECK_ASSUME_DEFAULT,
		     CFG_TL_DELAYED_TRGR_FRM_INT_DEFAULT,
		     CFG_TL_DELAYED_TRGR_FRM_INT_MIN,
		     CFG_TL_DELAYED_TRGR_FRM_INT_MAX),

	REG_VARIABLE(CFG_SCAN_AGING_PARAM_NAME, WLAN_PARAM_Integer,
		     struct hdd_config, scanAgingTimeout,
		     VAR_FLAGS_OPTIONAL,
		     CFG_SCAN_AGING_PARAM_DEFAULT,
		     CFG_SCAN_AGING_PARAM_MIN,
		     CFG_SCAN_AGING_PARAM_MAX),

	REG_VARIABLE(CFG_ENABLE_SNR_MONITORING_NAME, WLAN_PARAM_Integer,
		     struct hdd_config, fEnableSNRMonitoring,
		     VAR_FLAGS_OPTIONAL | VAR_FLAGS_RANGE_CHECK,
		     CFG_ENABLE_SNR_MONITORING_DEFAULT,
		     CFG_ENABLE_SNR_MONITORING_MIN,
		     CFG_ENABLE_SNR_MONITORING_MAX),

#ifdef DHCP_SERVER_OFFLOAD
	REG_VARIABLE_STRING(CFG_DHCP_SERVER_IP_NAME, WLAN_PARAM_String,
			    struct hdd_config, dhcpServerIP,
			    VAR_FLAGS_OPTIONAL,
			    (void *)CFG_DHCP_SERVER_IP_DEFAULT),
#endif /* DHCP_SERVER_OFFLOAD */

	REG_VARIABLE(CFG_ENABLE_MAC_ADDR_SPOOFING, WLAN_PARAM_Integer,
		     struct hdd_config, enable_mac_spoofing,
		     VAR_FLAGS_OPTIONAL | VAR_FLAGS_RANGE_CHECK_ASSUME_DEFAULT,
		     CFG_ENABLE_MAC_ADDR_SPOOFING_DEFAULT,
		     CFG_ENABLE_MAC_ADDR_SPOOFING_MIN,
		     CFG_ENABLE_MAC_ADDR_SPOOFING_MAX),

	REG_VARIABLE(CFG_SAP_11AC_OVERRIDE_NAME, WLAN_PARAM_Integer,
		     struct hdd_config, sap_11ac_override,
		     VAR_FLAGS_OPTIONAL |
				VAR_FLAGS_RANGE_CHECK_ASSUME_DEFAULT,
		     CFG_SAP_11AC_OVERRIDE_DEFAULT,
		     CFG_SAP_11AC_OVERRIDE_MIN,
		     CFG_SAP_11AC_OVERRIDE_MAX),

	REG_VARIABLE(CFG_GO_11AC_OVERRIDE_NAME, WLAN_PARAM_Integer,
		     struct hdd_config, go_11ac_override,
		     VAR_FLAGS_OPTIONAL |
				VAR_FLAGS_RANGE_CHECK_ASSUME_DEFAULT,
		     CFG_GO_11AC_OVERRIDE_DEFAULT,
		     CFG_GO_11AC_OVERRIDE_MIN,
		     CFG_GO_11AC_OVERRIDE_MAX),

#ifdef FEATURE_LFR_SUBNET_DETECTION
	REG_VARIABLE(CFG_ENABLE_LFR_SUBNET_DETECTION, WLAN_PARAM_Integer,
		     struct hdd_config, enable_lfr_subnet_detection,
		     VAR_FLAGS_OPTIONAL | VAR_FLAGS_RANGE_CHECK_ASSUME_DEFAULT,
		     CFG_ENABLE_LFR_SUBNET_DEFAULT,
		     CFG_ENABLE_LFR_SUBNET_MIN,
		     CFG_ENABLE_LFR_SUBNET_MAX),
#endif

	REG_VARIABLE(CFG_ADAPTIVE_EXTSCAN_DWELL_MODE_NAME, WLAN_PARAM_Integer,
		struct hdd_config, extscan_adaptive_dwell_mode,
		VAR_FLAGS_OPTIONAL | VAR_FLAGS_RANGE_CHECK_ASSUME_DEFAULT,
		CFG_ADAPTIVE_EXTSCAN_DWELL_MODE_DEFAULT,
		CFG_ADAPTIVE_EXTSCAN_DWELL_MODE_MIN,
		CFG_ADAPTIVE_EXTSCAN_DWELL_MODE_MAX),

	REG_VARIABLE(CFG_ENABLE_11D_IN_WORLD_MODE_NAME, WLAN_PARAM_Integer,
		     struct hdd_config, enable_11d_in_world_mode,
		     VAR_FLAGS_OPTIONAL | VAR_FLAGS_RANGE_CHECK_ASSUME_DEFAULT,
		     CFG_ENABLE_11D_IN_WORLD_MODE_DEFAULT,
		     CFG_ENABLE_11D_IN_WORLD_MODE_MIN,
		     CFG_ENABLE_11D_IN_WORLD_MODE_MAX),

	REG_VARIABLE(CFG_ENABLE_UNIT_TEST_FRAMEWORK_NAME,
		     WLAN_PARAM_Integer,
		     struct hdd_config, is_unit_test_framework_enabled,
		     VAR_FLAGS_OPTIONAL | VAR_FLAGS_RANGE_CHECK_ASSUME_DEFAULT,
		     CFG_ENABLE_UINT_TEST_FRAMEWORK_DEFAULT,
		     CFG_ENABLE_UNIT_TEST_FRAMEWORK_MIN,
		     CFG_ENABLE_UNIT_TEST_FRAMEWORK_MAX),

	REG_VARIABLE(CFG_CHANGE_CHANNEL_BANDWIDTH_NAME,
		     WLAN_PARAM_Integer,
		     struct hdd_config, enable_change_channel_bandwidth,
		     VAR_FLAGS_OPTIONAL | VAR_FLAGS_RANGE_CHECK_ASSUME_DEFAULT,
		     CFG_CHANGE_CHANNEL_BANDWIDTH_DEFAULT,
		     CFG_CHANGE_CHANNEL_BANDWIDTH_MIN,
		     CFG_CHANGE_CHANNEL_BANDWIDTH_MAX),
};


/**
 * get_next_line() - find and locate the new line pointer
 * @str: pointer to string
 *
 * This function returns a pointer to the character after the occurrence
 * of a new line character. It also modifies the original string by replacing
 * the '\n' character with the null character.
 *
 * Return: the pointer to the character at new line,
 *            or NULL if no new line character was found
 */
static char *get_next_line(char *str)
{
	char c;

	if (str == NULL || *str == '\0')
		return NULL;

	c = *str;
	while (c != '\n' && c != '\0' && c != 0xd) {
		str = str + 1;
		c = *str;
	}

	if (c == '\0')
		return NULL;

	*str = '\0';
	return str + 1;
}

/** look for space. Ascii values to look are
 * 0x09 == horizontal tab
 * 0x0a == Newline ("\n")
 * 0x0b == vertical tab
 * 0x0c == Newpage or feed form.
 * 0x0d == carriage return (CR or "\r")
 * Null ('\0') should not considered as space.
 */
#define i_isspace(ch)  (((ch) >= 0x09 && (ch) <= 0x0d) || (ch) == ' ')

/**
 * i_trim() - trims any leading and trailing white spaces
 * @str: pointer to string
 *
 * Return: the pointer of the string
 */
static char *i_trim(char *str)
{
	char *ptr;

	if (*str == '\0')
		return str;

	/* Find the first non white-space */
	ptr = str;
	while (i_isspace(*ptr))
		ptr++;

	if (*ptr == '\0')
		return str;

	/* This is the new start of the string */
	str = ptr;

	/* Find the last non white-space */
	ptr += strlen(ptr) - 1;

	while (ptr != str && i_isspace(*ptr))
		ptr--;

	/* Null terminate the following character */
	ptr[1] = '\0';

	return str;
}

/* Maximum length of the confgiuration name and value */
#define CFG_VALUE_MAX_LEN 256
#define CFG_ENTRY_MAX_LEN (32+CFG_VALUE_MAX_LEN)

/**
 * hdd_cfg_get_config() - get the configuration content
 * @reg_table: pointer to configuration table
 * @reg_table_count: number of @reg_table entries
 * @ini_struct: pointer to the hdd config blob
 * @hdd_ctx: pointer to hdd context
 * @print_fn: print function pointer
 *
 * Return: none
 */
static void hdd_cfg_get_config(struct reg_table_entry *reg_table,
			       unsigned long reg_table_count,
			       uint8_t *ini_struct, struct hdd_context *hdd_ctx,
			       void (*print_fn)(const char *))
{
	unsigned int idx;
	struct reg_table_entry *reg_entry = reg_table;
	uint32_t value;
	char value_str[CFG_VALUE_MAX_LEN];
	char config_str[CFG_ENTRY_MAX_LEN];
	char *fmt;
	void *field;
	struct qdf_mac_addr *mac_addr;
	int curlen;

	for (idx = 0; idx < reg_table_count; idx++, reg_entry++) {
		field = ini_struct + reg_entry->VarOffset;

		if ((WLAN_PARAM_Integer == reg_entry->RegType) ||
		    (WLAN_PARAM_SignedInteger == reg_entry->RegType) ||
		    (WLAN_PARAM_HexInteger == reg_entry->RegType)) {
			value = 0;

			if ((reg_entry->VarSize > sizeof(value)) ||
			    (reg_entry->VarSize == 0)) {
				pr_warn("Invalid length of %s: %d",
					reg_entry->RegName, reg_entry->VarSize);
				continue;
			}

			memcpy(&value, field, reg_entry->VarSize);
			if (WLAN_PARAM_HexInteger == reg_entry->RegType) {
				fmt = "%x";
			} else if (WLAN_PARAM_SignedInteger ==
				   reg_entry->RegType) {
				fmt = "%d";
				value = sign_extend32(
						value,
						reg_entry->VarSize * 8 - 1);
			} else {
				fmt = "%u";
			}
			snprintf(value_str, CFG_VALUE_MAX_LEN, fmt, value);
		} else if (WLAN_PARAM_String == reg_entry->RegType) {
			snprintf(value_str, CFG_VALUE_MAX_LEN, "%s",
				 (char *)field);
		} else if (WLAN_PARAM_MacAddr == reg_entry->RegType) {
			mac_addr = (struct qdf_mac_addr *) field;
			snprintf(value_str, CFG_VALUE_MAX_LEN,
				 "%02x:%02x:%02x:%02x:%02x:%02x",
				 mac_addr->bytes[0],
				 mac_addr->bytes[1],
				 mac_addr->bytes[2],
				 mac_addr->bytes[3],
				 mac_addr->bytes[4], mac_addr->bytes[5]);
		} else {
			snprintf(value_str, CFG_VALUE_MAX_LEN, "(unhandled)");
		}
		curlen = scnprintf(config_str, CFG_ENTRY_MAX_LEN,
				   "%s=%s%s\n",
				   reg_entry->RegName,
				   value_str,
				   test_bit(idx,
					    (void *)&hdd_ctx->config->
					    bExplicitCfg) ? "*" : "");
		(*print_fn)(config_str);
	}
}

/** struct hdd_cfg_entry - ini configuration entry
 * @name: name of the entry
 * @value: value of the entry
 */
struct hdd_cfg_entry {
	char *name;
	char *value;
};

/**
 * find_cfg_item() - find the configuration item
 * @iniTable: pointer to configuration table
 * @entries: number fo the configuration entries
 * @name: the interested configuration to find
 * @value: the value to read back
 *
 * Return: QDF_STATUS_SUCCESS if the interested configuration is found,
 *		otherwise QDF_STATUS_E_FAILURE
 */
static QDF_STATUS find_cfg_item(struct hdd_cfg_entry *iniTable,
				unsigned long entries,
				char *name, char **value)
{
	QDF_STATUS status = QDF_STATUS_E_FAILURE;
	unsigned long i;

	for (i = 0; i < entries; i++) {
		if (strcmp(iniTable[i].name, name) == 0) {
			*value = iniTable[i].value;
			hdd_debug("Found %s entry for Name=[%s] Value=[%s] ",
				  WLAN_INI_FILE, name, *value);
			return QDF_STATUS_SUCCESS;
		}
	}

	return status;
}

/**
 * parse_hex_digit() - conversion to hex value
 * @c: the character to convert
 *
 * Return: the hex value, otherwise 0
 */
static int parse_hex_digit(char c)
{
	if (c >= '0' && c <= '9')
		return c - '0';
	if (c >= 'a' && c <= 'f')
		return c - 'a' + 10;
	if (c >= 'A' && c <= 'F')
		return c - 'A' + 10;

	return 0;
}

/**
 * update_mac_from_string() - convert string to 6 bytes mac address
 * @hdd_ctx: the pointer to hdd context
 * @macTable: the macTable to carry the conversion
 * @num: number of the interface
 *
 * 00AA00BB00CC -> 0x00 0xAA 0x00 0xBB 0x00 0xCC
 *
 * Return: QDF_STATUS
 */
static QDF_STATUS update_mac_from_string(struct hdd_context *hdd_ctx,
					 struct hdd_cfg_entry *macTable,
					 int num)
{
	int i = 0, j = 0, res = 0;
	char *candidate = NULL;
	struct qdf_mac_addr macaddr[QDF_MAX_CONCURRENCY_PERSONA];
	QDF_STATUS status = QDF_STATUS_SUCCESS;

	memset(macaddr, 0, sizeof(macaddr));

	for (i = 0; i < num; i++) {
		candidate = macTable[i].value;
		for (j = 0; j < QDF_MAC_ADDR_SIZE; j++) {
			res =
				hex2bin(&macaddr[i].bytes[j], &candidate[(j << 1)],
					1);
			if (res < 0)
				break;
		}
		if (res == 0 && !qdf_is_macaddr_zero(&macaddr[i])) {
			qdf_mem_copy((uint8_t *)&hdd_ctx->
				     provisioned_mac_addr[i].bytes[0],
				     (uint8_t *) &macaddr[i].bytes[0],
				     QDF_MAC_ADDR_SIZE);
		} else {
			status = QDF_STATUS_E_FAILURE;
			break;
		}
	}
	return status;
}

/**
 * hdd_apply_cfg_ini() - apply the ini configuration file
 * @hdd_ctx: the pointer to hdd context
 * @iniTable: pointer to configuration table
 * @entries: number fo the configuration entries
 * It overwrites the MAC address if config file exist.
 *
 * Return: QDF_STATUS_SUCCESS if the ini configuration file is correctly parsed,
 *		otherwise QDF_STATUS_E_INVAL
 */
static QDF_STATUS hdd_apply_cfg_ini(struct hdd_context *hdd_ctx,
				    struct hdd_cfg_entry *iniTable,
				    unsigned long entries)
{
	QDF_STATUS match_status = QDF_STATUS_E_FAILURE;
	QDF_STATUS ret_status = QDF_STATUS_SUCCESS;
	unsigned int idx;
	void *pField;
	char *value_str = NULL;
	unsigned long len_value_str;
	char *candidate;
	uint32_t value;
	int32_t svalue;
	void *pStructBase = hdd_ctx->config;
	struct reg_table_entry *pRegEntry = g_registry_table;
	unsigned long cRegTableEntries = QDF_ARRAY_SIZE(g_registry_table);
	uint32_t cbOutString;
	int i;
	int rv;

	BUILD_BUG_ON(MAX_CFG_INI_ITEMS < cRegTableEntries);

	for (idx = 0; idx < cRegTableEntries; idx++, pRegEntry++) {
		/* Calculate the address of the destination field in the structure. */
		pField = ((uint8_t *) pStructBase) + pRegEntry->VarOffset;

		match_status =
			find_cfg_item(iniTable, entries, pRegEntry->RegName,
				      &value_str);

		if ((match_status != QDF_STATUS_SUCCESS)
		    && (pRegEntry->Flags & VAR_FLAGS_REQUIRED)) {
			/* If we could not read the cfg item and it is required, this is an error. */
			hdd_err("Failed to read required config parameter %s", pRegEntry->RegName);
			ret_status = QDF_STATUS_E_FAILURE;
			break;
		}

		if ((WLAN_PARAM_Integer == pRegEntry->RegType) ||
		    (WLAN_PARAM_HexInteger == pRegEntry->RegType)) {
			/* If successfully read from the registry, use the value read.
			 * If not, use the default value.
			 */
			if (match_status == QDF_STATUS_SUCCESS
			    && (WLAN_PARAM_Integer == pRegEntry->RegType)) {
				rv = kstrtou32(value_str, 10, &value);
				if (rv < 0) {
					hdd_warn("Reg Parameter %s invalid. Enforcing default", pRegEntry->RegName);
					value = pRegEntry->VarDefault;
				}
			} else if (match_status == QDF_STATUS_SUCCESS
				   && (WLAN_PARAM_HexInteger ==
				       pRegEntry->RegType)) {
				rv = kstrtou32(value_str, 16, &value);
				if (rv < 0) {
					hdd_warn("Reg parameter %s invalid. Enforcing default", pRegEntry->RegName);
					value = pRegEntry->VarDefault;
				}
			} else {
				value = pRegEntry->VarDefault;
			}

			/* Only if the parameter is set in the ini file, do the range check here */
			if (match_status == QDF_STATUS_SUCCESS &&
			    pRegEntry->Flags & VAR_FLAGS_RANGE_CHECK) {
				if (value > pRegEntry->VarMax) {
					hdd_warn("Reg Parameter %s > allowed Maximum [%u > %lu]. Enforcing Maximum", pRegEntry->RegName,
					       value, pRegEntry->VarMax);
					value = pRegEntry->VarMax;
				}

				if (value < pRegEntry->VarMin) {
					hdd_warn("Reg Parameter %s < allowed Minimum [%u < %lu]. Enforcing Minimum", pRegEntry->RegName,
					       value, pRegEntry->VarMin);
					value = pRegEntry->VarMin;
				}
			}
			/* Only if the parameter is set in the ini file, do the range check here */
			else if (match_status == QDF_STATUS_SUCCESS &&
				 pRegEntry->Flags &
					VAR_FLAGS_RANGE_CHECK_ASSUME_DEFAULT) {
				if (value > pRegEntry->VarMax) {
					hdd_warn("Reg Parameter %s > allowed Maximum [%u > %lu]. Enforcing Default: %lu", pRegEntry->RegName,
					       value, pRegEntry->VarMax,
					       pRegEntry->VarDefault);
					value = pRegEntry->VarDefault;
				}

				if (value < pRegEntry->VarMin) {
					hdd_warn("Reg Parameter %s < allowed Minimum [%u < %lu]. Enforcing Default: %lu", pRegEntry->RegName,
					       value, pRegEntry->VarMin,
					       pRegEntry->VarDefault);
					value = pRegEntry->VarDefault;
				}
			}
			/* Move the variable into the output field. */
			memcpy(pField, &value, pRegEntry->VarSize);
		} else if (WLAN_PARAM_SignedInteger == pRegEntry->RegType) {
			/* If successfully read from the registry, use the value read.
			 * If not, use the default value.
			 */
			if (QDF_STATUS_SUCCESS == match_status) {
				rv = kstrtos32(value_str, 10, &svalue);
				if (rv < 0) {
					hdd_warn("Reg Parameter %s invalid. Enforcing Default", pRegEntry->RegName);
					svalue =
						(int32_t) pRegEntry->VarDefault;
				}
			} else {
				svalue = (int32_t) pRegEntry->VarDefault;
			}

			/* Only if the parameter is set in the ini file, do the range check here */
			if (match_status == QDF_STATUS_SUCCESS &&
			    pRegEntry->Flags & VAR_FLAGS_RANGE_CHECK) {
				if (svalue > (int32_t) pRegEntry->VarMax) {
					hdd_warn("Reg Parameter %s > allowed Maximum "
					       "[%d > %d]. Enforcing Maximum", pRegEntry->RegName,
					       svalue, (int)pRegEntry->VarMax);
					svalue = (int32_t) pRegEntry->VarMax;
				}

				if (svalue < (int32_t) pRegEntry->VarMin) {
					hdd_warn("Reg Parameter %s < allowed Minimum "
					       "[%d < %d]. Enforcing Minimum", pRegEntry->RegName,
					       svalue, (int)pRegEntry->VarMin);
					svalue = (int32_t) pRegEntry->VarMin;
				}
			}
			/* Only if the parameter is set in the ini file, do the range check here */
			else if (match_status == QDF_STATUS_SUCCESS &&
				 pRegEntry->Flags &
					VAR_FLAGS_RANGE_CHECK_ASSUME_DEFAULT) {
				if (svalue > (int32_t) pRegEntry->VarMax) {
					hdd_warn("Reg Parameter %s > allowed Maximum "
					       "[%d > %d]. Enforcing Default: %d", pRegEntry->RegName,
					       svalue, (int)pRegEntry->VarMax,
					       (int)pRegEntry->VarDefault);
					svalue =
						(int32_t) pRegEntry->VarDefault;
				}

				if (svalue < (int32_t) pRegEntry->VarMin) {
					hdd_warn("Reg Parameter %s < allowed Minimum "
					       "[%d < %d]. Enforcing Default: %d", pRegEntry->RegName,
					       svalue, (int)pRegEntry->VarMin,
					       (int)pRegEntry->VarDefault);
					svalue = pRegEntry->VarDefault;
				}
			}
			/* Move the variable into the output field. */
			memcpy(pField, &svalue, pRegEntry->VarSize);
		}
		/* Handle string parameters */
		else if (WLAN_PARAM_String == pRegEntry->RegType) {
#ifdef WLAN_CFG_DEBUG
			hdd_debug("RegName = %s, VarOffset %u VarSize %u VarDefault %s",
				  pRegEntry->RegName, pRegEntry->VarOffset,
				  pRegEntry->VarSize,
				  (char *)pRegEntry->VarDefault);
#endif

			if (match_status == QDF_STATUS_SUCCESS) {
				len_value_str = strlen(value_str);

				if (len_value_str > (pRegEntry->VarSize - 1)) {
					hdd_err("Invalid Value=[%s] specified for Name=[%s] in %s", value_str,
					       pRegEntry->RegName,
					       WLAN_INI_FILE);
					cbOutString =
						QDF_MIN(strlen
							 ((char *)pRegEntry->
								 VarDefault),
							 pRegEntry->VarSize - 1);
					memcpy(pField,
					       (void *)(pRegEntry->VarDefault),
					       cbOutString);
					((uint8_t *) pField)[cbOutString] =
						'\0';
				} else {
					memcpy(pField, (void *)(value_str),
					       len_value_str);
					((uint8_t *) pField)[len_value_str] =
						'\0';
				}
			} else {
				/* Failed to read the string parameter from the registry.  Use the default. */
				cbOutString =
					QDF_MIN(strlen((char *)pRegEntry->VarDefault),
						 pRegEntry->VarSize - 1);
				memcpy(pField, (void *)(pRegEntry->VarDefault),
				       cbOutString);
				((uint8_t *) pField)[cbOutString] = '\0';
			}
		} else if (WLAN_PARAM_MacAddr == pRegEntry->RegType) {
			if (pRegEntry->VarSize != QDF_MAC_ADDR_SIZE) {
				hdd_warn("Invalid VarSize %u for Name=[%s]", pRegEntry->VarSize,
				       pRegEntry->RegName);
				continue;
			}
			candidate = (char *)pRegEntry->VarDefault;
			if (match_status == QDF_STATUS_SUCCESS) {
				len_value_str = strlen(value_str);
				if (len_value_str != (QDF_MAC_ADDR_SIZE * 2)) {
					hdd_err("Invalid MAC addr [%s] specified for Name=[%s] in %s", value_str,
					       pRegEntry->RegName,
					       WLAN_INI_FILE);
				} else
					candidate = value_str;
			}
			/* parse the string and store it in the byte array */
			for (i = 0; i < QDF_MAC_ADDR_SIZE; i++) {
				((char *)pField)[i] =
					(char)(parse_hex_digit(candidate[i * 2]) *
					       16 +
					       parse_hex_digit(candidate[i * 2 + 1]));
			}
		} else {
			hdd_warn("Unknown param type for name[%s] in registry table", pRegEntry->RegName);
		}

		/* did we successfully parse a cfg item for this parameter? */
		if ((match_status == QDF_STATUS_SUCCESS) &&
		    (idx < MAX_CFG_INI_ITEMS)) {
			set_bit(idx, (void *)&hdd_ctx->config->bExplicitCfg);
		}
	}

	return ret_status;
}

/**
 * hdd_execute_config_command() - executes an arbitrary configuration command
 * @reg_table: the pointer to configuration table
 * @tableSize: the size of the configuration table
 * @ini_struct: pointer to the hdd config knob
 * @hdd_ctx: the pointer to hdd context
 * @command: the command to run
 *
 * Return: QDF_STATUS_SUCCESS if the command is found and able to execute,
 *		otherwise the appropriate QDF_STATUS will be returned
 */
static QDF_STATUS hdd_execute_config_command(struct reg_table_entry *reg_table,
					     unsigned long tableSize,
					     uint8_t *ini_struct,
					     struct hdd_context *hdd_ctx,
					     char *command)
{
	struct reg_table_entry *pRegEntry;
	char *clone;
	char *pCmd;
	void *pField;
	char *name;
	char *value_str;
	uint32_t value;
	int32_t svalue;
	size_t len_value_str;
	unsigned int idx;
	unsigned int i;
	QDF_STATUS vstatus;
	int rv;

	/* assume failure until proven otherwise */
	vstatus = QDF_STATUS_E_FAILURE;

	/* clone the command so that we can manipulate it */
	clone = kstrdup(command, GFP_ATOMIC);
	if (NULL == clone)
		return vstatus;

	/* 'clone' will point to the beginning of the string so it can be freed
	 * 'pCmd' will be used to walk/parse the command
	 */
	pCmd = clone;

	/* get rid of leading/trailing whitespace */
	pCmd = i_trim(pCmd);
	if ('\0' == *pCmd) {
		/* only whitespace */
		hdd_err("invalid command, only whitespace:[%s]", command);
		goto done;
	}
	/* parse the <name> = <value> */
	name = pCmd;
	while (('=' != *pCmd) && ('\0' != *pCmd))
		pCmd++;

	if ('\0' == *pCmd) {
		/* did not find '=' */
		hdd_err("invalid command, no '=':[%s]", command);
		goto done;
	}
	/* replace '=' with NUL to terminate the <name> */
	*pCmd++ = '\0';
	name = i_trim(name);
	if ('\0' == *name) {
		/* did not find a name */
		hdd_err("invalid command, no <name>:[%s]", command);
		goto done;
	}

	value_str = i_trim(pCmd);
	if ('\0' == *value_str) {
		/* did not find a value */
		hdd_err("invalid command, no <value>:[%s]", command);
		goto done;
	}
	/* lookup the configuration item */
	for (idx = 0; idx < tableSize; idx++) {
		if (0 == strcmp(name, reg_table[idx].RegName)) {
			/* found a match */
			break;
		}
	}
	if (tableSize == idx) {
		/* did not match the name */
		hdd_err("invalid command, unknown configuration item:[%s]", command);
		goto done;
	}

	pRegEntry = &reg_table[idx];
	if (!(pRegEntry->Flags & VAR_FLAGS_DYNAMIC_CFG)) {
		/* does not support dynamic configuration */
		hdd_err("Global_Registry_Table. %s does not support "
		       "dynamic configuration", name);
		vstatus = QDF_STATUS_E_PERM;
		goto done;
	}

	pField = ini_struct + pRegEntry->VarOffset;

	switch (pRegEntry->RegType) {
	case WLAN_PARAM_Integer:
		rv = kstrtou32(value_str, 10, &value);
		if (rv < 0)
			goto done;
		if (value < pRegEntry->VarMin) {
			/* out of range */
			hdd_err("Invalid command, value %u < min value %lu", value, pRegEntry->VarMin);
			goto done;
		}
		if (value > pRegEntry->VarMax) {
			/* out of range */
			hdd_err("Invalid command, value %u > max value %lu", value, pRegEntry->VarMax);
			goto done;
		}
		memcpy(pField, &value, pRegEntry->VarSize);
		break;

	case WLAN_PARAM_HexInteger:
		rv = kstrtou32(value_str, 16, &value);
		if (rv < 0)
			goto done;
		if (value < pRegEntry->VarMin) {
			/* out of range */
			hdd_err("Invalid command, value %x < min value %lx", value, pRegEntry->VarMin);
			goto done;
		}
		if (value > pRegEntry->VarMax) {
			/* out of range */
			hdd_err("Invalid command, value %x > max value %lx", value, pRegEntry->VarMax);
			goto done;
		}
		memcpy(pField, &value, pRegEntry->VarSize);
		break;

	case WLAN_PARAM_SignedInteger:
		rv = kstrtos32(value_str, 10, &svalue);
		if (rv < 0)
			goto done;
		if (svalue < (int32_t) pRegEntry->VarMin) {
			/* out of range */
			hdd_err("Invalid command, value %d < min value %d", svalue, (int)pRegEntry->VarMin);
			goto done;
		}
		if (svalue > (int32_t) pRegEntry->VarMax) {
			/* out of range */
			hdd_err("Invalid command, value %d > max value %d", svalue, (int)pRegEntry->VarMax);
			goto done;
		}
		memcpy(pField, &svalue, pRegEntry->VarSize);
		break;

	case WLAN_PARAM_String:
		len_value_str = strlen(value_str);
		if (len_value_str > (pRegEntry->VarSize - 1)) {
			/* too big */
			hdd_err("Invalid command, string [%s] length "
			       "%zu exceeds maximum length %u", value_str,
			       len_value_str, (pRegEntry->VarSize - 1));
			goto done;
		}
		/* copy string plus NUL */
		memcpy(pField, value_str, (len_value_str + 1));
		break;

	case WLAN_PARAM_MacAddr:
		len_value_str = strlen(value_str);
		if (len_value_str != (QDF_MAC_ADDR_SIZE * 2)) {
			/* out of range */
			hdd_err("Invalid command, MAC address [%s] length "
			       "%zu is not expected length %u", value_str,
			       len_value_str, (QDF_MAC_ADDR_SIZE * 2));
			goto done;
		}
		/* parse the string and store it in the byte array */
		for (i = 0; i < QDF_MAC_ADDR_SIZE; i++) {
			((char *)pField)[i] = (char)
					      ((parse_hex_digit(value_str[(i * 2)]) * 16) +
					       parse_hex_digit(value_str[(i * 2) + 1]));
		}
		break;

	default:
		goto done;
	}

	/* if we get here, we had a successful modification */
	vstatus = QDF_STATUS_SUCCESS;

	/* config table has been modified, is there a notifier? */
	if (NULL != pRegEntry->pfnDynamicnotify)
		(pRegEntry->pfnDynamicnotify)(hdd_ctx, pRegEntry->notifyId);

	/* note that this item was explicitly configured */
	if (idx < MAX_CFG_INI_ITEMS)
		set_bit(idx, (void *)&hdd_ctx->config->bExplicitCfg);

done:
	kfree(clone);
	return vstatus;
}

/**
 * hdd_set_power_save_offload_config() - set power save offload configuration
 * @hdd_ctx: the pointer to hdd context
 *
 * Return: none
 */
static void hdd_set_power_save_offload_config(struct hdd_context *hdd_ctx)
{
	uint32_t listen_interval = 0;

	if (strcmp(ucfg_mlme_get_power_usage(hdd_ctx->psoc), "Min") == 0)
		ucfg_mlme_get_bmps_min_listen_interval(hdd_ctx->psoc,
						       &listen_interval);
	else if (strcmp(ucfg_mlme_get_power_usage(hdd_ctx->psoc), "Max") == 0)
		ucfg_mlme_get_bmps_max_listen_interval(hdd_ctx->psoc,
						       &listen_interval);
	/*
	 * Based on Mode Set the LI
	 * Otherwise default LI value of 1 will
	 * be taken
	 */
	if (listen_interval) {
		/*
		 * setcfg for listenInterval.
		 * Make sure CFG is updated because PE reads this
		 * from CFG at the time of assoc or reassoc
		 */
		ucfg_mlme_set_sap_listen_interval(hdd_ctx->psoc,
						  listen_interval);
	}
}

/**
 * hdd_update_mac_config() - update MAC address from cfg file
 * @hdd_ctx: the pointer to hdd context
 *
 * It overwrites the MAC address if config file exist.
 *
 * Return: QDF_STATUS_SUCCESS if the MAC address is found from cfg file
 *      and overwritten, otherwise QDF_STATUS_E_INVAL
 */
QDF_STATUS hdd_update_mac_config(struct hdd_context *hdd_ctx)
{
	int status, i = 0;
	const struct firmware *fw = NULL;
	char *line, *buffer = NULL;
	char *temp = NULL;
	char *name, *value;
	int max_mac_addr = QDF_MAX_CONCURRENCY_PERSONA;
	struct hdd_cfg_entry macTable[QDF_MAX_CONCURRENCY_PERSONA];
	tSirMacAddr customMacAddr;

	QDF_STATUS qdf_status = QDF_STATUS_SUCCESS;

	memset(macTable, 0, sizeof(macTable));
	status = request_firmware(&fw, WLAN_MAC_FILE, hdd_ctx->parent_dev);
	if (status) {
		/*
		 * request_firmware "fails" if the file is not found, which is a
		 * valid setup for us, so log using debug instead of error
		 */
		hdd_debug("request_firmware failed; status:%d", status);
		return QDF_STATUS_E_FAILURE;
	}

	if (!fw || !fw->data || !fw->size) {
		hdd_alert("invalid firmware");
		qdf_status = QDF_STATUS_E_INVAL;
		goto config_exit;
	}

	hdd_debug("wlan_mac.bin size %zu", fw->size);

	temp = qdf_mem_malloc(fw->size + 1);

	if (temp == NULL) {
		hdd_err("fail to alloc memory");
		qdf_status = QDF_STATUS_E_NOMEM;
		goto config_exit;
	}
	buffer = temp;
	qdf_mem_copy(buffer, fw->data, fw->size);
	buffer[fw->size] = 0x0;

	/* data format:
	 * Intf0MacAddress=00AA00BB00CC
	 * Intf1MacAddress=00AA00BB00CD
	 * END
	 */
	while (buffer != NULL) {
		line = get_next_line(buffer);
		buffer = i_trim(buffer);

		if (strlen((char *)buffer) == 0 || *buffer == '#') {
			buffer = line;
			continue;
		}
		if (strncmp(buffer, "END", 3) == 0)
			break;

		name = buffer;
		buffer = strnchr(buffer, strlen(buffer), '=');
		if (buffer) {
			*buffer++ = '\0';
			i_trim(name);
			if (strlen(name) != 0) {
				buffer = i_trim(buffer);
				if (strlen(buffer) == 12) {
					value = buffer;
					macTable[i].name = name;
					macTable[i++].value = value;
					if (i >= QDF_MAX_CONCURRENCY_PERSONA)
						break;
				}
			}
		}
		buffer = line;
	}

	if (i != 0 && i <= QDF_MAX_CONCURRENCY_PERSONA) {
		hdd_debug("%d Mac addresses provided", i);
	} else {
		hdd_err("invalid number of Mac address provided, nMac = %d", i);
		qdf_status = QDF_STATUS_E_INVAL;
		goto config_exit;
	}

	qdf_status = update_mac_from_string(hdd_ctx, &macTable[0], i);
	if (QDF_IS_STATUS_ERROR(qdf_status)) {
		hdd_err("Invalid MAC addresses provided");
		goto config_exit;
	}
	hdd_ctx->num_provisioned_addr = i;
	hdd_debug("Populating remaining %d Mac addresses",
		   max_mac_addr - i);
	hdd_populate_random_mac_addr(hdd_ctx, max_mac_addr - i);

	if (hdd_ctx->num_provisioned_addr)
		qdf_mem_copy(&customMacAddr,
			     &hdd_ctx->provisioned_mac_addr[0].bytes[0],
			     sizeof(tSirMacAddr));
	else
		qdf_mem_copy(&customMacAddr,
			     &hdd_ctx->derived_mac_addr[0].bytes[0],
			     sizeof(tSirMacAddr));

	sme_set_custom_mac_addr(customMacAddr);

config_exit:
	qdf_mem_free(temp);
	release_firmware(fw);
	return qdf_status;
}

/**
 * hdd_disable_runtime_pm() - Override to disable runtime_pm.
 * @cfg_ini: Handle to struct hdd_config
 *
 * Return: None
 */
#ifdef FEATURE_RUNTIME_PM
static void hdd_disable_runtime_pm(struct hdd_config *cfg_ini)
{
	cfg_ini->runtime_pm = 0;
}
#else
static void hdd_disable_runtime_pm(struct hdd_config *cfg_ini)
{
}
#endif

/**
 * hdd_disable_auto_shutdown() - Override to disable auto_shutdown.
 * @cfg_ini: Handle to struct hdd_config
 *
 * Return: None
 */
#ifdef FEATURE_WLAN_AUTO_SHUTDOWN
static void hdd_disable_auto_shutdown(struct hdd_config *cfg_ini)
{
	cfg_ini->wlan_auto_shutdown = 0;
}
#else
static void hdd_disable_auto_shutdown(struct hdd_config *cfg_ini)
{
}
#endif

void hdd_override_all_ps(struct hdd_context *hdd_ctx)
{
	struct hdd_config *cfg_ini = hdd_ctx->config;

	ucfg_mlme_override_bmps_imps(hdd_ctx->psoc);
	hdd_disable_runtime_pm(cfg_ini);
	hdd_disable_auto_shutdown(cfg_ini);
}

/**
 * hdd_parse_config_ini() - parse the ini configuration file
 * @hdd_ctx: the pointer to hdd context
 *
 * This function reads the qcom_cfg.ini file and
 * parses each 'Name=Value' pair in the ini file
 *
 * Return: QDF_STATUS_SUCCESS if the qcom_cfg.ini is correctly read,
 *		otherwise QDF_STATUS_E_INVAL
 */
QDF_STATUS hdd_parse_config_ini(struct hdd_context *hdd_ctx)
{
	int status = 0;
	int i = 0;
	int retry = 0;
	/** Pointer for firmware image data */
	const struct firmware *fw = NULL;
	char *buffer, *line, *pTemp = NULL;
	size_t size;
	char *name, *value;
	struct hdd_cfg_entry *cfg_ini_table;
	QDF_STATUS qdf_status = QDF_STATUS_SUCCESS;

	size = MAX_CFG_INI_ITEMS * sizeof(*cfg_ini_table);
	cfg_ini_table = qdf_mem_malloc(size);

	if (!cfg_ini_table) {
		hdd_err("Failed to alloc %zu bytes for cfg_ini_table", size);
		return QDF_STATUS_E_NOMEM;
	}

	do {
		if (status == -EAGAIN)
			msleep(HDD_CFG_REQUEST_FIRMWARE_DELAY);

		status = request_firmware(&fw, WLAN_INI_FILE,
					  hdd_ctx->parent_dev);

		retry++;
	} while ((retry < HDD_CFG_REQUEST_FIRMWARE_RETRIES) &&
		 (status == -EAGAIN));

	if (status) {
		hdd_alert("request_firmware failed %d", status);
		qdf_status = QDF_STATUS_E_FAILURE;
		goto config_exit;
	}
	if (!fw || !fw->data || !fw->size) {
		hdd_alert("%s download failed", WLAN_INI_FILE);
		qdf_status = QDF_STATUS_E_FAILURE;
		goto config_exit;
	}

	hdd_debug("qcom_cfg.ini Size %zu", fw->size);

	buffer = (char *)qdf_mem_malloc(fw->size);

	if (NULL == buffer) {
		hdd_err("qdf_mem_malloc failure");
		qdf_status = QDF_STATUS_E_NOMEM;
		goto config_exit;
	}
	pTemp = buffer;

	qdf_mem_copy((void *)buffer, (void *)fw->data, fw->size);
	size = fw->size;

	while (buffer != NULL) {
		line = get_next_line(buffer);
		buffer = i_trim(buffer);

		hdd_debug("%s: item", buffer);

		if (strlen((char *)buffer) == 0 || *buffer == '#') {
			buffer = line;
			continue;
		}

		if (strncmp(buffer, "END", 3) == 0)
			break;

		name = buffer;
		while (*buffer != '=' && *buffer != '\0')
			buffer++;
		if (*buffer != '\0') {
			*buffer++ = '\0';
			i_trim(name);
			if (strlen(name) != 0) {
				buffer = i_trim(buffer);
				if (strlen(buffer) > 0) {
					value = buffer;
					while (*buffer != '\0')
						buffer++;
					*buffer = '\0';
					cfg_ini_table[i].name = name;
					cfg_ini_table[i++].value = value;
					if (i >= MAX_CFG_INI_ITEMS) {
						hdd_err("Number of items in %s > %d",
							WLAN_INI_FILE,
							MAX_CFG_INI_ITEMS);
						break;
					}
				}
			}
		}
		buffer = line;
	}

	/* Loop through the registry table and apply all these configs */
	qdf_status = hdd_apply_cfg_ini(hdd_ctx, cfg_ini_table, i);

config_exit:
	release_firmware(fw);
	qdf_mem_free(pTemp);
	qdf_mem_free(cfg_ini_table);
	return qdf_status;
}

/**
 * hdd_cfg_xlate_to_csr_phy_mode() - convert PHY mode
 * @dot11Mode: the mode to convert
 *
 * Convert the configuration PHY mode to CSR PHY mode
 *
 * Return: the CSR phy mode value
 */
eCsrPhyMode hdd_cfg_xlate_to_csr_phy_mode(enum hdd_dot11_mode dot11Mode)
{
	if (cds_is_sub_20_mhz_enabled())
		return eCSR_DOT11_MODE_abg;

	switch (dot11Mode) {
	case (eHDD_DOT11_MODE_abg):
		return eCSR_DOT11_MODE_abg;
	case (eHDD_DOT11_MODE_11b):
		return eCSR_DOT11_MODE_11b;
	case (eHDD_DOT11_MODE_11g):
		return eCSR_DOT11_MODE_11g;
	default:
	case (eHDD_DOT11_MODE_11n):
		return eCSR_DOT11_MODE_11n;
	case (eHDD_DOT11_MODE_11g_ONLY):
		return eCSR_DOT11_MODE_11g_ONLY;
	case (eHDD_DOT11_MODE_11n_ONLY):
		return eCSR_DOT11_MODE_11n_ONLY;
	case (eHDD_DOT11_MODE_11b_ONLY):
		return eCSR_DOT11_MODE_11b_ONLY;
	case (eHDD_DOT11_MODE_11ac_ONLY):
		return eCSR_DOT11_MODE_11ac_ONLY;
	case (eHDD_DOT11_MODE_11ac):
		return eCSR_DOT11_MODE_11ac;
	case (eHDD_DOT11_MODE_AUTO):
		return eCSR_DOT11_MODE_AUTO;
	case (eHDD_DOT11_MODE_11a):
		return eCSR_DOT11_MODE_11a;
	case (eHDD_DOT11_MODE_11ax_ONLY):
		return eCSR_DOT11_MODE_11ax_ONLY;
	case (eHDD_DOT11_MODE_11ax):
		return eCSR_DOT11_MODE_11ax;
	}

}

/**
 * hdd_set_idle_ps_config() - set idle power save configuration
 * @hdd_ctx: the pointer to hdd context
 * @val: the value to configure
 *
 * Return: QDF_STATUS_SUCCESS if command set correctly,
 *		otherwise the QDF_STATUS return from SME layer
 */
QDF_STATUS hdd_set_idle_ps_config(struct hdd_context *hdd_ctx, bool val)
{
	QDF_STATUS status;

	hdd_debug("Enter Val %d", val);

	if (hdd_ctx->imps_enabled == val) {
		hdd_info("Already in the requested power state:%d", val);
		return QDF_STATUS_SUCCESS;
	}

	status = sme_set_idle_powersave_config(val);
	if (QDF_STATUS_SUCCESS != status) {
		hdd_err("Fail to Set Idle PS Config val %d", val);
		return status;
	}

	hdd_ctx->imps_enabled = val;

	return status;
}

/**
 * hdd_set_fine_time_meas_cap() - set fine timing measurement capability
 * @hdd_ctx: HDD context
 *
 * This function is used to pass fine timing measurement capability coming
 * from INI to SME. This function make sure that configure INI is supported
 * by the device. Use bit mask to mask out the unsupported capabilities.
 *
 * Return: None
 */
static void hdd_set_fine_time_meas_cap(struct hdd_context *hdd_ctx)
{
	uint32_t capability = 0;

	ucfg_mlme_get_fine_time_meas_cap(hdd_ctx->psoc, &capability);
	ucfg_wifi_pos_set_ftm_cap(hdd_ctx->psoc, capability);
	hdd_debug("fine time meas capability - Enabled: %04x", capability);
}

/**
 * hdd_convert_string_to_u8_array() - used to convert string into u8 array
 * @str: String to be converted
 * @hex_array: Array where converted value is stored
 * @len: Length of the populated array
 * @array_max_len: Maximum length of the array
 * @to_hex: true, if conversion required for hex string
 *
 * This API is called to convert string (each byte separated by
 * a comma) into an u8 array
 *
 * Return: QDF_STATUS
 */

static QDF_STATUS hdd_convert_string_to_array(char *str, uint8_t *array,
			     uint8_t *len, uint16_t array_max_len, bool to_hex)
{
	char *format, *s = str;

	if (str == NULL || array == NULL || len == NULL)
		return QDF_STATUS_E_INVAL;

	format = (to_hex) ? "%02x" : "%d";

	*len = 0;
	while ((s != NULL) && (*len < array_max_len)) {
		int val;
		/* Increment length only if sscanf successfully extracted
		 * one element. Any other return value means error.
		 * Ignore it.
		 */
		if (sscanf(s, format, &val) == 1) {
			array[*len] = (uint8_t) val;
			*len += 1;
		}

		s = strpbrk(s, ",");
		if (s)
			s++;
	}

	return QDF_STATUS_SUCCESS;
}

QDF_STATUS hdd_string_to_u8_array(char *str, uint8_t *array,
				  uint8_t *len, uint16_t array_max_len)
{
	return hdd_convert_string_to_array(str, array, len,
					   array_max_len, false);
}

/**
 * hdd_hex_string_to_u16_array() - convert a hex string to a uint16 array
 * @str: input string
 * @int_array: pointer to input array of type uint16
 * @len: pointer to number of elements which the function adds to the array
 * @int_array_max_len: maximum number of elements in input uint16 array
 *
 * This function is used to convert a space separated hex string to an array of
 * uint16_t. For example, an input string str = "a b c d" would be converted to
 * a unint16 array, int_array = {0xa, 0xb, 0xc, 0xd}, *len = 4.
 * This assumes that input value int_array_max_len >= 4.
 *
 * Return: QDF_STATUS_SUCCESS - if the conversion is successful
 *         non zero value     - if the conversion is a failure
 */
QDF_STATUS hdd_hex_string_to_u16_array(char *str,
		uint16_t *int_array, uint8_t *len, uint8_t int_array_max_len)
{
	char *s = str;
	uint32_t val = 0;

	if (str == NULL || int_array == NULL || len == NULL)
		return QDF_STATUS_E_INVAL;

	hdd_debug("str %pK intArray %pK intArrayMaxLen %d",
		s, int_array, int_array_max_len);

	*len = 0;

	while ((s != NULL) && (*len < int_array_max_len)) {
		/*
		 * Increment length only if sscanf successfully extracted one
		 * element. Any other return value means error. Ignore it.
		 */
		if (sscanf(s, "%x", &val) == 1) {
			int_array[*len] = (uint16_t) val;
			hdd_debug("s %pK val %x intArray[%d]=0x%x",
				s, val, *len, int_array[*len]);
			*len += 1;
		}
		s = strpbrk(s, " ");
		if (s)
			s++;
	}
	return QDF_STATUS_SUCCESS;
}

/**
 * hdd_update_config_cfg() - API to update INI setting based on hw/fw caps
 * @hdd_ctx: pointer to hdd_ctx
 *
 * This API reads the cfg file which is updated with hardware/firmware
 * capabilities and intersect it with INI setting provided by user. After
 * taking intersection it adjust cfg it self. For example, if user has enabled
 * RX LDPC through INI but hardware/firmware doesn't support it then disable
 * it in CFG file here.
 *
 * Return: true or false based on outcome.
 */
bool hdd_update_config_cfg(struct hdd_context *hdd_ctx)
{
	bool status = true;

	/*
	 * During the initialization both 2G and 5G capabilities should be same.
	 * So read 5G HT capablity and update 2G and 5G capablities.
	 */

	if (0 != hdd_update_he_cap_in_cfg(hdd_ctx)) {
		status = false;
		hdd_err("Couldn't set HE CAP in cfg");
	}

	return status;
}

/**
 * hdd_set_policy_mgr_user_cfg() -initializes the policy manager
 * configuration parameters
 *
 * @hdd_ctx: the pointer to hdd context
 *
 * Return: QDF_STATUS_SUCCESS if configuration is correctly applied,
 *		otherwise the appropriate QDF_STATUS would be returned
 */
QDF_STATUS hdd_set_policy_mgr_user_cfg(struct hdd_context *hdd_ctx)
{
	QDF_STATUS status;
	struct policy_mgr_user_cfg *user_cfg;

	user_cfg = qdf_mem_malloc(sizeof(*user_cfg));
	if (NULL == user_cfg) {
		hdd_err("unable to allocate user_cfg");
		return QDF_STATUS_E_NOMEM;
	}

	status = ucfg_mlme_get_vht_enable2x2(hdd_ctx->psoc,
					     &user_cfg->enable2x2);
	if (!QDF_IS_STATUS_SUCCESS(status))
		hdd_err("unable to get vht_enable2x2");

	user_cfg->sub_20_mhz_enabled = cds_is_sub_20_mhz_enabled();
	status = policy_mgr_set_user_cfg(hdd_ctx->psoc, user_cfg);
	qdf_mem_free(user_cfg);

	return status;
}

eCsrRoamWmmUserModeType hdd_to_csr_wmm_mode(uint8_t mode)
{
	switch (mode) {
	case HDD_WMM_USER_MODE_QBSS_ONLY:
		return eCsrRoamWmmQbssOnly;
	case HDD_WMM_USER_MODE_NO_QOS:
		return eCsrRoamWmmNoQos;
	case HDD_WMM_USER_MODE_AUTO:
	default:
		return eCsrRoamWmmAuto;
	}
}

static
QDF_STATUS hdd_set_sme_cfgs_related_to_plcy_mgr(struct hdd_context *hdd_ctx,
						tSmeConfigParams *sme_cfg)
{
	uint8_t mcc_to_scc_switch = 0, is_force_1x1 = 0, allow_diff_bi = 0;
	uint8_t conc_rule1 = 0, conc_rule2 = 0, sta_cxn_5g = 0;

	if (QDF_STATUS_SUCCESS !=
	    ucfg_policy_mgr_get_mcc_scc_switch(hdd_ctx->psoc,
					       &mcc_to_scc_switch)) {
		hdd_err("can't get mcc to scc switch");
		return QDF_STATUS_E_FAILURE;
	}
	sme_cfg->csrConfig.cc_switch_mode = mcc_to_scc_switch;

	if (QDF_STATUS_SUCCESS !=
	    ucfg_policy_mgr_get_conc_rule1(hdd_ctx->psoc,
					   &conc_rule1)) {
		hdd_err("can't get conc rule1");
		return QDF_STATUS_E_FAILURE;
	}
	sme_cfg->csrConfig.conc_custom_rule1 = conc_rule1;

	if (QDF_STATUS_SUCCESS !=
	    ucfg_policy_mgr_get_conc_rule2(hdd_ctx->psoc,
					   &conc_rule2)) {
		hdd_err("can't get conc rule2");
		return QDF_STATUS_E_FAILURE;
	}
	sme_cfg->csrConfig.conc_custom_rule2 = conc_rule2;

	if (QDF_STATUS_SUCCESS !=
	    ucfg_policy_mgr_get_sta_cxn_5g_band(hdd_ctx->psoc,
						&sta_cxn_5g)) {
		hdd_err("can't get conc rule2");
		return QDF_STATUS_E_FAILURE;
	}
	sme_cfg->csrConfig.is_sta_connection_in_5gz_enabled = sta_cxn_5g;

	if (QDF_STATUS_SUCCESS !=
	    ucfg_policy_mgr_get_force_1x1(hdd_ctx->psoc,
					  &is_force_1x1)) {
		hdd_err("can't get force 1x1 flag");
		return QDF_STATUS_E_FAILURE;
	}
	sme_cfg->csrConfig.is_force_1x1 = is_force_1x1;

	if (QDF_STATUS_SUCCESS !=
	    ucfg_policy_mgr_get_allow_mcc_go_diff_bi(hdd_ctx->psoc,
						     &allow_diff_bi)) {
		hdd_err("can't get allow mcc go diff BI flag");
		return QDF_STATUS_E_FAILURE;
	}
	sme_cfg->csrConfig.fAllowMCCGODiffBI = allow_diff_bi;

	return QDF_STATUS_SUCCESS;
}

#ifdef FEATURE_AP_MCC_CH_AVOIDANCE
static QDF_STATUS hdd_set_sap_mcc_chnl_avoid(tSmeConfigParams *sme_cfg,
					     uint8_t val)
{
	sme_cfg->csrConfig.sap_channel_avoidance = val;
	return QDF_STATUS_SUCCESS;
}
#else
static QDF_STATUS hdd_set_sap_mcc_chnl_avoid(tSmeConfigParams *sme_cfg,
					     uint8_t val)
{
	return QDF_STATUS_SUCCESS;
}
#endif

static
QDF_STATUS hdd_set_sme_cfgs_related_to_mlme(struct hdd_context *hdd_ctx,
					    tSmeConfigParams *sme_cfg)
{
	QDF_STATUS status;
	uint8_t wmm_mode = 0, enable_mcc = 0, sap_mcc_avoid = 0;
	uint8_t mcc_rts_cts = 0, mcc_bcast_prob_rsp = 0;
	uint32_t mcast_mcc_rest_time = 0;
	bool b80211e_enabled = 0;

	status = ucfg_mlme_get_80211e_is_enabled(hdd_ctx->psoc,
						 &b80211e_enabled);
	if (!QDF_IS_STATUS_SUCCESS(status)) {
		hdd_err("Get b80211e_enabled failed");
		return QDF_STATUS_E_FAILURE;
	}
	sme_cfg->csrConfig.Is11eSupportEnabled = b80211e_enabled;

	status = ucfg_mlme_get_wmm_mode(hdd_ctx->psoc, &wmm_mode);
	if (!QDF_IS_STATUS_SUCCESS(status)) {
		hdd_err("Get wmm_mode failed");
		return QDF_STATUS_E_FAILURE;
	}
	sme_cfg->csrConfig.WMMSupportMode = hdd_to_csr_wmm_mode(wmm_mode);
	hdd_debug("wmm_mode=%d 802_11e_enabled=%d", wmm_mode, b80211e_enabled);

	status = ucfg_mlme_get_mcc_feature(hdd_ctx->psoc, &enable_mcc);
	if (!QDF_IS_STATUS_SUCCESS(status)) {
		hdd_err("ucfg_mlme_get_mcc_feature fail, use def");
		return QDF_STATUS_E_FAILURE;
	}
	sme_cfg->csrConfig.fEnableMCCMode = enable_mcc;

	status = ucfg_mlme_get_mcc_rts_cts_prot(hdd_ctx->psoc, &mcc_rts_cts);
	if (!QDF_IS_STATUS_SUCCESS(status)) {
		hdd_err("ucfg_mlme_get_mcc_rts_cts_prot fail, use def");
		return QDF_STATUS_E_FAILURE;
	}
	sme_cfg->csrConfig.mcc_rts_cts_prot_enable = mcc_rts_cts;

	status = ucfg_mlme_get_mcc_bcast_prob_resp(hdd_ctx->psoc,
						   &mcc_bcast_prob_rsp);
	if (!QDF_IS_STATUS_SUCCESS(status)) {
		hdd_err("ucfg_mlme_get_mcc_bcast_prob_resp fail, use def");
		return QDF_STATUS_E_FAILURE;
	}
	sme_cfg->csrConfig.mcc_bcast_prob_resp_enable = mcc_bcast_prob_rsp;

	status = ucfg_mlme_get_sta_miracast_mcc_rest_time(hdd_ctx->psoc,
							  &mcast_mcc_rest_time);
	if (!QDF_IS_STATUS_SUCCESS(status)) {
		hdd_err("ucfg_mlme_get_sta_miracast_mcc_rest_time, use def");
		return QDF_STATUS_E_FAILURE;
	}
	sme_cfg->csrConfig.f_sta_miracast_mcc_rest_time_val =
							mcast_mcc_rest_time;
	status = ucfg_mlme_get_sap_mcc_chnl_avoid(hdd_ctx->psoc,
						  &sap_mcc_avoid);
	if (!QDF_IS_STATUS_SUCCESS(status)) {
		hdd_err("ucfg_mlme_get_sap_mcc_chnl_avoid, use def");
		return QDF_STATUS_E_FAILURE;
	}
	status = hdd_set_sap_mcc_chnl_avoid(sme_cfg, sap_mcc_avoid);

	return status;
}

/**
 * hdd_set_sme_config() -initializes the sme configuration parameters
 *
 * @hdd_ctx: the pointer to hdd context
 *
 * Return: QDF_STATUS_SUCCESS if configuration is correctly applied,
 *		otherwise the appropriate QDF_STATUS would be returned
 */
QDF_STATUS hdd_set_sme_config(struct hdd_context *hdd_ctx)
{
	QDF_STATUS status = QDF_STATUS_SUCCESS;
	tSmeConfigParams *smeConfig;
	mac_handle_t mac_handle = hdd_ctx->mac_handle;
	bool roam_scan_enabled;
	bool enable_dfs_scan = true;
	uint32_t channel_bonding_mode;

#ifdef FEATURE_WLAN_ESE
	bool ese_enabled;
#endif
	struct wlan_mlme_ibss_cfg ibss_cfg = {0};

	struct hdd_config *pConfig = hdd_ctx->config;

	if (QDF_IS_STATUS_ERROR(ucfg_mlme_get_ibss_cfg(
				hdd_ctx->psoc, &ibss_cfg))) {
		hdd_err("Unable to get IBSS config params");
		return QDF_STATUS_E_FAILURE;
	}

	smeConfig = qdf_mem_malloc(sizeof(*smeConfig));
	if (NULL == smeConfig) {
		hdd_err("unable to allocate smeConfig");
		return QDF_STATUS_E_NOMEM;
	}

	/* Config params obtained from the registry
	 * To Do: set regulatory information here
	 */
	smeConfig->csrConfig.phyMode =
		hdd_cfg_xlate_to_csr_phy_mode(pConfig->dot11Mode);

	if (pConfig->dot11Mode == eHDD_DOT11_MODE_abg ||
	    pConfig->dot11Mode == eHDD_DOT11_MODE_11b ||
	    pConfig->dot11Mode == eHDD_DOT11_MODE_11g ||
	    pConfig->dot11Mode == eHDD_DOT11_MODE_11b_ONLY ||
	    pConfig->dot11Mode == eHDD_DOT11_MODE_11g_ONLY) {
		smeConfig->csrConfig.channelBondingMode24GHz = 0;
		smeConfig->csrConfig.channelBondingMode5GHz = 0;
	} else {
		ucfg_mlme_get_channel_bonding_24ghz(hdd_ctx->psoc,
						    &channel_bonding_mode);
		smeConfig->csrConfig.channelBondingMode24GHz =
			channel_bonding_mode;
		ucfg_mlme_get_channel_bonding_5ghz(hdd_ctx->psoc,
						   &channel_bonding_mode);
		smeConfig->csrConfig.channelBondingMode5GHz =
			channel_bonding_mode;
	}
	/* Remaining config params not obtained from registry
	 * On RF EVB beacon using channel 1.
	 */
	/* This param cannot be configured from INI */
	smeConfig->csrConfig.send_smps_action = true;
	smeConfig->csrConfig.AdHocChannel5G = ibss_cfg.adhoc_ch_5g;
	smeConfig->csrConfig.AdHocChannel24 = ibss_cfg.adhoc_ch_2g;
	smeConfig->csrConfig.ProprietaryRatesEnabled = 0;
	smeConfig->csrConfig.HeartbeatThresh50 = 40;
	ucfg_scan_cfg_get_dfs_chan_scan_allowed(hdd_ctx->psoc,
						&enable_dfs_scan);
	smeConfig->csrConfig.fEnableDFSChnlScan = enable_dfs_scan;
	smeConfig->csrConfig.Csr11dinfo.Channels.numChannels = 0;

	hdd_set_power_save_offload_config(hdd_ctx);

#ifdef FEATURE_WLAN_ESE
	ucfg_mlme_is_ese_enabled(hdd_ctx->psoc, &ese_enabled);
	if (ese_enabled)
		ucfg_mlme_set_fast_transition_enabled(hdd_ctx->psoc, true);
#endif

	ucfg_mlme_is_roam_scan_offload_enabled(hdd_ctx->psoc,
					       &roam_scan_enabled);

	if (!roam_scan_enabled) {
		/* Disable roaming in concurrency if roam scan
		 * offload is disabled
		 */
		ucfg_mlme_set_fast_roam_in_concurrency_enabled(
					hdd_ctx->psoc, false);
	}

	smeConfig->csrConfig.isCoalesingInIBSSAllowed =
						ibss_cfg.coalesing_enable;

	/* Update maximum interfaces information */
	smeConfig->csrConfig.max_intf_count = hdd_ctx->max_intf_count;

	hdd_set_fine_time_meas_cap(hdd_ctx);

	cds_set_multicast_logging(hdd_ctx->config->multicast_host_fw_msgs);

	smeConfig->csrConfig.sta_roam_policy_params.dfs_mode =
		CSR_STA_ROAM_POLICY_DFS_ENABLED;
	smeConfig->csrConfig.sta_roam_policy_params.skip_unsafe_channels = 0;

	smeConfig->snr_monitor_enabled = hdd_ctx->config->fEnableSNRMonitoring;

	status = hdd_set_sme_cfgs_related_to_mlme(hdd_ctx, smeConfig);
	if (!QDF_IS_STATUS_SUCCESS(status))
		hdd_err("hdd_set_sme_cfgs_related_to_mlme() fail: %d", status);
	status = hdd_set_sme_cfgs_related_to_plcy_mgr(hdd_ctx, smeConfig);
	if (!QDF_IS_STATUS_SUCCESS(status))
		hdd_err("hdd_set_sme_cfgs_related_to_plcy_mgr fail: %d",
			status);
	hdd_debug("dot11Mode=%d", pConfig->dot11Mode);
	status = sme_update_config(mac_handle, smeConfig);
	if (!QDF_IS_STATUS_SUCCESS(status))
		hdd_err("sme_update_config() failure: %d", status);

	qdf_mem_free(smeConfig);
	return status;
}

/**
 * hdd_execute_global_config_command() - execute the global config command
 * @hdd_ctx: the pointer to hdd context
 * @command: the command to run
 *
 * Return: the QDF_STATUS return from hdd_execute_config_command
 */
QDF_STATUS hdd_execute_global_config_command(struct hdd_context *hdd_ctx,
					     char *command)
{
	return hdd_execute_config_command(g_registry_table,
					  ARRAY_SIZE(g_registry_table),
					  (uint8_t *) hdd_ctx->config,
					  hdd_ctx, command);
}

static void print_info_handler(const char *buf)
{
	hdd_nofl_info("%s", buf);
}

static void print_debug_handler(const char *buf)
{
	hdd_nofl_debug("%s", buf);
}

/**
 * hdd_cfg_get_global_config() - get the configuration table
 * @hdd_ctx: pointer to hdd context
 * @pBuf: buffer to store the configuration
 * @buflen: size of the buffer
 *
 * Return: none
 */
void hdd_cfg_get_global_config(struct hdd_context *hdd_ctx, char *buf,
			       int buflen)
{
	hdd_cfg_get_config(g_registry_table,
			   ARRAY_SIZE(g_registry_table),
			   (uint8_t *)hdd_ctx->config, hdd_ctx,
			   &print_info_handler);

	snprintf(buf, buflen,
		 "WLAN configuration written to system log");
}

/**
 * hdd_cfg_print_global_config() - print the configuration table
 * @hdd_ctx: pointer to hdd context
 *
 * Return: none
 */
void hdd_cfg_print_global_config(struct hdd_context *hdd_ctx)
{
	hdd_cfg_get_config(g_registry_table,
			   ARRAY_SIZE(g_registry_table),
			   (uint8_t *)hdd_ctx->config, hdd_ctx,
			   &print_debug_handler);
}

/**
 * hdd_get_pmkid_modes() - returns PMKID mode bits
 * @hdd_ctx: the pointer to hdd context
 *
 * Return: value of pmkid_modes
 */
void hdd_get_pmkid_modes(struct hdd_context *hdd_ctx,
			 struct pmkid_mode_bits *pmkid_modes)
{
	uint32_t cur_pmkid_modes;
	QDF_STATUS status;

	status = ucfg_mlme_get_pmkid_modes(hdd_ctx->psoc, &cur_pmkid_modes);
	if (status != QDF_STATUS_SUCCESS)
		hdd_err("get pmkid modes fail");

	pmkid_modes->fw_okc = (cur_pmkid_modes &
			       CFG_PMKID_MODES_OKC) ? 1 : 0;
	pmkid_modes->fw_pmksa_cache = (cur_pmkid_modes &
				       CFG_PMKID_MODES_PMKSA_CACHING) ? 1 : 0;
}

static void
hdd_populate_vdev_nss(struct wlan_mlme_nss_chains *user_cfg,
		      uint8_t tx_nss,
		      uint8_t rx_nss,
		      enum nss_chains_band_info  band)
{
	user_cfg->rx_nss[band] = rx_nss;
	user_cfg->tx_nss[band] = tx_nss;
}

static QDF_STATUS
hdd_set_nss_params(struct hdd_adapter *adapter,
		   uint8_t tx_nss,
		   uint8_t rx_nss)
{
	enum nss_chains_band_info band;
	struct wlan_mlme_nss_chains user_cfg;
	mac_handle_t mac_handle;
	struct hdd_context *hdd_ctx = WLAN_HDD_GET_CTX(adapter);

	qdf_mem_zero(&user_cfg, sizeof(user_cfg));

	mac_handle = hdd_ctx->mac_handle;
	if (!mac_handle) {
		hdd_err("NULL MAC handle");
		return QDF_STATUS_E_INVAL;
	}

	if (!hdd_is_vdev_in_conn_state(adapter)) {
		hdd_debug("Vdev (id %d) not in connected/started state, cannot accept command",
				adapter->session_id);
		return QDF_STATUS_E_FAILURE;
	}

	for (band = NSS_CHAINS_BAND_2GHZ; band < NSS_CHAINS_BAND_MAX; band++)
		hdd_populate_vdev_nss(&user_cfg, tx_nss,
				      rx_nss, band);
	if (QDF_IS_STATUS_ERROR(
		sme_nss_chains_update(mac_handle,
				      &user_cfg,
				      adapter->session_id)))
		return QDF_STATUS_E_FAILURE;

	/* Check TDLS status and update antenna mode */
	if ((adapter->device_mode == QDF_STA_MODE ||
	     adapter->device_mode == QDF_P2P_CLIENT_MODE) &&
	     policy_mgr_is_sta_active_connection_exists(hdd_ctx->psoc))
		wlan_hdd_tdls_antenna_switch(hdd_ctx, adapter, rx_nss);

	return QDF_STATUS_SUCCESS;
}
/**
 * hdd_update_nss() - Update the number of spatial streams supported.
 * Ensure that nss is either 1 or 2 before calling this.
 *
 * @adapter: the pointer to adapter
 * @nss: the number of spatial streams to be updated
 *
 * This function is used to modify the number of spatial streams
 * supported when not in connected state.
 *
 * Return: QDF_STATUS_SUCCESS if nss is correctly updated,
 *              otherwise QDF_STATUS_E_FAILURE would be returned
 */
QDF_STATUS hdd_update_nss(struct hdd_adapter *adapter, uint8_t nss)
{
	struct hdd_context *hdd_ctx = WLAN_HDD_GET_CTX(adapter);
	uint32_t rx_supp_data_rate, tx_supp_data_rate;
	bool status = true;
	QDF_STATUS qdf_status;
	qdf_size_t val_len;
	struct mlme_ht_capabilities_info ht_cap_info;
	uint8_t mcs_set[SIZE_OF_SUPPORTED_MCS_SET] = {0};
	uint8_t mcs_set_temp[SIZE_OF_SUPPORTED_MCS_SET];
	uint8_t enable2x2;
	mac_handle_t mac_handle;
	bool bval = 0;
	uint8_t tx_nss, rx_nss;

	if ((nss == 2) && (hdd_ctx->num_rf_chains != 2)) {
		hdd_err("No support for 2 spatial streams");
		return QDF_STATUS_E_INVAL;
	}

	if (nss > MAX_VDEV_NSS) {
		hdd_debug("Cannot support %d nss streams", nss);
		return QDF_STATUS_E_INVAL;
	}

	mac_handle = hdd_ctx->mac_handle;
	if (!mac_handle) {
		hdd_err("NULL MAC handle");
		return QDF_STATUS_E_INVAL;
	}

	/* Till now we dont have support for different rx, tx nss values */
	tx_nss = nss;
	rx_nss = nss;

	if (hdd_ctx->dynamic_nss_chains_support)
		return hdd_set_nss_params(adapter, tx_nss, rx_nss);

	/*
	 * The code below is executed only when fw doesn't support dynamic
	 * update of nss and chains per vdev feature, for the upcoming
	 * connection
	 */
	enable2x2 = (nss == 1) ? 0 : 1;

	qdf_status = ucfg_mlme_get_vht_enable2x2(hdd_ctx->psoc, &bval);
	if (!QDF_IS_STATUS_SUCCESS(qdf_status)) {
		hdd_err("unable to get vht_enable2x2");
		return QDF_STATUS_E_FAILURE;
	}

	if (bval == enable2x2) {
		hdd_debug("NSS same as requested");
		return QDF_STATUS_SUCCESS;
	}

	if (sme_is_any_session_in_connected_state(mac_handle)) {
		hdd_err("Connected sessions present, Do not change NSS");
		return QDF_STATUS_E_INVAL;
	}

	qdf_status = ucfg_mlme_set_vht_enable2x2(hdd_ctx->psoc, enable2x2);
	if (!QDF_IS_STATUS_SUCCESS(qdf_status)) {
		hdd_err("Failed to set vht_enable2x2");
		return QDF_STATUS_E_FAILURE;
	}

	if (!enable2x2) {
		/* 1x1 */
		rx_supp_data_rate = VHT_RX_HIGHEST_SUPPORTED_DATA_RATE_1_1;
		tx_supp_data_rate = VHT_TX_HIGHEST_SUPPORTED_DATA_RATE_1_1;
	} else {
		/* 2x2 */
		rx_supp_data_rate = VHT_RX_HIGHEST_SUPPORTED_DATA_RATE_2_2;
		tx_supp_data_rate = VHT_TX_HIGHEST_SUPPORTED_DATA_RATE_2_2;
	}

	/* Update Rx Highest Long GI data Rate */
	qdf_status =
		ucfg_mlme_cfg_set_vht_rx_supp_data_rate(hdd_ctx->psoc,
							rx_supp_data_rate);
	if (!QDF_IS_STATUS_SUCCESS(qdf_status)) {
		hdd_err("Failed to set rx_supp_data_rate");
		status = false;
	}
	/* Update Tx Highest Long GI data Rate */
	qdf_status =
		ucfg_mlme_cfg_set_vht_tx_supp_data_rate(hdd_ctx->psoc,
							tx_supp_data_rate);
	if (!QDF_IS_STATUS_SUCCESS(qdf_status)) {
		hdd_err("Failed to set tx_supp_data_rate");
		status = false;
	}

	qdf_status = ucfg_mlme_get_ht_cap_info(hdd_ctx->psoc, &ht_cap_info);
	if (!QDF_IS_STATUS_SUCCESS(qdf_status)) {
		hdd_err("Failed to get HT Cap info");
		goto skip_ht_cap_update;
	}

	if (!(hdd_ctx->ht_tx_stbc_supported && enable2x2)) {
		ht_cap_info.tx_stbc = 0;
	} else {
		qdf_status =
			ucfg_mlme_cfg_get_vht_tx_stbc(hdd_ctx->psoc, &bval);
		if (!QDF_IS_STATUS_SUCCESS(qdf_status)) {
			hdd_err("Failed to get vht_tx_stbc");
			ht_cap_info.tx_stbc = bval;
		}
	}

	qdf_status = ucfg_mlme_set_ht_cap_info(hdd_ctx->psoc, ht_cap_info);
	if (!QDF_IS_STATUS_SUCCESS(qdf_status)) {
		hdd_err("Could not set the HT_CAP_INFO");
	}
skip_ht_cap_update:
	qdf_status = ucfg_mlme_update_nss_vht_cap(hdd_ctx->psoc);
	if (!QDF_IS_STATUS_SUCCESS(qdf_status)) {
		hdd_err("Failed to set update_nss_vht_cap");
		status = false;
	}

#define WLAN_HDD_RX_MCS_ALL_NSTREAM_RATES 0xff
	val_len = SIZE_OF_SUPPORTED_MCS_SET;
	qdf_status = ucfg_mlme_get_supported_mcs_set(hdd_ctx->psoc,
						     mcs_set_temp,
						     &val_len);
	if (QDF_IS_STATUS_SUCCESS(qdf_status)) {
		mcs_set[0] = mcs_set_temp[0];
		if (enable2x2)
			for (val_len = 0; val_len < nss; val_len++)
				mcs_set[val_len] =
				WLAN_HDD_RX_MCS_ALL_NSTREAM_RATES;
		if (ucfg_mlme_set_supported_mcs_set(
			hdd_ctx->psoc, mcs_set,
			(qdf_size_t)SIZE_OF_SUPPORTED_MCS_SET) ==
			QDF_STATUS_E_FAILURE) {
			status = false;
			hdd_err("Could not pass on MCS SET to CFG");
		}
	} else {
		status = false;
		hdd_err("Could not get MCS SET from CFG");
	}
	sme_update_he_cap_nss(mac_handle, adapter->session_id, nss);
#undef WLAN_HDD_RX_MCS_ALL_NSTREAM_RATES

	if (QDF_STATUS_SUCCESS != sme_update_nss(mac_handle, nss))
		status = false;

	hdd_set_policy_mgr_user_cfg(hdd_ctx);
	return (status == false) ? QDF_STATUS_E_FAILURE : QDF_STATUS_SUCCESS;
}
