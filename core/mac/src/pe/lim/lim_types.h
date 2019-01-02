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

/*
 * This file lim_types.h contains the definitions used by all
 * all LIM modules.
 * Author:        Chandra Modumudi
 * Date:          02/11/02
 * History:-
 * Date           Modified by    Modification Information
 * --------------------------------------------------------------------
 *
 */
#ifndef __LIM_TYPES_H
#define __LIM_TYPES_H

#include "wni_api.h"
#include "sir_api.h"
#include "sir_common.h"
#include "sir_mac_prot_def.h"
#include "utils_api.h"

#include "lim_api.h"
#include "lim_trace.h"
#include "lim_send_sme_rsp_messages.h"
#include "sys_global.h"
#include "dph_global.h"
#include "parser_api.h"
#include "wma_if.h"

#define LINK_TEST_DEFER 1

#define TRACE_EVENT_CNF_TIMER_DEACT        0x6600
#define TRACE_EVENT_CNF_TIMER_ACT          0x6601
#define TRACE_EVENT_AUTH_RSP_TIMER_DEACT   0x6602
#define TRACE_EVENT_AUTH_RSP_TIMER_ACT     0x6603

/* MLM message types */
#define LIM_MLM_MSG_START           1000
#define LIM_MLM_SCAN_REQ            LIM_MLM_MSG_START
#define LIM_MLM_SCAN_CNF            (LIM_MLM_MSG_START + 1)
#define LIM_MLM_START_CNF           (LIM_MLM_MSG_START + 3)
#define LIM_MLM_JOIN_REQ            (LIM_MLM_MSG_START + 4)
#define LIM_MLM_JOIN_CNF            (LIM_MLM_MSG_START + 5)
#define LIM_MLM_AUTH_REQ            (LIM_MLM_MSG_START + 6)
#define LIM_MLM_AUTH_CNF            (LIM_MLM_MSG_START + 7)
#define LIM_MLM_AUTH_IND            (LIM_MLM_MSG_START + 8)
#define LIM_MLM_ASSOC_REQ           (LIM_MLM_MSG_START + 9)
#define LIM_MLM_ASSOC_CNF           (LIM_MLM_MSG_START + 10)
#define LIM_MLM_ASSOC_IND           (LIM_MLM_MSG_START + 11)
#define LIM_MLM_DISASSOC_REQ        (LIM_MLM_MSG_START + 12)
#define LIM_MLM_DISASSOC_CNF        (LIM_MLM_MSG_START + 13)
#define LIM_MLM_DISASSOC_IND        (LIM_MLM_MSG_START + 14)
#define LIM_MLM_REASSOC_REQ         (LIM_MLM_MSG_START + 15)
#define LIM_MLM_REASSOC_CNF         (LIM_MLM_MSG_START + 16)
#define LIM_MLM_REASSOC_IND         (LIM_MLM_MSG_START + 17)
#define LIM_MLM_DEAUTH_REQ          (LIM_MLM_MSG_START + 18)
#define LIM_MLM_DEAUTH_CNF          (LIM_MLM_MSG_START + 19)
#define LIM_MLM_DEAUTH_IND          (LIM_MLM_MSG_START + 20)
#define LIM_MLM_TSPEC_REQ           (LIM_MLM_MSG_START + 21)
#define LIM_MLM_TSPEC_CNF           (LIM_MLM_MSG_START + 22)
#define LIM_MLM_TSPEC_IND           (LIM_MLM_MSG_START + 23)
#define LIM_MLM_SETKEYS_REQ         (LIM_MLM_MSG_START + 24)
#define LIM_MLM_SETKEYS_CNF         (LIM_MLM_MSG_START + 25)
#define LIM_MLM_LINK_TEST_STOP_REQ  (LIM_MLM_MSG_START + 30)
#define LIM_MLM_PURGE_STA_IND       (LIM_MLM_MSG_START + 31)
/*
 * Values (LIM_MLM_MSG_START + 32) through
 * (LIM_MLM_MSG_START + 40) are unused.
 */

#define LIM_HASH_ADD            0
#define LIM_HASH_UPDATE         1

#define LIM_WEP_IN_FC           1
#define LIM_NO_WEP_IN_FC        0

#define LIM_DECRYPT_ICV_FAIL    1

/* / Definitions to distinquish between Association/Reassociaton */
#define LIM_ASSOC    0
#define LIM_REASSOC  1

/* / Minimum Memory blocks require for different scenario */
#define LIM_MIN_MEM_ASSOC       4

/* / Verifies whether given mac addr matches the CURRENT Bssid */
#define IS_CURRENT_BSSID(mac, addr, pe_session)  (!qdf_mem_cmp(addr, \
								      pe_session->bssId, \
								      sizeof(pe_session->bssId)))
/* / Verifies whether given addr matches the REASSOC Bssid */
#define IS_REASSOC_BSSID(mac, addr, pe_session)  (!qdf_mem_cmp(addr, \
								      pe_session->limReAssocbssId, \
								      sizeof(pe_session->limReAssocbssId)))

#define REQ_TYPE_REGISTRAR                   (0x2)
#define REQ_TYPE_WLAN_MANAGER_REGISTRAR      (0x3)

#define RESP_TYPE_REGISTRAR                  (0x2)
#define RESP_TYPE_ENROLLEE_INFO_ONLY         (0x0)
#define RESP_TYPE_ENROLLEE_OPEN_8021X        (0x1)
#define RESP_TYPE_AP                         (0x3)
#define LIM_TX_FRAMES_THRESHOLD_ON_CHIP       300


#define HAL_TXCOMP_REQUESTED_MASK           0x1 /* bit 0 for TxComp intr requested. */
#define HAL_USE_SELF_STA_REQUESTED_MASK     0x2 /* bit 1 for STA overwrite with selfSta Requested. */
#define HAL_TX_NO_ENCRYPTION_MASK           0x4 /* bit 2. If set, the frame is not to be encrypted */
#if defined(LIBRA_WAPI_SUPPORT)
#define HAL_WAPI_STA_MASK            0x8        /* bit 3. If set, this frame is for WAPI station */
#endif

