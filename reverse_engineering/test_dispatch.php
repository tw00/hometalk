#!/usr/bin/env php
<?php

// -----------------------------------------------------------------

function dispatch($class, $xml, $debug = false)
{
    $func = (string)$xml['name'];
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
    if($debug) {
        echo "-----------------------------\nCalling:\n";
        echo "  function " . $func . "( ";
        $list = array();
        foreach($params as $key => $value) $list[] = '$' . $key;
        echo implode(", ", $list);
        echo " )\n";
        echo "Arguments: ";
        print_r($params);
    }
    $res = call_user_func_array(array($class, $func), $params);
    if($debug) {
        echo "Result: ";
        print_r($res);
        echo "\n";
    }
    return $res;
}

// -----------------------------------------------------------------
class c4soapProtocol
{
    function AuthenticateLicensedDevice( $manufactureid, $deviceid )
    {
        return '<c4soap name="AuthenticateLicensedDevice" result="1"></c4soap>';
    }

    function GetSystemItems( $filter, $async )
    {
        return '<c4soap name="GetNetworkBindings" result="1"></c4soap>';
    }

    function GetNetworkBindings()
    {
        return '<c4soap name="GetNetworkBindings" result="1"></c4soap>';
    }

    function GetVersionInfo()
    {
        return '<c4soap name="GetVersionInfo" result="1">
                    <versions>
                        <version name="Director" version="1.3.2.438" buildtype="" builddate="Feb 19 2008" buildtime="20:49:26"/>
                        <version name="MediaManager" version="1.3.2.438" buildtype="" builddate="Feb 19 2008" buildtime="21:01:48"/>
                    </versions>
                </c4soap>';
    }
}
// -----------------------------------------------------------------

$xmlstr = '<c4soap name="AuthenticateLicensedDevice" async="False"><param name="manufactureid" type="string">ControlUIResidentialIPad</param><param name="deviceid" type="string">b9e2f893c856b21b3123bb83449c5e4e2679437d</param></c4soap>';
$xmlstr = '<c4soap name="GetVersionInfo" async="False" seq="5"/>';
//$xmlstr = '<c4soap name="GetNetworkBindings" async="False" seq="6"/>';
//$xmlstr = '<c4Soap name="GetSystemItems"> <param name="filter" type="number">0</param> <param name="async" type="number">0</param> </c4Soap>';

$xml = simplexml_load_string($xmlstr);
print_r($xml);

$c4 = new c4soapProtocol();
$res = dispatch($c4, $xml, true);

$output = ereg_replace("[ \t\n\r]","",$res).chr(0);
print_r($output);


echo "\n\n";
echo ord( $output{strlen($output)-1} );
