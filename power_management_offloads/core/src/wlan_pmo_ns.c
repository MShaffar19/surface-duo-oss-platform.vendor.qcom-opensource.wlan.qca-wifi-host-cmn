/*
* Copyright (c) 2017 The Linux Foundation. All rights reserved.
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
 * DOC: Implements ns offload feature API's
 */

#include "wlan_pmo_ns.h"
#include "wlan_pmo_tgt_api.h"
#include "wlan_pmo_main.h"
#include "wlan_pmo_obj_mgmt_public_struct.h"

static void pmo_core_fill_ns_addr(struct pmo_ns_offload_params *request,
	struct pmo_ns_req *ns_req)
{
	int i;

	for (i = 0; i < ns_req->count; i++) {
		/*
		 * Filling up the request structure
		 * Filling the selfIPv6Addr with solicited address
		 * A Solicited-Node multicast address is created by
		 * taking the last 24 bits of a unicast or anycast
		 * address and appending them to the prefix
		 *
		 * FF02:0000:0000:0000:0000:0001:FFXX:XXXX
		 *
		 * here XX is the unicast/anycast bits
		 */
		request->self_ipv6_addr[i][0] = 0xFF;
		request->self_ipv6_addr[i][1] = 0x02;
		request->self_ipv6_addr[i][11] = 0x01;
		request->self_ipv6_addr[i][12] = 0xFF;
		request->self_ipv6_addr[i][13] =
					ns_req->ipv6_addr[i][13];
		request->self_ipv6_addr[i][14] =
					ns_req->ipv6_addr[i][14];
		request->self_ipv6_addr[i][15] =
					ns_req->ipv6_addr[i][15];
		request->slot_idx = i;
		qdf_mem_copy(&request->target_ipv6_addr[i],
			&ns_req->ipv6_addr[i][0], PMO_MAC_IPV6_ADDR_LEN);

		request->target_ipv6_addr_valid[i] =
			PMO_IPV6_ADDR_VALID;
		request->target_ipv6_addr_ac_type[i] =
			ns_req->ipv6_addr_type[i];

		pmo_info("NSoffload solicitIp: %pI6 targetIp: %pI6 Index: %d",
			&request->self_ipv6_addr[i],
			&request->target_ipv6_addr[i], i);
	}
}

static QDF_STATUS pmo_core_cache_ns_in_vdev_priv(
			struct pmo_ns_req *ns_req,
			struct wlan_objmgr_vdev *vdev)
{
	QDF_STATUS status = QDF_STATUS_SUCCESS;
	struct pmo_psoc_priv_obj *psoc_ctx;
	struct pmo_vdev_priv_obj *vdev_ctx;
	struct pmo_ns_offload_params request;
	struct wlan_objmgr_peer *peer;

	PMO_ENTER();
	psoc_ctx = pmo_get_psoc_priv_ctx(ns_req->psoc);
	if (!psoc_ctx) {
		pmo_err("psoc_ctx is NULL");
		status = QDF_STATUS_E_INVAL;
		goto out;
	}

	vdev_ctx = pmo_get_vdev_priv_ctx(vdev);
	if (!vdev_ctx) {
		pmo_err("vdev_ctx is NULL");
		status = QDF_STATUS_E_INVAL;
		goto out;
	}

	qdf_mem_zero(&request, sizeof(request));
	pmo_core_fill_ns_addr(&request, ns_req);

	request.enable = PMO_OFFLOAD_ENABLE;
	qdf_mem_copy(&request.self_macaddr.bytes,
			  wlan_vdev_mlme_get_macaddr(vdev),
			  QDF_MAC_ADDR_SIZE);

	/* set number of ns offload address count */
	request.num_ns_offload_count = ns_req->count;

	peer = wlan_vdev_get_bsspeer(vdev);
	if (!peer) {
		pmo_err("peer is null");
		status = QDF_STATUS_E_INVAL;
		goto out;
	}
	pmo_info("vdev self mac addr: %pM bss peer mac addr: %pM",
		wlan_vdev_mlme_get_macaddr(vdev),
		wlan_peer_get_macaddr(peer));
	/* get peer and peer mac accdress aka ap mac address */
	qdf_mem_copy(&request.bssid,
		wlan_peer_get_macaddr(peer),
		QDF_MAC_ADDR_SIZE);
	/* cache ns request */
	qdf_spin_lock_bh(&vdev_ctx->pmo_vdev_lock);
	qdf_mem_copy(&vdev_ctx->vdev_ns_req, &request,
		sizeof(vdev_ctx->vdev_ns_req));
	qdf_spin_unlock_bh(&vdev_ctx->pmo_vdev_lock);
out:
	PMO_EXIT();