#define HAL_TRIGGER_ENABLED_AC_MASK         0x10        /* bit 4 for data frames belonging to trigger enabled AC */
#define HAL_USE_NO_ACK_REQUESTED_MASK       0x20

#define HAL_USE_BD_RATE2_FOR_MANAGEMENT_FRAME 0x40      /* Bit 6 will be used to control BD rate for Management frames */
#define HAL_USE_PEER_STA_REQUESTED_MASK   0x80  /* bit 7 will be used to control frames for p2p interface */

#ifdef FEATURE_WLAN_TDLS
#define HAL_TDLS_PEER_STA_MASK              0x80        /* bit 7 set for TDLS peer station */
#endif

#define LIM_DOS_PROTECTION_TIME 1000 //1000ms
#define LIM_MIN_RSSI 0 /* 0dbm */
/* enums used by LIM are as follows */

enum eLimDisassocTrigger {
	eLIM_HOST_DISASSOC,
	eLIM_PEER_ENTITY_DISASSOC,
	eLIM_LINK_MONITORING_DISASSOC,
	eLIM_PROMISCUOUS_MODE_DISASSOC,
	eLIM_HOST_DEAUTH,
	eLIM_PEER_ENTITY_DEAUTH,
	eLIM_LINK_MONITORING_DEAUTH,
	eLIM_JOIN_FAILURE,
	eLIM_REASSOC_REJECT,
	eLIM_DUPLICATE_ENTRY
};

/* Reason code to determine the channel change context while sending
 * WMA_CHNL_SWITCH_REQ message to HAL
 */
enum eChannelChangeReasonCodes {
	LIM_SWITCH_CHANNEL_REASSOC,
	LIM_SWITCH_CHANNEL_JOIN,
	LIM_SWITCH_CHANNEL_OPERATION,   /* Generic change channel */
	LIM_SWITCH_CHANNEL_SAP_DFS,     /* DFS channel change */
	LIM_SWITCH_CHANNEL_HT_WIDTH     /* HT channel width change */
};

typedef struct sLimAuthRspTimeout {
	tSirMacAddr peerMacAddr;
} tLimAuthRspTimeout;

typedef struct sLimMlmStartReq {
	tSirMacSSid ssId;
	tSirBssType bssType;
	tSirMacAddr bssId;
	tSirMacBeaconInterval beaconPeriod;
	uint8_t dtimPeriod;
	tSirMacCfParamSet cfParamSet;
	tSirMacChanNum channelNumber;
	ePhyChanBondState cbMode;
	uint16_t atimWindow;
	tSirMacRateSet rateSet;
	uint8_t sessionId;      /* Added For BT-AMP Support */

	/* Parameters reqd for new HAL (message) interface */
	tSirNwType nwType;
	uint8_t htCapable;
	tSirMacHTOperatingMode htOperMode;
	uint8_t dualCTSProtection;
	uint8_t txChannelWidthSet;
	uint8_t ssidHidden;
	uint8_t wps_state;
	uint8_t obssProtEnabled;
	uint16_t beacon_tx_rate;
	uint32_t cac_duration_ms;
	uint32_t dfs_regdomain;
} tLimMlmStartReq, *tpLimMlmStartReq;

typedef struct sLimMlmStartCnf {
	tSirResultCodes resultCode;
	uint8_t sessionId;
} tLimMlmStartCnf, *tpLimMlmStartCnf;

typedef struct sLimMlmScanCnf {
	tSirResultCodes resultCode;
	uint16_t scanResultLength;
	uint8_t sessionId;
	tSirBssDescription bssDescription[1];
	/*
	 * WARNING: Pls make bssDescription as last variable in struct
	 * tLimMlmScanCnf as it has ieFields followed after this bss
	 * description. Adding a variable after this corrupts the ieFields
	 */
} tLimMlmScanCnf, *tpLimMlmScanCnf;

typedef struct sLimScanResult {
	uint16_t numBssDescriptions;
	tSirBssDescription bssDescription[1];
} tLimScanResult;

typedef struct sLimMlmJoinCnf {
	tSirResultCodes resultCode;
	uint16_t protStatusCode;
	uint8_t sessionId;
} tLimMlmJoinCnf, *tpLimMlmJoinCnf;

typedef struct sLimMlmAssocReq {
	tSirMacAddr peerMacAddr;
	uint16_t capabilityInfo;
	tSirMacListenInterval listenInterval;
	uint8_t sessionId;
} tLimMlmAssocReq, *tpLimMlmAssocReq;

typedef struct sLimMlmAssocCnf {
	tSirResultCodes resultCode;     /* Internal status code. */
	uint16_t protStatusCode;        /* Protocol Status code. */
	uint8_t sessionId;
} tLimMlmAssocCnf, *tpLimMlmAssocCnf;

typedef struct sLimMlmAssocInd {
	tSirMacAddr peerMacAddr;
	uint16_t aid;
	tAniAuthType authType;
	tAniSSID ssId;
	tSirRSNie rsnIE;
	tSirWAPIie wapiIE;
	tSirAddie addIE;        /* additional IE received from the peer, which possibly includes WSC IE and/or P2P IE. */
	tSirMacCapabilityInfo capabilityInfo;
	bool spectrumMgtIndicator;
	tSirMacPowerCapInfo powerCap;
	tSirSupChnl supportedChannels;
	uint8_t sessionId;

	bool WmmStaInfoPresent;

	/* Required for indicating the frames to upper layer */
	uint32_t beaconLength;
	uint8_t *beaconPtr;
	uint32_t assocReqLength;
	uint8_t *assocReqPtr;
	tSirSmeChanInfo chan_info;
	bool ampdu;
	bool sgi_enable;
	bool tx_stbc;
	bool rx_stbc;
	tSirMacHTChannelWidth ch_width;
	enum sir_sme_phy_mode mode;
	uint8_t max_supp_idx;
	uint8_t max_ext_idx;
	uint8_t max_mcs_idx;
	uint8_t rx_mcs_map;
	uint8_t tx_mcs_map;
	uint8_t ecsa_capable;

	tDot11fIEHTCaps ht_caps;
	tDot11fIEVHTCaps vht_caps;
} tLimMlmAssocInd, *tpLimMlmAssocInd;

