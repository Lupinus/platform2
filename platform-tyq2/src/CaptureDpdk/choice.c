#include "choice.h"
#include "tools.h"
unsigned get_port_speed(uint8_t port_id)
{
    struct rte_eth_link link;
    rte_eth_link_get(port_id, &link);
    return link.link_speed;
}

/*Different number of rings are allocated on different network cards*/
int get_port_ringnum(void *handle)
{
    struct dpdk_context_t *ctx = (struct dpdk_context_t *)handle;
    if (ctx->port_num > ctx->ring_num)
        rte_exit(EXIT_FAILURE, "Error with initialization about ring_num smaller than port_num.\n");

    port_sort *sort;
    int portid = 0, weight = 0, speed = 0;
    double weight_sum = 0;
    for (portid = 0; portid != ctx->port_num; ++portid)
    {
        uint8_t t = ctx->port_list[portid];
        speed = get_port_speed(t);
        weight = find_weight(speed);
        if (weight == -1)
            rte_exit(EXIT_FAILURE, "Error with initialization about get the card weight.\n");

        weight_sum += weight;

        if (portid == 0)
        {
            sort = creatNode(speed, t);
        }
        else
        {
            addNode(sort, speed, t);
        }
    }

    port_sort *cur;
    if (weight > ctx->ring_num)
    {
        int i = 0;
        cur = sort;
        for (i = 0; i < ctx->ring_num; i++)
        {
            ctx->port_ringnum[cur->port_number]++;
            cur = cur->next;
            if (cur == NULL)
                cur = sort;
        }
    }
    else
    {
        for (portid = 0; portid != ctx->port_num; ++portid)
        {
            uint8_t t = ctx->port_list[portid];
            speed = get_port_speed(t);
            weight = find_weight(speed);
            ctx->port_ringnum[t] = (weight / weight_sum) * ctx->ring_num;
        }
        if (ctx->port_num != 1)
        {
            int mod = (int)ctx->ring_num % (int)weight_sum;
            cur = sort;
            while (mod != 0)
            {
                ctx->port_ringnum[cur->port_number]++;
                cur = cur->next;
                if (cur == NULL)
                    cur = sort;
                mod--;
            }
        }
    }
    return 0;
}
/*Create different numbers of threads according to the number of rings*/
int bind_core_length(void *handle)
{
    struct dpdk_context_t *ctx = (struct dpdk_context_t *)handle;
    int port_num = rte_eth_dev_count();
    int length = 0;
    unsigned thread_mode = ctx->thread_mode;
    int ring_num = ctx->ring_num;
    if (thread_mode == 1)
    {
        length = 2 * port_num;
    }
    else if (thread_mode == 2)
    {
        length = ring_num + port_num;
    }
    else
    {
        rte_exit(EXIT_FAILURE, "Error with initialization abount the bind_core.\n");
    }

    if (length > MAX_CORE_SIZE)
        rte_exit(EXIT_FAILURE, "Error with initialization abount Exceeding MAX_CORE_SIZE.\n");

    return length;
}

/*Bind the corresponding network card number to different cores*/
int get_core_portnum(void *handle)
{
    struct dpdk_context_t *ctx = (struct dpdk_context_t *)handle;
    unsigned core_id = 0;
    uint16_t start = ctx->start, end = ctx->end;
    uint16_t port_id = 0, portnum = ctx->nb_ports;
    if (ctx->thread_mode == 1)
    {
        uint16_t mid = (start + end + 1) / 2;
        port_id = 0;
        for (core_id = start; core_id < mid; core_id++)
        {
            ctx->core_portnum[core_id] = port_id;
            port_id++;
        }
        port_id = 0;
        for (core_id = mid; core_id <= end; core_id++)
        {
            ctx->core_portnum[core_id] = port_id;
            port_id++;
        }
    }
    else if (ctx->thread_mode == 2)
    {
        port_id = 0;
        for (core_id = start; core_id < start + portnum; core_id++)
        {
            ctx->core_portnum[core_id] = port_id;
            port_id++;
        }
        port_id = 0;
        for (core_id = start + portnum; port_id < ctx->nb_ports; port_id++)
        {
            unsigned ringnum = ctx->port_ringnum[port_id];
            while (ringnum != 0)
            {
                ctx->core_portnum[core_id] = port_id;
                ringnum--;
                core_id++;
            }
        }
    }

    return 0;
}

