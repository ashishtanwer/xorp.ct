// -*- c-basic-offset: 4; tab-width: 8; indent-tabs-mode: t -*-

// Copyright (c) 2001,2002 International Computer Science Institute
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software")
// to deal in the Software without restriction, subject to the conditions
// listed in the XORP LICENSE file. These conditions include: you must
// preserve this copyright notice, and you cannot mention the copyright
// holders in advertising related to the Software without their permission.
// The Software is provided WITHOUT ANY WARRANTY, EXPRESS OR IMPLIED. This
// notice is a summary of the XORP LICENSE file; the license in that file is
// legally binding.

#ident "$XORP: xorp/pim/pim_mre_join_prune.cc,v 1.2 2002/12/16 05:25:53 pavlin Exp $"

//
// PIM Multicast Routing Entry Join/Prune handling
//


#include "pim_module.h"
#include "pim_private.hh"
#include "pim_mre.hh"
#include "pim_mrt.hh"
#include "pim_vif.hh"


//
// Exported variables
//

//
// Local constants definitions
//

//
// Local structures/classes, typedefs and macros
//

//
// Local variables
//

//
// Local functions prototypes
//
static void	pim_mre_override_timer_timeout(void *data_pointer);


//
// J/P state (per interface)
//
// Note: applies for (*,*,RP), (*,G), (S,G), (S,G,rpt)
void
PimMre::set_downstream_noinfo_state(uint16_t vif_index)
{
    if (vif_index == Vif::VIF_INDEX_INVALID)
	return;
    
    if (is_downstream_noinfo_state(vif_index))
	return;			// Nothing changed
    
    _downstream_join_state.reset(vif_index);
    _downstream_prune_state.reset(vif_index);
    _downstream_prune_pending_state.reset(vif_index);
    _downstream_tmp_state.reset(vif_index);
    
    do {
	if (is_sg()) {
	    pim_mrt().add_task_downstream_jp_state_sg(vif_index,
						      source_addr(),
						      group_addr());
	    break;
	}
	if (is_sg_rpt()) {
	    pim_mrt().add_task_downstream_jp_state_sg_rpt(vif_index,
							  source_addr(),
							  group_addr());
	    // XXX: try to explicitly remove the (S,G,rpt) entry here, because
	    // a (S,G,rpt) entry may exist even if the upstream state is
	    // NoInfo. In other words, even if the state of all downstream
	    // interfaces changes to NoInfo, the upstream state may not change.
	    entry_try_remove();
	    break;
	}
	if (is_wc()) {
	    pim_mrt().add_task_downstream_jp_state_wc(vif_index, group_addr());
	    break;
	}
	if (is_rp()) {
	    pim_mrt().add_task_downstream_jp_state_rp(vif_index,
						      *rp_addr_ptr());
	    break;
	}
    } while (false);
}

// Note: applies for (*,*,RP), (*,G), (S,G), (S,G,rpt)
void
PimMre::set_downstream_join_state(uint16_t vif_index)
{
    if (vif_index == Vif::VIF_INDEX_INVALID)
	return;
    
    if (is_downstream_join_state(vif_index))
	return;			// Nothing changed
    
    _downstream_join_state.set(vif_index);
    _downstream_prune_state.reset(vif_index);
    _downstream_prune_pending_state.reset(vif_index);
    _downstream_tmp_state.reset(vif_index);
    
    do {
	if (is_sg()) {
	    pim_mrt().add_task_downstream_jp_state_sg(vif_index,
						      source_addr(),
						      group_addr());
	    break;
	}
	if (is_sg_rpt()) {
	    pim_mrt().add_task_downstream_jp_state_sg_rpt(vif_index,
							  source_addr(),
							  group_addr());
	    break;
	}
	if (is_wc()) {
	    pim_mrt().add_task_downstream_jp_state_wc(vif_index, group_addr());
	    break;
	}
	if (is_rp()) {
	    pim_mrt().add_task_downstream_jp_state_rp(vif_index,
						      *rp_addr_ptr());
	    break;
	}
    } while (false);
}

// Note: applies for (*,*,RP), (*,G), (S,G), (S,G,rpt)
void
PimMre::set_downstream_prune_state(uint16_t vif_index)
{
    if (vif_index == Vif::VIF_INDEX_INVALID)
	return;
    
    if (is_downstream_prune_state(vif_index))
	return;			// Nothing changed
    
    _downstream_join_state.reset(vif_index);
    _downstream_prune_state.set(vif_index);
    _downstream_prune_pending_state.reset(vif_index);
    _downstream_tmp_state.reset(vif_index);
    
    do {
	if (is_sg()) {
	    pim_mrt().add_task_downstream_jp_state_sg(vif_index,
						      source_addr(),
						      group_addr());
	    break;
	}
	if (is_sg_rpt()) {
	    pim_mrt().add_task_downstream_jp_state_sg_rpt(vif_index,
							  source_addr(),
							  group_addr());
	    break;
	}
	if (is_wc()) {
	    pim_mrt().add_task_downstream_jp_state_wc(vif_index, group_addr());
	    break;
	}
	if (is_rp()) {
	    pim_mrt().add_task_downstream_jp_state_rp(vif_index,
						      *rp_addr_ptr());
	    break;
	}
    } while (false);
}

// Note: applies for (*,*,RP), (*,G), (S,G), (S,G,rpt)
void
PimMre::set_downstream_prune_pending_state(uint16_t vif_index)
{
    if (vif_index == Vif::VIF_INDEX_INVALID)
	return;
    
    if (is_downstream_prune_pending_state(vif_index))
	return;			// Nothing changed
    
    _downstream_join_state.reset(vif_index);
    _downstream_prune_state.reset(vif_index);
    _downstream_prune_pending_state.set(vif_index);
    _downstream_tmp_state.reset(vif_index);
    
    do {
	if (is_sg()) {
	    pim_mrt().add_task_downstream_jp_state_sg(vif_index,
						      source_addr(),
						      group_addr());
	    break;
	}
	if (is_sg_rpt()) {
	    pim_mrt().add_task_downstream_jp_state_sg_rpt(vif_index,
							  source_addr(),
							  group_addr());
	    break;
	}
	if (is_wc()) {
	    pim_mrt().add_task_downstream_jp_state_wc(vif_index, group_addr());
	    break;
	}
	if (is_rp()) {
	    pim_mrt().add_task_downstream_jp_state_rp(vif_index,
						      *rp_addr_ptr());
	    break;
	}
    } while (false);
}

// Note: applies for (S,G,rpt)
void
PimMre::set_downstream_prune_tmp_state(uint16_t vif_index)
{
    if (vif_index == Vif::VIF_INDEX_INVALID)
	return;
    
    if (! is_sg_rpt())
	return;
    
    if (is_downstream_prune_tmp_state(vif_index))
	return;			// Nothing changed
    
    _downstream_join_state.reset(vif_index);
    _downstream_prune_state.set(vif_index);
    _downstream_prune_pending_state.reset(vif_index);
    _downstream_tmp_state.set(vif_index);
    
    // XXX: this state is transient, hence we don't call
    // add_task_downstream_jp_state_sg_rpt();
}

// Note: applies for (S,G,rpt)
void
PimMre::set_downstream_prune_pending_tmp_state(uint16_t vif_index)
{
    if (vif_index == Vif::VIF_INDEX_INVALID)
	return;
    
    if (! is_sg_rpt())
	return;
    
    if (is_downstream_prune_pending_tmp_state(vif_index))
	return;			// Nothing changed
    
    _downstream_join_state.reset(vif_index);
    _downstream_prune_state.reset(vif_index);
    _downstream_prune_pending_state.set(vif_index);
    _downstream_tmp_state.set(vif_index);

    // XXX: this state is transient, hence we don't call
    // add_task_downstream_jp_state_sg_rpt();
}

// Note: applies for (S,G,rpt)
void
PimMre::set_downstream_processed_wc_by_sg_rpt(uint16_t vif_index, bool v)
{
    if (vif_index == Vif::VIF_INDEX_INVALID)
	return;
    
    if (! is_sg_rpt())
	return;
    
    if (v)
	_downstream_processed_wc_by_sg_rpt.set(vif_index);
    else
	_downstream_processed_wc_by_sg_rpt.reset(vif_index);
}

// Note: applies for (*,*,RP), (*,G), (S,G), (S,G,rpt)
bool
PimMre::is_downstream_noinfo_state(uint16_t vif_index) const
{
    if (vif_index == Vif::VIF_INDEX_INVALID)
	return (true);		// XXX
    
    // XXX: we don't test for _downstream_tmp_state, because it is used
    // in combination with other state.
    return (! (_downstream_join_state.test(vif_index)
	       || _downstream_prune_state.test(vif_index)
	       || _downstream_prune_pending_state.test(vif_index))
	);
}

// Note: applies for (*,*,RP), (*,G), (S,G), (S,G,rpt)
bool
PimMre::is_downstream_join_state(uint16_t vif_index) const
{
    if (vif_index == Vif::VIF_INDEX_INVALID)
	return (false);
    
    return (_downstream_join_state.test(vif_index));
}

// Note: applies for (*,*,RP), (*,G), (S,G), (S,G,rpt)
bool
PimMre::is_downstream_prune_state(uint16_t vif_index) const
{
    if (vif_index == Vif::VIF_INDEX_INVALID)
	return (false);
    
    return (_downstream_prune_state.test(vif_index)
	    & !_downstream_tmp_state.test(vif_index));
}

