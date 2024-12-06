# FreeRTOS

xTaskCreate 创建的 缓存 是从后向前排序的,高地址向前增长， 如果创建的内部的堆栈大小是 100 ，创建内部 局部变量 Buffer\[1000\] 那么就会挤压前面的 针头 的内存；

Task1 的  是  \[header, TCB1(句柄)， task\_header，task\_connet(4 \*100),    \]

### FREERTOS

#### 六、任务状态

使用 vTaskDelay 可以交出控制权， 再切换任务的过程 会发生调度

####  七、同步互斥于通信

全局变量 不是一个 互斥 好的方法

#### 八、队列

1.  先进先出
    
2.  缓冲区 分为 头部 和 尾部， 也可以直接 放在头部， 是环形结构所以不会 效率比较高； 包含链表
    

4.  创建队列 
    
    1.   先先创建 环形结构体
        
    2.  环形结构体中包含 环形缓冲区的 指针 pcHead 指向一个 buffer；buffer 的大小根据传入的参数
        
    3.  xQueueCreate() 传入的参数是  每一项 item 的大小 item\_size 和 队列的长度 ；
        
    4.  pcHead 和  pcWriteTo 这两个指针都会指向 队列的头部
        
    5.  进行写操作
        
        1.  写指针 再写完会 跳回到队列的 头部
            
    6.  读操作
        
        1.  指定读 的指针 和存入的 buffer 大小
            
        2.  如果队列是 空 那么无法读取数据，可以让数据 进入阻塞状态， 会一直阻塞到 有数据写 读；等待就是  把自己放到队列中去， 等待其他函数 调 这个 item， 最先唤醒 的是  最高优先级的 任务， 也会是 等待时间最长的任务
            
        3.  读的指针是 pcReadFrom， 上一次读的位置， 每一读 是 把指针指向 item 头部
            
        4.  xQueueSendToFront 写入到队列的头部， 队列的头部是 pcReadFrom 指向的位置，所以  函数写如的数据 会被 最先读出来，
            

##### 定时器 

FreeRTOS 里  会有 固定的 时钟，每隔一段时间  tick 就加一，当 ticks  > t + 定时器间隔时间，就触发，对于周期性 的定时器 就会 周期性触发，再过同样的时间 又被触发一次，创建定时 的回调函数 会被调用

###### linux 定时器

1.  一般情况下，  tick 中断中 去调用 定时器的函数，去调用创建 定时器的 回调函数，
    
2.  在  linux 中 在 tick 中断 会调用 定时器的函数
    

###### FreeRTOS 定时器

1.  但是在 FreeRTOS 并不是在中断中去调用 定时器 函数，因为如果 中断中如果 某个任务 的时间 特别长会导致 其他 高优先级的任务 无法执行，
    
2.   FreeRTOS 对时效性要求很高；所以 FreeRTOS 是在某个任务中执行定时器
    
3.  执行过程： 
    
    1.  定一个 中断，在每发生一次的中断里看一下有没有 超时 的定时器，如果有那么 会唤醒这个任务
        
    2.  对于已经超时的定时器 会执行它的 回调函数，
        
    3.  这个任务 被称为 **守护任务**
        
    4.  当 tick 中断 把 守护任务 唤醒 的时候，如果 优先级比较高，就可以马上执行定时器的 回调函数，优先级 比较低 会被更高优先级的任务抢占，定时器的函数 并不会及时执行
        
    5.  使用定时的时候必须 定义 定时任务的优先级，还要定义队列长度，栈 的深度
        
4.  定时器调用实质
    
    1.  用户调用 定时器 函数的时候，他的实质就是把 某些命令发送到 定时器命令队列里；把数据发到这个队列，就会把 对应的任务 唤醒，去 队列中取出 这些命令，看看具体要做哪些操作，是 启动定时器还是删除定时器，还是修改周期，
        
    2.  如果定时器队列满了就无法写入了，就要等待 一会 所以 函数中会包含 ticksToWait
        
5.  定时器的超时函数，是在 守护任务里 执行的， 这个守护任务可以管理多个定时器。
    
6.  别的任务可以启动 停止 复位它，改变周期
    
7.  定时器内可以 调用到阻塞函数，但是 不建议；
    
8.   定时器回调函数 应该尽快执行完成
    

定时器状态

1.  两个状态 一个 Dormant 休眠状态、running 运行状态
    
2.  对于周期性的定时器 一旦被调用会一直处于 运行状态， 会继续等待下一个超时时间，不会停止一直执行
    
3.  对于一次性的定时器会 执行完一次之后，就不再执行 ，会从运行状态变为休眠状态
    
4.  如果其他的任务 停止定时器也会导致 从运行状态变为休眠状态；
    

##### 通知任务

句柄：

是指针

##### 中断

屏蔽中断是屏蔽所有调用 systemcall 的中断，从 191 到 255

task1 使用 屏蔽中断 屏蔽所有中断，

中断 B 可以使用 屏蔽中断 isr 屏蔽其他中断，这样，就不会有更高优先级的中断来打断当前中断了

##### 调试

1.  调试手段
    
    1.  打印 printf
        
    2.  断言 configASSERT
        

```c
// x 必须为 真 ，如果为 假 那么推出程序
void configASSERT(x){
  if(x){
    while(1);
  }

  // 如果为false 那么打印其 文件 函数 第几行
  if(!x){
    printf("%s %s %d \r\n", __FILE__, __FUNCTION__, __LINE__);
  }
}




应用分析
//在 队列操作中
//使用  configASSERT( pxQueue ), 判定传入的 xQueue 句柄是否为空 

configASSERT( pxQueue );


中断操作中

configASSERT(ucCurrentPriorty >= ucMaxSysCallPrority); // 判断 中断优先级 是否在  范围只能 ucCurrentPriorty当前优先级，ucMaxSysCallPrority系统最大优先级



```