typedef struct sLimMlmReassocReq {
	tSirMacAddr peerMacAddr;
	uint16_t capabilityInfo;
	tSirMacListenInterval listenInterval;
	uint8_t sessionId;
} tLimMlmReassocReq, *tpLimMlmReassocReq;

typedef struct sLimMlmReassocCnf {
	tSirResultCodes resultCode;
	uint16_t protStatusCode;        /* Protocol Status code. */
	uint8_t sessionId;
} tLimMlmReassocCnf, *tpLimMlmReassocCnf;

typedef struct sLimMlmReassocInd {
	tSirMacAddr peerMacAddr;
	tSirMacAddr currentApAddr;
	uint16_t aid;
	tAniAuthType authType;
	tAniSSID ssId;
	tSirRSNie rsnIE;
	tSirWAPIie wapiIE;
	tSirAddie addIE;        /* additional IE received from the peer, which can be WSC IE and/or P2P IE. */
	tSirMacCapabilityInfo capabilityInfo;
	bool spectrumMgtIndicator;
	tSirMacPowerCapInfo powerCap;
	tSirSupChnl supportedChannels;

	bool WmmStaInfoPresent;

	/* Required for indicating the frames to upper layer */
	uint32_t beaconLength;
	uint8_t *beaconPtr;
	uint32_t assocReqLength;
	uint8_t *assocReqPtr;
	uint8_t ecsa_capable;
} tLimMlmReassocInd, *tpLimMlmReassocInd;

typedef struct sLimMlmAuthCnf {
	tSirMacAddr peerMacAddr;
	tAniAuthType authType;
	tSirResultCodes resultCode;
	uint16_t protStatusCode;
	uint8_t sessionId;
} tLimMlmAuthCnf, *tpLimMlmAuthCnf;

typedef struct sLimMlmDeauthReq {
	struct qdf_mac_addr peer_macaddr;
	uint16_t reasonCode;
	uint16_t deauthTrigger;
	uint16_t aid;
	uint8_t sessionId;      /* Added for BT-AMP SUPPORT */

} tLimMlmDeauthReq, *tpLimMlmDeauthReq;

typedef struct sLimMlmDeauthCnf {
	struct qdf_mac_addr peer_macaddr;
	tSirResultCodes resultCode;
	uint16_t deauthTrigger;
	uint16_t aid;
	uint8_t sessionId;
} tLimMlmDeauthCnf, *tpLimMLmDeauthCnf;

typedef struct sLimMlmDeauthInd {
	tSirMacAddr peerMacAddr;
	uint16_t reasonCode;
	uint16_t deauthTrigger;
	uint16_t aid;
} tLimMlmDeauthInd, *tpLimMlmDeauthInd;

typedef struct sLimMlmDisassocReq {
	struct qdf_mac_addr peer_macaddr;
	uint16_t reasonCode;
	uint16_t disassocTrigger;
	uint16_t aid;
	uint8_t sessionId;
} tLimMlmDisassocReq, *tpLimMlmDisassocReq;

typedef struct sLimMlmDisassocCnf {
	tSirMacAddr peerMacAddr;
	tSirResultCodes resultCode;
	uint16_t disassocTrigger;
	uint16_t aid;
	uint8_t sessionId;
} tLimMlmDisassocCnf, *tpLimMlmDisassocCnf;

typedef struct sLimMlmDisassocInd {
	tSirMacAddr peerMacAddr;
	uint16_t reasonCode;
	uint16_t disassocTrigger;
	uint16_t aid;
	uint8_t sessionId;
} tLimMlmDisassocInd, *tpLimMlmDisassocInd;

typedef struct sLimMlmPurgeStaReq {
	tSirMacAddr peerMacAddr;
	uint16_t aid;
	uint8_t sessionId;      /* Added For BT-AMP Support */
} tLimMlmPurgeStaReq, *tpLimMlmPurgeStaReq;

typedef struct sLimMlmPurgeStaInd {
	tSirMacAddr peerMacAddr;
	uint16_t reasonCode;
	uint16_t purgeTrigger;
	uint16_t aid;
	uint8_t sessionId;
} tLimMlmPurgeStaInd, *tpLimMlmPurgeStaInd;

/**
 * struct sLimMlmSetKeysCnf - set key confirmation parameters
 * @peer_macaddr: peer mac address
 * @resultCode: Result of set key operation
 * @aid: association id
 * @sessionId: PE session id
 * @key_len_nonzero: Keys are non-zero length
 */
typedef struct sLimMlmSetKeysCnf {
	struct qdf_mac_addr peer_macaddr;
	uint16_t resultCode;
	uint16_t aid;
	uint8_t sessionId;
	bool key_len_nonzero;
} tLimMlmSetKeysCnf, *tpLimMlmSetKeysCnf;

typedef struct sLimMlmResetReq {
	tSirMacAddr macAddr;
	uint8_t performCleanup;
	uint8_t sessionId;
} tLimMlmResetReq, *tpLimMlmResetReq;

typedef struct sLimMlmResetCnf {
	tSirMacAddr macAddr;
	tSirResultCodes resultCode;
	uint8_t sessionId;
} tLimMlmResetCnf, *tpLimMlmResetCnf;

typedef struct sLimMlmLinkTestStopReq {
	tSirMacAddr peerMacAddr;
	uint8_t sessionId;
} tLimMlmLinkTestStopReq, *tpLimMlmLinkTestStopReq;

/* Function templates */