// Note: applies for (*,*,RP), (*,G), (S,G), (S,G,rpt)
bool
PimMre::is_downstream_prune_pending_state(uint16_t vif_index) const
{
    if (vif_index == Vif::VIF_INDEX_INVALID)
	return (false);
    
    return (_downstream_prune_pending_state.test(vif_index)
	    & !_downstream_tmp_state.test(vif_index));
}

// Note: applies for (S,G,rpt)
bool
PimMre::is_downstream_prune_tmp_state(uint16_t vif_index) const
{
    if (vif_index == Vif::VIF_INDEX_INVALID)
	return (false);
    
    return (_downstream_prune_state.test(vif_index)
	    & _downstream_tmp_state.test(vif_index));
}

// Note: applies for (S,G,rpt)
bool
PimMre::is_downstream_prune_pending_tmp_state(uint16_t vif_index) const
{
    if (vif_index == Vif::VIF_INDEX_INVALID)
	return (false);
    
    return (_downstream_prune_pending_state.test(vif_index)
	    & _downstream_tmp_state.test(vif_index));
}

// Note: applies for (S,G,rpt)
bool
PimMre::is_downstream_processed_wc_by_sg_rpt(uint16_t vif_index) const
{
    if (vif_index == Vif::VIF_INDEX_INVALID)
	return (false);
    
    return (_downstream_processed_wc_by_sg_rpt.test(vif_index));
}

// Note: applies for (*,*,RP), (*,G), (S,G), (S,G,rpt)
const Mifset&
PimMre::downstream_join_state() const
{
    return (_downstream_join_state);
}

// Note: applies for (*,*,RP), (*,G), (S,G), (S,G,rpt)
const Mifset&
PimMre::downstream_prune_state() const
{
    static Mifset mifs;
    
    if (! is_sg_rpt())
	return (_downstream_prune_state);
    
    mifs = _downstream_prune_state & ~_downstream_tmp_state;
    return (mifs);
}

// Note: applies for (*,*,RP), (*,G), (S,G), (S,G,rpt)
const Mifset&
PimMre::downstream_prune_pending_state() const
{
    static Mifset mifs;
    
    if (! is_sg_rpt())
	return (_downstream_prune_pending_state);
    
    mifs = _downstream_prune_pending_state & ~_downstream_tmp_state;
    return (mifs);
}

// Note: applies for (S,G,rpt)
const Mifset&
PimMre::downstream_prune_tmp_state() const
{
    static Mifset mifs;
    
    if (! is_sg_rpt()) {
	mifs.reset();
	return (mifs);
    }
    
    mifs = _downstream_prune_state & _downstream_tmp_state;
    return (mifs);
}

// Note: applies for (S,G,rpt)
const Mifset&
PimMre::downstream_prune_pending_tmp_state() const
{
    static Mifset mifs;
    
    if (! is_sg_rpt()) {
	mifs.reset();
	return (mifs);
    }
    
    mifs = _downstream_prune_pending_state & _downstream_tmp_state;
    return (mifs);
}

//
// J/P state for (*,*,RP), (*,G), (S,G)
//
void
PimMre::set_joined_state()
{
    _flags |= PIM_MRE_JOINED_STATE;
    
    if (is_sg())
	pim_mrt().add_task_upstream_jp_state_sg(source_addr(), group_addr());
}
void
PimMre::set_not_joined_state()
{
    _flags &= ~PIM_MRE_JOINED_STATE;
    
    if (is_sg())
	pim_mrt().add_task_upstream_jp_state_sg(source_addr(), group_addr());
}

//
// J/P state for (S,G,rpt)
//
bool
PimMre::is_rpt_not_joined_state() const
{
    return (! (_flags & (PIM_MRE_PRUNED_STATE | PIM_MRE_NOT_PRUNED_STATE)));
}
bool
PimMre::is_pruned_state() const
{
    return (_flags & PIM_MRE_PRUNED_STATE);
}
bool
PimMre::is_not_pruned_state() const
{
    return (_flags & PIM_MRE_NOT_PRUNED_STATE);
}
void
PimMre::set_rpt_not_joined_state()
{
    _flags &= ~(PIM_MRE_PRUNED_STATE | PIM_MRE_NOT_PRUNED_STATE);
}
void
PimMre::set_pruned_state()
{
    _flags &= ~PIM_MRE_NOT_PRUNED_STATE;
    _flags |= PIM_MRE_PRUNED_STATE;
}
void
PimMre::set_not_pruned_state()
{
    _flags &= ~PIM_MRE_PRUNED_STATE;
    _flags |= PIM_MRE_NOT_PRUNED_STATE;
}

//
// Join/Prune downstream machinery
//
void
PimMre::receive_join_rp(uint16_t vif_index, uint16_t holdtime)
{
    if (vif_index == Vif::VIF_INDEX_INVALID)
	return;
    
    if (! is_rp())
	return;
    
    if (is_downstream_join_state(vif_index))
	goto join_state_label;
    if (is_downstream_prune_pending_state(vif_index))
	goto prune_pending_state_label;
    goto noinfo_state_label;
    
 noinfo_state_label:
    // NoInfo state -> Join state
    mifset_timer_start(_downstream_expiry_timers, vif_index, holdtime, 0,
		       &PimMre::downstream_expiry_timer_timeout_rp);
    set_downstream_join_state(vif_index);
    return;
    
 join_state_label:
    // Join state
    if (mifset_timer_remain(_downstream_expiry_timers, vif_index) < holdtime)
	mifset_timer_start(_downstream_expiry_timers, vif_index, holdtime, 0,
			   &PimMre::downstream_expiry_timer_timeout_rp);
    return;

 prune_pending_state_label:
    // PrunePending state -> Join state
    mifset_timer_cancel(_downstream_prune_pending_timers, vif_index);
    if (mifset_timer_remain(_downstream_expiry_timers, vif_index) < holdtime)
	mifset_timer_start(_downstream_expiry_timers, vif_index, holdtime, 0,
			   &PimMre::downstream_expiry_timer_timeout_rp);
    set_downstream_join_state(vif_index);
    return;
}

void
PimMre::receive_prune_rp(uint16_t vif_index, uint16_t holdtime)
{
    PimVif *pim_vif;
    
    if (vif_index == Vif::VIF_INDEX_INVALID)
	return;
    
    if (! is_rp())
	return;
    
    if (is_downstream_join_state(vif_index))
	goto join_state_label;
    if (is_downstream_prune_pending_state(vif_index))
	goto prune_pending_state_label;
    goto noinfo_state_label;

 noinfo_state_label:
    // NoInfo state
    return;		// Ignore

 join_state_label:
    // Join state -> PrunePending state
    pim_vif = pim_mrt().vif_find_by_vif_index(vif_index);
    if (pim_vif == NULL)
	return;
    if (pim_vif->pim_nbrs_number() > 1) {
	struct timeval timeval = pim_vif->jp_override_interval();
	// TODO: make sure usec resolution works!!
	mifset_timer_start(_downstream_prune_pending_timers, vif_index,
			   TIMEVAL_SEC(&timeval), TIMEVAL_USEC(&timeval),
			   &PimMre::downstream_prune_pending_timer_timeout_rp);
    } else {
	// XXX: force to expire now
	mifset_timer_start(_downstream_prune_pending_timers, vif_index, 0, 0,
			   &PimMre::downstream_prune_pending_timer_timeout_rp);
    }
    set_downstream_prune_pending_state(vif_index);
    return;

 prune_pending_state_label:
    // PrunePending state
    return;		// Nothing to do (TODO: is it really true?)
    
    UNUSED(holdtime);
}

void
PimMre::receive_join_wc(uint16_t vif_index, uint16_t holdtime)
{
    if (vif_index == Vif::VIF_INDEX_INVALID)
	return;
    
    if (! is_wc())
	return;
    
    if (is_downstream_join_state(vif_index))
	goto join_state_label;
    if (is_downstream_prune_pending_state(vif_index))
	goto prune_pending_state_label;
    goto noinfo_state_label;
    
 noinfo_state_label:
    // NoInfo state -> Join state
    mifset_timer_start(_downstream_expiry_timers, vif_index, holdtime, 0,
		       &PimMre::downstream_expiry_timer_timeout_wc);
    set_downstream_join_state(vif_index);
    return;

 join_state_label:
    // Join state
    if (mifset_timer_remain(_downstream_expiry_timers, vif_index) < holdtime)
	mifset_timer_start(_downstream_expiry_timers, vif_index, holdtime, 0,
			   &PimMre::downstream_expiry_timer_timeout_wc);
    return;

 prune_pending_state_label:
    // PrunePending state -> Join state
    mifset_timer_cancel(_downstream_prune_pending_timers, vif_index);
    if (mifset_timer_remain(_downstream_expiry_timers, vif_index) < holdtime)
	mifset_timer_start(_downstream_expiry_timers, vif_index, holdtime, 0,
			   &PimMre::downstream_expiry_timer_timeout_wc);
    set_downstream_join_state(vif_index);
    return;
}

