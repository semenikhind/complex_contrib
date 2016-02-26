/* contrib/complex/complex--1.0.sql */

-- complain if script is sourced in psql, rather than via CREATE EXTENSION
\echo Use "CREATE EXTENSION complex" to load this file. \quit

CREATE FUNCTION complex_in(cstring)
RETURNS complex
AS 'MODULE_PATHNAME', 'complex_in'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION complex_out(complex)
RETURNS cstring
AS 'MODULE_PATHNAME', 'complex_out'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION complex_recv(internal)
RETURNS complex
AS 'MODULE_PATHNAME', 'complex_recv'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION complex_send(complex)
RETURNS bytea
AS 'MODULE_PATHNAME', 'complex_send'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION complex_add(complex, complex)
RETURNS complex
AS 'MODULE_PATHNAME'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION complex_del(complex, complex)
RETURNS complex
AS 'MODULE_PATHNAME'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION complex_mult(complex, complex)
RETURNS complex
AS 'MODULE_PATHNAME'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION complex_div(complex, complex)
RETURNS complex
AS 'MODULE_PATHNAME'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION complex_eq(complex, complex)
RETURNS bool
AS 'MODULE_PATHNAME'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION complex_ne(complex, complex)
RETURNS bool
AS 'MODULE_PATHNAME'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION complex_lt(complex, complex)
RETURNS bool
AS 'MODULE_PATHNAME'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION complex_le(complex, complex)
RETURNS bool
AS 'MODULE_PATHNAME'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION complex_gt(complex, complex)
RETURNS bool
AS 'MODULE_PATHNAME'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION complex_ge(complex, complex)
RETURNS bool
AS 'MODULE_PATHNAME'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION float8_to_complex(float8)
RETURNS complex
AS 'MODULE_PATHNAME', 'float8_to_Complex'
LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION int4_to_complex(int4)
RETURNS complex
AS 'MODULE_PATHNAME', 'int4_to_Complex'
LANGUAGE C IMMUTABLE STRICT;

CREATE TYPE complex (
	internallength = 16,
	input = complex_in,
	output = complex_out,
	receive = complex_recv,
	send = complex_send,
	alignment = float8
);

CREATE OPERATOR + (
	leftarg = complex, rightarg = complex,
	procedure = complex_add,
	commutator = +
);

CREATE OPERATOR - (
	leftarg = complex, rightarg = complex,
	procedure = complex_del,
	commutator = -
);

CREATE OPERATOR * (
	leftarg = complex, rightarg = complex,
	procedure = complex_mult,
	commutator = *
);

CREATE OPERATOR / (
	leftarg = complex, rightarg = complex,
	procedure = complex_div
);

CREATE OPERATOR = (
	leftarg = complex, rightarg = complex,
	procedure = complex_eq,
	commutator = =, negator = !=
);

CREATE OPERATOR != (
	leftarg = complex, rightarg = complex,
	procedure = complex_ne,
	commutator = !=, negator = =
);

CREATE OPERATOR < (
	leftarg = complex, rightarg = complex,
	procedure = complex_lt,
	commutator = >, negator = >=
);

CREATE OPERATOR <= (
	leftarg = complex, rightarg = complex,
	procedure = complex_le,
	commutator = >=, negator = >
);

CREATE OPERATOR > (
	leftarg = complex, rightarg = complex,
	procedure = complex_gt,
	commutator = <, negator = <=
);

CREATE OPERATOR >= (
	leftarg = complex, rightarg = complex,
	procedure = complex_ge,
	commutator = <=, negator = <
);

CREATE CAST (float8 AS complex)
	WITH FUNCTION float8_to_Complex (float8)
	AS IMPLICIT;

CREATE CAST (int4 AS complex)
	WITH FUNCTION int4_to_Complex (int4)
	AS IMPLICIT;