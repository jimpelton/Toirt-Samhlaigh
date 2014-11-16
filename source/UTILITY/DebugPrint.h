

#ifndef DebugPrint_h__
#define DebugPrint_h__

// #define DEBUG_PRINT
extern int p_prt_tab_depth;

#ifdef DEBUG_PRINT
#define DEBUG_PRINT_ENTER \
    do { \
        p_prt_tab_depth += 1; \
        fprintf(stderr, "%*d E:%s\n", p_prt_tab_depth, p_prt_tab_depth, __func__); \
    }while(0)

#define DEBUG_PRINT_LEAVE \
    do { \
        fprintf(stderr, "%*d L:%s\n", p_prt_tab_depth, p_prt_tab_depth, \
                __func__); \
        p_prt_tab_depth -= 1; \
    }while(0)

#else

#define DEBUG_PRINT_ENTER
#define DEBUG_PRINT_LEAVE

#endif // DEBUG_PRINT

#endif // DebugPrint_h__


