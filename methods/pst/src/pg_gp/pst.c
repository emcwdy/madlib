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
    int range = PG_GETARG_INT32(2);
    char *result = NULL;
    int s_size = 0; 
    int v_size = strlen(val);

    if (state != NULL)
    {
        s_size = strlen(state);
        result = (char *)palloc0((1 + strlen(state)+strlen(val)) * sizeof(char));
        memcpy(result, state, s_size * sizeof(char));
        memcpy(result + s_size, val, v_size * sizeof(char));
    }
    else
    {
        result = (char *)palloc0((strlen(val) + 1) * sizeof(char));
        memcpy(result, val, v_size * sizeof(char));
        //state = (char *)palloc0((range + 1) * sizeof(char));
        //memcpy(state, val, v_size * sizeof(char));
    }
    
    PG_RETURN_TEXT_P(cstring_to_text(result));
}
PG_FUNCTION_INFO_V1(pst_string_agg_sfunc);

Datum pst_string_agg_pfunc
(
     PG_FUNCTION_ARGS
)
{
    char *state1 = PG_ARGISNULL(0)? NULL : text_to_cstring(PG_GETARG_TEXT_PP(0));
    char *state2 = PG_ARGISNULL(0)? NULL : text_to_cstring(PG_GETARG_TEXT_PP(1));
    if (NULL == state1 && NULL == state2)
        PG_RETURN_NULL();
    if (NULL == state1 && NULL != state2)
        return PG_GETARG_DATUM(1);
    if (NULL != state1 && NULL == state2)
        return PG_GETARG_DATUM(0);

    int size1 = strlen(state1);
    int size2 = strlen(state2);
    char* result = (char*) palloc0((size1 + size2 + 1) * sizeof(char));
    memcpy(result, state1, size1 * sizeof(char));
    memcpy(result + size1, state2, size2 * sizeof(char));
    
    PG_RETURN_TEXT_P(cstring_to_text(result));
}
PG_FUNCTION_INFO_V1(pst_string_agg_pfunc);
