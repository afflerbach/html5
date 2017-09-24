--TEST--
Namespaces: https://www.w3.org/TR/html5/syntax.html#attributes-0
--SKIPIF--
<?php if (!extension_loaded("html5")) print 'skip: "html5" extension not available'; ?>
--FILE--
<?php
$xlinkNS = "http://www.w3.org/1999/xlink";
$html = '<svg xmlns:xlink="http://www.w3.org/1999/xlink"><image xlink:href="..."/></svg>';
$document = html5_parse($html);
var_dump(html5_dump($document));
?>
--EXPECT--
string(201) "<!DOCTYPE html>
<html><head><meta http-equiv="Content-Type" content="text/html; charset=UTF-8"></head><body><svg xmlns:xlink="http://www.w3.org/1999/xlink"><image xlink:href="..."/></svg></body></html>"
