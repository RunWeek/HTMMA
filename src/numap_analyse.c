#include <stdlib.h>
#include <stdio.h>

#include "numap.h"

int is_served_by_local_cache1(union perf_mem_data_src data_src)
{
    if (data_src.mem_lvl & PERF_MEM_LVL_HIT)
    {
        if (data_src.mem_lvl & PERF_MEM_LVL_L1)
        {
            return 1;
        }
    }
    return 0;
}

int is_served_by_local_cache2(union perf_mem_data_src data_src)
{
    if (data_src.mem_lvl & PERF_MEM_LVL_HIT)
    {
        if (data_src.mem_lvl & PERF_MEM_LVL_L2)
        {
            return 1;
        }
    }
    return 0;
}

int is_served_by_local_cache3(union perf_mem_data_src data_src)
{
    if (data_src.mem_lvl & PERF_MEM_LVL_HIT)
    {
        if (data_src.mem_lvl & PERF_MEM_LVL_L3)
        {
            return 1;
        }
    }
    return 0;
}

int is_served_by_local_lfb(union perf_mem_data_src data_src)
{
    if (data_src.mem_lvl & PERF_MEM_LVL_HIT)
    {
        if (data_src.mem_lvl & PERF_MEM_LVL_LFB)
        {
            return 1;
        }
    }
    return 0;
}

int is_served_by_local_cache(union perf_mem_data_src data_src)
{
    if (data_src.mem_lvl & PERF_MEM_LVL_HIT)
    {
        if (data_src.mem_lvl & PERF_MEM_LVL_L1)
        {
            return 1;
        }
        if (data_src.mem_lvl & PERF_MEM_LVL_LFB)
        {
            return 1;
        }
        if (data_src.mem_lvl & PERF_MEM_LVL_L2)
        {
            return 1;
        }
        if (data_src.mem_lvl & PERF_MEM_LVL_L3)
        {
            return 1;
        }
    }
    return 0;
}

int is_served_by_local_memory(union perf_mem_data_src data_src)
{
    if (data_src.mem_lvl & PERF_MEM_LVL_HIT)
    {
        if (data_src.mem_lvl & PERF_MEM_LVL_LOC_RAM)
        {
            return 1;
        }
    }
    return 0;
}

int is_served_by_remote_cache_or_local_memory(union perf_mem_data_src data_src)
{
    if (data_src.mem_lvl & PERF_MEM_LVL_HIT && data_src.mem_lvl & PERF_MEM_LVL_REM_CCE1)
    {
        return 1;
    }
    return 0;
}

int is_served_by_remote_memory(union perf_mem_data_src data_src)
{
    if (data_src.mem_lvl & PERF_MEM_LVL_HIT)
    {
        if (data_src.mem_lvl & PERF_MEM_LVL_REM_RAM1)
        {
            return 1;
        }
        else if (data_src.mem_lvl & PERF_MEM_LVL_REM_RAM2)
        {
            return 1;
        }
    }
    return 0;
}

int is_served_by_local_NA_miss(union perf_mem_data_src data_src)
{
    if (data_src.mem_lvl & PERF_MEM_LVL_NA)
    {
        return 1;
    }
    if (data_src.mem_lvl & PERF_MEM_LVL_MISS && data_src.mem_lvl & PERF_MEM_LVL_L3)
    {
        return 1;
    }
    return 0;
}

char *get_data_src_opcode(union perf_mem_data_src data_src)
{
    char *res = concat("", "");
    char *old_res;
    if (data_src.mem_op & PERF_MEM_OP_NA)
    {
        old_res = res;
        res = concat(res, "NA");
        free(old_res);
    }
    if (data_src.mem_op & PERF_MEM_OP_LOAD)
    {
        old_res = res;
        res = concat(res, "Load");
        free(old_res);
    }
    if (data_src.mem_op & PERF_MEM_OP_STORE)
    {
        old_res = res;
        res = concat(res, "Store");
        free(old_res);
    }
    if (data_src.mem_op & PERF_MEM_OP_PFETCH)
    {
        old_res = res;
        res = concat(res, "Prefetch");
        free(old_res);
    }
    if (data_src.mem_op & PERF_MEM_OP_EXEC)
    {
        old_res = res;
        res = concat(res, "Exec code");
        free(old_res);
    }
    return res;
}