bool lim_process_sme_req_messages(struct mac_context *, struct scheduler_msg *);
void lim_process_mlm_req_messages(struct mac_context *, struct scheduler_msg *);
void lim_process_mlm_rsp_messages(struct mac_context *, uint32_t, uint32_t *);
void lim_process_sme_del_bss_rsp(struct mac_context *, uint32_t, struct pe_session *);

/**
 * lim_process_mlm_start_cnf(): called to processes MLM_START_CNF message from
 * MLM State machine.
 * @mac_ctx: Pointer to Global MAC structure
 * @msg_buf: A pointer to the MLM message buffer
 *
 * Return: None
 */
void lim_process_mlm_start_cnf(struct mac_context *mac_ctx, uint32_t *msg_buf);

void lim_get_random_bssid(struct mac_context *mac, uint8_t *data);

/* Function to handle HT and HT IE CFG parameter intializations */
void handle_ht_capabilityand_ht_info(struct mac_context *mac,
				     struct pe_session *pe_session);

void lim_handle_param_update(struct mac_context *mac, eUpdateIEsType cfgId);

/* Function to apply CFG parameters before join/reassoc/start BSS */
void lim_apply_configuration(struct mac_context *, struct pe_session *);

/**
 * lim_set_cfg_protection() - sets lim global cfg cache from the config
 * @mac: global mac context
 * @pesessionEntry: PE session
 *
 * Return none
 */
void lim_set_cfg_protection(struct mac_context *mac, struct pe_session *pesessionEntry);

/* Function to Initialize MLM state machine on STA */
QDF_STATUS lim_init_mlm(struct mac_context *);

/* Function to cleanup MLM state machine */
void lim_cleanup_mlm(struct mac_context *);

/* Management frame handling functions */
void lim_process_beacon_frame(struct mac_context *, uint8_t *, struct pe_session *);
void lim_process_probe_req_frame(struct mac_context *, uint8_t *, struct pe_session *);
void lim_process_probe_rsp_frame(struct mac_context *, uint8_t *, struct pe_session *);
void lim_process_probe_req_frame_multiple_bss(struct mac_context *, uint8_t *,
					      struct pe_session *);

/* Process Auth frame when we have a session in progress. */
void lim_process_auth_frame(struct mac_context *, uint8_t *, struct pe_session *);
QDF_STATUS lim_process_auth_frame_no_session(struct mac_context *mac, uint8_t *,
						void *body);

void lim_process_assoc_req_frame(struct mac_context *, uint8_t *, uint8_t, struct pe_session *);
void lim_send_mlm_assoc_ind(struct mac_context *mac, tpDphHashNode pStaDs,
			    struct pe_session *pe_session);

void lim_process_assoc_rsp_frame(struct mac_context *, uint8_t *, uint8_t, struct pe_session *);
void lim_process_disassoc_frame(struct mac_context *, uint8_t *, struct pe_session *);
/*
 * lim_perform_disassoc() - Actual action taken after receiving disassoc
 * @mac_ctx: Global MAC context
 * @frame_rssi: RSSI of the frame
 * @rc: Reason code of the deauth
 * @pe_session: PE session entry pointer
 * @addr: BSSID from which the disassoc is received
 *
 * Return: None
 */
void lim_perform_disassoc(struct mac_context *mac_ctx, int32_t frame_rssi,
			  uint16_t rc, struct pe_session *pe_session,
			  tSirMacAddr addr);
/*
 * lim_disassoc_tdls_peers() - Disassoc action for tdls peers
 * @mac_ctx: Global MAC context
 * @pe_session: PE session entry pointer
 * @addr: BSSID from which the disassoc is received
 *
 * Return: None
 */
#ifdef FEATURE_WLAN_TDLS
void lim_disassoc_tdls_peers(struct mac_context *mac_ctx,
				    struct pe_session *pe_session, tSirMacAddr addr);
#else
static inline void lim_disassoc_tdls_peers(struct mac_context *mac_ctx,
				    struct pe_session *pe_session, tSirMacAddr addr)
{
}
#endif
void lim_process_deauth_frame(struct mac_context *, uint8_t *, struct pe_session *);
/*
 * lim_perform_deauth() - Actual action taken after receiving deauth
 * @mac_ctx: Global MAC context
 * @pe_session: PE session entry pointer
 * @rc: Reason code of the deauth
 * @addr: BSSID from which the deauth is received
 * @frame_rssi: RSSI of the frame
 *
 * Return: None
 */
void lim_perform_deauth(struct mac_context *mac_ctx, struct pe_session *pe_session,
			uint16_t rc, tSirMacAddr addr, int32_t frame_rssi);
void lim_process_action_frame(struct mac_context *, uint8_t *, struct pe_session *);
void lim_process_action_frame_no_session(struct mac_context *mac, uint8_t *pRxMetaInfo);

void lim_populate_mac_header(struct mac_context *, uint8_t *, uint8_t, uint8_t,
				      tSirMacAddr, tSirMacAddr);
QDF_STATUS lim_send_probe_req_mgmt_frame(struct mac_context *, tSirMacSSid *,
					    tSirMacAddr, uint8_t, tSirMacAddr,
					    uint32_t, uint16_t *, uint8_t *);
void lim_send_probe_rsp_mgmt_frame(struct mac_context *, tSirMacAddr, tpAniSSID, short,
				   uint8_t, struct pe_session *, uint8_t);
void lim_send_auth_mgmt_frame(struct mac_context *, tSirMacAuthFrameBody *, tSirMacAddr,
			      uint8_t, struct pe_session *);
void lim_send_assoc_req_mgmt_frame(struct mac_context *, tLimMlmAssocReq *, struct pe_session *);
#ifdef WLAN_FEATURE_HOST_ROAM
void lim_send_reassoc_req_with_ft_ies_mgmt_frame(struct mac_context *mac,
		tLimMlmReassocReq *pMlmReassocReq, struct pe_session *pe_session);
void lim_send_reassoc_req_mgmt_frame(struct mac_context *, tLimMlmReassocReq *,
				     struct pe_session *);