	return status;
}

static QDF_STATUS pmo_core_flush_ns_from_vdev_priv(
		struct wlan_objmgr_vdev *vdev)
{
	struct pmo_vdev_priv_obj *vdev_ctx;
	QDF_STATUS status = QDF_STATUS_SUCCESS;

	PMO_ENTER();
	vdev_ctx = pmo_get_vdev_priv_ctx(vdev);
	if (!vdev_ctx) {
		pmo_err("vdev_ctx is NULL");
		status = QDF_STATUS_E_INVAL;
		goto out;
	}

	/* clear ns request */
	qdf_spin_lock_bh(&vdev_ctx->pmo_vdev_lock);
	qdf_mem_zero(&vdev_ctx->vdev_ns_req, sizeof(vdev_ctx->vdev_ns_req));
	vdev_ctx->vdev_ns_req.enable = PMO_OFFLOAD_DISABLE;
	qdf_spin_unlock_bh(&vdev_ctx->pmo_vdev_lock);
out:
	PMO_EXIT();

	return status;
}

static QDF_STATUS pmo_core_do_enable_ns_offload(struct wlan_objmgr_vdev *vdev,
		uint8_t vdev_id, enum pmo_offload_trigger trigger)
{
	QDF_STATUS status = QDF_STATUS_SUCCESS;
	struct pmo_psoc_priv_obj *psoc_ctx;
	struct pmo_vdev_priv_obj *vdev_ctx;

	PMO_ENTER();
	vdev_ctx = pmo_get_vdev_priv_ctx(vdev);
	if (!vdev_ctx) {
		pmo_err("vdev_ctx is NULL");
		status = QDF_STATUS_E_INVAL;
		goto out;
	}

	psoc_ctx = vdev_ctx->pmo_psoc_ctx;
	if (!psoc_ctx) {
		pmo_err("psoc_ctx is NULL");
		status = QDF_STATUS_E_INVAL;
		goto out;
	}

	switch (trigger) {
	case pmo_ipv6_change_notify:
	case pmo_ns_offload_dynamic_update:
		if (!psoc_ctx->psoc_cfg.active_mode_offload) {
			pmo_info("active offload is disabled, skip in mode:%d",
				trigger);
			status = QDF_STATUS_E_INVAL;
			goto out;
		}
		/* enable arp when active offload is true (ipv6 notifier) */
		status = pmo_tgt_enable_ns_offload_req(vdev, vdev_id);
		break;
	case pmo_apps_suspend:
		if (psoc_ctx->psoc_cfg.active_mode_offload) {
			pmo_info("active offload is enabled, skip in mode: %d",
				trigger);
			status = QDF_STATUS_E_INVAL;
			goto out;
		}
		/* enable arp when active offload is false (apps suspend) */
		status = pmo_tgt_enable_ns_offload_req(vdev, vdev_id);
		break;
	default:
		status = QDF_STATUS_E_INVAL;
		pmo_err("invalid pmo trigger");
		break;
	}
out:
	PMO_EXIT();

	return status;
}

static QDF_STATUS pmo_core_do_disable_ns_offload(struct wlan_objmgr_vdev *vdev,
		uint8_t vdev_id, enum pmo_offload_trigger trigger)
{
	QDF_STATUS status = QDF_STATUS_SUCCESS;
	struct pmo_psoc_priv_obj *psoc_ctx;
	struct pmo_vdev_priv_obj *vdev_ctx;

	PMO_ENTER();
	vdev_ctx = pmo_get_vdev_priv_ctx(vdev);
	if (!vdev_ctx) {
		pmo_err("vdev_ctx is NULL");
		status = QDF_STATUS_E_INVAL;
		goto out;
	}

	psoc_ctx = vdev_ctx->pmo_psoc_ctx;
	if (!psoc_ctx) {
		pmo_err("psoc_ctx is NULL");
		status = QDF_STATUS_E_INVAL;
		goto out;
	}

	switch (trigger) {
	case pmo_ipv6_change_notify:
	case pmo_ns_offload_dynamic_update:
		if (!psoc_ctx->psoc_cfg.active_mode_offload) {
			pmo_info("active offload is disabled, skip in mode:%d",
				trigger);
			status = QDF_STATUS_E_INVAL;
			goto out;
		}
		/* config ns when active offload is enable */
		status = pmo_tgt_disable_ns_offload_req(vdev, vdev_id);
		break;
	case pmo_apps_resume:
		if (psoc_ctx->psoc_cfg.active_mode_offload) {
			pmo_info("active offload is enabled, skip in mode: %d",
				trigger);
			status = QDF_STATUS_E_INVAL;
			goto out;
		}
		/* config arp/ns when active offload is disable */
		status = pmo_tgt_disable_ns_offload_req(vdev, vdev_id);
		break;
	default:
		status = QDF_STATUS_E_INVAL;
		pmo_err("invalid pmo trigger");
		break;
	}
out:
	PMO_EXIT();

	return status;
}