void
PimMre::receive_prune_wc(uint16_t vif_index, uint16_t holdtime)
{
    PimVif *pim_vif;

    if (vif_index == Vif::VIF_INDEX_INVALID)
	return;
    
    if (! is_wc())
	return;
    
    if (is_downstream_join_state(vif_index))
	goto join_state_label;
    if (is_downstream_prune_pending_state(vif_index))
	goto prune_pending_state_label;
    goto noinfo_state_label;
    
 noinfo_state_label:
    // NoInfo state
    return;		// Ignore
    
 join_state_label:
    // Join state -> PrunePending state
    pim_vif = pim_mrt().vif_find_by_vif_index(vif_index);
    if (pim_vif == NULL)
	return;
    if (pim_vif->pim_nbrs_number() > 1) {
	struct timeval timeval = pim_vif->jp_override_interval();
	// TODO: make sure usec resolution works!!
	mifset_timer_start(_downstream_prune_pending_timers, vif_index,
			   TIMEVAL_SEC(&timeval), TIMEVAL_USEC(&timeval),
			   &PimMre::downstream_prune_pending_timer_timeout_wc);
    } else {
	// XXX: force to expire now
	mifset_timer_start(_downstream_prune_pending_timers, vif_index, 0, 0,
			   &PimMre::downstream_prune_pending_timer_timeout_wc);
    }
    set_downstream_prune_pending_state(vif_index);
    return;

 prune_pending_state_label:
    // PrunePending state
    return;		// Nothing to do
    
    UNUSED(holdtime);
}

void
PimMre::receive_join_sg(uint16_t vif_index, uint16_t holdtime)
{
    if (vif_index == Vif::VIF_INDEX_INVALID)
	return;
    
    if (! is_sg())
	return;
    
    if (is_downstream_join_state(vif_index))
	goto join_state_label;
    if (is_downstream_prune_pending_state(vif_index))
	goto prune_pending_state_label;
    goto noinfo_state_label;
    
 noinfo_state_label:
    // NoInfo state -> Join state
    mifset_timer_start(_downstream_expiry_timers, vif_index, holdtime, 0,
		       &PimMre::downstream_expiry_timer_timeout_sg);
    set_downstream_join_state(vif_index);
    return;
    
 join_state_label:
    // Join state
    if (mifset_timer_remain(_downstream_expiry_timers, vif_index) < holdtime)
	mifset_timer_start(_downstream_expiry_timers, vif_index, holdtime, 0,
			   &PimMre::downstream_expiry_timer_timeout_sg);
    return;
    
 prune_pending_state_label:
    // PrunePending state -> Join state
    mifset_timer_cancel(_downstream_prune_pending_timers, vif_index);
    if (mifset_timer_remain(_downstream_expiry_timers, vif_index) < holdtime)
	mifset_timer_start(_downstream_expiry_timers, vif_index, holdtime, 0,
			   &PimMre::downstream_expiry_timer_timeout_sg);
    set_downstream_join_state(vif_index);
    return;
}

void
PimMre::receive_prune_sg(uint16_t vif_index, uint16_t holdtime)
{
    PimVif *pim_vif;
    
    if (vif_index == Vif::VIF_INDEX_INVALID)
	return;
    
    if (! is_sg())
	return;
    
    if (is_downstream_join_state(vif_index))
	goto join_state_label;
    if (is_downstream_prune_pending_state(vif_index))
	goto prune_pending_state_label;
    goto noinfo_state_label;
    
 noinfo_state_label:
    // NoInfo state
    return;		// Ignore
    
 join_state_label:
    // Join state -> PrunePending state
    pim_vif = pim_mrt().vif_find_by_vif_index(vif_index);
    if (pim_vif == NULL)
	return;
    if (pim_vif->pim_nbrs_number() > 1) {
	struct timeval timeval = pim_vif->jp_override_interval();
	// TODO: make sure usec resolution works!!
	mifset_timer_start(_downstream_prune_pending_timers, vif_index,
			   TIMEVAL_SEC(&timeval), TIMEVAL_USEC(&timeval),
			   &PimMre::downstream_prune_pending_timer_timeout_sg);
    } else {
	// XXX: force to expire now
	mifset_timer_start(_downstream_prune_pending_timers, vif_index, 0, 0,
			   &PimMre::downstream_prune_pending_timer_timeout_sg);
    }
    set_downstream_prune_pending_state(vif_index);
    return;
    
 prune_pending_state_label:
    // PrunePending state    
    return;		// Nothing to do
    
    UNUSED(holdtime);
}

void
PimMre::receive_join_wc_by_sg_rpt(uint16_t vif_index)
{
    if (vif_index == Vif::VIF_INDEX_INVALID)
	return;
    
    if (! is_sg_rpt())
	return;
    
    if (is_downstream_processed_wc_by_sg_rpt(vif_index))
	return;
    set_downstream_processed_wc_by_sg_rpt(vif_index, true);
    
    if (is_downstream_prune_state(vif_index))
	goto prune_state_label;
    
    if (is_downstream_prune_pending_state(vif_index))
	goto prune_pending_state_label;
    
    return;		// Ignore
    
 prune_state_label:
    // Prune state -> PruneTmp state
    set_downstream_prune_tmp_state(vif_index);
    return;
    
 prune_pending_state_label:
    // PrunePending state -> PrunePendingTmp state
    set_downstream_prune_pending_tmp_state(vif_index);
    return;
}

void
PimMre::receive_join_sg_rpt(uint16_t vif_index, uint16_t holdtime)
{
    if (vif_index == Vif::VIF_INDEX_INVALID)
	return;
    
    if (! is_sg_rpt())
	return;
    
    if (is_downstream_prune_state(vif_index))
	goto prune_state_label;
    if (is_downstream_prune_pending_state(vif_index))
	goto prune_pending_state_label;
    goto noinfo_state_label;
    
 noinfo_state_label:
    // NoInfo state
    return;		// Ignore
    
 prune_state_label:
    // Prune state -> NoInfo state
    mifset_timer_cancel(_downstream_expiry_timers, vif_index);
    mifset_timer_cancel(_downstream_prune_pending_timers, vif_index);
    set_downstream_noinfo_state(vif_index);
    return;
    
 prune_pending_state_label:
    // PrunePending state -> NoInfo state
    mifset_timer_cancel(_downstream_expiry_timers, vif_index);
    mifset_timer_cancel(_downstream_prune_pending_timers, vif_index);
    set_downstream_noinfo_state(vif_index);
    return;
    
    UNUSED(holdtime);
}

// @join_wc_received_bool is true if there was a (*,G)Join together
// with this (S,G,rpt)Prune message.
void
PimMre::receive_prune_sg_rpt(uint16_t vif_index, uint16_t holdtime,
			     bool join_wc_received_bool)
{
    PimVif *pim_vif;
    
    if (vif_index == Vif::VIF_INDEX_INVALID)
	return;
    
    if (! is_sg_rpt())
	return;
    
    if (join_wc_received_bool)
	receive_join_wc_by_sg_rpt(vif_index);
    
    if (is_downstream_prune_state(vif_index))
	goto prune_state_label;
    
    if (is_downstream_prune_pending_state(vif_index))
	goto prune_pending_state_label;
    
    if (is_downstream_prune_tmp_state(vif_index))
	goto prune_tmp_state_label;
    
    if (is_downstream_prune_pending_tmp_state(vif_index))
	goto prune_pending_tmp_state_label;
    
    goto noinfo_state_label;
    
 noinfo_state_label:
    // NoInfo state -> PrunePending state
    pim_vif = pim_mrt().vif_find_by_vif_index(vif_index);
    if (pim_vif == NULL)
	return;
    mifset_timer_start(_downstream_expiry_timers, vif_index, holdtime, 0,
		       &PimMre::downstream_expiry_timer_timeout_sg_rpt);
    if (pim_vif->pim_nbrs_number() > 1) {
	struct timeval timeval = pim_vif->jp_override_interval();
	// TODO: make sure usec resolution works!!
	mifset_timer_start(_downstream_prune_pending_timers, vif_index,
			   TIMEVAL_SEC(&timeval), TIMEVAL_USEC(&timeval),
			   &PimMre::downstream_prune_pending_timer_timeout_sg_rpt);
    } else {
	// XXX: force to expire now
	mifset_timer_start(_downstream_prune_pending_timers, vif_index, 0, 0,
			   &PimMre::downstream_prune_pending_timer_timeout_sg_rpt);
    }
    set_downstream_prune_pending_state(vif_index);
    return;
    
 prune_tmp_state_label:
    // PruneTmp state -> Prune state
    set_downstream_prune_state(vif_index);
    // FALLTHROUGH to Prune state (XXX: note that the action is same)
 prune_state_label:
    // Prune state
    if (mifset_timer_remain(_downstream_expiry_timers, vif_index) < holdtime)
	mifset_timer_start(_downstream_expiry_timers, vif_index, holdtime, 0,
			   &PimMre::downstream_expiry_timer_timeout_sg_rpt);
    return;
    
 prune_pending_tmp_state_label:
    // PrunePendingTmp state -> PrunePending state
    set_downstream_prune_pending_state(vif_index);
    if (mifset_timer_remain(_downstream_expiry_timers, vif_index) < holdtime)
	mifset_timer_start(_downstream_expiry_timers, vif_index, holdtime, 0,
			   &PimMre::downstream_expiry_timer_timeout_sg_rpt);
    // FALLTHROUGH to PrunePending state
 prune_pending_state_label:
    // PrunePending state
    return;		// XXX: TODO: really ignore (S,G,rpt)P in PP state??
}

