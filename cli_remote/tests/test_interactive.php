#!/usr/bin/php
<?php

function getch()
{
    return exec('./cbreak');
}

while(true) {
    $key = getch();
    echo "KEY: $key\n";
}
