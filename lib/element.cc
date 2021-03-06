#include "element.hh"
#include "elementgraph.hh"
#include "config.hh"
#include "packet.hh"
#include "packetbatch.hh"

#include <rte_config.h>
#include <rte_common.h>
#include <rte_memory.h>
#include <rte_mempool.h>
#include <rte_mbuf.h>
#include <rte_prefetch.h>
#include <rte_branch_prediction.h>
#include <rte_cycles.h>

#include <string>
#include <vector>
#include <cassert>
#include <cstdint>
#include <climits>

using namespace std;
using namespace nba;

Element::Element() : next_elems(), next_connected_inputs()
{
    num_min_inputs = num_max_inputs = 0;
    num_min_outputs = num_max_outputs = 0;
    memset(branch_count,0,sizeof(uint64_t)*ElementGraph::num_max_outputs);
}

Element::~Element()
{
}

int Element::_process_batch(int input_port, PacketBatch *batch)
{
    for (unsigned p = 0; p < batch->count; p++) {
        if (likely(!batch->excluded[p])) {
            batch->results[p] = this->process(input_port, Packet::from_base(batch->packets[p]));
        }
    }
    batch->has_results = true;
    return 0; // this value will be ignored.
}

int PerBatchElement::_process_batch(int input_port, PacketBatch *batch)
{
    int ret = this->process_batch(input_port, batch);
    batch->has_results = true;
    return ret;
}

void Element::update_port_count()
{
    size_t i;

    string *port_str = new string(port_count());
    size_t delim_idx = port_str->find("/");
    assert(delim_idx != string::npos);

    string input_spec = port_str->substr(0, delim_idx);
    string output_spec = port_str->substr(delim_idx + 1, port_str->length() - delim_idx);

    size_t range_delim_idx = input_spec.find("-");
    if (range_delim_idx == string::npos) {
        num_min_inputs = num_max_inputs = atoi(input_spec.c_str());
        assert(num_min_inputs >= 0);
    } else {
        string range_left = input_spec.substr(0, range_delim_idx);
        string range_right = input_spec.substr(range_delim_idx, input_spec.length() - range_delim_idx);
        num_min_inputs = atoi(range_left.c_str());
        num_max_inputs = atoi(range_right.c_str());
    }

    range_delim_idx = output_spec.find("-");
    if (range_delim_idx == string::npos) {
        if (output_spec == "*") {
            num_min_outputs = 0;
            num_max_outputs = -1;
        } else {
            num_min_outputs = num_max_outputs = atoi(output_spec.c_str());
        }
    } else {
        string range_left = output_spec.substr(0, range_delim_idx);
        string range_right = output_spec.substr(range_delim_idx, output_spec.length() - range_delim_idx);
        num_min_outputs = atoi(range_left.c_str());
        num_max_outputs = atoi(range_right.c_str());
    }
    if (num_max_outputs > NBA_MAX_ELEM_NEXTS) {
        rte_panic("Element::update_port_count(): Too many possible output ports (max: %d)\n", NBA_MAX_ELEM_NEXTS);
    }

    delete port_str;
}

int Element::initialize_global() {
    return 0;
}

int Element::initialize_per_node() {
    return 0;
}

int Element::initialize() {
    return 0;
}

int Element::configure(comp_thread_context *ctx, vector<string> &args) {
    this->ctx = ctx;
    return 0;
}

void OffloadableElement::dummy_compute_handler(ComputeContext *ctx,
                                               struct resource_param *res)
{
    return;
}
// vim: ts=8 sts=4 sw=4 et
