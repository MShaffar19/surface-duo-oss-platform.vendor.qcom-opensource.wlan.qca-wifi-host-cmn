/*
 * Copyright (c) 2013-2016 The Linux Foundation. All rights reserved.
 *
 * Previously licensed under the ISC license by Qualcomm Atheros, Inc.
 *
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

/*
 * This file was originally distributed by Qualcomm Atheros, Inc.
 * under proprietary terms before Copyright ownership was assigned
 * to the Linux Foundation.
 */

/**=========================================================================

   \file  lim_trace.c

   \brief implementation for trace related APIs

   \author Sunit Bhatia

   ========================================================================*/

/*--------------------------------------------------------------------------
   Include Files
   ------------------------------------------------------------------------*/

#include "ani_global.h"          /* for tpAniSirGlobal */

#include "lim_trace.h"
#include "lim_timer_utils.h"
#include "cdf_trace.h"

#ifdef LIM_TRACE_RECORD
uint32_t g_mgmt_frame_stats[14];

#define LIM_TRACE_MAX_SUBTYPES 14

static uint8_t *__lim_trace_get_timer_string(uint16_t timerId)
{
	switch (timerId) {
		CASE_RETURN_STRING(eLIM_MIN_CHANNEL_TIMER);
		CASE_RETURN_STRING(eLIM_MAX_CHANNEL_TIMER);
		CASE_RETURN_STRING(eLIM_JOIN_FAIL_TIMER);
		CASE_RETURN_STRING(eLIM_AUTH_FAIL_TIMER);
		CASE_RETURN_STRING(eLIM_AUTH_RESP_TIMER);
		CASE_RETURN_STRING(eLIM_ASSOC_FAIL_TIMER);
		CASE_RETURN_STRING(eLIM_REASSOC_FAIL_TIMER);
		CASE_RETURN_STRING(eLIM_PRE_AUTH_CLEANUP_TIMER);
		CASE_RETURN_STRING(eLIM_CNF_WAIT_TIMER);
		CASE_RETURN_STRING(eLIM_AUTH_RSP_TIMER);
		CASE_RETURN_STRING(eLIM_UPDATE_OLBC_CACHE_TIMER);
		CASE_RETURN_STRING(eLIM_PROBE_AFTER_HB_TIMER);
		CASE_RETURN_STRING(eLIM_ADDTS_RSP_TIMER);
		CASE_RETURN_STRING(eLIM_CHANNEL_SWITCH_TIMER);
		CASE_RETURN_STRING(eLIM_LEARN_DURATION_TIMER);
		CASE_RETURN_STRING(eLIM_QUIET_TIMER);
		CASE_RETURN_STRING(eLIM_QUIET_BSS_TIMER);
		CASE_RETURN_STRING(eLIM_WPS_OVERLAP_TIMER);
		CASE_RETURN_STRING(eLIM_FT_PREAUTH_RSP_TIMER);
		CASE_RETURN_STRING(eLIM_REMAIN_CHN_TIMER);
		CASE_RETURN_STRING(eLIM_PERIODIC_PROBE_REQ_TIMER);
		CASE_RETURN_STRING(eLIM_DISASSOC_ACK_TIMER);
		CASE_RETURN_STRING(eLIM_DEAUTH_ACK_TIMER);
		CASE_RETURN_STRING(eLIM_PERIODIC_JOIN_PROBE_REQ_TIMER);
		CASE_RETURN_STRING(eLIM_INSERT_SINGLESHOT_NOA_TIMER);
		CASE_RETURN_STRING(eLIM_CONVERT_ACTIVE_CHANNEL_TO_PASSIVE);
		CASE_RETURN_STRING(eLIM_AUTH_RETRY_TIMER);
	default:
		return ("UNKNOWN");
		break;
	}
}

static uint8_t *__lim_trace_get_mgmt_drop_reason_string(uint16_t dropReason)
{

	switch (dropReason) {
		CASE_RETURN_STRING(eMGMT_DROP_INFRA_BCN_IN_IBSS);
		CASE_RETURN_STRING(eMGMT_DROP_INVALID_SIZE);
		CASE_RETURN_STRING(eMGMT_DROP_NON_SCAN_MODE_FRAME);
		CASE_RETURN_STRING(eMGMT_DROP_NOT_LAST_IBSS_BCN);
		CASE_RETURN_STRING(eMGMT_DROP_NO_DROP);
		CASE_RETURN_STRING(eMGMT_DROP_SCAN_MODE_FRAME);

	default:
		return ("UNKNOWN");
		break;
	}
}