//
// Receive "End of Message"
// Note: applies for (S,G,rpt)
//
void
PimMre::receive_end_of_message_sg_rpt(uint16_t vif_index)
{
    if (vif_index == Vif::VIF_INDEX_INVALID)
	return;
    
    if (! is_sg_rpt())
	return;
    
    set_downstream_processed_wc_by_sg_rpt(vif_index, false);
    
    if (is_downstream_prune_tmp_state(vif_index))
	goto prune_tmp_state_label;
    
    if (is_downstream_prune_pending_tmp_state(vif_index))
	goto prune_pending_tmp_state_label;
    
    return;		// Ignore
    
 prune_tmp_state_label:
    // PruneTmp state -> NoInfo state
    mifset_timer_cancel(_downstream_expiry_timers, vif_index);
    mifset_timer_cancel(_downstream_prune_pending_timers, vif_index);
    set_downstream_noinfo_state(vif_index);
    return;
    
 prune_pending_tmp_state_label:
    // PrunePendingTmp state -> NoInfo state
    mifset_timer_cancel(_downstream_expiry_timers, vif_index);
    mifset_timer_cancel(_downstream_prune_pending_timers, vif_index);
    set_downstream_noinfo_state(vif_index);
    return;
}

//
// Downstream J/P timers
//
void
PimMre::downstream_expiry_timer_timeout_rp(uint16_t vif_index)
{
    if (vif_index == Vif::VIF_INDEX_INVALID)
	return;
    
    if (! is_rp())
	return;
    
    if (is_downstream_join_state(vif_index))
	goto join_state_label;
    if (is_downstream_prune_pending_state(vif_index))
	goto prune_pending_label;
    return;
    
 join_state_label:
    // Join state -> NoInfo state
 prune_pending_label:
    // PrunePending state -> NoInfo state
    set_downstream_noinfo_state(vif_index);
}

void
PimMre::downstream_prune_pending_timer_timeout_rp(uint16_t vif_index)
{
    PimVif *pim_vif;
    PimNbr *pim_nbr_me;
    
    if (vif_index == Vif::VIF_INDEX_INVALID)
	return;
    
    if (! is_rp())
	return;
    
    if (is_downstream_prune_pending_state(vif_index))
	goto prune_pending_label;
    return;
    
 prune_pending_label:
    // PrunePending state -> NoInfo state
    // Send PruneEcho(*,*,RP) if more than one PIM neighbors
    pim_vif = pim_mrt().vif_find_by_vif_index(vif_index);
    if (pim_vif == NULL)
	return;
    if (pim_vif->pim_nbrs().size() > 1) {
	pim_nbr_me = &pim_vif->pim_nbr_me();
	bool new_group_bool = false;	// Group together all (*,*,RP) entries
	pim_nbr_me->jp_entry_add(*rp_addr_ptr(), IPvX::MULTICAST_BASE(family()),
				 IPvX::ip_multicast_base_address_masklen(family()),
				 MRT_ENTRY_RP,
				 ACTION_PRUNE,
				 pim_nbr_me->pim_vif().join_prune_holdtime().get(),
				 new_group_bool);
    }
    set_downstream_noinfo_state(vif_index);
}

void
PimMre::downstream_expiry_timer_timeout_wc(uint16_t vif_index)
{
    if (vif_index == Vif::VIF_INDEX_INVALID)
	return;
    
    if (! is_wc())
	return;
    
    if (is_downstream_join_state(vif_index))
	goto join_state_label;
    if (is_downstream_prune_pending_state(vif_index))
	goto prune_pending_label;
    return;
    
 join_state_label:
    // Join state -> NoInfo state
 prune_pending_label:
    // PrunePending state -> NoInfo state
    set_downstream_noinfo_state(vif_index);
}

void
PimMre::downstream_prune_pending_timer_timeout_wc(uint16_t vif_index)
{
    PimVif *pim_vif;
    PimNbr *pim_nbr_me;
    const IPvX *my_rp_addr_ptr = NULL;
    
    if (vif_index == Vif::VIF_INDEX_INVALID)
	return;
    
    if (! is_wc())
	return;
    
    if (is_downstream_prune_pending_state(vif_index))
	goto prune_pending_label;
    return;
    
 prune_pending_label:
    // PrunePending state -> NoInfo state
    // Send PruneEcho(*,G) if more than one PIM neighbors
    pim_vif = pim_mrt().vif_find_by_vif_index(vif_index);
    if (pim_vif == NULL)
	return;
    if (pim_vif->pim_nbrs().size() > 1) {
	pim_nbr_me = &pim_vif->pim_nbr_me();
	bool new_group_bool = false;	// Group together all (*,G) entries
	my_rp_addr_ptr = rp_addr_ptr();
	if (my_rp_addr_ptr == NULL) {
	    XLOG_WARNING("Sending PruneEcho(*,G): "
			 "RP for group %s: not found",
			 cstring(group_addr()));
	} else {
	    // We have RP
	    pim_nbr_me->jp_entry_add(*my_rp_addr_ptr, group_addr(),
				     IPvX::addr_bitlen(family()),
				     MRT_ENTRY_WC,
				     ACTION_PRUNE,
				     pim_nbr_me->pim_vif().join_prune_holdtime().get(),
				     new_group_bool);
	}
    }
    set_downstream_noinfo_state(vif_index);
}

void
PimMre::downstream_expiry_timer_timeout_sg(uint16_t vif_index)
{
    if (vif_index == Vif::VIF_INDEX_INVALID)
	return;
    
    if (! is_sg())
	return;
    
    if (is_downstream_join_state(vif_index))
	goto join_state_label;
    if (is_downstream_prune_pending_state(vif_index))
	goto prune_pending_label;
    return;
    
 join_state_label:
    // Join state -> NoInfo state
 prune_pending_label:
    // PrunePending state -> NoInfo state
    set_downstream_noinfo_state(vif_index);
}

void
PimMre::downstream_prune_pending_timer_timeout_sg(uint16_t vif_index)
{
    PimVif *pim_vif;
    PimNbr *pim_nbr_me;
    
    if (vif_index == Vif::VIF_INDEX_INVALID)
	return;
    
    if (! is_sg())
	return;
    
    if (is_downstream_prune_pending_state(vif_index))
	goto prune_pending_label;
    return;
    
 prune_pending_label:
    // PrunePending state -> NoInfo state
    // Send PruneEcho(S,G) if more than one PIM neighbors
    pim_vif = pim_mrt().vif_find_by_vif_index(vif_index);
    if (pim_vif == NULL)
	return;
    if (pim_vif->pim_nbrs().size() > 1) {
	pim_nbr_me = &pim_vif->pim_nbr_me();
	bool new_group_bool = false;	// Group together all (S,G) entries
	pim_nbr_me->jp_entry_add(source_addr(), group_addr(),
				 IPvX::addr_bitlen(family()),
				 MRT_ENTRY_SG,
				 ACTION_PRUNE,
				 pim_nbr_me->pim_vif().join_prune_holdtime().get(),
				 new_group_bool);
    }
    set_downstream_noinfo_state(vif_index);
}

void
PimMre::downstream_expiry_timer_timeout_sg_rpt(uint16_t vif_index)
{
    if (vif_index == Vif::VIF_INDEX_INVALID)
	return;
    
    if (! is_sg_rpt())
	return;
    
    if (is_downstream_prune_state(vif_index))
	goto prune_state_label;
    return;
    
 prune_state_label:
    // Prune state -> NoInfo state
    mifset_timer_cancel(_downstream_expiry_timers, vif_index);
    mifset_timer_cancel(_downstream_prune_pending_timers, vif_index);
    set_downstream_noinfo_state(vif_index);
    return;
}

void
PimMre::downstream_prune_pending_timer_timeout_sg_rpt(uint16_t vif_index)
{
    if (vif_index == Vif::VIF_INDEX_INVALID)
	return;
    
    if (! is_sg_rpt())
	return;
    
    if (is_downstream_prune_pending_state(vif_index))
	goto prune_pending_state_label;
    return;
    
 prune_pending_state_label:
    // PrunePending state -> Prune state
    set_downstream_prune_state(vif_index);
    return;
}

//
// Upstream J/P (*,*,RP) state machine
//
void
PimMre::rp_see_join_rp(uint16_t vif_index, const IPvX& target_nbr_addr)
{
    PimNbr *my_mrib_next_hop_rp;
    PimVif *pim_vif;
    
    if (vif_index == Vif::VIF_INDEX_INVALID)
	return;
    
    if (! is_rp())
	return;
    
    if (is_joined_state())
	goto joined_state_label;
    return;
    
 joined_state_label:
    my_mrib_next_hop_rp = mrib_next_hop_rp();
    if (my_mrib_next_hop_rp == NULL)
	return;			// XXX: I don't know the MRIB.next_hop(RP).
    if (my_mrib_next_hop_rp->vif_index() != vif_index)
	return;
    if (my_mrib_next_hop_rp->addr() != target_nbr_addr)
	return;
    
    // `target_nbr_addr' is MRIB.next_hop(RP)
    // Restart JoinTimer if it is larger than t_suppressed
    struct timeval t_suppressed, timeval_left;
    pim_vif = pim_mrt().vif_find_by_vif_index(vif_index);
    if (pim_vif == NULL)
	return;
    t_suppressed = pim_vif->upstream_join_timer_t_suppressed();
    join_timer().left_timeval(&timeval_left);
    if (TIMEVAL_CMP(&timeval_left, &t_suppressed, < )) {
	// Restart the timer with `t_suppressed'
	join_timer().start(TIMEVAL_SEC(&t_suppressed),
			   TIMEVAL_USEC(&t_suppressed),
			   pim_mre_join_timer_timeout, this);
    }
}

