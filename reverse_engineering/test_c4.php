#!/usr/bin/env php
<?php

include "functions/socket_read_normal.php";

// example of "daemon" in UDP
$stop = false;
while(!$stop) { // never stop the daemon
//   $socketD = socket_create(AF_INET, SOCK_DGRAM, SOL_UDP); // create an UDP socket
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

   // this is a special way of socket_read()'ing what's on the socket once someone establishes a connection
   /*socket_recvfrom($socketD, $buf, 65535, 0, $clientIP, $clientPort);
   if($buf === FALSE) { // something went wrong
       echo 'socket_read() returned false : '.socket_strerror(socket_last_error())."\n";
       continue;
   } elseif(strlen($buf) === 0) { // this should mean "client closed the connection"
       echo 'socket_read() returned an empty string : '.socket_strerror(socket_last_error())."\n";
       continue;
   }*/
   $client = socket_accept($socketD);

   echo "Client\n=========\n";
   print_r($client);
   //echo 'Incoming connection from '.$clientIP.':'.$clientPort.' ... ';
   //
   if($client != false)
   {
       libxml_use_internal_errors(true);
       while(true) {
//            $input = socket_read($client, 1024);
            echo "----------------------------- [ waiting ]";
            $input = socket_read_normal($client, array("\x00"));
            $xml = simplexml_load_string($xmlstr);
            if (!$xml) {
                echo "Laden des XML fehlgeschlagen\n";
                foreach(libxml_get_errors() as $error) {
                    echo "\t", $error->message;
                }
            } else {
                print_r($xml);
                $name = (string)$xml['name'];
            }
       }
       socket_close($client);
   }

   // ok now thats the tricky part
   // if you want to reply to the client on the UDP socket it opened, you need to connect your listening socket on them
   // this won't work unless the client puts their IP in the IP  header of the packets it's sending to your socket
   // UDP sessions don't require them to do so, and if they don't, you have no way to tell who's talking to you on the socket, and therefore no way to reply
   // so if you want to interact with the client, you have to connect your socket on them, but by doing so, you're no longer listening to other incoming connections
   // one way to fixe that would be to fork the script when an incoming connection arrives and then stop the forked script once the client exits
/*   if(!socket_connect($socketD, $clientIP, $clientPort)) {
       // echo 'socket_create failed to connect the clientsock on '.$clientIP.':'.$clientPort.' : '.socket_strerror(socket_last_error())."\n";
       socket_close($socketD);
       continue;
}*/
   echo 'connected'."\n"; // cool, now we can reply to the client
   socket_close($socketD);

   // now you can interact with the client using socket_write() and socket_read()

   // once you're finished, you need to socket_close() your sockets and let the while loop and re-create the listenning socket (because you're no longer listenning for new incoming connections)
   $stop = true;
} // end of the infinite while

?>
