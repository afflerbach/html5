--TEST--
Unkown elements
--SKIPIF--
<?php if (!extension_loaded("html5")) print 'skip: "html5" extension not available'; ?>
--FILE--
<?php
$html = '<math><msqrt><mi>x</mi></msqrt></math>';
var_dump(html5_parse($html)->saveHTML());
?>
--EXPECT--
string(161) "<!DOCTYPE html>
<html><head><meta http-equiv="Content-Type" content="text/html; charset=UTF-8"></head><body><math><msqrt><mi>x</mi></msqrt></math></body></html>
"