void
PimMre::rp_see_prune_rp(uint16_t vif_index, const IPvX& target_nbr_addr)
{
    PimNbr *my_mrib_next_hop_rp;
    PimVif *pim_vif;
    
    if (vif_index == Vif::VIF_INDEX_INVALID)
	return;
    
    if (! is_rp())
	return;
    
    if (is_joined_state())
	goto joined_state_label;
    return;
    
 joined_state_label:
    my_mrib_next_hop_rp = mrib_next_hop_rp();
    if (my_mrib_next_hop_rp == NULL)
	return;			// XXX: I don't know the MRIB.next_hop(RP)
    if (my_mrib_next_hop_rp->vif_index() != vif_index)
	return;
    if (my_mrib_next_hop_rp->addr() != target_nbr_addr)
	return;
    
    // `target_nbr_addr' is MRIB.next_hop(RP)
    // Restart JoinTimer if it is larger than t_override
    struct timeval t_override, timeval_left;
    pim_vif = pim_mrt().vif_find_by_vif_index(vif_index);
    if (pim_vif == NULL)
	return;
    t_override = pim_vif->upstream_join_timer_t_override();
    join_timer().left_timeval(&timeval_left);
    if (TIMEVAL_CMP(&timeval_left, &t_override, > )) {
	// Restart the timer with `t_override'
	join_timer().start(TIMEVAL_SEC(&t_override),
			   TIMEVAL_USEC(&t_override),
			   pim_mre_join_timer_timeout, this);
    }
}

void
PimMre::wc_see_join_wc(uint16_t vif_index, const IPvX& target_nbr_addr)
{
    PimNbr *my_rpfp_nbr_wc;
    PimVif *pim_vif;
    
    if (vif_index == Vif::VIF_INDEX_INVALID)
	return;
    
    if (! is_wc())
	return;
    
    if (is_joined_state())
	goto joined_state_label;
    return;
    
 joined_state_label:
    my_rpfp_nbr_wc = rpfp_nbr_wc();
    if (my_rpfp_nbr_wc == NULL)
	return;			// XXX: I don't know the RPF'(*,G) nbr
    if (my_rpfp_nbr_wc->vif_index() != vif_index)
	return;
    if (my_rpfp_nbr_wc->addr() != target_nbr_addr)
	return;
    
    // `target_nbr_addr' is RPF'(*,G)
    // Restart JoinTimer if it is larger than t_suppressed
    struct timeval t_suppressed, timeval_left;
    pim_vif = pim_mrt().vif_find_by_vif_index(vif_index);
    if (pim_vif == NULL)
	return;
    t_suppressed = pim_vif->upstream_join_timer_t_suppressed();
    join_timer().left_timeval(&timeval_left);
    if (TIMEVAL_CMP(&timeval_left, &t_suppressed, < )) {
	// Restart the timer with `t_suppressed'
	join_timer().start(TIMEVAL_SEC(&t_suppressed),
			   TIMEVAL_USEC(&t_suppressed),
			   pim_mre_join_timer_timeout, this);
    }
}

void
PimMre::wc_see_prune_wc(uint16_t vif_index, const IPvX& target_nbr_addr)
{
    PimNbr *my_rpfp_nbr_wc;
    PimVif *pim_vif;
    
    if (vif_index == Vif::VIF_INDEX_INVALID)
	return;
    
    if (! is_wc())
	return;
    
    if (is_joined_state())
	goto joined_state_label;
    return;
    
 joined_state_label:
    my_rpfp_nbr_wc = rpfp_nbr_wc();
    if (my_rpfp_nbr_wc == NULL)
	return;			// XXX: I don't know the RPF'(*,G) nbr
    if (my_rpfp_nbr_wc->vif_index() != vif_index)
	return;
    if (my_rpfp_nbr_wc->addr() != target_nbr_addr)
	return;
    
    // `target_nbr_addr' is RPF'(*,G)
    // Restart JoinTimer if it is larger than t_override
    struct timeval t_override, timeval_left;
    pim_vif = pim_mrt().vif_find_by_vif_index(vif_index);
    if (pim_vif == NULL)
	return;
    t_override = pim_vif->upstream_join_timer_t_override();
    join_timer().left_timeval(&timeval_left);
    if (TIMEVAL_CMP(&timeval_left, &t_override, > )) {
	// Restart the timer with `t_override'
	join_timer().start(TIMEVAL_SEC(&t_override),
			   TIMEVAL_USEC(&t_override),
			   pim_mre_join_timer_timeout, this);
    }
}

void
PimMre::sg_see_join_sg(uint16_t vif_index, const IPvX& target_nbr_addr)
{
    PimNbr *my_rpfp_nbr_sg;
    PimVif *pim_vif;
    
    if (vif_index == Vif::VIF_INDEX_INVALID)
	return;
    
    if (! is_sg())
	return;
    
    if (is_joined_state())
	goto joined_state_label;
    return;
    
 joined_state_label:
    my_rpfp_nbr_sg = rpfp_nbr_sg();
    if (my_rpfp_nbr_sg == NULL)
	return;			// XXX: I don't know the RPF'(S,G) nbr
    if (my_rpfp_nbr_sg->vif_index() != vif_index)
	return;
    if (my_rpfp_nbr_sg->addr() != target_nbr_addr)
	return;
    
    // `target_nbr_addr' is RPF'(S,G)
    // Restart JoinTimer if it is larger than t_suppressed
    struct timeval t_suppressed, timeval_left;
    pim_vif = pim_mrt().vif_find_by_vif_index(vif_index);
    if (pim_vif == NULL)
	return;
    t_suppressed = pim_vif->upstream_join_timer_t_suppressed();
    join_timer().left_timeval(&timeval_left);
    if (TIMEVAL_CMP(&timeval_left, &t_suppressed, < )) {
	// Restart the timer with `t_suppressed'
	join_timer().start(TIMEVAL_SEC(&t_suppressed),
			   TIMEVAL_USEC(&t_suppressed),
			   pim_mre_join_timer_timeout, this);
    }
}

void
PimMre::sg_see_prune_sg(uint16_t vif_index, const IPvX& target_nbr_addr)
{
    PimNbr *my_rpfp_nbr_sg;
    PimVif *pim_vif;
    
    if (vif_index == Vif::VIF_INDEX_INVALID)
	return;
    
    if (! is_sg())
	return;
    
    if (is_joined_state())
	goto joined_state_label;
    return;
    
 joined_state_label:
    my_rpfp_nbr_sg = rpfp_nbr_sg();
    if (my_rpfp_nbr_sg == NULL)
	return;			// XXX: I don't know the RPF'(S,G) nbr
    if (my_rpfp_nbr_sg->vif_index() != vif_index)
	return;
    if (my_rpfp_nbr_sg->addr() != target_nbr_addr)
	return;
    
    // `target_nbr_addr' is RPF'(S,G)
    // Restart JoinTimer if it is larger than t_override
    struct timeval t_override, timeval_left;
    pim_vif = pim_mrt().vif_find_by_vif_index(vif_index);
    if (pim_vif == NULL)
	return;
    t_override = pim_vif->upstream_join_timer_t_override();
    join_timer().left_timeval(&timeval_left);
    if (TIMEVAL_CMP(&timeval_left, &t_override, > )) {
	// Restart the timer with `t_override'
	join_timer().start(TIMEVAL_SEC(&t_override),
			   TIMEVAL_USEC(&t_override),
			   pim_mre_join_timer_timeout, this);
    }
}

void
PimMre::sg_see_prune_wc(uint16_t vif_index, const IPvX& target_nbr_addr)
{
    PimNbr *my_rpfp_nbr_sg;
    PimVif *pim_vif;
    
    if (vif_index == Vif::VIF_INDEX_INVALID)
	return;
    
    if (! is_sg())
	return;
    
    if (is_joined_state())
	goto joined_state_label;
    return;
    
 joined_state_label:
    my_rpfp_nbr_sg = rpfp_nbr_sg();
    if (my_rpfp_nbr_sg == NULL)
	return;			// XXX: I don't know the RPF'(S,G) nbr
    if (my_rpfp_nbr_sg->vif_index() != vif_index)
	return;
    if (my_rpfp_nbr_sg->addr() != target_nbr_addr)
	return;
    
    // `target_nbr_addr' is RPF'(S,G)
    // Restart JoinTimer if it is larger than t_override
    struct timeval t_override, timeval_left;
    pim_vif = pim_mrt().vif_find_by_vif_index(vif_index);
    if (pim_vif == NULL)
	return;
    t_override = pim_vif->upstream_join_timer_t_override();
    join_timer().left_timeval(&timeval_left);
    if (TIMEVAL_CMP(&timeval_left, &t_override, > )) {
	// Restart the timer with `t_override'
	join_timer().start(TIMEVAL_SEC(&t_override),
			   TIMEVAL_USEC(&t_override),
			   pim_mre_join_timer_timeout, this);
    }
}

