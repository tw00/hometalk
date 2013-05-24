#!/usr/bin/env php
<?php

$fp = fsockopen("192.168.11.85", 3001, $errno, $errstr, 30);
if (!$fp) {
        echo "Error: $errstr ($errno)\n";
} else {
    $out = "test!\r\n";
    fwrite($fp, $out);
    #while (!feof($fp)) {
    #    echo fgets($fp, 128);
    #}
    fclose($fp);
}