static QDF_STATUS pmo_core_ns_offload_sanity(struct wlan_objmgr_vdev *vdev)
{
	struct pmo_vdev_priv_obj *vdev_ctx;

	vdev_ctx = pmo_get_vdev_priv_ctx(vdev);
	if (!vdev_ctx) {
		pmo_err("vdev_ctx is NULL");
		return QDF_STATUS_E_INVAL;
	}

	if (!vdev_ctx->pmo_psoc_ctx->psoc_cfg.ns_offload_enable_static) {
		pmo_info("ns offload statically disable");
		return QDF_STATUS_E_INVAL;
	}

	if (!vdev_ctx->pmo_psoc_ctx->psoc_cfg.ns_offload_enable_dynamic) {
		pmo_info("ns offload dynamically disable");
		return QDF_STATUS_E_INVAL;
	}

	if (!pmo_core_is_vdev_supports_offload(vdev)) {
		pmo_info("vdev in invalid opmode for ns offload %d",
			pmo_get_vdev_opmode(vdev));
		return QDF_STATUS_E_INVAL;
	}

	if (pmo_core_is_vdev_connected(vdev) == false)
		return QDF_STATUS_E_INVAL;

	return QDF_STATUS_SUCCESS;
}

QDF_STATUS pmo_core_cache_ns_offload_req(
		struct pmo_ns_req *ns_req)
{
	QDF_STATUS status;
	struct wlan_objmgr_vdev *vdev;

	PMO_ENTER();
	if (!ns_req) {
		pmo_err("ns is NULL");
		status = QDF_STATUS_E_INVAL;
		goto out;
	}

	if (!ns_req->psoc) {
		pmo_err("psoc is NULL");
		status = QDF_STATUS_E_INVAL;
		goto out;
	}
	vdev = wlan_objmgr_get_vdev_by_id_from_psoc(ns_req->psoc,
			ns_req->vdev_id, WLAN_PMO_ID);
	if (!vdev) {
		pmo_err("vdev is NULL");
		status = QDF_STATUS_E_INVAL;
		goto out;
	}

	status = pmo_core_ns_offload_sanity(vdev);
	if (status != QDF_STATUS_SUCCESS)
		goto dec_ref;

	if (ns_req->count == 0) {
		pmo_info("skip ns offload caching as ns count is 0");
		status = QDF_STATUS_E_INVAL;
		goto dec_ref;
	}

	status = pmo_core_cache_ns_in_vdev_priv(ns_req, vdev);
dec_ref:
	wlan_objmgr_vdev_release_ref(vdev, WLAN_PMO_ID);
out:
	PMO_EXIT();

	return status;
}

QDF_STATUS pmo_core_flush_ns_offload_req(struct wlan_objmgr_vdev *vdev)
{
	QDF_STATUS status;
	uint8_t vdev_id;

	PMO_ENTER();
	if (!vdev) {
		pmo_err("vdev is NULL");
		status = QDF_STATUS_E_INVAL;
		goto out;
	}

	status = wlan_objmgr_vdev_try_get_ref(vdev, WLAN_PMO_ID);
	if (status != QDF_STATUS_SUCCESS)
		goto out;

	status = pmo_core_ns_offload_sanity(vdev);
	if (status != QDF_STATUS_SUCCESS)
		goto dec_ref;

	vdev_id = pmo_get_vdev_id(vdev);
	pmo_info("Flush ns offload on vdev id: %d vdev: %p", vdev_id, vdev);

	status = pmo_core_flush_ns_from_vdev_priv(vdev);
dec_ref:
	wlan_objmgr_vdev_release_ref(vdev, WLAN_PMO_ID);
out:
	PMO_EXIT();

	return status;
}

QDF_STATUS pmo_core_enable_ns_offload_in_fwr(struct wlan_objmgr_vdev *vdev,
		enum pmo_offload_trigger trigger)
{
	QDF_STATUS status;
	struct pmo_vdev_priv_obj *vdev_ctx;
	uint8_t vdev_id;