void
PimMre::sg_see_prune_sg_rpt(uint16_t vif_index, const IPvX& target_nbr_addr)
{
    PimNbr *my_rpfp_nbr_sg;
    PimVif *pim_vif;
    
    if (vif_index == Vif::VIF_INDEX_INVALID)
	return;
    
    if (! is_sg())
	return;
    
    if (is_joined_state())
	goto joined_state_label;
    return;
    
 joined_state_label:
    my_rpfp_nbr_sg = rpfp_nbr_sg();
    if (my_rpfp_nbr_sg == NULL)
	return;			// XXX: I don't know the RPF'(S,G) nbr
    if (my_rpfp_nbr_sg->vif_index() != vif_index)
	return;
    if (my_rpfp_nbr_sg->addr() != target_nbr_addr)
	return;
    
    // `target_nbr_addr' is RPF'(S,G)
    // Restart JoinTimer if it is larger than t_override
    struct timeval t_override, timeval_left;
    pim_vif = pim_mrt().vif_find_by_vif_index(vif_index);
    if (pim_vif == NULL)
	return;
    t_override = pim_vif->upstream_join_timer_t_override();
    join_timer().left_timeval(&timeval_left);
    if (TIMEVAL_CMP(&timeval_left, &t_override, > )) {
	// Restart the timer with `t_override'
	join_timer().start(TIMEVAL_SEC(&t_override),
			   TIMEVAL_USEC(&t_override),
			   pim_mre_join_timer_timeout, this);
    }
}

void
PimMre::sg_rpt_see_join_sg_rpt(uint16_t vif_index, const IPvX& target_nbr_addr)
{
    PimNbr *my_rpfp_nbr_sg_rpt;
    
    if (vif_index == Vif::VIF_INDEX_INVALID)
	return;
    
    if (! is_sg_rpt())
	return;
    
    if (is_not_pruned_state())
	goto not_pruned_state_label;
    return;
    
 not_pruned_state_label:
    my_rpfp_nbr_sg_rpt = rpfp_nbr_sg_rpt();
    if (my_rpfp_nbr_sg_rpt == NULL)
	return;			// XXX: I don't know the RPF'(S,G,rpt) nbr
    if (my_rpfp_nbr_sg_rpt->vif_index() != vif_index)
	return;
    if (my_rpfp_nbr_sg_rpt->addr() != target_nbr_addr)
	return;
    
    // `target_nbr_addr' is RPF'(S,G,rpt)
    // Cancel OverrideTimer
    override_timer().cancel();
}

void
PimMre::sg_rpt_see_prune_sg_rpt(uint16_t vif_index,
				const IPvX& target_nbr_addr)
{
    PimNbr *my_rpfp_nbr_sg_rpt;
    PimVif *pim_vif;
    
    if (vif_index == Vif::VIF_INDEX_INVALID)
	return;
    
    if (! is_sg_rpt())
	return;
    
    if (is_not_pruned_state())
	goto not_pruned_state_label;
    return;
    
 not_pruned_state_label:
    my_rpfp_nbr_sg_rpt = rpfp_nbr_sg_rpt();
    if (my_rpfp_nbr_sg_rpt == NULL)
	return;			// XXX: I don't know the RPF'(S,G,rpt) nbr
    if (my_rpfp_nbr_sg_rpt->vif_index() != vif_index)
	return;
    if (my_rpfp_nbr_sg_rpt->addr() != target_nbr_addr)
	return;
    
    // `target_nbr_addr' is RPF'(S,G)
    // Restart OverrideTimer if it is larger than t_override
    struct timeval t_override, timeval_left;
    pim_vif = pim_mrt().vif_find_by_vif_index(vif_index);
    if (pim_vif == NULL)
	return;
    t_override = pim_vif->upstream_join_timer_t_override();
    override_timer().left_timeval(&timeval_left);
    if (TIMEVAL_CMP(&timeval_left, &t_override, > )) {
	// Restart the timer with `t_override'
	override_timer().start(TIMEVAL_SEC(&t_override),
			       TIMEVAL_USEC(&t_override),
			       pim_mre_override_timer_timeout, this);
    }
}

void
PimMre::sg_rpt_see_prune_sg(uint16_t vif_index, const IPvX& target_nbr_addr)
{
    PimNbr *my_rpfp_nbr_sg_rpt;
    PimVif *pim_vif;
    
    if (vif_index == Vif::VIF_INDEX_INVALID)
	return;
    
    if (! is_sg_rpt())
	return;
    
    if (is_not_pruned_state())
	goto not_pruned_state_label;
    return;
    
 not_pruned_state_label:
    my_rpfp_nbr_sg_rpt = rpfp_nbr_sg_rpt();
    if (my_rpfp_nbr_sg_rpt == NULL)
	return;			// XXX: I don't know the RPF'(S,G,rpt) nbr
    if (my_rpfp_nbr_sg_rpt->vif_index() != vif_index)
	return;
    if (my_rpfp_nbr_sg_rpt->addr() != target_nbr_addr)
	return;
    
    // `target_nbr_addr' is RPF'(S,G)
    // Restart OverrideTimer if it is larger than t_override
    struct timeval t_override, timeval_left;
    pim_vif = pim_mrt().vif_find_by_vif_index(vif_index);
    if (pim_vif == NULL)
	return;
    t_override = pim_vif->upstream_join_timer_t_override();
    override_timer().left_timeval(&timeval_left);
    if (TIMEVAL_CMP(&timeval_left, &t_override, > )) {
	// Restart the timer with `t_override'
	override_timer().start(TIMEVAL_SEC(&t_override),
			       TIMEVAL_USEC(&t_override),
			       pim_mre_override_timer_timeout, this);
    }
}

//
// Upstream J/P (*,*,RP) state machine
//
// Note: applies only for (*,*,RP)
// Return true if state has changed, otherwise return false.
bool
PimMre::recompute_is_join_desired_rp()
{
    PimNbr *pim_nbr;
    uint16_t join_prune_period = PIM_JOIN_PRUNE_PERIOD_DEFAULT;
    
    if (! is_rp())
	return (false);
    
    if (is_not_joined_state())
	goto not_joined_state_label;
    if (is_joined_state())
	goto joined_state_label;
    XLOG_ASSERT(false);
    return (false);
    
 not_joined_state_label:
    // NotJoined state -> Joined state
    if (! is_join_desired_rp())
	return (false);		// Nothing changed
    // Send Join(*,*,RP)
    pim_nbr = mrib_next_hop_rp();
    if (pim_nbr == NULL) {
	if (! i_am_rp()) {
	    XLOG_WARNING("JoinDesired(*,*,RP) = true: "
			 "upstream neighbor for RP %s: not found",
			 cstring(*rp_addr_ptr()));
	}
    } else {
	bool new_group_bool = false;	// Group together all (*,*,RP) entries
	pim_nbr->jp_entry_add(*rp_addr_ptr(), IPvX::MULTICAST_BASE(family()),
			      IPvX::ip_multicast_base_address_masklen(family()),
			      MRT_ENTRY_RP,
			      ACTION_JOIN,
			      pim_nbr->pim_vif().join_prune_holdtime().get(),
			      new_group_bool);
	join_prune_period = pim_nbr->pim_vif().join_prune_period().get();
    }
    // Restart the JoinTimer
    join_timer().start(join_prune_period, 0,
		       pim_mre_join_timer_timeout, this);
    // Set the new state
    set_joined_state();
    return (true);
    
 joined_state_label:
    // Joined state -> NotJoined state
    if (is_join_desired_rp())
	return (false);		// Nothing changed
    // Send Prune(*,*,RP);
    pim_nbr = mrib_next_hop_rp();
    if (pim_nbr == NULL) {
	if (! i_am_rp()) {
	    XLOG_WARNING("JoinDesired(*,*,RP) = false: "
			 "upstream neighbor for RP %s: not found",
			 cstring(*rp_addr_ptr()));
	}
    } else {
	bool new_group_bool = false;	// Group together all (*,*,RP) entries
	pim_nbr->jp_entry_add(*rp_addr_ptr(), IPvX::MULTICAST_BASE(family()),
			      IPvX::ip_multicast_base_address_masklen(family()),
			      MRT_ENTRY_RP,
			      ACTION_PRUNE,
			      pim_nbr->pim_vif().join_prune_holdtime().get(),
			      new_group_bool);
    }
    // Cancel the JoinTimer
    join_timer().cancel();
    // Set the new state
    set_not_joined_state();
    entry_try_remove();
    return (true);
}

