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
string(164) "<!DOCTYPE html>
<html><head><meta http-equiv="Content-Type" content="text/html; charset=UTF-8"></head><body><svg><rect></rect><circle></circle></svg></body></html>
"