void lim_trace_init(tpAniSirGlobal pMac)
{
	cdf_trace_register(CDF_MODULE_ID_PE, (tp_cdf_trace_cb) & lim_trace_dump);
}

void lim_trace_dump(tpAniSirGlobal pMac, tp_cdf_trace_record pRecord,
		    uint16_t recIndex)
{

	static char *frameSubtypeStr[LIM_TRACE_MAX_SUBTYPES] = {
		"Association request",
		"Association response",
		"Reassociation request",
		"Reassociation response",
		"Probe request",
		"Probe response",
		NULL,
		NULL,
		"Beacon",
		"ATIM",
		"Disassociation",
		"Authentication",
		"Deauthentication",
		"Action"
	};

	switch (pRecord->code) {
	case TRACE_CODE_MLM_STATE:
		lim_log(pMac, LOG1,
			"%04d %012llu S%d %-14s %-30s(0x%x)", recIndex,
			pRecord->time, pRecord->session, "MLM State:",
			lim_trace_get_mlm_state_string((uint16_t) pRecord->data),
			pRecord->data);
		break;
	case TRACE_CODE_SME_STATE:
		lim_log(pMac, LOG1,
			"%04d %012llu S%d %-14s %-30s(0x%x)", recIndex,
			pRecord->time, pRecord->session, "SME State:",
			lim_trace_get_sme_state_string((uint16_t) pRecord->data),
			pRecord->data);
		break;
	case TRACE_CODE_TX_MGMT:
		lim_log(pMac, LOG1,
			"%04d %012llu S%d %-14s %-30s(0x%x)", recIndex,
			pRecord->time, pRecord->session, "TX Mgmt:",
			frameSubtypeStr[pRecord->data], pRecord->data);
		break;

	case TRACE_CODE_RX_MGMT:
		if (LIM_TRACE_MAX_SUBTYPES <=
		    LIM_TRACE_GET_SUBTYPE(pRecord->data)) {
			lim_log(pMac, LOG1, "Wrong Subtype - %d",
				LIM_TRACE_GET_SUBTYPE(pRecord->data));
		} else {
			lim_log(pMac, LOG1,
				"%04d %012llu S%d %-14s %-30s(%d) SN: %d",
				recIndex, pRecord->time, pRecord->session,
				"RX Mgmt:",
				frameSubtypeStr[LIM_TRACE_GET_SUBTYPE
							(pRecord->data)],
				LIM_TRACE_GET_SUBTYPE(pRecord->data),
				LIM_TRACE_GET_SSN(pRecord->data));
		}
		break;
	case TRACE_CODE_RX_MGMT_DROP:
		lim_log(pMac, LOG1, "%04d %012llu S%d %-14s %-30s(%d)",
			recIndex, pRecord->time, pRecord->session,
			"Drop RX Mgmt:",
			__lim_trace_get_mgmt_drop_reason_string((uint16_t) pRecord->
								data), pRecord->data);
		break;

	case TRACE_CODE_RX_MGMT_TSF:
		lim_log(pMac, LOG1,
			"%04d %012llu S%d %-14s %-30s0x%x(%d)",
			recIndex, pRecord->time, pRecord->session,
			"RX Mgmt TSF:", " ", pRecord->data, pRecord->data);
		break;

	case TRACE_CODE_TX_COMPLETE:
		lim_log(pMac, LOG1, "%04d %012llu S%d %-14s %d", recIndex,
			pRecord->time, pRecord->session, "TX Complete",
			pRecord->data);
		break;

	case TRACE_CODE_TX_SME_MSG:
		lim_log(pMac, LOG1,
			"%04d %012llu S%d %-14s %-30s(0x%x)", recIndex,
			pRecord->time, pRecord->session, "TX SME Msg:",
			mac_trace_get_sme_msg_string((uint16_t) pRecord->data),
			pRecord->data);
		break;
	case TRACE_CODE_RX_SME_MSG:
		lim_log(pMac, LOG1,
			"%04d %012llu S%d %-14s %-30s(0x%x)", recIndex,
			pRecord->time, pRecord->session,
			LIM_TRACE_GET_DEFRD_OR_DROPPED(pRecord->
						       data) ? "Def/Drp LIM Msg:"
			: "RX Sme Msg:",
			mac_trace_get_sme_msg_string((uint16_t) pRecord->data),
			pRecord->data);
		break;

	case TRACE_CODE_TX_WMA_MSG:
		lim_log(pMac, LOG1,
			"%04d %012llu S%d %-14s %-30s(0x%x)", recIndex,
			pRecord->time, pRecord->session, "TX WMA Msg:",
			mac_trace_get_wma_msg_string((uint16_t) pRecord->data),
			pRecord->data);
		break;

	case TRACE_CODE_RX_WMA_MSG:
		lim_log(pMac, LOG1,
			"%04d %012llu S%d %-14s %-30s(0x%x)", recIndex,
			pRecord->time, pRecord->session,
			LIM_TRACE_GET_DEFRD_OR_DROPPED(pRecord->
						       data) ? "Def/Drp LIM Msg:"
			: "RX WMA Msg:",
			mac_trace_get_wma_msg_string((uint16_t) pRecord->data),
			pRecord->data);
		break;

	case TRACE_CODE_TX_LIM_MSG:
		lim_log(pMac, LOG1,
			"%04d %012llu S%d %-14s %-30s(0x%x)", recIndex,
			pRecord->time, pRecord->session, "TX LIM Msg:",
			mac_trace_get_lim_msg_string((uint16_t) pRecord->data),
			pRecord->data);
		break;
	case TRACE_CODE_RX_LIM_MSG:
		lim_log(pMac, LOG1,
			"%04d %012llu S%d %-14s %-30s(0x%x)", recIndex,
			pRecord->time, pRecord->session,
			LIM_TRACE_GET_DEFRD_OR_DROPPED(pRecord->
						       data) ? "Def/Drp LIM Msg:"
			: "RX LIM Msg",
			mac_trace_get_lim_msg_string((uint16_t) pRecord->data),
			pRecord->data);
		break;
	case TRACE_CODE_TX_CFG_MSG:
		lim_log(pMac, LOG1,
			"%04d %012llu S%d %-14s %-30s(0x%x) ", recIndex,
			pRecord->time, pRecord->session, "TX CFG Msg:",
			mac_trace_get_cfg_msg_string((uint16_t) pRecord->data),
			pRecord->data);
		break;
	case TRACE_CODE_RX_CFG_MSG:
		lim_log(pMac, LOG1,
			"%04d %012llu S%d %-14s %-30s(0x%x) ", recIndex,
			pRecord->time, pRecord->session,
			LIM_TRACE_GET_DEFRD_OR_DROPPED(pRecord->
						       data) ? "Def/Drp LIM Msg:"
			: "RX CFG Msg:",
			mac_trace_get_cfg_msg_string((uint16_t)
						     MAC_TRACE_GET_MSG_ID(pRecord->
									  data)),
			pRecord->data);
		break;

	case TRACE_CODE_TIMER_ACTIVATE:
		lim_log(pMac, LOG1,
			"%04d %012llu S%d %-14s %-30s(0x%x)", recIndex,
			pRecord->time, pRecord->session, "Timer Actvtd",
			__lim_trace_get_timer_string((uint16_t) pRecord->data),
			pRecord->data);
		break;
	case TRACE_CODE_TIMER_DEACTIVATE:
		lim_log(pMac, LOG1,
			"%04d %012llu S%d %-14s %-30s(0x%x)", recIndex,
			pRecord->time, pRecord->session, "Timer DeActvtd",
			__lim_trace_get_timer_string((uint16_t) pRecord->data),
			pRecord->data);
		break;

	case TRACE_CODE_INFO_LOG:
		lim_log(pMac, LOG1,
			"%04d %012llu S%d %-14s %-30s(0x%x)",
			recIndex, pRecord->time, pRecord->session,
			"INFORMATION_LOG",
			mac_trace_get_info_log_string((uint16_t) pRecord->data),
			pRecord->data);
		break;
	default:
		lim_log(pMac, LOG1, "%04d %012llu S%d %-14s(%d) (0x%x)",
			recIndex, pRecord->time, pRecord->session,
			"Unknown Code", pRecord->code, pRecord->data);
		break;
	}
}

