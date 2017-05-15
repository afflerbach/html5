--TEST--
Encoding set in DOMDocument
--SKIPIF--
<?php if (!extension_loaded("html5")) print 'skip: "html5" extension not available'; ?>
--FILE--
<?php
var_dump(html5_parse('<html></html>')->encoding);
?>
--EXPECT--
string(5) "UTF-8"