char *get_data_src_level(union perf_mem_data_src data_src)
{
    char *res = concat("", "");
    char *old_res;
    if (data_src.mem_lvl & PERF_MEM_LVL_NA)
    {
        old_res = res;
        res = concat(res, "NA");
        free(old_res);
    }
    if (data_src.mem_lvl & PERF_MEM_LVL_L1)
    {
        old_res = res;
        res = concat(res, "L1");
        free(old_res);
    }
    else if (data_src.mem_lvl & PERF_MEM_LVL_LFB)
    {
        old_res = res;
        res = concat(res, "LFB");
        free(old_res);
    }
    else if (data_src.mem_lvl & PERF_MEM_LVL_L2)
    {
        old_res = res;
        res = concat(res, "L2");
        free(old_res);
    }
    else if (data_src.mem_lvl & PERF_MEM_LVL_L3)
    {
        old_res = res;
        res = concat(res, "L3");
        free(old_res);
    }
    else if (data_src.mem_lvl & PERF_MEM_LVL_LOC_RAM)
    {
        old_res = res;
        res = concat(res, "Local_RAM");
        free(old_res);
    }
    else if (data_src.mem_lvl & PERF_MEM_LVL_REM_RAM1)
    {
        old_res = res;
        res = concat(res, "Remote_RAM_1_hop");
        free(old_res);
    }
    else if (data_src.mem_lvl & PERF_MEM_LVL_REM_RAM2)
    {
        old_res = res;
        res = concat(res, "Remote_RAM_2_hops");
        free(old_res);
    }
    else if (data_src.mem_lvl & PERF_MEM_LVL_REM_CCE1)
    {
        old_res = res;
        res = concat(res, "Remote_Cache_1_hop");
        free(old_res);
    }
    else if (data_src.mem_lvl & PERF_MEM_LVL_REM_CCE2)
    {
        old_res = res;
        res = concat(res, "Remote_Cache_2_hops");
        free(old_res);
    }
    else if (data_src.mem_lvl & PERF_MEM_LVL_IO)
    {
        old_res = res;
        res = concat(res, "I/O_Memory");
        free(old_res);
    }
    else if (data_src.mem_lvl & PERF_MEM_LVL_UNC)
    {
        old_res = res;
        res = concat(res, "Uncached_Memory");
        free(old_res);
    }
    if (data_src.mem_lvl & PERF_MEM_LVL_HIT)
    {
        old_res = res;
        res = concat(res, "_Hit");
        free(old_res);
    }
    else if (data_src.mem_lvl & PERF_MEM_LVL_MISS)
    {
        old_res = res;
        res = concat(res, "_Miss");
        free(old_res);
    }
    return res;
}

int get_index(uint32_t tid, struct numap_sampling_measure *measure)
{
    uint32_t thread;
    int index = 0;
    for (thread = 0; thread < measure->nb_threads; thread++)
    {
        if (measure->tids[index] == tid)
        {
            return index;
        }
        index++;
    }
    return -1;
}

/* This function analyzes a set of samples
 * @param samples : a buffer that contains samples
 * @return nb_samples : the number of samples that were in the buffer
 * @return found_samples :  the number of samples that were matched to a memory object
 */
