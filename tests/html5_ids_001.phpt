--TEST--
IDs
--SKIPIF--
<?php if (!extension_loaded("html5")) print 'skip: "html5" extension not available'; ?>
--FILE--
<?php
$html = ' <html><div id=foo>Foo</div><p id=foo>???</p></html>';
$document = html5_parse($html);
var_dump($document->getElementById('foo')->textContent);
var_dump($document->getElementById('bar'));
?>
--EXPECT--
string(3) "Foo"
NULL
