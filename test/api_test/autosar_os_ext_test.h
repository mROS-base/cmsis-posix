#ifndef _AUTOSAR_OS_EXT_TEST_H_
#define _AUTOSAR_OS_EXT_TEST_H_

/*********************************************
 * Test Functions
 *********************************************/

/*
 * Message Queue
 */
extern void cmsis_message_queue_test_init(void);
extern void cmsis_message_queue_test_start(void);
extern void cmsis_message_queue_test_end(void);
extern int cmsis_message_queue_test_get_exec_num(void);

/*
 * Generic
 */
extern void cmsis_generic_test_init(void);
extern void cmsis_generic_test_start(void);
extern void cmsis_generic_test_end(void);
extern int cmsis_generic_test_get_exec_num(void);

/*
 * Thread
 */
extern void cmsis_thread_test_init(void);
extern void cmsis_thread_test_start(void);
extern void cmsis_thread_test_end(void);
extern int cmsis_thread_test_get_exec_num(void);

/*
 * Memory
 */
extern void autosar_os_memory_test_init(void);
extern void autosar_os_memory_test_start(void);
extern void autosar_os_memory_test_end(void);
extern int autosar_os_memory_test_get_exec_num(void);

/*
 * Kernel
 */
extern void cmsis_kernel_test_init(void);
extern void cmsis_kernel_test_start(void);
extern void cmsis_kernel_test_end(void);
extern int cmsis_kernel_test_get_exec_num(void);

/*
 * Mutexes
 */
extern void cmsis_mutexes_test_init(void);
extern void cmsis_mutexes_test_end(void);
extern void cmsis_mutexes_test_start(void);
extern int cmsis_mutexes_test_get_exec_num(void);

/*
 * Semaphores
 */
extern void cmsis_semaphores_test_init(void);
extern void cmsis_semaphores_test_end(void);
extern void cmsis_semaphores_test_start(void);
extern int cmsis_semaphores_test_get_exec_num(void);

#endif /* _AUTOSAR_OS_EXT_TEST_H_ */
