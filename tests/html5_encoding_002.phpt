--TEST--
Parsed with encoding
--SKIPIF--
<?php if (!extension_loaded("html5")) print 'skip: "html5" extension not available'; ?>
--FILE--
<?php
var_dump(html5_parse("<html>Köln</html>")->saveHTML());
var_dump(html5_parse("<html>Köln</html>", 'utf-8')->saveHTML());

$document = html5_parse("<html>K\xF6ln</html>", ' Iso-8859-1 ');
var_dump($document->saveHTML());
var_dump($document->getElementsByTagName('body')[0]->textContent);

$document = html5_parse("<html>K\xF6ln</html>", 'Windows-1252');
var_dump($document->saveHTML());
var_dump($document->getElementsByTagName('body')[0]->textContent);
?>
--EXPECT--
string(128) "<!DOCTYPE html>
<html><head><meta http-equiv="Content-Type" content="text/html; charset=UTF-8"></head><body>Köln</body></html>
"
string(128) "<!DOCTYPE html>
<html><head><meta http-equiv="Content-Type" content="text/html; charset=UTF-8"></head><body>Köln</body></html>
"
string(134) "<!DOCTYPE html>
<html><head><meta http-equiv="Content-Type" content="text/html; charset=WINDOWS-1252"></head><body>K�ln</body></html>
"
string(5) "Köln"
string(134) "<!DOCTYPE html>
<html><head><meta http-equiv="Content-Type" content="text/html; charset=WINDOWS-1252"></head><body>K�ln</body></html>
"
string(5) "Köln"
