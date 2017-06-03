--TEST--
Parsed with encoding
--SKIPIF--
<?php if (!extension_loaded("html5")) print 'skip: "html5" extension not available'; ?>
--FILE--
<?php
var_dump(html5_parse("<html>Köln</html>")->saveHTML());
var_dump(html5_parse("<html>Köln</html>", 'utf-8')->saveHTML());
var_dump(html5_parse("<html>K\xF6ln</html>", 'ISO-8859-1')->saveHTML());
var_dump(html5_parse("<html>K\xF6ln</html>", 'Windows-1252')->saveHTML());
?>
--EXPECT--
string(65) "<!DOCTYPE html>
<html><head></head><body>K&ouml;ln</body></html>
"
string(65) "<!DOCTYPE html>
<html><head></head><body>K&ouml;ln</body></html>
"
string(65) "<!DOCTYPE html>
<html><head></head><body>K&ouml;ln</body></html>
"
string(65) "<!DOCTYPE html>
<html><head></head><body>K&ouml;ln</body></html>
"
