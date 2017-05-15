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
string(121) "<!DOCTYPE html>
<html lang="fr" class="test"><head><title>Salut!</title></head><body><p>42&nbsp;&euro;</p></body></html>
"
