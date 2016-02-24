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

CREATE FUNCTION float8_to_Complex(float8)
RETURNS complex
AS 'MODULE_PATHNAME', 'complex_send'
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

CREATE CAST (float8 AS complex)
    WITH FUNCTION float8_to_Complex (float8)
    AS IMPLICIT;