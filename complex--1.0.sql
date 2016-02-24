/* contrib/complex/complex--1.0.sql */

-- complain if script is sourced in psql, rather than via CREATE EXTENSION
\echo Use "CREATE EXTENSION complex" to load this file. \quit


CREATE FUNCTION complex_hello(int, int)
RETURNS int
AS 'MODULE_PATHNAME', 'complex_hello'
LANGUAGE C STRICT;

CREATE FUNCTION complex_read(text)
RETURNS text
AS 'MODULE_PATHNAME', 'complex_read'
LANGUAGE C STRICT;