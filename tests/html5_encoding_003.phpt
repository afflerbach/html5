--TEST--
Encoding determined via meta charset
--SKIPIF--
<?php if (!extension_loaded("html5")) print 'skip: "html5" extension not available'; ?>
--FILE--
<?php
$document = html5_parse("<html><meta charset=ISO-8859-1>K\xF6ln</html>");
var_dump($document->encoding);
var_dump($document->getElementsByTagName('body')[0]->textContent);
?>
--EXPECT--
string(12) "WINDOWS-1252"
string(5) "Köln"
