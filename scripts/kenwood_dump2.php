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

echo "#include <HashMap.h>\n";
echo "\n";
echo "#define KENWOOD_CODE_LENGTH 32\n";
echo "\n";
$n = count($remote['codes']);
echo "const byte KENWOOD_HASH_SIZE = $n;\n";
echo "HashType<char*,unsigned long> kenwoodHashType[KENWOOD_HASH_SIZE];\n";
echo "HashMap<char*,unsigned long> kenwood_code_map = HashMap<char*,unsigned long>(kenwoodHashType, KENWOOD_HASH_SIZE);\n";
echo "\n";
$i = 0;
echo "void load_kenwood_codes()\n{\n";
echo "  // Name: ".$remote['name']."\n";
foreach($remote['codes'] as $key => $code)
{
    echo "  ";
    echo "kenwood_code_map[$i]("; 
    echo '"'.$key.'"';
    echo ", ";
    echo $remote['pre_data'];
    echo substr($code,2);
    echo ");";
    echo "\n";
    $i++;
}
echo "}\n\n";
