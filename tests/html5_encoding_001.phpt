--TEST--
Encoding set in DOMDocument
--SKIPIF--
<?php if (!extension_loaded("html5")) print 'skip: "html5" extension not available'; ?>
--FILE--
<?php
var_dump(html5_parse("<html>Köln</html>")->encoding);
var_dump(html5_parse("<html>Köln</html>", 'auto')->encoding);
var_dump(html5_parse("<html>Köln</html>", 'utf-8')->encoding);
var_dump(html5_parse("<html>K\xF6ln</html>", ' ISO-8859-1   ')->encoding);
var_dump(html5_parse("<html>K\xF6ln</html>", 'Windows-1252')->encoding);
?>
--EXPECT--
string(5) "UTF-8"
string(5) "UTF-8"
string(5) "UTF-8"
string(12) "WINDOWS-1252"
string(12) "WINDOWS-1252"
