#ifndef __FREERTOS_TASKS_C_ADDITIONS_H__
#define __FREERTOS_TASKS_C_ADDITIONS_H__

uint32_t uxTaskGetStackSize( TaskHandle_t xTask )
{
    uint32_t uxReturn;
    TCB_t *pxTCB;

    if ( NULL != xTask )
    {
        pxTCB = ( TCB_t * ) xTask;
        uxReturn = ( pxTCB->pxEndOfStack - pxTCB->pxStack ) * sizeof(StackType_t);
    }
    else
    {
        uxReturn = 0;
    }

    return uxReturn;
}

#endif /* __FREERTOS_TASKS_C_ADDITIONS_H__ */

