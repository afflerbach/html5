--TEST--
XLink namespace: https://www.w3.org/TR/SVG2/linking.html#XLinkRefAttrs
--SKIPIF--
<?php if (!extension_loaded("html5")) print 'skip: "html5" extension not available'; ?>
--FILE--
<?php
$xlinkNS = "http://www.w3.org/1999/xlink";
$html = '<svg xmlns:xlink="http://www.w3.org/1999/xlink"><image xlink:href="..."/></svg>';
$document = html5_parse($html);
var_dump($document->saveHTML());
var_dump($document->getElementsByTagName('image')[0]->getAttributeNS($xlinkNS, 'href'));
?>
--EXPECT--
string(209) "<!DOCTYPE html>
<html><head><meta http-equiv="Content-Type" content="text/html; charset=UTF-8"></head><body><svg xmlns:xlink="http://www.w3.org/1999/xlink"><image xlink:href="..."></image></svg></body></html>
"
string(3) "..."