/*Bind the corresponding network card queue number to different cores*/
int get_core_queuenum(void *handle)
{
    struct dpdk_context_t *ctx = (struct dpdk_context_t *)handle;
    int core_id = 0, port = 0, queue = 0;
    uint16_t start = ctx->start, end = ctx->end;
    core_id = start;
    for (port = 0; port < ctx->port_num; port++)
    {
        for (queue = 0; queue < ctx->port_queuenum[port]; queue++)
        {
            ctx->core_queuenum[core_id] = queue;
            core_id++;
        }
    }

    return 0;
}

/*Get the number of queues corresponding to the network card*/
int get_port_queuenum(void *handle)
{
    struct dpdk_context_t *ctx = (struct dpdk_context_t *)handle;
    uint16_t port_id;
    if (ctx->queue_mode == 1)
    {
        for (port_id = 0; port_id < ctx->port_num; port_id++)
        {
            uint8_t t = ctx->port_list[port_id];
            uint16_t speed = get_port_speed(t);
            ctx->port_queuenum[t] = find_queue(speed);
            if (ctx->port_queuenum[t] == -1)
            {
                rte_exit(EXIT_FAILURE, "Error with distribute queues for port.\n");
            }
        }
    }
    else if (ctx->queue_mode == 2)
    {
        for (port_id = 0; port_id < ctx->port_num; port_id++)
        {
            uint8_t t = ctx->port_list[port_id];
            ctx->port_queuenum[t] = ctx->port_ringnum[t];
        }
    }
    return 0;
}

/*Get the network card number corresponding to the second layer processing thread*/
int get_thread_portinfo(void *handle)
{
    struct dpdk_context_t *ctx = (struct dpdk_context_t *)handle;
    int port_id = 0, ring_id = 0, thread_id = 0;
    for (port_id = 0; port_id < ctx->port_num; port_id++)
    {
        for (ring_id = 0; ring_id < ctx->port_ringnum[port_id]; ring_id++)
        {
            ctx->thread_portnum[thread_id] = port_id;
            ctx->thread_ringnum[thread_id] = ring_id;
            thread_id++;
        }
    }

    return 0;
}

int choice_init(void *handle)
{
    struct dpdk_context_t *ctx = (struct dpdk_context_t *)handle;

    // read json
    char path[300];
    getCurDllPath(path);

    strcat(path, "../../../conf/port_infomation.txt");
    dofile(path);

    /*Different number of rings are allocated on different network cards*/
    int res = get_port_ringnum(ctx);
    if (res != 0)
    {
        rte_exit(EXIT_FAILURE, "Error with initialization about the method get_port_ringnum.\n");
    }

    /*Get the number of queues corresponding to the network card*/
    res = get_port_queuenum(ctx);
    if (res != 0)
    {
        rte_exit(EXIT_FAILURE, "Error with initialization abount the method get_port_queuenum.\n");
    }

    /*Get the network card number corresponding to the second layer processing thread*/
    res = get_thread_portinfo(ctx);
    if (res != 0)
    {
        rte_exit(EXIT_FAILURE, "Error with initialization abount the method get_thread_portnum.\n");
    }

    /*the range of bounding cpu core*/
    ctx->end = ctx->start + bind_core_length(ctx) - 1;

    /*Bind the corresponding network card infomation to different cores*/
    uint16_t id = 0;
    for (id = ctx->start; id <= ctx->end; id++)
    {
        ctx->core_portnum[id] = 0;
        ctx->core_queuenum[id] = 0;
    }
    res = get_core_portnum(ctx);
    if (res != 0)
    {
        rte_exit(EXIT_FAILURE, "Error with initialization about the method get_core_portnum.\n");
    }

    res = get_core_queuenum(ctx);
    if (res != 0)
    {
        rte_exit(EXIT_FAILURE, "Error with initialization about the method get_core_queuenum.\n");
    }
    return 0;
}
