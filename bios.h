
#pragma once
#include <stdbool.h>
#include <stdint.h>

/*
 * SYSCALLS
 */

/**
 * Enter critical section.
 *
 * Disables interrupts so code executes atomically.
 */
void EnterCriticalSection();

/**
 * Exit critical section.
 *
 * Re-enables interrupts.
 */
void ExitCriticalSection();

/*
 * A-FUNCTIONS
 */

/**
 * File for read access.
 */
#define FILE_READ     0x00000001

/**
 * File for write access.
 */
#define FILE_WRITE    0x00000002

/**
 * When reading from TTY, returns without waiting to read requested amount of bytes.
 */
#define FILE_TTY_LAZY 0x00000004

/**
 * Create a new file.
 */
#define FILE_CREATE   0x00000200

/**
 * Read asynchronously from memory card.
 */
#define FILE_MC_ASYNC 0x00008000

/**
 * Specify size of file in blocks for memory card.
 */
#define FILE_SIZE(blocks) ((blocks) << 16)

/**
 * Opens a file on the target device for I/O.
 *
 * Table A, call 0x00.
 *
 * @param path file path
 * @param access access flags
 * @returns file handle, or -1 on error.
 */
int32_t FileOpen(const char * filename, uint32_t accessmode);

/**
 * Reads the number of bytes from the specified open file.
 *
 * Must be a multiple of 128 bytes for memory card, and 2048 for CD-ROM.
 *
 * Table A, call 0x02.
 *
 * @param fd file handle
 * @param dst data buffer
 * @param length max data length
 * @returns number of bytes read
 */
int32_t FileRead(int32_t fd, void * dst, uint32_t length);

/**
 * Closes an open file.
 *
 * Table A, call 0x04.
 *
 * @param fd file handle
 */
void FileClose(int32_t fd);

/**
 * Converts a digit to its numeric value:
 *
 * - '0' to '9' returns 0 to 9
 * - 'A' to 'Z' returns 10 to 35
 * - 'a' to 'z' returns 10 to 35 as well
 *
 * For other values, this returns 9,999,999 in decimal.
 *
 * Table A, call 0x0A.
 *
 * @param c the character
 * @returns parsed digit
 */
uint32_t todigit(char c);

/**
 * Compares two strings.
 *
 * Table A, call 0x17.
 *
 * @param a first string
 * @param b second string
 * @returns zero if they are equal, or the difference between the first different byte.
 */
int32_t strcmp(const char * a, const char * b);

/**
 * Compares two strings with a length limit.
 *
 * Table A, call 0x18.
 *
 * @param a first string
 * @param b second string
 * @param maxlen max length
 * @returns zero if they are equal, or the difference between the first different byte.
 */
int32_t strncmp(const char * a, const char * b, uint32_t maxlen);

/**
 * Calculates the length of a string.
 *
 * Table A, call 0x1B.
 *
 * @param str string
 * @returns string length, or zero if null
 */
uint32_t strlen(const char * str);

/**
 * Returns a pointer to the first occurence of the character in the string, or NULL if not found.
 *
 * Table A, call 0x1B.
 *
 * @param haystack string in which to search
 * @param needle character to look for
 * @returns pointer to first occurence, or NULL
 */
char * strchr(const char * haystack, int needle);

/**
 * Write string to TTY.
 *
 * Table A, call 0x3E.
 *
 * @param txt text to display.
 */
void std_out_puts(const char * txt);

/**
 * Prints a formatted string to standard output.
 *
 * Table A, call 0x3F.
 *
 * @param txt text to display.
 * @returns garbage.
 */
int printf(const char * txt, ...);

/**
 * Loads an executable to main memory.
 *
 * @param filename executable path
 * @param headerbuf header buffer
 *
 * Table A, call 0x42.
 */
bool LoadExeFile(const char * filename, uint8_t * headerbuf);

/**
 * Starts a previously loaded executable.
 *
 * @param headerbuf header buffer
 * @param param1 first argument sent to the executable
 * @param param2 second argument sent to the executable
 *
 * Table A, call 0x43.
 */
void DoExecute(uint8_t * headerbuf, uint32_t param1, uint32_t param2);

/**
 * Copies the three default four-opcode handlers for the A(NNh),B(NNh),C(NNh) functions to A00000A0h..A00000CFh.
 *
 * Table A, call 0x45.
 */
void init_a0_b0_c0_vectors(void);

/**
 * Initializes the CD drive and filesystem
 *
 * Table A, call 0x54.
 */
void CdInit(void);

/**
 * (Re-)initializes kernel resources.
 *
 * @param evcb max number of events
 * @param tcb max number of threads
 * @param stacktop stack top
 *
 * Table A, call 0x9C.
 */
void SetConf(uint32_t evcb, uint32_t tcb, uint32_t stacktop);

/*
 * C-FUNCTIONS
 */

/**
 * Configures the Vblank and timer handlers.
 *
 * @param priority IRQ priority.
 *
 * Table C, call 0x00.
 */
void EnqueueTimerAndVblankIrqs(uint32_t priority);

/**
 * Configures the syscall handler.
 *
 * @param priority IRQ priority.
 *
 * Table C, call 0x01.
 */
void EnqueueSyscallHandler(uint32_t priority);

/**
 * Copies the default four-opcode exception handler to the exception vector at 0x80000080h~0x8000008F.
 *
 * Table C, call 0x07.
 */
void InstallExceptionHandlers(void);

/**
 * Initializes the address and size of the allocatable Kernel Memory region.
 *
 * Table C, call 0x08.
 */
void SysInitMemory(uint32_t start_address, uint32_t size);

/**
 * Configures some IRQ handlers.
 *
 * @param priority IRQ priority.
 *
 * Table C, call 0x0C.
 */
void InitDefInt(uint32_t priority);

/**
 * Initializes the default device drivers for the TTY, CDROM and memory cards.
 *
 * The flags controls whether the TTY device should be a dummy (retail console) or an actual UART (dev console).
 * Note this will call will freeze if the UART is enabled but there is no such device.
 *
 * @param enable_tty 0 to use a dummy TTY, 1 to use a real UART.
 *
 * Table C, call 0x12.
 */
void InstallDevices(uint32_t enable_tty);

/**
 * Fixes the A call table, copying missing entries from the C table.
 *
 * Table C, call 0x1C.
 */
void AdjustA0Table(void);