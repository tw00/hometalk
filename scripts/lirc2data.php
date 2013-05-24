#!/usr/bin/env php
<?php

if($argc == 2) {
    $filename = $argv[1];
} else {
    die('Usage: lirc2data <filename>'."\n");
}

$text = file_get_contents($filename);
$lines = explode("\n", $text);

$count = 0;
$kv_array = array();
foreach($lines as $nr => $line)
{
    $line = trim($line);
    $line = preg_replace(array('/\s{2,}/', '/[\t\n]/'), ' ', $line);
    if($line[0] == '#' or $line == '') continue;

    $words = explode(' ', $line);
    $key = $words[0];
    unset($words[0]);
    if( count($words) == 1 ) {
        $value = end(array_values($words));
    } else {
        $value = array_values($words);
        foreach($value as $k => $v) {
            if( is_int($v) )
                $value[$k] = intval($v);
        }
    }
    $kv_array[$count] = array('k' => $key, 'v' => $value);
    $count++;
}

$results = array();
$remotes = array();
foreach($kv_array as $kv)
{
    $key = $kv['k'];
    $value = $kv['v'];

    if( $key == 'begin' )
    {
        $mode = $value;
        $results[$mode] = array();
        continue;
    }
    if( $key == 'end' )
    {
        if( $value == 'remote')
        {
            $remote = $results['remote'];
            $remote['codes'] = $results['codes'];
            $name = $remote['name'];
            $remotes[$name] = $remote;
            $results = array();
        }
        continue;
    }
    $results[$mode][$key] = $value;
}

echo serialize($remotes);
