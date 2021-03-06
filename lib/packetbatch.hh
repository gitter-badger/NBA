#ifndef __NBA_PACKETBATCH_HH__
#define __NBA_PACKETBATCH_HH__
#include "types.hh"
#include <cstdint>
#include <cstring>
#include <vector>
#include <rte_config.h>
#include <rte_memory.h>
#include <rte_mempool.h>
#include <rte_mbuf.h>

#include "config.hh"
#include "annotation.hh"
#include "datablock.hh"

namespace nba {

enum BatchDisposition {
    KEPT_BY_ELEMENT = -1,
    CONTINUE_TO_PROCESS = 0,
};

class PacketBatch {
public:
    PacketBatch()
        : count(0), datablock_states(nullptr), recv_timestamp(0),
          generation(0), batch_id(0), element(nullptr), input_port(0), has_results(false),
          delay_start(0), compute_time(0)
    {
        #ifdef DEBUG
        memset(&results[0], 0xdd, sizeof(int) * NBA_MAX_COMP_BATCH_SIZE);
        memset(&excluded[0], 0xcc, sizeof(bool) * NBA_MAX_COMP_BATCH_SIZE);
        memset(&packets[0], 0xbb, sizeof(struct rte_mbuf*) * NBA_MAX_COMP_BATCH_SIZE);
        #endif
    }

    virtual ~PacketBatch()
    {
    }

    unsigned count;
    struct datablock_tracker *datablock_states;
    uint64_t recv_timestamp;
    uint64_t generation;
    uint64_t batch_id;
    Element* element;
    int input_port;
    bool has_results;
    uint64_t delay_start;
    uint64_t delay_time;
    double compute_time;

    struct annotation_set banno __rte_cache_aligned;  /** Batch-level annotations. */
    bool excluded[NBA_MAX_COMP_BATCH_SIZE] __rte_cache_aligned;
    struct rte_mbuf *packets[NBA_MAX_COMP_BATCH_SIZE] __rte_cache_aligned;
    int results[NBA_MAX_COMP_BATCH_SIZE] __rte_cache_aligned;
};

}

#endif

// vim: ts=8 sts=4 sw=4 et
