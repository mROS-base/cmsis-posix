#include "cmsis_os.h"
#include "posix_os_ext_common_private.h"
#include "cmsis_posix_os_message_queue.h"
#include "cmsis_posix_os_thread_sync.h"
#include <string.h>

osMessageQueueId_t osMessageQueueNew(
    uint32_t 	msg_count,
    uint32_t 	msg_size,
    const osMessageQueueAttr_t* attr)
{
    uint32_t aligned_msg_size;
    PosixOsMessageQueueConfigType config;

    if (CurrentContextIsISR()) {
        return NULL;
    }
    else if (attr != NULL) {
        CMSIS_IMPL_ERROR("ERROR:%s %s() %d attr must be null\n", __FILE__, __FUNCTION__, __LINE__);
        return NULL;
    }
    else if (msg_count == 0) {
        CMSIS_IMPL_ERROR("ERROR:%s %s() %d msg_count should not be 0\n", __FILE__, __FUNCTION__, __LINE__);
        return NULL;
    }
    else if (msg_size == 0) {
        CMSIS_IMPL_ERROR("ERROR:%s %s() %d msg_size should not be 0\n", __FILE__, __FUNCTION__, __LINE__);
        return NULL;
    }
    aligned_msg_size = MESSAGE_QUEUE_ENTRY_ALIGNED_SIZE(msg_size);
    config.entry_size = aligned_msg_size;
    config.prealloc_num = msg_count;
    config.control_datap = NULL;
    config.entries_datap = NULL;

    return (osMessageQueueId_t*)PosixOsMessageQueueCreate(&config);
}

osStatus_t osMessageQueueDelete(osMessageQueueId_t mq_id)
{
    osStatus_t err = osOK;
    PosixOsMessageQueueType* qh = (PosixOsMessageQueueType*)mq_id;

    if (CurrentContextIsISR()) {
        return osErrorISR;
    }
    else if (qh == NULL) {
        CMSIS_IMPL_ERROR("ERROR:%s %s() %d mq_id is invalid value(%p)\n", __FILE__, __FUNCTION__, __LINE__, mq_id);
        return osErrorParameter;
    }
    PosixOsThreadSyncLock();
    if (!PosixOsMessageQueueIsValid(qh)) {
        err = osErrorParameter;
    }
    else if ((qh->used.count == 0) && (qh->getter_waiting.count == 0) && (qh->putter_waiting.count == 0)) {
        err = PosixOsMessageQueueDelete((PosixOsMessageQueueType*)mq_id);
    }
    else {
        err = osErrorResource;
    }
    PosixOsThreadSyncUnlock();
    return err;
}

osStatus_t osMessageQueueGet(
    osMessageQueueId_t mq_id,
    void* msg_ptr,
    uint8_t* msg_prio,
    uint32_t timeout)
{
    uint32_t arg_timeout = timeout;
    osStatus_t err = osErrorParameter;
    osStatus_t ercd;
    PosixOsMessageQueueType* qh = (PosixOsMessageQueueType*)mq_id;
    //printf("osMessageQueueGet:enter\n");
    if (CurrentContextIsISR() && (timeout != 0)) {
        return osErrorParameter;
    }
    else if (qh == NULL) {
        CMSIS_IMPL_ERROR("ERROR:%s %s() %d mq_id is invalid value(%p)\n", __FILE__, __FUNCTION__, __LINE__, mq_id);
        return osErrorParameter;
    }
    else if (msg_ptr == NULL) {
        CMSIS_IMPL_ERROR("ERROR:%s %s() %d msg_ptr is invalid value(%p)\n", __FILE__, __FUNCTION__, __LINE__, msg_ptr);
        return osErrorParameter;
    }
    else if (msg_prio != NULL) {
        CMSIS_IMPL_ERROR("ERROR:%s %s() %d msg_prio must be null\n", __FILE__, __FUNCTION__, __LINE__);
        return osErrorParameter;
    }
    if (timeout == 0) {
        err = osErrorResource;
    }
    else {
        if (timeout == osWaitForever) {
            arg_timeout = POSIX_OS_THREAD_SYNC_WAIT_FOREVER;
        }
        err = osErrorTimeout;
    }
    ercd = PosixOsMessageQueueGet(qh, msg_ptr, msg_prio, arg_timeout);
    if (ercd == osOK) {
        err = osOK;
    }
    else if (ercd == osErrorParameter) {
        err = osErrorParameter;
    }
    else {
        //CMSIS_IMPL_ERROR("ERROR:%s %s() %d ercd = %d\n", __FILE__, __FUNCTION__, __LINE__, ercd);
    }
    //printf("osMessageQueueGet:exit\n");
    return err;
}

