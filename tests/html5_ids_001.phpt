--TEST--
IDs
--SKIPIF--
<?php if (!extension_loaded("html5")) print 'skip: "html5" extension not available'; ?>
--FILE--
<?php
$html = ' <html><div id=foo>Foo</div><p id=foo>???</p><b xml:id="bar">Bar</b></html>';
$document = html5_parse($html);
var_dump($document->saveHTML());
var_dump($document->getElementById('foo')->textContent);
var_dump($document->getElementById('bar'));
?>
--EXPECT--
string(188) "<!DOCTYPE html>
<html><head><meta http-equiv="Content-Type" content="text/html; charset=UTF-8"></head><body><div id="foo">Foo</div><p id="foo">???</p><b xml:id="bar">Bar</b></body></html>
"
string(3) "Foo"
NULL
