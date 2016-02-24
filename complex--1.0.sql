/* contrib/complex/complex--1.0.sql */

-- complain if script is sourced in psql, rather than via CREATE EXTENSION
\echo Use "CREATE EXTENSION complex" to load this file. \quit

CREATE FUNCTION complex_in(cstring)
RETURNS complex
AS 'MODULE_PATHNAME', 'complex_in'
LANGUAGE C STRICT
IMMUTABLE;

CREATE FUNCTION complex_out(complex)
RETURNS cstring
AS 'MODULE_PATHNAME', 'complex_out'
LANGUAGE C STRICT
IMMUTABLE;

CREATE FUNCTION complex_recv(internal)
RETURNS complex
AS 'MODULE_PATHNAME', 'complex_recv'
LANGUAGE C STRICT
IMMUTABLE;

CREATE FUNCTION complex_send(complex)
RETURNS bytea
AS 'MODULE_PATHNAME', 'complex_send'
LANGUAGE C STRICT
IMMUTABLE;

CREATE TYPE complex (
	internallength = 16,
	input = complex_in,
	output = complex_out,
	receive = complex_recv,
	send = complex_send,
	alignment = double
);