--TEST--
Unkown elements
--SKIPIF--
<?php if (!extension_loaded("html5")) print 'skip: "html5" extension not available'; ?>
--FILE--
<?php
$html = '<svg><rect/><circle/></svg>';
var_dump(html5_parse($html)->saveHTML());
?>
--EXPECT--
string(97) "<!DOCTYPE html>
<html><head></head><body><svg><rect></rect><circle></circle></svg></body></html>
"