uint32_t osMessageQueueGetCount(osMessageQueueId_t mq_id)
{
    uint32_t count = 0;
    PosixOsMessageQueueType* qh = (PosixOsMessageQueueType*)mq_id;
    if (qh == NULL) {
        CMSIS_IMPL_ERROR("ERROR:%s %s() %d mq_id is invalid value(%p)\n", __FILE__, __FUNCTION__, __LINE__, mq_id);
        return 0;
    }
    PosixOsThreadSyncLock();
    if (PosixOsMessageQueueIsValid(qh)) {
        count = qh->used.count;
    }
    else {
        CMSIS_IMPL_ERROR("ERROR:%s %s() %d invalid magicno=%d\n", __FILE__, __FUNCTION__, __LINE__, qh->magicno);
    }
    PosixOsThreadSyncUnlock();
    return count;
}

osStatus_t osMessageQueuePut(
    osMessageQueueId_t 	mq_id,
    const void* msg_ptr,
    uint8_t msg_prio,
    uint32_t timeout)
{
    uint32_t arg_timeout = timeout;
    osStatus_t err = osErrorParameter;
    osStatus_t ercd;
    PosixOsMessageQueueType* qh = (PosixOsMessageQueueType*)mq_id;

    //printf("osMessageQueuePut:enter\n");
    if (CurrentContextIsISR() && (timeout != 0)) {
        return osErrorParameter;
    }
    else if (qh == NULL) {
        CMSIS_IMPL_ERROR("ERROR:%s %s() %d mq_id is invalid value(%p)\n", __FILE__, __FUNCTION__, __LINE__, mq_id);
        return osErrorParameter;
    }
    else if (msg_ptr == NULL) {
        CMSIS_IMPL_ERROR("ERROR:%s %s() %d msg_ptr is invalid value(%p)\n", __FILE__, __FUNCTION__, __LINE__, msg_ptr);
        return osErrorParameter;
    }
    else if (msg_prio != 0) {
        CMSIS_IMPL_ERROR("ERROR:%s %s() %d msg_prio must be null\n", __FILE__, __FUNCTION__, __LINE__);
        return osErrorParameter;
    }
    if (timeout == 0) {
        err = osErrorResource;
    }
    else {
        if (timeout == osWaitForever) {
            arg_timeout = POSIX_OS_THREAD_SYNC_WAIT_FOREVER;
        }
        err = osErrorTimeout;
    }
    ercd = PosixOsMessageQueuePut(qh, msg_ptr, msg_prio, arg_timeout);
    if (ercd == osOK) {
        err = osOK;
    }
    else if (ercd == osErrorParameter) {
        err = osErrorParameter;
    }
    else {
        CMSIS_IMPL_ERROR("ERROR:%s %s() %d ercd = %d\n", __FILE__, __FUNCTION__, __LINE__, ercd);
    }
    //printf("osMessageQueuePut:exit\n");
    return err;
}
/*
 * Version 1
 */
osMessageQId osMessageCreate(
    const osMessageQDef_t* queue_def,
    osThreadId thread_id)
{
    if (queue_def == NULL) {
        CMSIS_IMPL_ERROR("ERROR:%s %s() %d queue_def should not be null\n", __FILE__, __FUNCTION__, __LINE__);
        return NULL;
    }
    return osMessageQueueNew(queue_def->queue_sz, sizeof(uint32_t), NULL);
}

osEvent osMessageGet(
    osMessageQId queue_id,
    uint32_t millisec)
{
    osStatus_t status;
    osEvent    event;
    uint32_t   message;

    status = osMessageQueueGet((osMessageQueueId_t)queue_id, &message, NULL, millisec);
    switch (status) {
    case osOK:
        event.status = osEventMessage;
        event.value.v = message;
        break;
    case osErrorParameter:
        event.status = osErrorParameter;
        break;
    case osErrorResource:
        event.status = osErrorResource;
        break;
    case osErrorTimeout:
        event.status = osEventTimeout;
        break;
    default:
        event.status = osErrorOS;
        break;
    }
    return event;
}

osStatus osMessagePut(
    osMessageQId queue_id,
    uint32_t info,
    uint32_t millisec)
{
    return osMessageQueuePut((osMessageQueueId_t)queue_id, &info, 0U, millisec);
}