static void __analyze_buffer(struct sample_list *samples,
                             int *nb_samples,
                             int *found_samples)
{
    unsigned ByteCnt = 0;
    unsigned tmpCnt = 0;
    char TmpData[128] = {0};

    if (samples->data_tail == samples->data_head)
        /* nothing to do */
        return;

    unsigned start_cpt = samples->data_tail;
    unsigned stop_cpt = samples->data_head;
    uintptr_t reset_cpt = samples->buffer_size;
    unsigned cur_cpt = start_cpt;

    if (stop_cpt < start_cpt)
    {
        /* the buffer is a ring buffer and we need to explore both parts of the "ring": */

        // ------------------------------------------------------
        // | second_block   |                  |first_block      |
        // -------------------------------------------------------
        //               stop_cpt            start_cpt         reset_cpt

        /* in order to make the while condition easier to understand, let's first analyze
         * the first block. When the end of the buffer is reached, we reset counters so that
         * the second block is analyzed
         */
        stop_cpt = reset_cpt;
    }

    /* browse the buffer and process each sample */
    while (cur_cpt < stop_cpt)
    {

        struct perf_event_header *event = (struct perf_event_header *)((uintptr_t)samples->buffer + cur_cpt);

        if (event->size == 0)
        {
            fprintf(stderr, "Error: invalid header size = 0. %p\n", samples);
            abort();
        }

        if (event->type == PERF_RECORD_SAMPLE)
        {
            // struct mem_sample *sample = (struct mem_sample *)((char *)(event) + sizeof(struct perf_event_header));
            struct sample *sample = (struct sample *)((char *)(event) + sizeof(struct perf_event_header));

            uint8_t frontier_buffer[event->size];
            if (cur_cpt + event->size > reset_cpt)
            {
                // we reached the end of the buffer. The event is split in two parts:
                // ------------------------------------------------------
                // | second_part    |                  | first_part      |
                // -------------------------------------------------------
                //                                   cur_cpt         reset_cpt
                size_t first_part_size = reset_cpt - cur_cpt;
                size_t second_part_size = event->size - first_part_size;

                // copy the event in a contiguous buffer
                memcpy(frontier_buffer, sample, first_part_size); // copy the first part
                memcpy(&frontier_buffer[first_part_size], samples->buffer, second_part_size);
                sample = (struct sample *)frontier_buffer;
            }
            // memset(TmpData, 0, 128);
            // fprintf(TmpData, "%04d tid=%d, addr=%" PRIx64 ", time=%ld\n", tmpCnt++, sample->tid, sample->addr, sample->time);
            // printf("%s", TmpData);

            // fprintf(fp, "tid=%d, addr=%" PRIx64 ", time=%ld\n", sample->tid, sample->addr, sample->time);
            printf("%04d tid=%d, addr=%" PRIx64 ", time=%ld\n", tmpCnt++, sample->tid, sample->addr, sample->time);

            // fprintf(TmpString, "tid=%d, virtual_addr=%" PRIx64 ", time=%ld\n", sample->tid, sample->addr, sample->time);
            // fprintf(fp, "123...\n");
            // fprintf(fp, "123\n");
            // printf("%04d tid=%d, addr=%" PRIx64 ", time=%ld\n", tmpCnt++, sample->tid, sample->addr, sample->time);

            (*nb_samples)++;
            // update_counters(global_counters, sample, access_type);
        }
    next_sample:
        /* go to the next sample */
        cur_cpt += event->size;
        ByteCnt += event->size;

        if (cur_cpt >= reset_cpt && reset_cpt != samples->data_head)
        {
            cur_cpt -= reset_cpt;
            stop_cpt = samples->data_head;
        }
    }

    printf("ByteCnt=%ld\n", ByteCnt);
}

