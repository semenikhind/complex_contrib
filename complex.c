#include "postgres.h"
#include "funcapi.h"
#include "utils/memutils.h"
#include "utils/builtins.h"

#include <string.h>
#include <stdio.h>



int complex_worker(int c, int d);

PG_MODULE_MAGIC;

PG_FUNCTION_INFO_V1(complex_hello);
PG_FUNCTION_INFO_V1(complex_read);

Datum
complex_hello(PG_FUNCTION_ARGS)
{
	int a = PG_GETARG_INT32(0);
	int b = PG_GETARG_INT32(1);

	PG_RETURN_INT32(complex_worker(a, b));
}

Datum
complex_read(PG_FUNCTION_ARGS)
{
	text * str = PG_GETARG_TEXT_P(0);
	char * filename = text_to_cstring(str);
	int alloc_size = 1;
	int data_size = 256;
	int temp = 0;
	char buf[256];
    char * result = (char *)palloc0(alloc_size);
	FILE * file = fopen(filename, "r");

	while (fgets(buf, sizeof(buf), file))
	{
		while (alloc_size < data_size)
		{
			alloc_size *= 2;
			result = (char *)repalloc(result, alloc_size * 2);
		}
		memcpy(&result[temp], buf, sizeof(buf));
		temp += sizeof(buf);
		data_size += sizeof(buf);
	}

	PG_RETURN_TEXT_P(cstring_to_text(result));
}


int
complex_worker(int c, int d)
{
	char* buffer;
	MemoryContext old_cxt;
	MemoryContext func_cxt;

	func_cxt = AllocSetContextCreate(CurrentMemoryContext,
									 "abc_worker context",
									 ALLOCSET_DEFAULT_MINSIZE,
									 ALLOCSET_DEFAULT_INITSIZE,
									 ALLOCSET_DEFAULT_MAXSIZE);
	old_cxt = MemoryContextSwitchTo(func_cxt);

	buffer = palloc0(100);
	sprintf(buffer, "a = %d, b = %d", c, d);
	elog(INFO, "%s", buffer);

	MemoryContextSwitchTo(old_cxt);
	MemoryContextDelete(func_cxt);


	return c + d;
}

