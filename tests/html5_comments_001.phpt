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
string(153) "<!DOCTYPE html>
<!-- A --><html><!-- B --><head><meta http-equiv="Content-Type" content="text/html; charset=UTF-8"></head><body></body></html><!-- C -->
"
