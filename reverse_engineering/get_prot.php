#!/usr/bin/env php
<?php
$arr=array("ip","icmp","ggp","tcp",
    "egp","pup","udp","hmp","xns-idp",
    "rdp","rvd" );
//Reads the names of protocols into an array..
for($i=0;$i<11;$i++)
{
    $proname=$arr[$i];
    echo $proname .":", getprotobyname ($proname)."\n";
}

?>