//
// Upstream J/P (*,G) state machine
//
// Note: applies only for (*,G)
// Return true if state has changed, otherwise return false.
bool
PimMre::recompute_is_join_desired_wc()
{
    PimNbr *pim_nbr;
    uint16_t join_prune_period = PIM_JOIN_PRUNE_PERIOD_DEFAULT;
    const IPvX *my_rp_addr_ptr = NULL;
    
    if (! is_wc())
	return (false);
    
    if (is_not_joined_state())
	goto not_joined_state_label;
    if (is_joined_state())
	goto joined_state_label;
    XLOG_ASSERT(false);
    return (false);
    
 not_joined_state_label:
    // NotJoined state -> Joined state
    if (! is_join_desired_wc())
	return (false);		// Nothing changed
    // Send Join(*,G)
    my_rp_addr_ptr = rp_addr_ptr();
    if (my_rp_addr_ptr == NULL) {
	XLOG_WARNING("JoinDesired(*,G) = true: "
		     "RP for group %s: not found",
		     cstring(group_addr()));
    } else {
	// We have RP
	pim_nbr = rpfp_nbr_wc();
	if (pim_nbr == NULL) {
	    if (! i_am_rp()) {
		XLOG_WARNING("JoinDesired(*,G) = true: "
			     "upstream neighbor for RP %s for group %s: not found",
			     cstring(*my_rp_addr_ptr), cstring(group_addr()));
	    }
	} else {
	    bool new_group_bool = false; // Allow merging entries for same G
	    pim_nbr->jp_entry_add(*my_rp_addr_ptr, group_addr(),
				  IPvX::addr_bitlen(family()),
				  MRT_ENTRY_WC,
				  ACTION_JOIN,
				  pim_nbr->pim_vif().join_prune_holdtime().get(),
				  new_group_bool);
	    join_prune_period = pim_nbr->pim_vif().join_prune_period().get();
	}
    }
    // Restart the JoinTimer
    join_timer().start(join_prune_period, 0,
		       pim_mre_join_timer_timeout, this);
    // Set the new state
    set_joined_state();
    return (true);
    
 joined_state_label:
    // Joined state -> NotJoined state
    if (is_join_desired_wc())
	return (false);		// Nothing changed
    // Send Prune(*,G);
    my_rp_addr_ptr = rp_addr_ptr();
    if (my_rp_addr_ptr == NULL) {
	XLOG_WARNING("JoinDesired(*,G) = false: "
		     "RP for group %s: not found",
		     cstring(group_addr()));
    } else {
	// We have RP
	pim_nbr = rpfp_nbr_wc();
	if (pim_nbr == NULL) {
	    if (! i_am_rp()) {
		XLOG_WARNING("JoinDesired(*,G) = false: "
			     "upstream neighbor for RP %s for group %s: not found",
			     rp_addr_string().c_str(), cstring(group_addr()));
	    }
	} else {
	    bool new_group_bool = false; // Allow merging entries for same G
	    pim_nbr->jp_entry_add(*my_rp_addr_ptr, group_addr(),
				  IPvX::addr_bitlen(family()),
				  MRT_ENTRY_WC,
				  ACTION_PRUNE,
				  pim_nbr->pim_vif().join_prune_holdtime().get(),
				  new_group_bool);
	}
    }
    // Cancel the JoinTimer
    join_timer().cancel();
    // Set the new state
    set_not_joined_state();
    entry_try_remove();
    return (true);
}

//
// Upstream J/P (S,G) state machine
//
// Note: applies only for (S,G)
// Return true if state has changed, otherwise return false.
bool
PimMre::recompute_is_join_desired_sg()
{
    PimNbr *pim_nbr;
    uint16_t join_prune_period = PIM_JOIN_PRUNE_PERIOD_DEFAULT;
    
    if (! is_sg())
	return (false);
    
    if (is_not_joined_state())
	goto not_joined_state_label;
    if (is_joined_state())
	goto joined_state_label;
    XLOG_ASSERT(false);
    return (false);
    
 not_joined_state_label:
    // NotJoined state -> Joined state
    if (! is_join_desired_sg())
	return (false);		// Nothing changed
    // Send Join(S,G)
    pim_nbr = rpfp_nbr_sg();
    if (pim_nbr == NULL) {
	if (! is_directly_connected_s()) {
	    XLOG_WARNING("JoinDesired(S,G) = true: "
			 "upstream neighbor for source %s and group %s: not found",
			 cstring(source_addr()), cstring(group_addr()));
	}
    } else {
	bool new_group_bool = false; // Allow merging entries for same G
	pim_nbr->jp_entry_add(source_addr(), group_addr(),
			      IPvX::addr_bitlen(family()),
			      MRT_ENTRY_SG,
			      ACTION_JOIN,
			      pim_nbr->pim_vif().join_prune_holdtime().get(),
			      new_group_bool);
	join_prune_period = pim_nbr->pim_vif().join_prune_period().get();
    }
    // Restart the JoinTimer
    join_timer().start(join_prune_period, 0,
		       pim_mre_join_timer_timeout, this);
    // Set the new state
    set_joined_state();
    return (true);
    
 joined_state_label:
    // Joined state -> NotJoined state
    if (is_join_desired_sg())
	return (false);		// Nothing changed
    // Send Prune(S,G);
    pim_nbr = rpfp_nbr_sg();
    if (pim_nbr == NULL) {
	if (! is_directly_connected_s()) {
	    XLOG_WARNING("JoinDesired(S,G) = false: "
			 "upstream neighbor for source %s and group %s: not found",
			 cstring(source_addr()), cstring(group_addr()));
	}
    } else {
	bool new_group_bool = false;	// Allow merging entries for same group
	pim_nbr->jp_entry_add(source_addr(), group_addr(),
			      IPvX::addr_bitlen(family()),
			      MRT_ENTRY_SG,
			      ACTION_PRUNE,
			      pim_nbr->pim_vif().join_prune_holdtime().get(),
			      new_group_bool);
    }
    // Cancel the JoinTimer
    join_timer().cancel();
    // SPTbit(S,G) to FALSE
    set_spt(false);
    // Set the new state
    set_not_joined_state();
    entry_try_remove();
    return (true);
}

//
// Upstream J/P (S,G,rpt) state machine
//
// Note: applies only for (S,G,rpt)
// Return true if state has changed, otherwise return false.
bool
PimMre::recompute_is_prune_desired_sg_rpt()
{
    PimNbr *pim_nbr;
    const IPvX *my_rp_addr_ptr = NULL;
    
    if (! is_sg_rpt())
	return (false);
    
    if (is_rpt_not_joined_state())
	goto rpt_not_joined_state_label;
    if (is_pruned_state())
	goto pruned_state_label;
    if (is_not_pruned_state())
	goto not_pruned_state_label;
    XLOG_ASSERT(false);
    return (false);
    
 rpt_not_joined_state_label:
    // RPTNotJoined state -> [Pruned state]
    if (! is_prune_desired_sg_rpt())
	return (false);		// Nothing changed
    // PruneDesired(S,G,rpt) -> true
    // Set the new state
    set_pruned_state();
    return (true);
    
 pruned_state_label:
    // Pruned state -> [RPTNotJoined state] [NotPruned state]
    if (is_prune_desired_sg_rpt())
	return (false);		// Nothing changed
    // PruneDesired(S,G,rpt) -> false
    // XXX: RPTJoinDesired(G) should be true, otherwise this is
    // a different state transition.
    if (! is_rpt_join_desired_g()) {
	// XXX: this situation should have been handled earlier by
	// recompute_is_rpt_join_desired_g(), but anyway, just in case....
	set_rpt_not_joined_state();
	return (true);
    }
    // RPTJoinDesired(G) == true
    // Send Join(S,G,rpt) to RPF'(S,G,rpt)
    my_rp_addr_ptr = rp_addr_ptr();
    if (my_rp_addr_ptr == NULL) {
        XLOG_WARNING("PruneDesired(S,G,rpt) = false: "
                     "RP for group %s: not found",
                     cstring(group_addr()));
    } else {
	// We have RP
	pim_nbr = rpfp_nbr_sg_rpt();
	if (pim_nbr == NULL) {
	    if (! i_am_rp()) {
		XLOG_WARNING("PruneDesired(S,G,rpt) = false: "
			     "upstream neighbor for RP %s for "
			     "source %s and group %s: not found",
			     cstring(*my_rp_addr_ptr),
			     cstring(source_addr()), cstring(group_addr()));
	    }
	} else {
	    bool new_group_bool = false; // Allow merging entries for same G
	    pim_nbr->jp_entry_add(source_addr(), group_addr(),
				  IPvX::addr_bitlen(family()),
				  MRT_ENTRY_SG_RPT,
				  ACTION_JOIN,
				  pim_nbr->pim_vif().join_prune_holdtime().get(),
				  new_group_bool);
	}
    }
    set_not_pruned_state();
    return (true);
    
 not_pruned_state_label:
    // NotPruned state -> [Pruned state]
    if (! is_prune_desired_sg_rpt())
	return (false);		// Nothing changed
    // PruneDesired(S,G,rpt) -> true
    // Send Prune(S,G,rpt);
    my_rp_addr_ptr = rp_addr_ptr();
    if (my_rp_addr_ptr == NULL) {
        XLOG_WARNING("PruneDesired(S,G,rpt) = true: "
                     "RP for group %s: not found",
                     cstring(group_addr()));
    } else {
	// We have RP
	pim_nbr = rpfp_nbr_sg_rpt();
	if (pim_nbr == NULL) {
	    if (! i_am_rp()) {
		XLOG_WARNING("PruneDesired(S,G,rpt) = true: "
			     "upstream neighbor for RP %s for "
			     "source %s and group %s: not found",
			     cstring(*my_rp_addr_ptr),
			     cstring(source_addr()), cstring(group_addr()));
	    }
	} else {
	    bool new_group_bool = false; // Allow merging entries for same G
	    pim_nbr->jp_entry_add(source_addr(), group_addr(),
				  IPvX::addr_bitlen(family()),
				  MRT_ENTRY_SG_RPT,
				  ACTION_PRUNE,
				  pim_nbr->pim_vif().join_prune_holdtime().get(),
				  new_group_bool);
	}
    }
    // Cancel the OverrideTimer
    override_timer().cancel();
    set_pruned_state();
    return (true);
}

