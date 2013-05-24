#!/usr/bin/env php
<?php

if($argc == 2) {
    $filename = $argv[1];
} else {
    die('Usage: panasonic_dump <filename>'."\n");
}

$text = file_get_contents($filename);
$remotes = unserialize($text);

$remote = $remotes['Panasonic_N2QAYB000239'];
// print_r($remote);

echo "#include <HashMap.h>\n";
echo "\n";
echo "#define PANASONIC_ADDRESS ".$remote['pre_data']." // Panasonic address (Pre data)\n";
//echo "#define PANASONIC_CODE_LENGTH 32\n";
echo "\n";
$n = count($remote['codes']);
echo "const byte PANASONIC_HASH_SIZE = $n;\n";
echo "HashType<char*,unsigned long> panasonicHashType[PANASONIC_HASH_SIZE];\n";
echo "HashMap<char*,unsigned long> panasonic_code_map = HashMap<char*,unsigned long>(panasonicHashType, PANASONIC_HASH_SIZE);\n";
echo "\n";
$i = 0;
echo "void load_panasonic_codes()\n{\n";
echo "  // Name: ".$remote['name']."\n";
foreach($remote['codes'] as $key => $code)
{
    echo "  ";
    echo "panasonic_code_map[$i]("; 
    echo '"'.$key.'"';
    echo ", ";
    echo $code;
    //echo substr($code,2);
    echo ");";
    echo "\n";
    $i++;
}
echo "}\n\n";
