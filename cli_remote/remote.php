#!/usr/bin/php
<?php

// ===========================================================================

function getch()
{
    return exec('./cbreak');
}


function str_is_int($v)
{
    $i = intval($v);
    if ("$i" == "$v") return true;
    return false;
}

function check_argv($trigger, &$argv)
{
    if( $i = array_search($trigger, $argv ) ) {
        unset($argv[$i]);
        return true;
    } 
    return false;
}

function print_map_info($map)
{
    echo "Code\t\tKey\n-----------------------\n";
    foreach($map as $code => $key) {
        if(strlen($code) < 8) {
            echo "$code\t\t$key\n";
        } else {
            echo "$code\t$key\n";
        }
    }
}

// ===========================================================================

$VERBOSE_MODE = false;
$INFO_MODE = false;
$DRY_MODE = false;

if($argc >= 2) {
    $VERBOSE_MODE = check_argv('-v', $argv);
    $INFO_MODE = check_argv('-i', $argv);
    $DRY_MODE = check_argv('-d', $argv);
    $argv = array_values($argv);
    $filename = $argv[1];
} else {
    die('Usage: remote [-v] [-i] [-d] <mapfile>'."\n");
}

if( !file_exists( $filename ) ) {
    die("file <$filename> does not exist.\n");
}

try {
    $ini = parse_ini_file($filename, true);
} catch(Exception $e) {
    die("Parse error");
}
if( $ini == false ) die();

if( !array_key_exists('CLIRemoteMap', $ini) ) {
    die("CLIRemoteMap not found in ini file\n");
}

$map = $ini['CLIRemoteMap'];
if( $VERBOSE_MODE ) print_r($map);
if( $INFO_MODE ) print_map_info($map);

$keymap = array();
foreach($map as $code => $key)
{
    $key = trim($key);
    if( $VERBOSE_MODE ) echo $key . " > ";
    if( str_is_int($key) ) {
        $key = intval($key);
    } else if( strlen($key) == 1 ) {
        $key = ord($key[0]);
    } else {
        echo "WARNING: <$key> is not a valid key\n";
    }
    if( $VERBOSE_MODE ) echo "$key\n";

    if( array_key_exists($key, $keymap ) ) {
        $code_old = $keymap[$key];
        echo "WARNING: <$code>: <$key> alread exists for <$code_old>\n";
    }
    $keymap[$key] = $code;

}
if( $VERBOSE_MODE ) print_r($keymap);

// ===========================================================================
include "php-serial/php_serial.class.php";
// TODO: Allow Different Devices

$serial = new phpSerial;
$serial->deviceSet("/dev/tty.usbmodemfa131");
$serial->confBaudRate(9600);
$serial->confParity("none");
$serial->confCharacterLength(8);
$serial->confStopBits(1);
$serial->confFlowControl("none");
// ===========================================================================

while(true) {
    $key = getch();
    echo "KEY: $key";
    if( array_key_exists($key, $keymap) ) {
        $cmd = $keymap[$key];
        echo " > [$cmd]";
        // $cmd = "echo -n '".trim($cmd)."!' > /dev/tty.usbmodemfa131 2>&1";
//        $cmd = "echo -n '".trim($cmd)."!' > ./foo.txt 2>&1";
        if( !$DRY_MODE ) {
            /*
            $fd = dio_open('/dev/tty.usbmodemfa131', O_RDWR | O_NOCTTY | O_NONBLOCK);
            dio_fcntl($fd, F_SETFL, O_SYNC);
            dio_tcsetattr($fd, array(
                  'baud'   => 9600,
                  'bits'   => 8,
                  'stop'   => 1,
                  'parity' => 0
            )); 
            dio_write($fd, $cmd . "!");
            dio_close($fd);*/

            $serial->deviceOpen();
            $serial->sendMessage($cmd.'!');
            $serial->deviceClose();

            #shell_exec($cmd);
        } else {
            $cmd = escapeshellcmd($cmd);
            echo "\t> \"$cmd\"";
        }
    } elseif($key == 63) {
        echo " > Help ...\n";
        print_map_info($map);
    } else {
        echo " > Command not found ...";
    }
    echo "\n";
}
