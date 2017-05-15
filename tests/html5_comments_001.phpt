--TEST--
Comments
--SKIPIF--
<?php if (!extension_loaded("html5")) print 'skip: "html5" extension not available'; ?>
--FILE--
<?php
$html = '<!-- A --><html><!-- B --></html><!-- C -->';
var_dump(html5_parse($html)->saveHTML());
?>
--EXPECT--
string(86) "<!DOCTYPE html>
<!-- A --><html><!-- B --><head></head><body></body></html><!-- C -->
"