3.  Trace （跟踪）
    
    1.  FreeRTOS 中定义了很多 trace 开头的宏，这些宏放到了 关键的位置，一般都是空宏 ，不会影响到吗，不影响程序大小，不影响运行时间，如果要调试某些功能，可以修改宏，修改标记的变量打印信息等
        

|  **trace宏**  |  **描述**  |  备注  |
| --- | --- | --- |
|  traceTASK\_INCREMENT\_TICK(xTickCount)  |  当tick计数自增之前此宏函数被调用。参数xTickCount当前的Tick值，它还没有增加。  |   |
|  traceTASK\_SWITCHED\_OUT()  |  vTaskSwitchContext中，把当前任务切换出去之前调用此宏函数。  |   |
|  traceTASK\_SWITCHED\_IN()  |  vTaskSwitchContext中，新的任务已经被切换进来了，就调用此函数。  |   |
|  traceBLOCKING\_ON\_QUEUE\_RECEIVE(pxQueue)  |  当正在执行的当前任务因为试图去读取一个空的队列、信号或者互斥量而进入阻塞状态时，此函数会被立即调用。参数pxQueue保存的是试图读取的目标队列、信号或者互斥量的句柄，传递给此宏函数。  |   |
|  traceBLOCKING\_ON\_QUEUE\_SEND(pxQueue)  |  当正在执行的当前任务因为试图往一个已经写满的队列或者信号或者互斥量而进入了阻塞状态时，此函数会被立即调用。参数pxQueue保存的是试图写入的目标队列、信号或者互斥量的句柄，传递给此宏函数。  |   |
|  traceQUEUE\_SEND(pxQueue)  |  当一个队列或者信号发送成功时，此宏函数会在内核函数xQueueSend(),xQueueSendToFront(),xQueueSendToBack(),以及所有的信号give函数中被调用，参数pxQueue是要发送的目标队列或信号的句柄，传递给此宏函数。  |   |
|  traceQUEUE\_SEND\_FAILED(pxQueue)  |  当一个队列或者信号发送失败时，此宏函数会在内核函数xQueueSend(),xQueueSendToFront(),xQueueSendToBack(),以及所有的信号give函数中被调用，参数pxQueue是要发送的目标队列或信号的句柄，传递给此宏函数。  |   |
|  traceQUEUE\_RECEIVE(pxQueue)  |  当读取一个队列或者接收信号成功时，此宏函数会在内核函数xQueueReceive()以及所有的信号take函数中被调用，参数pxQueue是要接收的目标队列或信号的句柄，传递给此宏函数。  |   |
|  traceQUEUE\_RECEIVE\_FAILED(pxQueue)  |  当读取一个队列或者接收信号失败时，此宏函数会在内核函数xQueueReceive()以及所有的信号take函数中被调用，参数pxQueue是要接收的目标队列或信号的句柄，传递给此宏函数。  |   |
|  traceQUEUE\_SEND\_FROM\_ISR(pxQueue)  |  当在中断中发送一个队列成功时，此函数会在xQueueSendFromISR()中被调用。参数pxQueue是要发送的目标队列的句柄。  |   |
|  traceQUEUE\_SEND\_FROM\_ISR\_FAILED(pxQueue)  |  当在中断中发送一个队列失败时，此函数会在xQueueSendFromISR()中被调用。参数pxQueue是要发送的目标队列的句柄。  |   |
|  traceQUEUE\_RECEIVE\_FROM\_ISR(pxQueue)  |  当在中断中读取一个队列成功时，此函数会在xQueueReceiveFromISR()中被调用。参数pxQueue是要发送的目标队列的句柄。  |   |
|  traceQUEUE\_RECEIVE\_FROM\_ISR\_FAILED(pxQueue)  |  当在中断中读取一个队列失败时，此函数会在xQueueReceiveFromISR()中被调用。参数pxQueue是要发送的目标队列的句柄。  |   |
|  traceTASK\_DELAY\_UNTIL()  |  当一个任务因为调用了vTaskDelayUntil()进入了阻塞状态的前一刻此宏函数会在vTaskDelayUntil()中被立即调用。  |   |
|  traceTASK\_DELAY()  |  当一个任务因为调用了vTaskDelay()进入了阻塞状态的前一刻此宏函数会在vTaskDelay中被立即调用。  |   |

4.  Hook 函数（回调函数 callBack）
    
    1.  Malloc Hook 函数
        
        1.  内粗越界一般是在 堆使用的 过程中， 堆， 就是使用 malloc 得到的内存
            
        2.  并没有好的方法检测内存越界，，但是可以提供一个回调函数
            
            1.  使用 pvPortMalloc 失败时，如果在 如果在FreeRTOSConfig.h里配置`configUSE_MALLOC_FAILED_HOOK`为1，会调用：void vApplicationMallocFailedHook( void );  不常用
                

```c
#if ( configUSE_MALLOC_FAILED_HOOK == 1 )
{
    if( pvReturn == NULL ) // 创建不成功的时候  
    {
        vApplicationMallocFailedHook();
    }
    else
    {
        mtCOVERAGE_TEST_MARKER();
    }
}
```

2.  栈溢出 Hook 函数
    
    1.  在切换任务函数 vTaskSwitchContext 调用的时候 ，会 执行 taskCHECK\_FOR\_STACK\_OVERFLOW 来检测栈是否 溢出，如果 溢出 会调用 
        

```c

void vApplicationStackOverflowHook( TaskHandle_t xTask, char * pcTaskName );



// taskCHECK_FOR_STACK_OVERFLOW
taskCHECK_FOR_STACK_OVERFLOW() 是一个 宏 会根据 用户的定义来具体实现，现在看来是 可以配置两种 方法， 是否可以自定义 看之后再深入的理解
```

2.  判断 栈溢出的方法
    
    1.  栈溢出是 
        
    2.  在创建 vTask 任务的时候 会 创建 TCB 结构体 tskTaskControlBlock， TCB 结构提会包含栈的信息
        

