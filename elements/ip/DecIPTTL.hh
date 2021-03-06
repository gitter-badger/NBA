#ifndef __NBA_ELEMENT_IP_DECIPTTL_HH__
#define __NBA_ELEMENT_IP_DECIPTTL_HH__


#include <rte_config.h>
#include <rte_memory.h>
#include <rte_mbuf.h>
#include <rte_ether.h>

#include "../../lib/element.hh"
#include "../../lib/annotation.hh"
#include <vector>
#include <string>

#include <netinet/ip.h>

namespace nba {

class DecIPTTL : public Element {
public:
    DecIPTTL(): Element()
    {
    }

    ~DecIPTTL()
    {
    }

    const char *class_name() const { return "DecIPTTL"; }
    const char *port_count() const { return "1/1"; }

    int initialize();
    int initialize_global() { return 0; };      // per-system configuration
    int initialize_per_node() { return 0; };    // per-node configuration
    int configure(comp_thread_context *ctx, std::vector<std::string> &args);

    int process(int input_port, Packet *pkt);
};

EXPORT_ELEMENT(DecIPTTL);

}

#endif

// vim: ts=8 sts=4 sw=4 et