void mac_trace_msg_tx(tpAniSirGlobal pMac, uint8_t session, uint32_t data)
{

	uint16_t msgId = (uint16_t) MAC_TRACE_GET_MSG_ID(data);
	uint8_t moduleId = (uint8_t) MAC_TRACE_GET_MODULE_ID(data);

	switch (moduleId) {
	case SIR_LIM_MODULE_ID:
		if (msgId >= SIR_LIM_ITC_MSG_TYPES_BEGIN)
			mac_trace(pMac, TRACE_CODE_TX_LIM_MSG, session, data);
		else
			mac_trace(pMac, TRACE_CODE_TX_SME_MSG, session, data);
		break;
	case SIR_WMA_MODULE_ID:
		mac_trace(pMac, TRACE_CODE_TX_WMA_MSG, session, data);
		break;
	case SIR_CFG_MODULE_ID:
		mac_trace(pMac, TRACE_CODE_TX_CFG_MSG, session, data);
		break;
	default:
		mac_trace(pMac, moduleId, session, data);
		break;
	}
}

void mac_trace_msg_tx_new(tpAniSirGlobal pMac, uint8_t module, uint8_t session,
			  uint32_t data)
{
	uint16_t msgId = (uint16_t) MAC_TRACE_GET_MSG_ID(data);
	uint8_t moduleId = (uint8_t) MAC_TRACE_GET_MODULE_ID(data);

	switch (moduleId) {
	case SIR_LIM_MODULE_ID:
		if (msgId >= SIR_LIM_ITC_MSG_TYPES_BEGIN)
			mac_trace_new(pMac, module, TRACE_CODE_TX_LIM_MSG,
				      session, data);
		else
			mac_trace_new(pMac, module, TRACE_CODE_TX_SME_MSG,
				      session, data);
		break;
	case SIR_WMA_MODULE_ID:
		mac_trace_new(pMac, module, TRACE_CODE_TX_WMA_MSG, session, data);
		break;
	case SIR_CFG_MODULE_ID:
		mac_trace_new(pMac, module, TRACE_CODE_TX_CFG_MSG, session, data);
		break;
	default:
		mac_trace(pMac, moduleId, session, data);
		break;
	}
}