```c
typedef struct tskTaskControlBlock       /* The old naming convention is used to prevent breaking kernel aware debuggers. */
{
    volatile StackType_t * pxTopOfStack; 这里是栈的顶部 /**< Points to the location of the last item placed on the tasks stack.  THIS MUST BE THE FIRST MEMBER OF THE TCB STRUCT. */       // 任务堆栈 栈顶

    ListItem_t xStateListItem;                  /**< The list that the state list item of a task is reference from denotes the state of that task (Ready, Blocked, Suspended ). */      // 状态列表项
    ListItem_t xEventListItem;                  /**< Used to reference a task from an event list. */            // 事件列表项
    UBaseType_t uxPriority;                     /**< The priority of the task.  0 is the lowest priority. */    // 任务优先级
    StackType_t * pxStack;            这里是栈的底部          /**< Points to the start of the stack. */                       // 任务堆栈起始地址

    char pcTaskName[ configMAX_TASK_NAME_LEN ]; /**< Descriptive name given to the task when created.  Facilitates debugging only. */   // 任务名字
}
```

3.  方法一：执行程序的时候会陆续往栈里 放数据，  pxTopOfStack 会从高位 变为低位，如果 pxTopOfStack 小于等于 pxStack，那么判定是 栈溢出
    
    1.  但是再实际执行过程中 pxTopOfStack 并不好记录，比如说有个任务函数 A，任务函数中有 for 循环 里 分配巨大的 charBuffer，那么在运行过程中栈 就会自动 划分出 100 字节，但是再运行 任务函数 A 的过程中 并没有 来 维护pxTopOfStack，所以这个值并不能真是的反应 栈的使用情况
        
    2.  只有在 任务 A 被切换出去，被调度出去了，在 **中断函数** 里面 他会保存现场 ，这时候 才会把 pxTopOfStack 更新 到 TCB 结构体里， 在平时运行的过程中 这个值是不更新的 
        
    3.  ++**任务调度 函数 vTaskSwitchContext**++
        
    4.  ++把++  #define configCHECK\_FOR\_STACK\_OVERFLOW         0      这个宏 设置成1 那么是方法一， 大于0时启用堆栈溢出检测功能，如果使用此功能用户必须提供一个栈移出钩子函数每如果使用的话，可以是 1或者 2 ，因为由两种栈溢出检测方法（一般调试阶段会使用）
        

```c
configCHECK_FOR_STACK_OVERFLOW 为 1 时 使用方法1； 

#if( ( configCHECK_FOR_STACK_OVERFLOW == 1 ) && ( portSTACK_GROWTH < 0 ) )

	/* Only the current stack state is to be checked. */
	#define taskCHECK_FOR_STACK_OVERFLOW()																\
	{																									\
		/* Is the currently saved stack pointer within the stack limit? */								\
		if( pxCurrentTCB->pxTopOfStack <= pxCurrentTCB->pxStack )	// 这里判定是否 栈溢出									\
		{																								\
			vApplicationStackOverflowHook( ( TaskHandle_t ) pxCurrentTCB, pxCurrentTCB->pcTaskName );	\
		}																								\
	}

#endif
```

4.  方法二 ,当 configCHECK\_FOR\_STACK\_OVERFLOW 为 2 时 使用第二种方法
    

```c

#if( ( configCHECK_FOR_STACK_OVERFLOW > 1 ) && ( portSTACK_GROWTH < 0 ) )

	#define taskCHECK_FOR_STACK_OVERFLOW()																\
	{																									\
		const uint32_t * const pulStack = ( uint32_t * ) pxCurrentTCB->pxStack;							\
		const uint32_t ulCheckValue = ( uint32_t ) 0xa5a5a5a5;											\
		// 判定 十六个 字节 不是 全部 的	ulCheckValue  0xA5 的时候 判定溢出																							\
		if( ( pulStack[ 0 ] != ulCheckValue ) ||												\
			( pulStack[ 1 ] != ulCheckValue ) ||												\
			( pulStack[ 2 ] != ulCheckValue ) ||												\
			( pulStack[ 3 ] != ulCheckValue ) )												\
		{
          // 调这个  hook 函数
			vApplicationStackOverflowHook( ( TaskHandle_t ) pxCurrentTCB, pxCurrentTCB->pcTaskName );	\
		}																								\
	}

#endif /* #if( configCHECK_FOR_STACK_OVERFLOW > 1 ) */
```

5.  在创建任务的时候 ，会分配 栈 然后 整个栈会全部写入 0xA5， 所有的栈控件都写入 0xA5;在执行函数的时候 往 栈写入的 缓存数据 一般并不会都是 0xA5,写入的数据会 覆盖掉  0xA5;
    
6.  方法二的判断方式时，入股最后 16 个字节 不是 全部的 0xA5 的时候，这个时候 说明 由其他的一些数据已经存入了栈中，如果 分配栈 128个字节，那么当 占用 113 个字节的时候就判定栈 快要溢出了， 被认为 溢出 ，实际上可能并没有溢出
    
7.  什么时候判定栈 是否溢出
    
8.  taskCHECK\_FOR\_STACK\_OVERFLOW(), 这个函数 是在 ++**vTaskSwitchContext()**++  中调用的，所以不管方法一 、方法二 都不是 实时的检测 溢出，都要在 切换任务 任务调度的时候检测
    

```c
    void vTaskSwitchContext( void )
    {
        traceENTER_vTaskSwitchContext();

        if( uxSchedulerSuspended != ( UBaseType_t ) 0U )
        {
            /* The scheduler is currently suspended - do not allow a context
             * switch. */
            xYieldPendings[ 0 ] = pdTRUE;
        }
        else
        {
            xYieldPendings[ 0 ] = pdFALSE;
            traceTASK_SWITCHED_OUT();

            /* Check for stack overflow, if configured. */
            taskCHECK_FOR_STACK_OVERFLOW();//在任务调度中 会调取这个函数 ，去判定 栈 是否溢出
}
```

