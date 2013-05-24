#!/usr/bin/env php
<?php

if($argc == 2) {
    $filename = $argv[1];
} else {
    die('Usage: kenwood_dump <filename>'."\n");
}

$text = file_get_contents($filename);
$remotes = unserialize($text);

$remote = $remotes['Kenwood_RC-R0825_Tuner'];
//print_r($remote);

echo "// Name: ".$remote['name']."\n";
echo "\n";
echo "typedef struct {\n";
echo "    const char* key;\n";
echo "    unsigned long data;\n";
echo "}  remote_code;\n";
echo "\n";
$n = count($remote['codes']);
echo "remote_code kenwood_codes[$n];\n";
echo "\n";
$i = 0;
echo "void load_kenwood_codes()\n{\n";
foreach($remote['codes'] as $key => $code)
{
    echo "    ";
    echo "kenwood_codes[$i] = "; 
    echo "(remote_code) {"; 
    echo '"'.$key.'"';
    echo ", ";
    echo $remote['pre_data'];
    echo substr($code,2);
    echo "};";
    echo "\n";
    $i++;
}
echo "}\n\n";
