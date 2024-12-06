//
// Created by 20614 on 24-12-3.
//
#include "inc/init_task.h"
/* FreeRTOS头文件 */
#include "cmsis_os.h"

#include "wwd_network.h"
#include "wwd_management.h"
#include "wwd_wifi.h"
#include "wwd_debug.h"
#include "wwd_assert.h"
#include "wwd_platform_interface.h"
#include "RTOS/wwd_rtos_interface.h"
#include "wwd_buffer_interface.h"

#include "wifi_base_config.h"
#include "client.h"

/*****************************************************
 *               静态函数声明
 ******************************************************/

static void tcpip_init_done( void * arg );


void wifi_init(void)
{
    SemaphoreHandle_t lwip_done_sema;
    // WPRINT_APP_INFO( ( "\nPlatform " PLATFORM " initialised\n" ) );
    WPRINT_APP_INFO( ( "Started FreeRTOS" FreeRTOS_VERSION "\n" ) );
    WPRINT_APP_INFO( ( "Starting LwIP " LwIP_VERSION "\n" ) );

    /* 创建信号量以在LwIP完成初始化时发出信号 */
    lwip_done_sema = xSemaphoreCreateCounting( 1, 0 );
    if ( lwip_done_sema == NULL )
    {
        /*无法创建信号量 */
        WPRINT_APP_ERROR(("Could not create LwIP init semaphore"));
        return;
    }

    /*初始化LwIP，提供回调函数和回调信号量 */
    tcpip_init( tcpip_init_done, (void*) &lwip_done_sema );
    xSemaphoreTake( lwip_done_sema, portMAX_DELAY );
    vQueueDelete( lwip_done_sema );

    /*配置wifi lwip信息*/
    Config_WIFI_LwIP_Info();

    client_init();
}


/**
 *  LwIP初始化完成回调
 * @param arg : the handle for the semaphore to post (cast to a void pointer)
 */

static void tcpip_init_done( void * arg )
{
    SemaphoreHandle_t * lwip_done_sema = (SemaphoreHandle_t *) arg;
    xSemaphoreGive( *lwip_done_sema );
}