/**
 * lim_process_rx_scan_handler() -
 *	process the event for scan which is issued by LIM
 * @vdev: wlan objmgr vdev pointer
 * @event: scan event
 * @arg: global mac context pointer
 *
 * Return: void
 */
void lim_process_rx_scan_handler(struct wlan_objmgr_vdev *vdev,
				 struct scan_event *event, void *arg);
#else
static inline void lim_send_reassoc_req_with_ft_ies_mgmt_frame(
		struct mac_context *mac, tLimMlmReassocReq *pMlmReassocReq,
		struct pe_session *pe_session)
{}
static inline void lim_send_reassoc_req_mgmt_frame(struct mac_context *mac_ctx,
		tLimMlmReassocReq *reassoc_req, struct pe_session *pe_session)
{}
static inline void lim_process_rx_scan_handler(struct wlan_objmgr_vdev *vdev,
				 struct scan_event *event, void *arg)
{}
#endif
#ifdef WLAN_FEATURE_11AX_BSS_COLOR
/**
 * lim_process_set_he_bss_color() - process the set he bss color request
 *
 * @mac_ctx: global mac context pointer
 * @msg_buf: message buffer pointer
 *
 * Return: void
 */
void lim_process_set_he_bss_color(struct mac_context *mac_ctx, uint32_t *msg_buf);

/**
 * lim_process_obss_color_collision_info() - Process the obss color collision
 *  request.
 * @mac_ctx: global mac context pointer
 * @msg_buf: message buffer pointer
 *
 * Return: void
 */
void lim_process_obss_color_collision_info(struct mac_context *mac_ctx,
					   uint32_t *msg_buf);

/**
 * lim_send_obss_color_collision_cfg() - Send obss color collision cfg.
 * @mac_ctx: global mac context pointer
 * @session: Pointer to session
 * @event_type: obss color collision detection type
 *
 * Return: void
 */
void lim_send_obss_color_collision_cfg(struct mac_context *mac_ctx,
				       struct pe_session *session,
				       enum wmi_obss_color_collision_evt_type
				       event_type);
#else
static inline void lim_process_set_he_bss_color(struct mac_context *mac_ctx,
		uint32_t *msg_buf)
{}
static inline void lim_process_obss_color_collision_info(struct mac_context *mac_ctx,
							 uint32_t *msg_buf)
{}
static inline void lim_send_obss_color_collision_cfg(struct mac_context *mac_ctx,
			struct pe_session *session,
			enum wmi_obss_color_collision_evt_type event_type)
{}
#endif
void lim_send_delts_req_action_frame(struct mac_context *mac, tSirMacAddr peer,
				     uint8_t wmmTspecPresent,
				     struct mac_ts_info * pTsinfo,
				     struct mac_tspec_ie * pTspecIe,
				     struct pe_session *pe_session);
void lim_send_addts_req_action_frame(struct mac_context *mac, tSirMacAddr peerMacAddr,
				     tSirAddtsReqInfo *addts, struct pe_session *);
void lim_send_addts_rsp_action_frame(struct mac_context *mac, tSirMacAddr peerMacAddr,
				     uint16_t statusCode, tSirAddtsReqInfo *addts,
				     tSirMacScheduleIE *pSchedule, struct pe_session *);

void lim_send_assoc_rsp_mgmt_frame(struct mac_context *, uint16_t, uint16_t, tSirMacAddr,
				   uint8_t, tpDphHashNode pSta, struct pe_session *);

void lim_send_disassoc_mgmt_frame(struct mac_context *, uint16_t, tSirMacAddr,
				  struct pe_session *, bool waitForAck);
void lim_send_deauth_mgmt_frame(struct mac_context *, uint16_t, tSirMacAddr, struct pe_session *,
				bool waitForAck);

void lim_process_mlm_update_hidden_ssid_rsp(struct mac_context *mac_ctx,
		struct scheduler_msg *msg);

tSirResultCodes lim_mlm_add_bss(struct mac_context *, tLimMlmStartReq *,
				struct pe_session *pe_session);

QDF_STATUS lim_send_channel_switch_mgmt_frame(struct mac_context *, tSirMacAddr,
						 uint8_t, uint8_t, uint8_t,
						 struct pe_session *);

QDF_STATUS lim_send_extended_chan_switch_action_frame(struct mac_context *mac_ctx,
	tSirMacAddr peer, uint8_t mode, uint8_t new_op_class,
	uint8_t new_channel, uint8_t count, struct pe_session *session_entry);
QDF_STATUS lim_p2p_oper_chan_change_confirm_action_frame(
	struct mac_context *mac_ctx, tSirMacAddr peer,
	struct pe_session *session_entry);

QDF_STATUS lim_send_neighbor_report_request_frame(struct mac_context *,
						     tpSirMacNeighborReportReq,
						     tSirMacAddr, struct pe_session *);
QDF_STATUS lim_send_link_report_action_frame(struct mac_context *, tpSirMacLinkReport,
						tSirMacAddr, struct pe_session *);

/**
 * lim_send_radio_measure_report_action_frame - Send RRM report action frame
 * @mac: pointer to global MAC context
 * @dialog_token: Dialog token to be used in the action frame
 * @num_report: number of reports in pRRMReport
 * @is_last_frame: is the current report last or more reports to follow
 * @pRRMReport: Pointer to the RRM report structure
 * @peer: MAC address of the peer
 * @pe_session: Pointer to the PE session entry
 *
 * Return: Ret Status
 */
QDF_STATUS
lim_send_radio_measure_report_action_frame(struct mac_context *mac,
				uint8_t dialog_token,
				uint8_t num_report,
				bool is_last_frame,
				tpSirMacRadioMeasureReport pRRMReport,
				tSirMacAddr peer,
				struct pe_session *pe_session);

#ifdef FEATURE_WLAN_TDLS
void lim_init_tdls_data(struct mac_context *, struct pe_session *);
QDF_STATUS lim_process_sme_tdls_mgmt_send_req(struct mac_context *mac,
						 uint32_t *pMsgBuf);