/*
 * bit31: Rx message defferred or not
 * bit 0-15: message ID:
 */
void mac_trace_msg_rx(tpAniSirGlobal pMac, uint8_t session, uint32_t data)
{
	uint16_t msgId = (uint16_t) MAC_TRACE_GET_MSG_ID(data);
	uint8_t moduleId = (uint8_t) MAC_TRACE_GET_MODULE_ID(data);

	switch (moduleId) {
	case SIR_LIM_MODULE_ID:
		if (msgId >= SIR_LIM_ITC_MSG_TYPES_BEGIN)
			mac_trace(pMac, TRACE_CODE_RX_LIM_MSG, session, data);
		else
			mac_trace(pMac, TRACE_CODE_RX_SME_MSG, session, data);
		break;
	case SIR_WMA_MODULE_ID:
		mac_trace(pMac, TRACE_CODE_RX_WMA_MSG, session, data);
		break;
	case SIR_CFG_MODULE_ID:
		mac_trace(pMac, TRACE_CODE_RX_CFG_MSG, session, data);
		break;
	default:
		mac_trace(pMac, moduleId, session, data);
		break;
	}
}

/*
 * bit31: Rx message defferred or not
 * bit 0-15: message ID:
 */
void mac_trace_msg_rx_new(tpAniSirGlobal pMac, uint8_t module, uint8_t session,
			  uint32_t data)
{
	uint16_t msgId = (uint16_t) MAC_TRACE_GET_MSG_ID(data);
	uint8_t moduleId = (uint8_t) MAC_TRACE_GET_MODULE_ID(data);

	switch (moduleId) {
	case SIR_LIM_MODULE_ID:
		if (msgId >= SIR_LIM_ITC_MSG_TYPES_BEGIN)
			mac_trace_new(pMac, module, TRACE_CODE_RX_LIM_MSG,
				      session, data);
		else
			mac_trace_new(pMac, module, TRACE_CODE_RX_SME_MSG,
				      session, data);
		break;
	case SIR_WMA_MODULE_ID:
		mac_trace_new(pMac, module, TRACE_CODE_RX_WMA_MSG, session, data);
		break;
	case SIR_CFG_MODULE_ID:
		mac_trace_new(pMac, module, TRACE_CODE_RX_CFG_MSG, session, data);
		break;
	default:
		mac_trace(pMac, moduleId, session, data);
		break;
	}
}

