#!/usr/bin/env php
<?php

$xmlstr = '<c4soap name="AuthenticateLicensedDevice" async="False"><param name="manufactureid" type="string">ControlUIResidentialIPad</param><param name="deviceid" type="string">b9e2f893c856b21b3123bb83449c5e4e2679437d</param></c4soap>';
$N = 10000;

$before = microtime(true);
for ($i=0 ; $i<$N ; $i++) {
    $xml = new SimpleXMLElement($xmlstr);
}
print_r($xml);
$after = microtime(true);
echo ($after-$before) . " sec\n";

$before = microtime(true);
for ($i=0 ; $i<$N ; $i++) {
    $dom = new domDocument;
    $dom->loadXML($xmlstr);
    /*if (!$dom) {
        echo 'Fehler beim Parsen des Dokuments';
        exit;
    }*/
    $s = simplexml_import_dom($dom);
}
print_r($s);
$after = microtime(true);
echo ($after-$before) . " sec\n";

$before = microtime(true);
for ($i=0 ; $i<$N ; $i++) {
    $xml = simplexml_load_string($xmlstr);
}
print_r($xml);
$after = microtime(true);
echo ($after-$before) . " sec\n";

print_r( (string)$xml['name'] );
echo "\n";
$params = array();
foreach($xml->param as $p) {
    $key = (string)$p['name'];
    $type = (string)$p['type'];
    switch( $type ) {
        case 'string':
            $value = (string)$p;
            break;
        case 'number':
            $value = floatval($p);
            break;
        default:
            $value = $p;
            echo "Unknown type <".$type.">";
            break;
    }
    $params[$key] = $value;
}
print_r($params);
