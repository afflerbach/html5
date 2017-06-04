--TEST--
Parse HTML5.
--SKIPIF--
<?php if (!extension_loaded("html5")) print 'skip: "html5" extension not available'; ?>
--FILE--
<?php
$html = '<html LANG=fr class="test"><title>Salut!</title><p>42&#160;&euro;</p></html>';
var_dump(html5_parse($html)->saveHTML());
?>
--EXPECT--
string(181) "<!DOCTYPE html>
<html lang="fr" class="test"><head><meta http-equiv="Content-Type" content="text/html; charset=UTF-8"><title>Salut!</title></head><body><p>42 €</p></body></html>
"
