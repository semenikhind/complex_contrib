#include "postgres.h"
#include "funcapi.h"
#include "utils/memutils.h"
#include "utils/builtins.h"
#include "libpq/pqformat.h"

#include <string.h>
#include <stdio.h>

typedef struct Complex
{
	float8 x;
	float8 y;
} Complex;

PG_MODULE_MAGIC;

PG_FUNCTION_INFO_V1(complex_in);
PG_FUNCTION_INFO_V1(complex_out);
PG_FUNCTION_INFO_V1(complex_send);
PG_FUNCTION_INFO_V1(complex_recv);
PG_FUNCTION_INFO_V1(complex_add);
PG_FUNCTION_INFO_V1(complex_del);
PG_FUNCTION_INFO_V1(complex_mult);
PG_FUNCTION_INFO_V1(complex_div);
PG_FUNCTION_INFO_V1(complex_eq);
PG_FUNCTION_INFO_V1(complex_ne);
PG_FUNCTION_INFO_V1(complex_lt);
PG_FUNCTION_INFO_V1(complex_le);
PG_FUNCTION_INFO_V1(complex_gt);
PG_FUNCTION_INFO_V1(complex_ge);
PG_FUNCTION_INFO_V1(float8_to_Complex);
PG_FUNCTION_INFO_V1(int4_to_Complex);

Datum
complex_in(PG_FUNCTION_ARGS)
{
	char *str = PG_GETARG_CSTRING(0);
	float8 x,
	       y;
	Complex *result;
	if (sscanf(str, " ( %lf , %lf )", &x, &y) != 2)
		ereport(ERROR,
			    (errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
				 errmsg("invalid input syntax for complex: \"%s\"",
				 str)));

	result = (Complex *) palloc(sizeof(Complex));
	result->x = x;
	result->y = y;
	PG_RETURN_POINTER(result);
}

Datum
complex_out(PG_FUNCTION_ARGS)
{
	Complex *complex = (Complex *) PG_GETARG_POINTER(0);
	char *result;

	result = psprintf("(%g,%g)", complex->x, complex->y);
	PG_RETURN_CSTRING(result);
}

Datum
complex_send(PG_FUNCTION_ARGS)
{
	Complex *complex = (Complex *) PG_GETARG_POINTER(0);
	StringInfoData buf;

	pq_begintypsend(&buf);
	pq_sendfloat8(&buf, complex->x);
	pq_sendfloat8(&buf, complex->y);
	PG_RETURN_BYTEA_P(pq_endtypsend(&buf));
}

Datum
complex_recv(PG_FUNCTION_ARGS)
{
	StringInfo buf = (StringInfo) PG_GETARG_POINTER(0);
	Complex *result;

	result = (Complex *) palloc(sizeof(Complex));
	result->x = pq_getmsgfloat8(buf);
	result->y = pq_getmsgfloat8(buf);
	PG_RETURN_POINTER(result);
}

Datum
complex_add(PG_FUNCTION_ARGS)
{
	Complex *a = (Complex *) PG_GETARG_POINTER(0);
	Complex *b = (Complex *) PG_GETARG_POINTER(1), *result;
	result = (Complex *) palloc(sizeof(Complex));
	result->x = a->x + b->x;
	result->y = a->y + b->y;
	PG_RETURN_POINTER(result);
}

Datum
complex_del(PG_FUNCTION_ARGS)
{
	Complex *a = (Complex *) PG_GETARG_POINTER(0);
	Complex *b = (Complex *) PG_GETARG_POINTER(1), *result;
	result = (Complex *) palloc(sizeof(Complex));
	result->x = a->x - b->x;
	result->y = a->y - b->y;
	PG_RETURN_POINTER(result);
}

Datum
complex_mult(PG_FUNCTION_ARGS)
{
	Complex *a = (Complex *) PG_GETARG_POINTER(0);
	Complex *b = (Complex *) PG_GETARG_POINTER(1), *result;
	result = (Complex *) palloc(sizeof(Complex));
	result->x = a->x * b->x - a->y * b->y;
	result->y = a->x * b->y + a->y * b->x;
	PG_RETURN_POINTER(result);
}

