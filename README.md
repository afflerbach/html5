# html5

[![Build Status](https://travis-ci.org/afflerbach/html5.svg?branch=master)](https://travis-ci.org/afflerbach/html5)

`html5` is an extension for [PHP](https://github.com/php/php-src) bringing a
[fast HTML5 compatible parser](https://github.com/lexborisov/myhtml) to PHP.

## Usage

```php
    function html5_parse(string $html [, string $encoding = "UTF-8" ]) : DOMDocument
```

```php
    <?php
    $document = html5_parse('<html>Hello World!</html>');
    $document->formatOutput = true;
    print($document->saveHTML());

    $body = $document->getElementsByTagName('body')[0];
    var_dump(get_class($body));
```

provides

```
    <!DOCTYPE html>
    <html>
    <head></head>
    <body>Hello World!</body>
    </html>
    string(10) "DOMElement"
```

## Reqirements

* php development package
* libxml2 development package
* myhtml

## Installation

```
git clone 'https://github.com/afflerbach/html5.git'
cd html5
phpize
./configure
make
sudo make install
```

## License

BSD