uint8_t *lim_trace_get_mlm_state_string(uint32_t mlmState)
{
	switch (mlmState) {
		CASE_RETURN_STRING(eLIM_MLM_OFFLINE_STATE);
		CASE_RETURN_STRING(eLIM_MLM_IDLE_STATE);
		CASE_RETURN_STRING(eLIM_MLM_WT_PROBE_RESP_STATE);
		CASE_RETURN_STRING(eLIM_MLM_PASSIVE_SCAN_STATE);
		CASE_RETURN_STRING(eLIM_MLM_WT_JOIN_BEACON_STATE);
		CASE_RETURN_STRING(eLIM_MLM_JOINED_STATE);
		CASE_RETURN_STRING(eLIM_MLM_BSS_STARTED_STATE);
		CASE_RETURN_STRING(eLIM_MLM_WT_AUTH_FRAME2_STATE);
		CASE_RETURN_STRING(eLIM_MLM_WT_AUTH_FRAME3_STATE);
		CASE_RETURN_STRING(eLIM_MLM_WT_AUTH_FRAME4_STATE);
		CASE_RETURN_STRING(eLIM_MLM_AUTH_RSP_TIMEOUT_STATE);
		CASE_RETURN_STRING(eLIM_MLM_AUTHENTICATED_STATE);
		CASE_RETURN_STRING(eLIM_MLM_WT_ASSOC_RSP_STATE);
		CASE_RETURN_STRING(eLIM_MLM_WT_REASSOC_RSP_STATE);
		CASE_RETURN_STRING(eLIM_MLM_ASSOCIATED_STATE);
		CASE_RETURN_STRING(eLIM_MLM_REASSOCIATED_STATE);
		CASE_RETURN_STRING(eLIM_MLM_LINK_ESTABLISHED_STATE);
		CASE_RETURN_STRING(eLIM_MLM_WT_ASSOC_CNF_STATE);
		CASE_RETURN_STRING(eLIM_MLM_LEARN_STATE);
		CASE_RETURN_STRING(eLIM_MLM_WT_ADD_BSS_RSP_STATE);
		CASE_RETURN_STRING(eLIM_MLM_WT_DEL_BSS_RSP_STATE);
		CASE_RETURN_STRING(eLIM_MLM_WT_ADD_BSS_RSP_ASSOC_STATE);
		CASE_RETURN_STRING(eLIM_MLM_WT_ADD_BSS_RSP_PREASSOC_STATE);
		CASE_RETURN_STRING(eLIM_MLM_WT_ADD_BSS_RSP_REASSOC_STATE);
		CASE_RETURN_STRING(eLIM_MLM_WT_ADD_STA_RSP_STATE);
		CASE_RETURN_STRING(eLIM_MLM_WT_DEL_STA_RSP_STATE);
		CASE_RETURN_STRING(eLIM_MLM_WT_ASSOC_DEL_STA_RSP_STATE);
		CASE_RETURN_STRING(eLIM_MLM_WT_SET_BSS_KEY_STATE);
		CASE_RETURN_STRING(eLIM_MLM_WT_SET_STA_KEY_STATE);
		CASE_RETURN_STRING(eLIM_MLM_WT_SET_STA_BCASTKEY_STATE);
		CASE_RETURN_STRING(eLIM_MLM_WT_SET_MIMOPS_STATE);
	default:
		return ("UNKNOWN");
		break;
	}
}

uint8_t *lim_trace_get_sme_state_string(uint32_t smeState)
{
	switch (smeState) {

		CASE_RETURN_STRING(eLIM_SME_OFFLINE_STATE);
		CASE_RETURN_STRING(eLIM_SME_IDLE_STATE);
		CASE_RETURN_STRING(eLIM_SME_SUSPEND_STATE);
		CASE_RETURN_STRING(eLIM_SME_WT_SCAN_STATE);
		CASE_RETURN_STRING(eLIM_SME_WT_JOIN_STATE);
		CASE_RETURN_STRING(eLIM_SME_WT_AUTH_STATE);
		CASE_RETURN_STRING(eLIM_SME_WT_ASSOC_STATE);
		CASE_RETURN_STRING(eLIM_SME_WT_REASSOC_STATE);
		CASE_RETURN_STRING(eLIM_SME_WT_REASSOC_LINK_FAIL_STATE);
		CASE_RETURN_STRING(eLIM_SME_JOIN_FAILURE_STATE);
		CASE_RETURN_STRING(eLIM_SME_ASSOCIATED_STATE);
		CASE_RETURN_STRING(eLIM_SME_REASSOCIATED_STATE);
		CASE_RETURN_STRING(eLIM_SME_LINK_EST_STATE);
		CASE_RETURN_STRING(eLIM_SME_LINK_EST_WT_SCAN_STATE);
		CASE_RETURN_STRING(eLIM_SME_WT_PRE_AUTH_STATE);
		CASE_RETURN_STRING(eLIM_SME_WT_DISASSOC_STATE);
		CASE_RETURN_STRING(eLIM_SME_WT_DEAUTH_STATE);
		CASE_RETURN_STRING(eLIM_SME_WT_START_BSS_STATE);
		CASE_RETURN_STRING(eLIM_SME_WT_STOP_BSS_STATE);
		CASE_RETURN_STRING(eLIM_SME_NORMAL_STATE);
		CASE_RETURN_STRING(eLIM_SME_CHANNEL_SCAN_STATE);
		CASE_RETURN_STRING(eLIM_SME_NORMAL_CHANNEL_SCAN_STATE);
	default:
		return ("UNKNOWN");
		break;
	}
}

#endif