Datum
complex_div(PG_FUNCTION_ARGS)
{
	Complex *a = (Complex *) PG_GETARG_POINTER(0);
	Complex *b = (Complex *) PG_GETARG_POINTER(1), *result;
	result = (Complex *) palloc(sizeof(Complex));
	result->x = (a->x * b->x + a->y * b->y)/(b->x * b->x + b->y * b->y);
	result->y = (b->x * a->y - b->y * a->x)/(b->x * b->x + b->y * b->y);
	PG_RETURN_POINTER(result);
}

Datum
complex_eq(PG_FUNCTION_ARGS)
{
	Complex *a = (Complex *) PG_GETARG_POINTER(0);
	Complex *b = (Complex *) PG_GETARG_POINTER(1);
	bool x = DatumGetBool(DirectFunctionCall2(float8eq,
											  Float8GetDatum(a->x),
											  Float8GetDatum(b->x)));
	bool y = DatumGetBool(DirectFunctionCall2(float8eq,
											  Float8GetDatum(a->y),
											  Float8GetDatum(b->y)));
	PG_RETURN_BOOL(x&&y);
}

Datum
complex_ne(PG_FUNCTION_ARGS)
{
	Complex *a = (Complex *) PG_GETARG_POINTER(0);
	Complex *b = (Complex *) PG_GETARG_POINTER(1);
	bool x = DatumGetBool(DirectFunctionCall2(float8ne,
											  Float8GetDatum(a->x),
											  Float8GetDatum(b->x)));
	bool y = DatumGetBool(DirectFunctionCall2(float8ne,
											  Float8GetDatum(a->y),
											  Float8GetDatum(b->y)));
	PG_RETURN_BOOL(x||y);
}

Datum
complex_lt(PG_FUNCTION_ARGS)
{
	Complex *a = (Complex *) PG_GETARG_POINTER(0);
	Complex *b = (Complex *) PG_GETARG_POINTER(1);
	float8 res_a = a->x * a->x + a->y * a->y;
	float8 res_b = b->x * b->x + b->y * b->y;
	bool result = DatumGetBool(DirectFunctionCall2(float8lt,
												   Float8GetDatum(res_a),
												   Float8GetDatum(res_b)));
	PG_RETURN_BOOL(result);
}

Datum
complex_le(PG_FUNCTION_ARGS)
{
	Complex *a = (Complex *) PG_GETARG_POINTER(0);
	Complex *b = (Complex *) PG_GETARG_POINTER(1);
	float8 res_a = a->x * a->x + a->y * a->y;
	float8 res_b = b->x * b->x + b->y * b->y;
	bool result = DatumGetBool(DirectFunctionCall2(float8le,
												   Float8GetDatum(res_a),
												   Float8GetDatum(res_b)));
	PG_RETURN_BOOL(result);
}

Datum
complex_gt(PG_FUNCTION_ARGS)
{
	Complex *a = (Complex *) PG_GETARG_POINTER(0);
	Complex *b = (Complex *) PG_GETARG_POINTER(1);
	float8 res_a = a->x * a->x + a->y * a->y;
	float8 res_b = b->x * b->x + b->y * b->y;
	bool result = DatumGetBool(DirectFunctionCall2(float8gt,
												   Float8GetDatum(res_a),
												   Float8GetDatum(res_b)));
	PG_RETURN_BOOL(result);
}

Datum
complex_ge(PG_FUNCTION_ARGS)
{
	Complex *a = (Complex *) PG_GETARG_POINTER(0);
	Complex *b = (Complex *) PG_GETARG_POINTER(1);
	float8 res_a = a->x * a->x + a->y * a->y;
	float8 res_b = b->x * b->x + b->y * b->y;
	bool result = DatumGetBool(DirectFunctionCall2(float8ge,
												   Float8GetDatum(res_a),
												   Float8GetDatum(res_b)));
	PG_RETURN_BOOL(result);
}

Datum
float8_to_Complex(PG_FUNCTION_ARGS)
{
	float8 a = PG_GETARG_FLOAT8(0);
	Complex *result;
	result = (Complex *) palloc(sizeof(Complex));
	result->x = a;
	result->y = (float8) 0;
	PG_RETURN_POINTER(result);
}

Datum
int4_to_Complex(PG_FUNCTION_ARGS)
{
	int32 a = PG_GETARG_INT32(0);
	Complex *result;
	result = (Complex *) palloc(sizeof(Complex));
	result->x = a;
	result->y = 0;
	PG_RETURN_POINTER(result);
}