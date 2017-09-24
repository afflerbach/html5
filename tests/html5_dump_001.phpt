--TEST--
Dump HTML5
--SKIPIF--
<?php if (!extension_loaded("html5")) print 'skip: "html5" extension not available'; ?>
--FILE--
<?php
$document = new DOMDocument();
var_dump(html5_dump($document));

$document->loadHTML("<html></html>");
var_dump(html5_dump($document));
//
$document->loadHTML("<html lang=en><!-- Comment --><p>Dump me!</p></html>");
var_dump(html5_dump($document));
?>
--EXPECT--
string(0) ""
string(29) "<!DOCTYPE html>
<html></html>"
string(83) "<!DOCTYPE html>
<html lang="en"><!-- Comment --><body><p>Dump me!</p></body></html>"
