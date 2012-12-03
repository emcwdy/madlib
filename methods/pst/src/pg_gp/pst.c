/*
 *
 * @file pst.c
 *
 * @brief
 *
 * @date April 10, 2012
 */

#include <float.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#include "postgres.h"
#include "fmgr.h"
#include "access/tupmacs.h"
#include "utils/array.h"
#include "utils/lsyscache.h"
#include "utils/builtins.h"
#include "utils/typcache.h"
#include "catalog/pg_type.h"
#include "catalog/namespace.h"
#include "nodes/execnodes.h"
#include "nodes/nodes.h"
#include "funcapi.h"

#ifndef NO_PG_MODULE_MAGIC
PG_MODULE_MAGIC;
#endif
/*#define __DT_SHOW_DEBUG_INFO__*/
#ifdef __DT_SHOW_DEBUG_INFO__
#define dtelog(...) elog(__VA_ARGS__)
#else
#define dtelog(...)
#endif


/*
 * Postgres8.4 doesn't have such macro, so we add here
 */
#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x) (sizeof(x) / sizeof(*(x)))
#endif


Datum pst_string_agg_sfunc
(
    PG_FUNCTION_ARGS
)
{
    char *state = PG_ARGISNULL(0)? NULL : text_to_cstring(PG_GETARG_TEXT_PP(0));
    char *val = text_to_cstring(PG_GETARG_TEXT_PP(1));
    //char val = PG_GETARG_CHAR(1);
    int range = PG_GETARG_INT32(2); 
    int s_size = 0; 

    if (state != NULL)
    {
        for(; state[s_size] != ' '; ++s_size);
        state[s_size] = *val;
        //state[s_size] = val;
    }
    else
    {
        state = (char *)palloc0((range + 1) * sizeof(char));
        state[s_size] = *val;
        //state[s_size] = val;
        memset(state + 1, ' ', range - 1);
    }
    
    PG_RETURN_TEXT_P(cstring_to_text(state));
}
PG_FUNCTION_INFO_V1(pst_string_agg_sfunc);