int numap_sampling_print(struct numap_sampling_measure *measure, char print_samples)
{
    int thread;
    int tmpCnt = 0;
    static unsigned preDataHead = 0;

    for (thread = 0; thread < measure->nb_threads; thread++)
    {

        struct perf_event_mmap_page *metadata_page = measure->metadata_pages_per_tid[thread];
        uint64_t head = metadata_page->data_head;
        rmb();
        struct perf_event_header *header = (struct perf_event_header *)((char *)metadata_page + measure->page_size);
        uint64_t consumed = 0;
        int na_miss_count = 0;
        int cache1_count = 0;
        int cache2_count = 0;
        int cache3_count = 0;
        int lfb_count = 0;
        int memory_count = 0;
        int remote_memory_count = 0;
        int remote_cache_count = 0;
        int total_count = 0;
        int nb_samples = 0;
        int found_samples = 0;

        struct sample_list samples = {
            .next = NULL,
            .buffer = (struct perf_event_header *)((uint8_t *)metadata_page + metadata_page->data_offset),
            .data_tail = metadata_page->data_tail,
            .data_head = metadata_page->data_head % metadata_page->data_size,
            .buffer_size = metadata_page->data_size,
            .access_type = ACCESS_READ,
            .thread_rank = 0,
        };
        // __analyze_buffer(&samples, &nb_samples, &found_samples);

        printf("func=%s, data_tail=%ld, data_head=%ld\n", __func__, metadata_page->data_tail, metadata_page->data_head);
        metadata_page->data_tail = metadata_page->data_head;
        /* while (consumed < head)
         {
             if (header->size == 0)
             {
                 fprintf(stderr, "Error: invalid header size = 0\n");
                 return -1;
             }
             // if (header->type == PERF_RECORD_SAMPLE)
             // {
             //     struct sample *sample = (struct sample *)((char *)(header) + 8);

             //     // fprintf(fp, "virtuall_addr=%" PRIx64 ", timestamps=%ld\n", sample->addr, sample->time);
             //     printf("%04d tid=%d, addr=%" PRIx64 ", time=%ld\n", tmpCnt++, sample->tid, sample->addr, sample->time);
             // }
             consumed += header->size;
             metadata_page->data_tail = consumed;
             header = (struct perf_event_header *)((char *)header + header->size);
             rmb();
             if (header->type == PERF_RECORD_SAMPLE)
             {
                 struct sample *sample = (struct sample *)((char *)(header) + 8);

                 // fprintf(fp, "virtuall_addr=%" PRIx64 ", timestamps=%ld\n", sample->addr, sample->time);
                 printf("%04d tid=%d, addr=%" PRIx64 ", time=%ld, data_head=%ld, data_tail=%ld\n", tmpCnt++, sample->tid, sample->addr, sample->time,
                        metadata_page->data_head, metadata_page->data_tail);
             }
             // printf("data head=%ld, data_tail=%ld\n", metadata_page->data_head, metadata_page->data_tail);
         }*/
        // printf("consumed=%ld, data_head=%ld\n", consumed, metadata_page->data_head);
        printf("\n");
        // printf("head = %" PRIu64 " compared to max = %zu\n", head, measure->mmap_len);
        // printf("Thread %d: %-8d samples\n", thread, total_count);
        // printf("Thread %d: %-8d %-30s %0.3f%%\n", thread, cache1_count, "local cache 1", (100.0 * cache1_count / total_count));
        // printf("Thread %d: %-8d %-30s %0.3f%%\n", thread, cache2_count, "local cache 2", (100.0 * cache2_count / total_count));
        // printf("Thread %d: %-8d %-30s %0.3f%%\n", thread, cache3_count, "local cache 3", (100.0 * cache3_count / total_count));
        // printf("Thread %d: %-8d %-30s %0.3f%%\n", thread, lfb_count, "local cache LFB", (100.0 * lfb_count / total_count));
        // printf("Thread %d: %-8d %-30s %0.3f%%\n", thread, memory_count, "local memory", (100.0 * memory_count / total_count));
        // printf("Thread %d: %-8d %-30s %0.3f%%\n", thread, remote_cache_count, "remote cache or local memory", (100.0 * remote_cache_count / total_count));
        // printf("Thread %d: %-8d %-30s %0.3f%%\n", thread, remote_memory_count, "remote memory", (100.0 * remote_memory_count / total_count));
        // printf("Thread %d: %-8d %-30s %0.3f%%\n", thread, na_miss_count, "unknown l3 miss", (100.0 * na_miss_count / total_count));
        if (measure->nb_refresh > 0)
        {
            measure->total_samples += (total_count % measure->nb_refresh);
        }
    }
    // printf("\nTotal sample number : %d\n", measure->total_samples);

    return 0;
}

int numap_sampling_read_print(struct numap_sampling_measure *measure, char print_samples)
{
    return numap_sampling_print(measure, print_samples);
}

int numap_sampling_write_print(struct numap_sampling_measure *measure, char print_samples)
{
    return numap_sampling_print(measure, print_samples);
}
