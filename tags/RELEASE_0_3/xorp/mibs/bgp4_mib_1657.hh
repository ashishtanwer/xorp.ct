/*
 * Note: this file originally auto-generated by mib2c using
 *        : mib2c.int_watch.conf,v 5.0 2002/04/20 07:30:13 hardaker Exp $
 */
#ifndef __MIBS_BGP4_MIB_1657_HH__
#define __MIBS_BGP4_MIB_1657_HH__ 

#include "config.h"
#include <queue>
#include "libxorp/xorp.h"
#include "libxorp/xlog.h"
#include "libxipc/xrl_std_router.hh"
#include "libxipc/xrl_args.hh"
#include "xrl/interfaces/bgp_xif.hh"
#include "bgp4_mib_xrl_target.hh"


/* dlopen functions */
extern "C" {
void            init_bgp4_mib_1657 (void);
void          deinit_bgp4_mib_1657 (void);
}




/**
 * @short Class that implements the BGP MIB tree 
 *
 */

class BgpMib : public XrlBgpV0p2Client {

public:
    static BgpMib& the_instance();
    static void explicit_destructor();
    const char * name()    { return _name.c_str(); };

private:
    BgpMib();
    XrlStdRouter _xrl_router;
    XrlBgpMibTarget _xrl_target;

    const string  _name;

    static BgpMib* _bgp_mib;
};

#endif    /* __MIBS_BGP4_MIB_1657_HH__ */                      
