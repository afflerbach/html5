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
string(94) "<!DOCTYPE html>
<html><head></head><body><math><msqrt><mi>x</mi></msqrt></math></body></html>
"