##### 统计任务信息

调试与优化

1.  调试
    
    1.  打印
        
    2.  断言
        
    3.  trace
        
    4.  Malloc Hook
        
2.  优化
    
    1.  栈 使用情况
        
        1.  方法二中，是在创建任务的时候 将栈 全部填充 0xA5 ,在统计的时候，如果检测到连续 的 0xA5 ,说明这部分时候空闲的，如果发现不是 0xA5 从这个字节开始 上面的 都是曾经使用过的栈，下面都是空闲的空间，也被称为 水位，当水位接近0 的时候，会被认定为 即将溢出，水位越大，说明被分配的空间太大了
            
        2.  水位：(空闲栈字节数 / 4)
            
        3.  创建任务的时候 传入 堆栈大小 例如：128，这里传入的是 单位 是 4 个字节，这个数值最开始是预估的 ，如果水位很大说明 分配的 是 浪费空间，一般来说保证这个水位有 几十个字节就可以了
            

```c
#if ( INCLUDE_uxTaskGetStackHighWaterMark == 1 )

    UBaseType_t uxTaskGetStackHighWaterMark( TaskHandle_t xTask )
    {
        TCB_t * pxTCB;
        uint8_t * pucEndOfStack;
        UBaseType_t uxReturn;

        traceENTER_uxTaskGetStackHighWaterMark( xTask );

        pxTCB = prvGetTCBFromHandle( xTask );// 根据 传入的指针获得到一个 TCB 结构体

        #if portSTACK_GROWTH < 0
        {
            pucEndOfStack = ( uint8_t * ) pxTCB->pxStack;// 栈底
        }
        #else
        {
            pucEndOfStack = ( uint8_t * ) pxTCB->pxEndOfStack;
        }
        #endif

        uxReturn = ( UBaseType_t ) prvTaskCheckFreeStackSpace( pucEndOfStack );

        traceRETURN_uxTaskGetStackHighWaterMark( uxReturn );

        return uxReturn;
    }

#endif /* INCLUDE_uxTaskGetStackHighWaterMark */






/*-----------------------------------------------------------*/
空闲函数
#if ( ( configUSE_TRACE_FACILITY == 1 ) || ( INCLUDE_uxTaskGetStackHighWaterMark == 1 ) || ( INCLUDE_uxTaskGetStackHighWaterMark2 == 1 ) )

    static configSTACK_DEPTH_TYPE prvTaskCheckFreeStackSpace( const uint8_t * pucStackByte )
    {
        configSTACK_DEPTH_TYPE uxCount = 0U;
        // 如果 等于 0xA5 就 执行 while 循环 ，向上查找，统计 0xA5的数量，最终返回 (字节数 / 4)
        while( *pucStackByte == ( uint8_t ) tskSTACK_FILL_BYTE )// tskSTACK_FILL_BYTE 是 0xA5
        {
            pucStackByte -= portSTACK_GROWTH;// portSTACK_GROWTH 为 -1
            uxCount++;
        }

        uxCount /= ( configSTACK_DEPTH_TYPE ) sizeof( StackType_t );

        return uxCount;
    }

#endif /* ( ( configUSE_TRACE_FACILITY == 1 ) || ( INCLUDE_uxTaskGetStackHighWaterMark == 1 ) || ( INCLUDE_uxTaskGetStackHighWaterMark2 == 1 ) ) */
/*-
```

2.  任务运行时间统计
    
    1.  任务1 执行 ，任务2 执行， 任务1 执行 ，然后被任务3 中断，任务3 执行
        
    2.  时间统计 
        
        1.  开始 先记录 Ta   ， 任务2 开始的时间 Tb，那么Tb- Ta 就是 任务1 执行的时间
            
        2.  任务2  ： Tb 到 Tc
            
        3.  任务1 Tc 到 Td
            
        4.  任务3 ： Tc 到 Te
            
    3.  任务1 的时间是  （Tb- Ta） + （Td- Tc）， Cpu 占用率占比是  (（Tb- Ta） + （Td- Tc）)/ T\_all
        

