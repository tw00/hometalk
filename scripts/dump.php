#!/usr/bin/env php
<?php

if($argc == 2) {
    $filename = $argv[1];
} else {
    die('Usage: dump <filename>'."\n");
}

$text = file_get_contents($filename);
$remotes = unserialize($text);

print_r($remotes);
