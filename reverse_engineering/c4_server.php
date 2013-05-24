#!/usr/bin/env php
<?php

include "functions/socket_read_normal.php";

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
        echo "  Arguments: ";
        print_r($params);
    }
    $res = call_user_func_array(array($class, $func), $params);
    $res = ereg_replace("[\t\n\r]"," ",$res).chr(0);
    if($debug) {
        echo "Result:\n  ";
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

    function GetVariable( $iddevice, $idvariable )
    {
    }
}

// -----------------------------------------------------------------
function signal_handler($signal)
{
    global $socketD, $client;

    switch($signal) {
        case SIGTERM:
            print "Caught SIGTERM\n";
        case SIGKILL:
            print "Caught SIGKILL\n";
        case SIGINT:
            print "Caught SIGINT\n";
    }
    if( $client ) {
        print "Shutting down client ... \n";
        socket_close($client);
    }
    if( $socketD ) {
        print "Shutting down socket ... \n";
        socket_close($socketD);
    }
    exit;
}


// -----------------------------------------------------------------

declare(ticks=1);
pcntl_signal(SIGTERM, "signal_handler");
pcntl_signal(SIGINT, "signal_handler");

$c4 = new c4soapProtocol();
$socketD = false;
$client = false;

$stop = false;
while(!$stop) { // never stop the daemon
   $socketD = socket_create(AF_INET, SOCK_STREAM, SOL_TCP); // create an TCP socket
   echo 'Creating socket... '; // YES, the socket is creating EACH TIME the while loops
  
   if($socketD === FALSE) { //omg, something went wrong
       echo 'socket_create failed: '.socket_strerror(socket_last_error())."\n";
       exit(1);
   }
  
   echo "OK\nBinding the socket on 192.168.11.72:5020 ... ";
   // let the daemon listen on the port 5020 on all interfaces
   if(!socket_bind($socketD, "192.168.11.72", 5020)) {
       socket_close($socketD);
       echo 'socket_bind failed: '.socket_strerror(socket_last_error())."\n";
       exit(1);
   }
  
   socket_listen($socketD);
   echo "OK\nNow ready to accept connections.\nListening on the socket ... \n";
   // so far, our socket is open and bound to a port: it's listening for ONE incoming connection

   $client = socket_accept($socketD);

   echo "Client: $client \n==============================================\n";
   //echo 'Incoming connection from '.$clientIP.':'.$clientPort.' ... ';

   if($client != false)
   {
       libxml_use_internal_errors(true);
       while(true) {
//          $input = socket_read($client, 1024);
            echo "----------------------------- [ waiting ]\n";
            $xmlstr = socket_read_normal($client, array("\x00"));
            $xml = simplexml_load_string($xmlstr);
            if (!$xml) {
                echo "Laden des XML fehlgeschlagen\n";
                foreach(libxml_get_errors() as $error) {
                    echo "\t", $error->message;
                }
            } else {
                // print_r($xml);
                $res = dispatch($c4, $xml, true);
            }
            // echo "### writing ...\n____________________________\n";
            // print_r($output);
            echo "\n\n";
            socket_write($client, $res);
       }
       socket_close($client);
   }

   echo 'connected'."\n"; // cool, now we can reply to the client
   socket_close($socketD);
   $stop = true;
}