![image.png](https://alidocs.oss-cn-zhangjiakou.aliyuncs.com/res/8oLl9y0Nw6W4lapY/img/78080ea4-b3bd-4321-aa6b-a582e66bd6aa.png)

4.  Ta、Tb、Tc Td  是否精确，因为 系统里会有 ++**Tick 中断**++  每 1ms 执行一次，如果使用 Tick 中断的化 task1 的(Tb -Ta) 是 4个 Tick也就是  4ms，
    
    5.  但是 (Td - Tc) 是 大概 是 2.3个 Tick ，大事 Tick 是 无法精确到 1ms 以下的，所以就要引入更精确的时间，更快的定时器，单位间隔更短，用更快的定时来计算；
        
    6.  更快的 Timer 的中断，每隔间隔 T 发生中断 Count++计数，在切换任务的时候，会去记录 count 的值， 这样就记录了 每隔任务 计数几个 count ，并且还会读取这个定时器Timer 的值，
        
    7.  portALT\_GET\_RUN\_TIME\_COUNTER\_VALUE 定义这个宏 实现 portALT\_GET\_RUN\_TIME\_COUNTER\_VALUE()  或 portGET\_RUN\_TIME\_COUNTER\_VALUE（） 这两个函数，然后初始化 定时器 ，应该是更快的定时器， 定时器速度 大概是  0.1ms ~0.01ms 十倍到 100 倍
        

```c
/*-----------------------------------------------------------*/
// 切换任务
void vTaskSwitchContext( void )
{
    if( uxSchedulerSuspended != ( UBaseType_t ) pdFALSE )
    {
       /* The scheduler is currently suspended - do not allow a context
       switch. */
       xYieldPending = pdTRUE;
    }
    else
    {
       xYieldPending = pdFALSE;
       traceTASK_SWITCHED_OUT();

       #if ( configGENERATE_RUN_TIME_STATS == 1 )
       {
            // 如果定义宏   portALT_GET_RUN_TIME_COUNTER_VALUE 那么会有 函数要定义， 
            // 作用是 获取 总的运行时间, 这里要得到的是 要用更快的定时器 得到总的运行时间，除此之外还要 初始化 定时器
             #ifdef portALT_GET_RUN_TIME_COUNTER_VALUE
                portALT_GET_RUN_TIME_COUNTER_VALUE( ulTotalRunTime );
             #else
                ulTotalRunTime = portGET_RUN_TIME_COUNTER_VALUE();
             #endif

             /* Add the amount of time the task has been running to the
             accumulated time so far.  The time the task started running was
             stored in ulTaskSwitchedInTime.  Note that there is no overflow
             protection here so count values are only valid until the timer
             overflows.  The guard against negative values is to protect
             against suspect run time stat counter implementations - which
             are provided by the application, not the kernel. */

            // 这里 总的 运行时间  ulTotalRunTime  减去 切换时 切入记录的时间，累加进  ulRunTimeCounter 时间中
             if( ulTotalRunTime > ulTaskSwitchedInTime )
             {
                pxCurrentTCB->ulRunTimeCounter += ( ulTotalRunTime - ulTaskSwitchedInTime );
             }
             else
             {
                mtCOVERAGE_TEST_MARKER();
             }
             ulTaskSwitchedInTime = ulTotalRunTime;
       }
       #endif /* configGENERATE_RUN_TIME_STATS */

       /* Check for stack overflow, if configured. */
       taskCHECK_FOR_STACK_OVERFLOW();

       /* Select a new task to run using either the generic C or port
       optimised asm code. */
       taskSELECT_HIGHEST_PRIORITY_TASK();
       traceTASK_SWITCHED_IN();

       #if ( configUSE_NEWLIB_REENTRANT == 1 )
       {
          /* Switch Newlib's _impure_ptr variable to point to the _reent
          structure specific to this task. */
          _impure_ptr = &( pxCurrentTCB->xNewLib_reent );
       }
       #endif /* configUSE_NEWLIB_REENTRANT */
    }
}
/*-----------------------------------------------------------*/
```

3.  涉及代码
    
    1.  代码执行流程：
        
        1.  初始化更快的定时器：启动调度器时  
            ![image.png](https://alidocs.oss-cn-zhangjiakou.aliyuncs.com/res/8oLl9y0Nw6W4lapY/img/d15fc2ef-0821-462b-98b7-4e7cfea8c654.png)
            
        2.  在任务切换时统计运行时间
            

![image.png](https://alidocs.oss-cn-zhangjiakou.aliyuncs.com/res/8oLl9y0Nw6W4lapY/img/b12147ce-0234-4407-a33c-dd3b1db7ca7b.png)

3.  获得统计信息，可以使用下列函数
    
    1.  uxTaskGetSystemState：对于每个任务它的统计信息都放在一个TaskStatus\_t结构体里
        
        2.  vTaskList：得到的信息是可读的字符串，
            
            3.  vTaskGetRunTimeStats： 得到的信息是可读的字符串，比如
                

##### 任务调度器开启 （主流程）

流程

1.  vTaskStartScheduler
    
    1.  开启任务调度器，首先会创建一个空闲任务
        
    2.  （判断）是否使用 软件定时器  任务
        
    3.  disabled Inpurret 关闭中断， 然后之后 在 SVC\_handle 中断服务函数里打开
        
    4.  初始化一些全局变量：
        
        1.  运行调度器 状态 设置 正在运行
            
        2.  时钟节拍计数器清零
            
    5.  （判断）如果开启统计运行时间的 话 会 执行宏
        
    6.  xPortStartScheduler()
        
        1.  设置中断优先级
            
        2.  开启定时器 使用节拍中断
            
        3.  管理临界区计数器 
            
        4.  开始调度
            
2.  任务创建
    
    1.  申请 任务 的 堆栈内存
        
    2.  申请任务控制块的内存， 返回任务控制块的句柄
        

```c

// pvPortMalloc 是 FreeRTOS 中的内存分配函数，它的返回值是 void*（即一个指向未指定类型的指针）。
// void* 类型的指针可以指向任何类型的数据，但在访问或使用这个指针之前，必须将其转换为适当的类型
// 进行强制转换 : 将返回的 void* 指针强制转换为 TCB_t* 类型, 这表明您希望将这块内存用于存储 TCB_t 结构体
// 内存中按照特定的布局分配空间，相同的数据类型具有特定的结构对齐要求。使用强制转换时，确保内存的大小和布局完全符合您预期的结构
pxNewTCB = ( TCB_t * ) pvPortMalloc( sizeof( TCB_t ) )
```

3.  把任务块结构体 与 任务堆栈 关联起来
    
4.  初始化 新任务 ， 创建过程是描述 任务的属性的
    
    1.  传入 任务控制块 等 参数
        
    2.  (判断) 是否检查堆栈溢出
        

```c
    #if ( tskSET_NEW_STACKS_TO_KNOWN_VALUE == 1 )// 检查堆栈溢出
    {
        /* Fill the stack with a known value to assist debugging. */
        // memset 给一段内存 填入 一些数据 ; 第一个参数  某一段内存 , tskSTACK_FILL_BYTE 填入的数据， 填入的大小
        // tskSTACK_FILL_BYTE 是 0x5A ， 这个 方法 二 为了计算 内存使用大小 是否溢出 的 所填入 的参数
        ( void ) memset( pxNewTCB->pxStack, ( int ) tskSTACK_FILL_BYTE, ( size_t ) uxStackDepth * sizeof( StackType_t ) );
    }
```

3.  获取 栈顶
    
    4.  保存任务名称到 任务 控制块 的pcTaskName
        
    5.  判定 优先级 ，如果太大 设置成  31
        
    6.  pxNewTCB->uxPriority 给任务控制块 传值 优先级
        
    7.  初始化 两个列表项 ：
        
        1.  状态列表项 ： pxItem->pxContainer = NULL 就是内容设置成空
            
        2.   事件列表项 ： pxItem->pxContainer = NULL;
            
    8.  初始化 各种任务堆栈 ，任务控制块成员变量
        
    9.  调用 pxPortInitialiseStack()  初始化  堆栈
        
        1.  操作指针， 首先  栈顶指针减一 ，保存 portINITIAL\_XPSR 的值，设置 xPSR 寄存器的初始值，表示程序状态， xPSR 时 内核寄存器，叫做 程序状态寄存器， 0x01000000 表示 这个寄存器的 bit24 为 1，表示处于 Thumb 状态， 即使用 的 Thumb 指令
            
        2.  接下来 指针减一 保存 pxCode， 就是 任务 函数 的指针 task1
            
        3.   指针减一 保存  XXX
            
        4.  指针减一 保存  R0 R1
            
        5.  指针减8  保存  R0 pvParameters 参数
            
        6.  创建完任务 后 栈顶指针 就到了 应该道德位置了，栈顶指针 到栈顶 保存的 时候 函数创建 时 需要的 数据
            
        7.  这个时 按顺序 保存，如果将来 任务 恢复的 时候 ，要 按顺序恢复到这些寄存器中
            

```c
StackType_t * pxPortInitialiseStack( StackType_t * pxTopOfStack,
                                     TaskFunction_t pxCode,
                                     void * pvParameters )
{
    /* 通过模拟上下文切换中断所创建的栈帧来初始化任务的栈。 */
    /* Simulate the stack frame as it would be created by a context switch
     * interrupt. */
    pxTopOfStack--;                                                      /* Offset added to account for the way the MCU uses the stack on entry/exit of interrupts. */  /* 将栈顶指针向下移动一个位置，适应MCU在进入/退出中断时使用的栈。 */
    *pxTopOfStack = portINITIAL_XPSR;                                    /* xPSR */                                                                                     /* 设置 xPSR 寄存器的初始值，表示程序状态。 */
    pxTopOfStack--;                                                                                                                                                     /* 将栈顶指针向下移动一个位置，为下一项准备空间。 */
    *pxTopOfStack = ( ( StackType_t ) pxCode ) & portSTART_ADDRESS_MASK; /* PC */                                                                                       /* 设置程序计数器 (PC)，即任务的启动地址。  注意使用 portSTART_ADDRESS_MASK 确保地址对齐。 */
    pxTopOfStack--;                                                                                                                                                          /* 将栈顶指针向下移动一个位置，为LR准备空间。 */
    *pxTopOfStack = ( StackType_t ) portTASK_RETURN_ADDRESS;             /* LR */                                                                                       /* 设置链接寄存器 (LR)，用于从任务中返回到调度器。 */
    pxTopOfStack -= 5;                                                   /* R12, R3, R2 and R1. */                                                                           /* 向下移动5个位置，为R12, R3, R2 和 R1 留出空间。 */
    *pxTopOfStack = ( StackType_t ) pvParameters;                        /* R0 */                                                                                       /* 设置函数参数 (R0)，传递给任务的参数。 */
    pxTopOfStack -= 8;                                                   /* R11, R10, R9, R8, R7, R6, R5 and R4. */                                                     /* 向下移动8个位置，为R11, R10, R9, R8, R7, R6, R5 和 R4 留出空间。 */

    return pxTopOfStack;
}
```

![image.png](https://alidocs.oss-cn-zhangjiakou.aliyuncs.com/res/8oLl9y0Nw6W4lapY/img/db915c1a-fe3c-49e3-a6ee-f2096c9f9ca2.png)

10.  根据老师 说 为什么内存要排列成这样的形势。因为 内核文档中 说 CPU 会自动的 入栈出栈，按照内核文档 ARM 公司这种规定 这种排列方式， 关键词 ： 入栈出栈
    
    11.  ![image](https://static.dingtalk.com/media/lQDPD20PwCQjBvnNBJLNCOywFb5Uu04LmRMHN0DDb4p3AA_2284_1170.jpg)
        

12.  任务 句柄 赋值 任务控制块
    

5.  任务创建成功后 ，把任务控制块放入到 就绪列表，prvAddNewTaskToReadyList 函数
    
    1.  记录系统中 有多少任务
        
    2.  如果创建的是 第一个 任务 ，初始化相关列表，  使用 prvInitialiseTaskLists()
        
        1.  初始化就绪列表
            
        2.  初始化 延时列表1
            
        3.  初始化 延时列表2
            
        4.  初始化 pending 列表
            
        5.  初始化 waiting 列表
            
    3.   如果创建的是 不是 第一个 任务,判断 任务调度器是否被 挂起
        
    4.  判定优先级 ， 如果最新任务优先级最大 那么 当前任务 赋值为 最新 创建的任务
        
    5.  调用 prvAddTaskToReadyList 将任务 添加到 就绪列表中， 包含列表和列表项的插入
        
    6.  如果当前任务 调度器在运行
        
        1.  taskYIELD\_ANY\_CORE\_IF\_USING\_PREEMPTION() 
            
            1.   如果传入的任务 比当前任务优先级 高，那么 启动任务调度 抢占执行
                
            2.  **实现上下文切换**：`taskYIELD_ANY_CORE_IF_USING_PREEMPTION` 的主要作用是触发任务切换。这通常在支持抢占的多核环境中使用，以允许系统在一个核心上运行的任务有机会切换到另一个任务，可能是在另一个核心上运行的任务
                
            3.  **支持抢占式调度**：在抢占式调度中，即使一个任务正在执行，也可以通过中断或其他机制强制切换到另一个优先级更高的任务。这个宏的名称暗示它可能会在多核系统中检查是否启用了抢占式调度，并在需要时执行上下文切换。
                

```plaintext
        #define taskYIELD_TASK_CORE_IF_USING_PREEMPTION( pxTCB ) \
    do {                                                         \
        ( void ) ( pxTCB );                                      \
        portYIELD_WITHIN_API();                                  \
    } while( 0 )

        #define taskYIELD_ANY_CORE_IF_USING_PREEMPTION( pxTCB ) \
    do {                                                        \
        if( pxCurrentTCB->uxPriority < ( pxTCB )->uxPriority )  \
        {                                                       \
            portYIELD_WITHIN_API();                             \
        }                                                       \
        else                                                    \
        {                                                       \
            mtCOVERAGE_TEST_MARKER();                           \
        }                                                       \
    } while( 0 
```

4.  任务切换的时候，需要 查找 ，有硬件查找，通用方法查找
    
    1.  硬件查找： 使用 uxTopReadPriority 32 位 记录优先级， 如果 bit0 代表优先级 0 ，bit32 代表优先级 1 如果当某个优先级有就绪任务的化就将其对应的 bit 置 1 
        
        2.  通用方法
            

##### 任务 删除 挂起 恢复 流程

1.  任务删除 vTaskDelete (TaskHandle\_t xTaskToDelete)
    
    1.  根据任务句柄获取任务控制块，如果任务句柄位NULL，表示删除任务自身
        
    2.  将要删除的任务从就绪列表中移除，并且如果相应的优先级下只有这一个任务那么复位 相应的优先级位， 这个 优先级对应的列表 空了
        
    3.  如果任务等待某些事件的的话那就将此任务从相应的 事件列表中移除，因为 被删除的事件 不需要再 等待 其他事件的发生，比如说 通知任务 等
        
    4.  如果删除的是任务自身，那么将 任务添加到 列表 xTasksWatitingTermination 中， 任务堆栈，控制块的内存释放再空闲任务中完成。任务删除自己的时候，会把删除任务交给空闲任务去完成，内存的释放 也是空闲任务完成的，还有一些 任务调度的工作要以身完成
        
    5.  uxCurrentNumberOfTask 减一， 表示当前系统任务减一，如果删除的不是任务自身
        
    6.  释放任务 控制块和任务堆栈的内存
        
    7.  如果删除的是任务自身，最后需要主动发起任务切换
        

2.  任务挂起 vTaskSupend （TaskHandle\_t xTaskSupend）
    
    1.  根据任务句柄后 获取任务控制块，如果任务句柄为 NULL ，那么表示要挂起自身任务
        
    2.  将要挂起的任务 从相应的状态切换列表和射箭中 移除，因为挂起后要去挂起列表，等待的事件不需要再等待，
        
    3.  将要改期的任务 添加到挂起任务列表 SuspendedTaskList 里面去
        
    4.  如果挂起恩物是 任务自身 而且任务 调度器正在运行，那么需要进行一次任务切换
        
3.  任务恢复 vTaskResume (TaskHandle\_t  xTaskResume)
    
    1.  判断 要恢复的任务是否位挂起任务
        
    2.  将要 恢复的任务 从挂起任务列表 xSuspenderTaskList 中移除
        
    3.  调用函数的prvAddTaskToReadyList () 将任务 添加到 就绪列表中
        
    4.  如果刚刚恢复任务 优先级比当前正在隐形任务的优先级高， 那么进行一次任务切换
        

##### 任务切换

1.  任务切换 是 在中断中进行的 
    
2.  不同的架构 实现的方式不一样，如果在 其他架构中有可能使用 ticks 中断
    
3.  再CortxM 中 会使用 SendSV 异常，
    
    1.   PendSV （可挂起系统调用）异常对 OS 非常重要，其优先级可以通过编程设置，可以通过将中断控制状态 寄存器 ICSR 的 bit28，也就是 PendSV 的挂起位置 1 来触发 PendSV 中断 ，与 **SVC 异常**不同，他是不精确的，因此他的挂起 状态可在更高优先级异常处理内设置，且会在该优先级处理完成后执行
        
    2.  利用该特性，若将 PendSV 设置为 最低异常优先级，可以让 PendSV 异常处理在所有其他中断处理完成后执行，这对于上下文切换非常有用，也是各种 OS 设计的关键
        
    3.  在具有嵌入 OS 的系统中，处理时间被划分了多个时间片，若系统中有两个任务，这两个任务会交替执行
        
    4.  pendVS异常的原因就是让大家知道， FreeRTOS 系统任务切换最终都是在PendSV 中断服务函数中完成的， us/OS 也是 PendSV中断中完成任务切换的
        
    

```plaintext
#define portNVIC_INT_CTRL_REG     ( *( ( volatile uint32_t * ) 0xe000ed04 ) )
#define portNVIC_PENDSVSET_BIT    ( 1UL << 28UL )// ICSR bit28 置 1
```

7.  任务切换的场合
    
8.  上下文（任务） 切换被 触发的场合1，
    
    1.  可以执行一个系统调用？？？？？
        
    2.  系统滴答定时器 Systick 中断
        
9.  执行系统调用就是执行FreeRTOS 系统提供相关API 函数，比如任务切换函数 taskYELD(), 有些其他的函数也会调用  taskYELD() ,这些API 函数都会导致任务切换，这些API 函数和任务切换函数 taskYELD() 都同城位系统调用，函数 taskYELD()  是 宏
    
10.  例如  vTaskDelay() 
    
    1.  首先判断 延时时间 是否大于0
        
    2.  然后挂起 当前 任务
        
    3.  把当前任务  插入到 延时列表里 prvAddCurrentTaskDelayList（）
        
    4.  然后 源码里 恢复了任务 xAlreadyYielded = xTaskResumeAll(); 
        
    5.  判定 xAlreadyYielded 恢复失败 调用 taskYIELD\_WITHIN\_API(); 切换任务
        
11.  再  sysTick\_Handler() 中
    
    1.  ePortSyeYickHandler()  会 去 置 ICSR 寄存器 的28 位
        
12.  任务切换的时候
    
    1.  任务 A 的  寄存器 保存到堆栈 stack， 每个任务都申请一个堆栈， 对于堆栈而言，都是向下增长的所以操作的是栈顶置针， 栈顶置针保存在 TCB 任务控制块里， TCB 的第一个成员变量 就是 栈顶置针
        
    2.  切换之前 先把任务 A 的 寄存器的 值 和状态， 保存在 （ 任务自己的堆栈） 堆栈里面，  这时要切换 到 任务 B ， 这是就从任务 B 堆栈 里面，把保存的置取出，在 FreeRTOS 的任务切换过程中，从任务堆栈中取出的寄存器内容会被直接**加载到 CPU 寄存器**中。这是任务切换的核心：将被切换到的任务的运行状态恢复到 CPU，从而继续运行该任务。 取出来后复制给 R0 R11， 这样 任务切换就完成了
        

3.  在任务切换中，FreeRTOS 的设计目标是保证每个任务在被切换出去时，当前的运行状态（寄存器值、程序计数器等）能够正确保存，而在切换回来时，能够恢复到切换时的状态。
    
    1.  以下是详细过程：
        
        1.  **保存当前任务的上下文**
            
            1.  当当前任务被切换出去时：
                
                1.  **寄存器内容存入任务堆栈：**
                    
                    1.  在触发 PendSV 中断后，FreeRTOS 会将 CPU 的寄存器内容（如 R0-R15、xPSR 等）压栈保存到当前任务的堆栈。
                        
                    2.  Cortex-M 处理器提供了硬件支持，会自动保存部分寄存器（R0-R3, R12, LR, PC, xPSR）到堆栈中。
                        
                    3.  FreeRTOS 的 `xPortPendSVHandler` 会额外保存 R4-R11（通常是任务使用的非易失性寄存器）到堆栈。
                        
                2.  保存后的任务堆栈示意：
                    

```plaintext
任务堆栈：
+-------------------+
|  R4, R5, ..., R11 |  <- FreeRTOS 保存
|  R0, R1, ..., R3  |  <- 硬件自动保存
|  R12, LR, PC, xPSR|  
+-------------------+

```

3.  最后，当前任务的栈指针（PSP，Process Stack Pointer）被保存到任务控制块（TCB）中
    
    4.  3. 恢复新任务的上下文
        
        1.  从新任务的堆栈中恢复寄存器：
            
            1.  `xPortPendSVHandler` 会从新任务堆栈中弹出保存的寄存器值（R4-R11 等）。
                
                2.  Cortex-M 处理器硬件会自动从堆栈中恢复 R0-R3、R12、LR、PC、xPSR。
                    
                    3.  最终，CPU 寄存器被完全恢复到新任务的状态，程序计数器（PC）也会指向新任务的代码位置。
                        

恢复后的任务堆栈结构：

```plaintext
任务堆栈：
+-------------------+
|  (堆栈已被弹空)   |
+-------------------+

```

### 总结

从任务堆栈中取出的寄存器内容直接存入到 CPU 的寄存器中，这是 Cortex-M 处理器任务切换的设计特点。整个过程是通过硬件（自动压栈/弹栈）和软件（FreeRTOS 中 `xPortPendSVHandler` 实现）共同完成的：

*   **保存时：** CPU 寄存器 → 任务堆栈。
    
*   **恢复时：** 任务堆栈 → CPU 寄存器。
    

R14 （LR）链接寄存器， 在异常处理期间， LR 会自动的更新为 特殊 的异常返回值 

![image](https://alidocs.oss-cn-zhangjiakou.aliyuncs.com/res/8oLl9y0Nw6W4lapY/img/dc86fc81-646c-420f-8483-a6eea08a0b2c.png)

vTaskSwitchContext（）

1.  判断 uxSchedulerSuspended 是否为 FALSE ，任务调度器是否 被挂起，如果被挂起 那么 xYieldPendings 为 pdTRUE
    
2.  没有挂起，查找下一个运行的任务
    
3.  更新当前任务 运行时间
    
4.  调用 taskSELECT\_HIGHEST\_PRIORITY\_TASK()  通用方法或者 硬件方法
    
    1.  硬件方法：
        
        1.  调用 portGET\_HIGHEST\_PRIORITY() 获取 最高优先级 任务，uxTopPriority 就不代表就绪态的最高优先级 了，而是使用每个 bit 代表一个优先级，bit0 代表优先级 0 ， bit31 代表优先级 31， 当某个优先级有就绪任务的话就将其对应的 bit 置 1 ，从这里就可以看出如果 使用硬件方法最多只能 有 32 个 优先级； \_\_clz(uxReadPriorities) 就是 uxReadPriorities 的前导 零个数， 前导零 个数就是从最高位开始 bit31 倒数第一个 为 1 的 bit 期间 0 的个数
            
            1.  例如 1000 0000 0000 前导零个数为 0
                
            2.  0000 1001 1111 0001 的前导零个数 为 4
                
        2.  的到 uxReadPriorities 的前导零个数以后在用 31 减去这个 0 个数的到的 就是处于就绪台的最高优先级了， 
            
        3.  已经找到 处于就绪台的最该优先级了，接下来就是从对应的列表中找出下一个要运行的任务，
            
        4.  查找方法就是使用函 listGET\_OWNER\_OF\_NEXT\_ENTRY() 获取到下一个要运行的任务的 任务控制块， 并将其 保存在 pxCurrentTCB 中 
            
    2.  通用方法
        
        1.  获取 处于 就绪态的最高优先级任务， uxReadPriorities 代表 处于就绪态 的 最该优先级，
            
        2.  查找方法就是使用函 listGET\_OWNER\_OF\_NEXT\_ENTRY() 获取到下一个要运行的任务的 任务控制块， 并将其 保存在 pxCurrentTCB 中