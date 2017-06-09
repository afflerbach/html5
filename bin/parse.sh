#!/usr/bin/env php
<?php

try {
    $file = $_SERVER['argv'][1] ?? '';

    if ($file === '') {
        throw new Exception("Usage: {$_SERVER['argv'][0]} <HTML file>");
    }

    $html = $file === '-' ? readFromStdin() : readFromFile($file);
    $document = html5_parse($html);
    print($document->saveHTML() . "\n");
} catch (Throwable $e) {
    fprintf(STDERR, $e->getMessage() . "\n");
    exit(1);
}

/* ========================================================================= */

function readFromStdin() : string {
    $text = '';
    while ($line = fgets(STDIN)) {
        $text .= $line;
    }

    return $text;
}

function readFromFile(string $filename) : string {
    if (!is_readable($filename)) {
        throw new Exception("Error: Could not read file '$filename'.");
    }

    return (string) file_get_contents($filename);
}
