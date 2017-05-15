--TEST--
MathML namespace: https://www.w3.org/TR/MathML/chapter2.html#interf.namespace

HTML does not support namespace extensibility in the same way, the HTML parser
has in-built knowledge of the HTML, SVG and MathML namespaces. 
xmlns attributes are just treated as normal attributes. 
Thus when using the HTML serialisation of MathML, prefixed element names must
not be used. xmlns="http://www.w3.org/1998/Math/MathML" may be used on the math
element, it will be ignored by the HTML parser, which always places math 
elements and its descendents in the MathML namespace.
--SKIPIF--
<?php if (!extension_loaded("html5")) print 'skip: "html5" extension not available'; ?>
--FILE--
<?php
$html = '<math><msqrt><mi>x</mi></msqrt></math>';
$document = html5_parse($html);
var_dump($document->saveHTML());
var_dump($document->documentElement->namespaceURI);
var_dump($document->getElementsByTagName('math')[0]->namespaceURI);
var_dump($document->getElementsByTagName('msqrt')[0]->namespaceURI);
var_dump($document->getElementsByTagName('mi')[0]->namespaceURI);

$html = '<math xmlns="foo"><msqrt><mi>x</mi></msqrt></math>';
$document = html5_parse($html);
var_dump($document->saveHTML());
var_dump($document->documentElement->namespaceURI);
var_dump($document->getElementsByTagName('math')[0]->namespaceURI);
var_dump($document->getElementsByTagName('msqrt')[0]->namespaceURI);
var_dump($document->getElementsByTagName('mi')[0]->namespaceURI);
?>
--EXPECT--
string(94) "<!DOCTYPE html>
<html><head></head><body><math><msqrt><mi>x</mi></msqrt></math></body></html>
"
NULL
string(34) "http://www.w3.org/1998/Math/MathML"
string(34) "http://www.w3.org/1998/Math/MathML"
string(34) "http://www.w3.org/1998/Math/MathML"
string(106) "<!DOCTYPE html>
<html><head></head><body><math xmlns="foo"><msqrt><mi>x</mi></msqrt></math></body></html>
"
NULL
string(34) "http://www.w3.org/1998/Math/MathML"
string(34) "http://www.w3.org/1998/Math/MathML"
string(34) "http://www.w3.org/1998/Math/MathML"
