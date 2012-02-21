
#define MODULE_BIT      12
#define STATUS_BIT      10
#define STATUS_BIT      10
#define SLL_MOD_ID      1UL
#define DLL_MOD_ID      2UL
#define STACK_MOD_ID    3UL


/* XXX: replace unsigned long with a 32 bit type */
#define MODULE_MASK     (1UL << (8*sizeof (unsigned long) - MODULE_BIT))