	PMO_ENTER();
	if (!vdev) {
		pmo_err("vdev is NULL");
		status = QDF_STATUS_E_INVAL;
		goto out;
	}

	status = wlan_objmgr_vdev_try_get_ref(vdev, WLAN_PMO_ID);
	if (status != QDF_STATUS_SUCCESS)
		goto out;

	vdev_ctx = pmo_get_vdev_priv_ctx(vdev);
	if (!vdev_ctx) {
		pmo_err("vdev_ctx is NULL");
		status = QDF_STATUS_E_INVAL;
		goto dec_ref;
	}

	status = pmo_core_ns_offload_sanity(vdev);
	if (status != QDF_STATUS_SUCCESS)
		goto dec_ref;

	if (trigger == pmo_ns_offload_dynamic_update) {
		/*
		 * user disable ns offload using ioctl/vendor cmd dynamically.
		 */
		vdev_ctx->pmo_psoc_ctx->psoc_cfg.ns_offload_enable_dynamic =
			true;
		goto skip_ns_dynamic_check;
	}

	if (!vdev_ctx->pmo_psoc_ctx->psoc_cfg.ns_offload_enable_dynamic) {
		pmo_info("ns offload dynamically disable");
		status = QDF_STATUS_E_INVAL;
		goto dec_ref;
	}

skip_ns_dynamic_check:
	qdf_spin_lock_bh(&vdev_ctx->pmo_vdev_lock);
	if (vdev_ctx->vdev_ns_req.num_ns_offload_count == 0) {
		qdf_spin_unlock_bh(&vdev_ctx->pmo_vdev_lock);
		pmo_info("skip ns offload enable as ns count is 0");
		status = QDF_STATUS_E_INVAL;
		goto out;
	}
	qdf_spin_unlock_bh(&vdev_ctx->pmo_vdev_lock);

	vdev_id = pmo_get_vdev_id(vdev);
	pmo_info("Enable ns offload in fwr vdev id: %d vdev: %p trigger: %d",
		vdev_id, vdev, trigger);
	status = pmo_core_do_enable_ns_offload(vdev, vdev_id, trigger);
dec_ref:
	wlan_objmgr_vdev_release_ref(vdev, WLAN_PMO_ID);
out:
	PMO_EXIT();

	return status;
}

QDF_STATUS pmo_core_disable_ns_offload_in_fwr(struct wlan_objmgr_vdev *vdev,
		enum pmo_offload_trigger trigger)
{
	QDF_STATUS status;
	uint8_t vdev_id;
	struct pmo_vdev_priv_obj *vdev_ctx;

	PMO_ENTER();
	if (!vdev) {
		pmo_err("vdev is NULL");
		status = QDF_STATUS_E_INVAL;
		goto out;
	}

	status = wlan_objmgr_vdev_try_get_ref(vdev, WLAN_PMO_ID);
	if (status != QDF_STATUS_SUCCESS)
		goto out;

	vdev_ctx = pmo_get_vdev_priv_ctx(vdev);
	if (!vdev_ctx) {
		pmo_err("vdev_ctx is NULL");
		status = QDF_STATUS_E_INVAL;
		goto dec_ref;
	}

	status = pmo_core_ns_offload_sanity(vdev);
	if (status != QDF_STATUS_SUCCESS)
		goto dec_ref;

	if (trigger == pmo_ns_offload_dynamic_update) {
		/*
		 * user disable ns offload using ioctl/vendor cmd dynamically.
		 */
		vdev_ctx->pmo_psoc_ctx->psoc_cfg.ns_offload_enable_dynamic =
			false;
		goto skip_ns_dynamic_check;
	}

	if (!vdev_ctx->pmo_psoc_ctx->psoc_cfg.ns_offload_enable_dynamic) {
		pmo_info("ns offload dynamically disable");
		status = QDF_STATUS_E_INVAL;
		goto dec_ref;
	}

skip_ns_dynamic_check:
	vdev_id = pmo_get_vdev_id(vdev);
	pmo_info("disable ns offload in fwr vdev id: %d vdev: %p trigger: %d",
		vdev_id, vdev, trigger);

	status = pmo_core_do_disable_ns_offload(vdev, vdev_id, trigger);
dec_ref:
	wlan_objmgr_vdev_release_ref(vdev, WLAN_PMO_ID);
out:
	PMO_EXIT();

	return status;
}

