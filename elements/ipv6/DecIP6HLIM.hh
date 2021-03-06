#ifndef __NBA_ELEMENT_IPv6_DECIP6HLIM_HH__
#define __NBA_ELEMENT_IPv6_DECIP6HLIM_HH__


#include <rte_config.h>
#include <rte_memory.h>
#include <rte_mbuf.h>
#include <rte_ether.h>

#include "../../lib/element.hh"
#include "../../lib/annotation.hh"
#include <vector>
#include <string>

#include <netinet/ip6.h>

namespace nba {

class DecIP6HLIM : public Element {
public:
    DecIP6HLIM(): Element()
    {
    }

    ~DecIP6HLIM()
    {
    }

    const char *class_name() const { return "DecIP6HLIM"; }
    const char *port_count() const { return "1/1"; }

    int initialize();
    int initialize_global() { return 0; };      // per-system configuration
    int initialize_per_node() { return 0; };    // per-node configuration
    int configure(comp_thread_context *ctx, std::vector<std::string> &args);

    int process(int input_port, Packet *pkt);
};

EXPORT_ELEMENT(DecIP6HLIM);

}

#endif

// vim: ts=8 sts=4 sw=4 et