QDF_STATUS lim_process_sme_tdls_add_sta_req(struct mac_context *mac,
					       uint32_t *pMsgBuf);
QDF_STATUS lim_process_sme_tdls_del_sta_req(struct mac_context *mac,
					       uint32_t *pMsgBuf);
void lim_send_sme_mgmt_tx_completion(
		struct mac_context *mac,
		uint32_t sme_session_id,
		uint32_t txCompleteStatus);
QDF_STATUS lim_delete_tdls_peers(struct mac_context *mac_ctx,
				    struct pe_session *session_entry);
QDF_STATUS lim_process_tdls_add_sta_rsp(struct mac_context *mac, void *msg, struct pe_session *);
void lim_process_tdls_del_sta_rsp(struct mac_context *mac_ctx,
				  struct scheduler_msg *lim_msg,
				  struct pe_session *session_entry);
#else
static inline QDF_STATUS lim_delete_tdls_peers(struct mac_context *mac_ctx,
						struct pe_session *session_entry)
{
	return QDF_STATUS_SUCCESS;
}
static inline void lim_init_tdls_data(struct mac_context *mac,
					struct pe_session *pe_session)
{

}
#endif

/* Algorithms & Link Monitoring related functions */
/* / Function that handles heartbeat failure */
void lim_handle_heart_beat_failure(struct mac_context *, struct pe_session *);

/* / Function that triggers link tear down with AP upon HB failure */
void lim_tear_down_link_with_ap(struct mac_context *, uint8_t, tSirMacReasonCodes);

/* / Function that defers the messages received */
uint32_t lim_defer_msg(struct mac_context *, struct scheduler_msg *);

/* / Function that Switches the Channel and sets the CB Mode */
void lim_set_channel(struct mac_context *mac, uint8_t channel,
		uint8_t ch_center_freq_seg0, uint8_t ch_center_freq_seg1,
		enum phy_ch_width ch_width, int8_t maxTxPower,
		uint8_t peSessionId, uint32_t cac_duration_ms,
		uint32_t dfs_regdomain);


#ifdef ANI_SUPPORT_11H
/* / Function that sends Measurement Report action frame */
QDF_STATUS lim_send_meas_report_frame(struct mac_context *, tpSirMacMeasReqActionFrame,
					 tSirMacAddr, struct pe_session *pe_session);

/* / Function that sends TPC Report action frame */
QDF_STATUS lim_send_tpc_report_frame(struct mac_context *, tpSirMacTpcReqActionFrame,
					tSirMacAddr, struct pe_session *pe_session);
#endif

/* Function(s) to handle responses received from HAL */
void lim_process_mlm_add_bss_rsp(struct mac_context *mac,
				 struct scheduler_msg *limMsgQ);
void lim_process_mlm_add_sta_rsp(struct mac_context *mac,
				struct scheduler_msg *limMsgQt,
				 struct pe_session *pe_session);
void lim_process_mlm_del_sta_rsp(struct mac_context *mac,
				 struct scheduler_msg *limMsgQ);
void lim_process_mlm_del_bss_rsp(struct mac_context *mac,
				 struct scheduler_msg *limMsgQ,
				 struct pe_session *);
void lim_process_sta_mlm_add_sta_rsp(struct mac_context *mac,
				     struct scheduler_msg *limMsgQ,
				     struct pe_session *pe_session);
void lim_process_sta_mlm_del_sta_rsp(struct mac_context *mac,
				     struct scheduler_msg *limMsgQ,
				     struct pe_session *pe_session);
void lim_process_sta_mlm_del_bss_rsp(struct mac_context *mac,
				     struct scheduler_msg *limMsgQ,
				     struct pe_session *pe_session);
void lim_process_mlm_set_sta_key_rsp(struct mac_context *mac,
				     struct scheduler_msg *limMsgQ);
void lim_process_mlm_set_bss_key_rsp(struct mac_context *mac,
				     struct scheduler_msg *limMsgQ);

/* Function to process WMA_SWITCH_CHANNEL_RSP message */
void lim_process_switch_channel_rsp(struct mac_context *mac, void *);

QDF_STATUS lim_sta_send_down_link(join_params *param);

#ifdef WLAN_FEATURE_11W
/* 11w send SA query request action frame */
QDF_STATUS lim_send_sa_query_request_frame(struct mac_context *mac, uint8_t *transId,
					      tSirMacAddr peer,
					      struct pe_session *pe_session);
/* 11w SA query request action frame handler */
QDF_STATUS lim_send_sa_query_response_frame(struct mac_context *mac,
					       uint8_t *transId, tSirMacAddr peer,
					       struct pe_session *pe_session);
#endif

/* Inline functions */

/**
 * lim_post_sme_message()
 *
 ***FUNCTION:
 * This function is called by limProcessMlmMessages(). In this
 * function MLM sub-module invokes MLM ind/cnf primitives.
 *
 ***LOGIC:
 * Initially MLM makes an SME function call to invoke MLM ind/cnf
 * primitive. In future this can be enhanced to 'post' messages to SME.
 *
 ***ASSUMPTIONS:
 * NA
 *
 ***NOTE:
 * NA
 *
 * @param mac      Pointer to Global MAC structure
 * @param msgType   Indicates the MLM primitive message type
 * @param *pMsgBuf  A pointer to the MLM message buffer
 *
 * @return None
 */
static inline void
lim_post_sme_message(struct mac_context *mac, uint32_t msgType, uint32_t *pMsgBuf)
{
	struct scheduler_msg msg = {0};

	if (pMsgBuf == NULL) {
		pe_err("Buffer is Pointing to NULL");
		return;
	}

	msg.type = (uint16_t) msgType;
	msg.bodyptr = pMsgBuf;
	msg.bodyval = 0;
	if (msgType > eWNI_SME_MSG_TYPES_BEGIN) {
		MTRACE(mac_trace(mac, TRACE_CODE_TX_SME_MSG, NO_SESSION,
				 msg.type));
		lim_process_sme_req_messages(mac, &msg);
	} else {
		lim_process_mlm_rsp_messages(mac, msgType, pMsgBuf);
	}
} /*** end lim_post_sme_message() ***/