//
// Note: applies only for (S,G,rpt)
// Return true if state has changed, otherwise return false.
bool
PimMre::recompute_is_rpt_join_desired_g()
{
    if (! is_sg_rpt())		// XXX: Yes, applies only to (S,G,rpt) 
	return (false);
    
    if (is_rpt_not_joined_state())
	goto rpt_not_joined_state_label;
    if (is_pruned_state())
	goto pruned_state_label;
    if (is_not_pruned_state())
	goto not_pruned_state_label;
    XLOG_ASSERT(false);
    return (false);
    
 rpt_not_joined_state_label:
    // RPTNotJoined state
    return (false);		// Nothing changed
    
 pruned_state_label:
    // Pruned state -> [RPTNotJoined state]
    if (is_rpt_join_desired_g())
	return (false);		// Nothing changed
    // RPTJoinDesired(G) -> false
    set_rpt_not_joined_state();
    return (true);
    
 not_pruned_state_label:
    // NotPruned state -> [RPTNotJoined state]
    if (is_rpt_join_desired_g())
	return (false);		// Nothing changed
    // RPTJoinDesired(G) -> false
    override_timer().cancel();
    set_rpt_not_joined_state();
    return (true);
}

// Note: applies for (S,G,rpt)
// Return true if state has changed, otherwise return false.
bool
PimMre::recompute_inherited_olist_sg_rpt()
{
    if (! is_sg_rpt())
	return (false);
    
    if (is_rpt_not_joined_state())
	goto rpt_not_joined_state_label;
    if (is_pruned_state())
	goto pruned_state_label;
    if (is_not_pruned_state())
	goto not_pruned_state_label;
    XLOG_ASSERT(false);
    return (false);
    
 rpt_not_joined_state_label:
    // RPTNotJoined state -> [NotPruned state]
    if (! inherited_olist_sg_rpt().any())
	return (false);		// Nothing changed
    // inherited_olist(S,G,rpt)->non-NULL
    set_not_pruned_state();
    return (true);
    
 pruned_state_label:
    // Pruned state
    return (false);		// Nothing changed
    
 not_pruned_state_label:
    return (false);		// Nothing changed
}

// TODO: create a PimMre:: function that calls this one, and elimininate
// it as a global function.
void
pim_mre_join_timer_timeout(void *data_pointer)
{
    PimNbr *pim_nbr = NULL;
    uint16_t join_prune_period = PIM_JOIN_PRUNE_PERIOD_DEFAULT;
    PimMre *pim_mre = (PimMre *)data_pointer;
    const IPvX *my_rp_addr_ptr = NULL;
    
    if (pim_mre->is_sg())
	goto sg_entry_label;
    if (pim_mre->is_wc())
	goto wc_entry_label;
    if (pim_mre->is_rp())
	goto rp_entry_label;
    
    // TODO: XXX: implement the rest if needed
    // TODO: XXX: PAVPAVPAV: what about is_sg_rpt() ?
    return;
	
 rp_entry_label:
    // (*,*,RP) state
    if (! pim_mre->is_joined_state())
	return;		// Wrong state	TODO: trigger state deletion?
    
    // Joined state
    // Send Join(*,*,RP)
    pim_nbr = pim_mre->mrib_next_hop_rp();  
    if (pim_nbr == NULL) {
	if (! pim_mre->i_am_rp()) {
	    XLOG_WARNING("JoinDesired(*,*,RP) = true: "
			 "upstream neighbor for RP %s: not found",
			 cstring(*pim_mre->rp_addr_ptr()));
	}
    } else {
	bool new_group_bool = false; // Group together all (*,*,RP) entries
	pim_nbr->jp_entry_add(*pim_mre->rp_addr_ptr(), IPvX::MULTICAST_BASE(pim_mre->family()),
			      IPvX::ip_multicast_base_address_masklen(pim_mre->family()),
			      MRT_ENTRY_RP,
			      ACTION_JOIN,
			      pim_nbr->pim_vif().join_prune_holdtime().get(),
			      new_group_bool);
	join_prune_period = pim_nbr->pim_vif().join_prune_period().get();
    }
    // Restart the JoinTimer
    pim_mre->join_timer().start(join_prune_period, 0,
				pim_mre_join_timer_timeout, pim_mre);
    return;
    
 wc_entry_label:
    // (*,G) state
    if (! pim_mre->is_joined_state())
	return;		// Wrong state	TODO: trigger state deletion?
    
    // Joined state
    // Send Join(*,G)
    my_rp_addr_ptr = pim_mre->rp_addr_ptr();
    if (my_rp_addr_ptr == NULL) {
	XLOG_WARNING("JoinDesired(*,G) = true: "
		     "RP for group %s: not found",
		     cstring(pim_mre->group_addr()));
    } else {
	// We have RP
	pim_nbr = pim_mre->rpfp_nbr_wc();
	if (pim_nbr == NULL) {
	    if (! pim_mre->i_am_rp()) {
		XLOG_WARNING("JoinDesired(*,G) = true: "
			     "upstream neighbor for RP %s for group %s: not found",
			     cstring(*my_rp_addr_ptr),
			     cstring(pim_mre->group_addr()));
	    }
	} else {
	    bool new_group_bool = false; // Allow merging entries for same G
	    pim_nbr->jp_entry_add(*my_rp_addr_ptr, pim_mre->group_addr(),
				  IPvX::addr_bitlen(pim_mre->family()),
				  MRT_ENTRY_WC,
				  ACTION_JOIN,
				  pim_nbr->pim_vif().join_prune_holdtime().get(),
				  new_group_bool);
	    join_prune_period = pim_nbr->pim_vif().join_prune_period().get();
	}
    }
    // Restart the JoinTimer
    pim_mre->join_timer().start(join_prune_period, 0,
				pim_mre_join_timer_timeout, pim_mre);
    return;
    
 sg_entry_label:
    // (S,G) state
    if (! pim_mre->is_joined_state())
	return;		// Wrong state	TODO: trigger state deletion?
    
    // Joined state
    // Send Join(S,G)
    pim_nbr = pim_mre->rpfp_nbr_sg();
    if (pim_nbr == NULL) {
	if (! pim_mre->is_directly_connected_s()) {
	    XLOG_WARNING("JoinDesired(S,G) = true: "
			 "upstream neighbor for source %s and group %s: not found",
			 cstring(pim_mre->source_addr()),
			 cstring(pim_mre->group_addr()));
	}
    } else {
	bool new_group_bool = false;	// Allow merging entries for same group
	pim_nbr->jp_entry_add(pim_mre->source_addr(), pim_mre->group_addr(),
			      IPvX::addr_bitlen(pim_mre->family()),
			      MRT_ENTRY_SG,
			      ACTION_JOIN,
			      pim_nbr->pim_vif().join_prune_holdtime().get(),
			      new_group_bool);
	join_prune_period = pim_nbr->pim_vif().join_prune_period().get();
    }
    // Restart the JoinTimer
    pim_mre->join_timer().start(join_prune_period, 0,
				pim_mre_join_timer_timeout, pim_mre);
    return;
}

static void
pim_mre_override_timer_timeout(void *data_pointer)
{
    PimNbr *pim_nbr = NULL;
    PimNbr *pim_nbr_rpfp_nbr_wc = NULL;
    PimMre *pim_mre = (PimMre *)data_pointer;
    PimMre *pim_mre_wc = pim_mre->wc_entry();
    
    if (pim_mre->is_sg_rpt())
	goto sg_rpt_entry_label;
    
    // TODO: XXX: implement the rest if needed
    return;

 sg_rpt_entry_label:
    // (S,G,rpt) state
    if (! pim_mre->is_not_pruned_state())
	return;		// Wrong state	TODO: trigger state deletion?
    
    // NotPruned state
    // Send Join(S,G,rpt) to RPF'(S,G,rpt)
    pim_nbr = pim_mre->rpfp_nbr_sg_rpt();
    if (pim_mre_wc != NULL)
	pim_nbr_rpfp_nbr_wc = pim_mre_wc->rpfp_nbr_wc();
    if (pim_nbr == NULL) {
	XLOG_WARNING("Join(S,G,rpt) = true: "
		     "upstream RPT neighbor for RP %s for "
		     "source %s group %s: not found",
		     pim_mre->rp_addr_string().c_str(),
		     cstring(pim_mre->source_addr()),
		     cstring(pim_mre->group_addr()));
    } else if (pim_nbr != pim_nbr_rpfp_nbr_wc) {
	// RPF'(S,G,rpt) != RPF'(*,G)
	// Ignore. TODO: it is unclear what exactly the spec suggests
	return;
    } else {
	// RPF'(S,G,rpt) == RPF'(*,G)
	// Send Join(S,G,rpt) to RPF'(S,G,rpt)
	bool new_group_bool = false;	// Allow merging entries for same group
	pim_nbr->jp_entry_add(pim_mre->source_addr(), pim_mre->group_addr(),
			      IPvX::addr_bitlen(pim_mre->family()),
			      MRT_ENTRY_SG_RPT,
			      ACTION_JOIN,
			      pim_nbr->pim_vif().join_prune_holdtime().get(),
			      new_group_bool);
    }
    
    return;
}
