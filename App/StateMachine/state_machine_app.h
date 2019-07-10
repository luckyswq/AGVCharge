#ifndef __STATE_MACHINE_APP_H
#define __STATE_MACHINE_APP_H
#include "app.h"
#include "bsp.h"
#include "bsp_usart.h"
#include "bsp_timer.h"
#include "task.h"
#include <stdlib.h>

typedef struct
{
    uint32_t status1;
    uint32_t status2;
    uint32_t status3;
} StatusManager_TypeDef;

extern StatusManager_TypeDef StatusManager;


#define State0         	0
#define State1         	1
#define State2         	2
#define State3         	3
#define Status_Init		0

extern uint32_t g_status;

void StatusManager_Init(void);


#endif