/**
 * lim_post_mlm_message()
 *
 ***FUNCTION:
 * This function is called by limProcessSmeMessages(). In this
 * function SME invokes MLME primitives.
 *
 ***PARAMS:
 *
 ***LOGIC:
 * Initially SME makes an MLM function call to invoke MLM primitive.
 * In future this can be enhanced to 'post' messages to MLM.
 *
 ***ASSUMPTIONS:
 * NA
 *
 ***NOTE:
 * NA
 *
 * @param mac      Pointer to Global MAC structure
 * @param msgType   Indicates the MLM primitive message type
 * @param *pMsgBuf  A pointer to the MLM message buffer
 *
 * @return None
 */
static inline void
lim_post_mlm_message(struct mac_context *mac, uint32_t msgType, uint32_t *pMsgBuf)
{
	struct scheduler_msg msg = {0};

	if (pMsgBuf == NULL) {
		pe_err("Buffer is Pointing to NULL");
		return;
	}
	msg.type = (uint16_t) msgType;
	msg.bodyptr = pMsgBuf;
	msg.bodyval = 0;
	MTRACE(mac_trace_msg_rx(mac, NO_SESSION, msg.type));
	lim_process_mlm_req_messages(mac, &msg);
} /*** end lim_post_mlm_message() ***/

/**
 * lim_get_ielen_from_bss_description()
 *
 ***FUNCTION:
 * This function is called in various places to get IE length
 * from tSirBssDescription structure
 * number being scanned.
 *
 ***PARAMS:
 *
 ***LOGIC:
 *
 ***ASSUMPTIONS:
 * NA
 *
 ***NOTE:
 * NA
 *
 * @param     pBssDescr
 * @return    Total IE length
 */

static inline uint16_t
lim_get_ielen_from_bss_description(tpSirBssDescription pBssDescr)
{
	uint16_t ielen;

	if (!pBssDescr)
		return 0;

	/*
	 * Length of BSS desription is without length of
	 * length itself and length of pointer
	 * that holds ieFields
	 *
	 * <------------sizeof(tSirBssDescription)-------------------->
	 * +--------+---------------------------------+---------------+
	 * | length | other fields                    | pointer to IEs|
	 * +--------+---------------------------------+---------------+
	 *                                            ^
	 *                                            ieFields
	 */

	ielen = (uint16_t)(pBssDescr->length + sizeof(pBssDescr->length) -
			   GET_FIELD_OFFSET(tSirBssDescription, ieFields));

	return ielen;
} /*** end lim_get_ielen_from_bss_description() ***/

/**
 * lim_send_beacon_ind() - send the beacon indication
 * @mac_ctx: pointer to mac structure
 * @session: pe session
 * @reason: beacon update reason
 *
 * return: success: QDF_STATUS_SUCCESS failure: QDF_STATUS_E_FAILURE
 */
QDF_STATUS lim_send_beacon_ind(struct mac_context *mac_ctx, struct pe_session *session,
			       enum sir_bcn_update_reason reason);

void
lim_send_vdev_restart(struct mac_context *mac, struct pe_session *pe_session,
		      uint8_t sessionId);

void limWPSPBCTimeout(struct mac_context *mac, struct pe_session *pe_session);
void lim_wpspbc_close(struct mac_context *mac, struct pe_session *pe_session);

#define LIM_WPS_OVERLAP_TIMER_MS                 10000

void lim_process_disassoc_ack_timeout(struct mac_context *mac);
void lim_process_deauth_ack_timeout(struct mac_context *mac);
QDF_STATUS lim_send_disassoc_cnf(struct mac_context *mac);
QDF_STATUS lim_send_deauth_cnf(struct mac_context *mac);

/**
 * lim_disassoc_tx_complete_cnf() - callback to indicate Tx completion
 * @context: pointer to mac structure
 * @txCompleteSuccess: indicates tx success/failure
 * @params: tx completion params
 *
 * function will be invoked on receiving tx completion indication
 *
 * return: success: QDF_STATUS_SUCCESS failure: QDF_STATUS_E_FAILURE
 */
QDF_STATUS lim_disassoc_tx_complete_cnf(void *context,
					uint32_t txCompleteSuccess,
					void *params);

/**
 * lim_deauth_tx_complete_cnf() - callback to indicate Tx completion
 * @context: pointer to mac structure
 * @txCompleteSuccess: indicates tx success/failure
 * @params: tx completion params
 *
 * function will be invoked on receiving tx completion indication
 *
 * return: success: QDF_STATUS_SUCCESS failure: QDF_STATUS_E_FAILURE
 */
QDF_STATUS lim_deauth_tx_complete_cnf(void *context,
				      uint32_t txCompleteSuccess,
				      void *params);

typedef struct sSetLinkCbackParams {
	void *cbackDataPtr;
} tSetLinkCbackParams;

void lim_send_sme_disassoc_deauth_ntf(struct mac_context *mac_ctx,
				QDF_STATUS status, uint32_t *ctx);

#ifdef FEATURE_WLAN_TDLS
QDF_STATUS lim_process_sme_del_all_tdls_peers(struct mac_context *p_mac,
						 uint32_t *msg_buf);
#else
static inline
QDF_STATUS lim_process_sme_del_all_tdls_peers(struct mac_context *p_mac,
						 uint32_t *msg_buf)
{
	return QDF_STATUS_SUCCESS;
}
#endif

/**
 * lim_send_bcn_rsp() - handle beacon send response
 * @mac_ctx Pointer to Global MAC structure
 * @rsp: beacon send response
 *
 * Return: None
 */
void lim_send_bcn_rsp(struct mac_context *mac_ctx, tpSendbeaconParams rsp);

