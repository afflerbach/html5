--TEST--
Dump HTML5 void elements: https://www.w3.org/TR/html5/syntax.html#void-elements
--SKIPIF--
<?php if (!extension_loaded("html5")) print 'skip: "html5" extension not available'; ?>
--FILE--
<?php
$html = '<html><head><meta http-equiv="..."><link rel="..."></head><body><img src="..."><br><input type="text"/><hr></body></html>';
var_dump(html5_dump(html5_parse($html)));
?>
--EXPECT--
string(203) "<!DOCTYPE html>
<html><head><meta http-equiv="Content-Type" content="text/html; charset=UTF-8"><meta http-equiv="..."><link rel="..."></head><body><img src="..."><br><input type="text"><hr></body></html>"