/**
 * lim_remove_duplicate_bssid_node() - remove duplicate bssid from the
 * @entry: entry to check for which the duplicate entry is present
 * @list:  mac_ctx->roam.rssi_disallow_bssid list
 *
 * Return: None
 */
void lim_remove_duplicate_bssid_node(struct sir_rssi_disallow_lst *entry,
				     qdf_list_t *list);

/**
 * lim_add_roam_blacklist_ap() - handle the blacklist bssid list received from
 * firmware
 * @mac_ctx: Pointer to Global MAC structure
 * @list: roam blacklist ap list
 *
 * Return: None
 */
void lim_add_roam_blacklist_ap(struct mac_context *mac_ctx,
			       struct roam_blacklist_event *src_lst);

/**
 * lim_process_rx_channel_status_event() - processes
 * event WDA_RX_CHN_STATUS_EVENT
 * @mac_ctx Pointer to Global MAC structure
 * @buf: Received message info
 *
 * Return: None
 */
void lim_process_rx_channel_status_event(struct mac_context *mac_ctx, void *buf);

/* / Bit value data structure */
typedef enum sHalBitVal         /* For Bit operations */
{
	eHAL_CLEAR,
	eHAL_SET
} tHalBitVal;

enum {
	eHI_PRI,
	ePROT,
	eDBG
};

QDF_STATUS lim_send_addba_response_frame(struct mac_context *mac_ctx,
					 tSirMacAddr peer_mac, uint16_t tid,
					 struct pe_session *session,
					 uint8_t addba_extn_present,
					 uint8_t amsdu_support);
/**
 * lim_process_join_failure_timeout() - This function is called to process
 * JoinFailureTimeout
 *
 * @mac_ctx: Pointer to Global MAC structure
 *
 * This function is called to process JoinFailureTimeout
 *
 * @Return None
 */
void lim_process_join_failure_timeout(struct mac_context *mac_ctx);

/**
 * lim_process_auth_failure_timeout() - This function is called to process Min
 * Channel Timeout during channel scan.
 *
 * @mac_ctx: Pointer to Global MAC structure
 *
 * This function is called to process Min Channel Timeout during channel scan.
 *
 * @Return: None
 */
void lim_process_auth_failure_timeout(struct mac_context *mac_ctx);

/**
 * lim_process_assoc_failure_timeout() - This function is called to process Min
 * Channel Timeout during channel scan.
 *
 * @mac_ctx: Pointer to Global MAC structure
 * @msg_type: Assoc or reassoc
 *
 * This function is called to process Min Channel Timeout during channel scan.
 *
 * @Return: None
 */
void lim_process_assoc_failure_timeout(struct mac_context *mac_ctx,
				       uint32_t msg_type);

/**
 * lim_send_mgmt_frame_tx() - Sends mgmt frame
 * @mac_ctx Pointer to Global MAC structure
 * @msg: Received message info
 *
 * Return: None
 */
void lim_send_mgmt_frame_tx(struct mac_context *mac_ctx,
		struct scheduler_msg *msg);

/**
 * lim_send_csa_restart_req() - send csa restart req
 * @mac_ctx Pointer to Global MAC structure
 * @vdev_id: vdev id
 *
 * Return: None
 */
void lim_send_csa_restart_req(struct mac_context *mac_ctx, uint8_t vdev_id);

/**
 * lim_process_mlm_start_req() - process MLM_START_REQ message
 *
 * @mac_ctx: global MAC context
 * @mlm_start_req: Pointer to start req
 *
 * This function is called to process MLM_START_REQ message
 * from SME. MLME now waits for HAL to send WMA_ADD_BSS_RSP.
 *
 * Return: None
 */
void lim_process_mlm_start_req(struct mac_context *mac_ctx,
					  tLimMlmStartReq *mlm_start_req);

/**
 * lim_process_mlm_join_req() - process mlm join request.
 *
 * @mac_ctx:    Pointer to Global MAC structure
 * @msg:        Pointer to the MLM message buffer
 *
 * This function is called to process MLM_JOIN_REQ message
 * from SME. It does following:
 * 1) Initialize LIM, HAL, DPH
 * 2) Configure the BSS for which the JOIN REQ was received
 *   a) Send WMA_ADD_BSS_REQ to HAL -
 *   This will identify the BSS that we are interested in
 *   --AND--
 *   Add a STA entry for the AP (in a STA context)
 *   b) Wait for WMA_ADD_BSS_RSP
 *   c) Send WMA_ADD_STA_REQ to HAL
 *   This will add the "local STA" entry to the STA table
 * 3) Continue as before, i.e,
 *   a) Send a PROBE REQ
 *   b) Wait for PROBE RSP/BEACON containing the SSID that
 *   we are interested in
 *   c) Then start an AUTH seq
 *   d) Followed by the ASSOC seq
 *
 * @Return: None
 */
void lim_process_mlm_join_req(struct mac_context *mac_ctx,
			      tLimMlmJoinReq *mlm_join_req);

/*
 * lim_process_mlm_deauth_req() - This function is called to process
 * MLM_DEAUTH_REQ message from SME
 *
 * @mac_ctx:      Pointer to Global MAC structure
 * @msg_buf:      A pointer to the MLM message buffer
 *
 * This function is called to process MLM_DEAUTH_REQ message from SME
 *
 * @Return: None
 */
void lim_process_mlm_deauth_req(struct mac_context *mac_ctx, uint32_t *msg_buf);

#ifdef CONFIG_VDEV_SM
/**
 * lim_sta_mlme_vdev_disconnect_bss() - Disconnect from BSS
 * @vdev_mlme_obj:  VDEV MLME comp object
 * @data_len: data size
 * @data: event data
 *
 * API invokes BSS disconnection
 *
 * Return: SUCCESS on successful completion of disconnection
 *         FAILURE, if it fails due to any
 */
QDF_STATUS lim_sta_mlme_vdev_disconnect_bss(struct vdev_mlme_obj *vdev_mlme,
					    uint16_t data_len, void *data);
#endif
#endif /* __LIM_TYPES_H */
