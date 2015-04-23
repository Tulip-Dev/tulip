<?php

/*
fetch.php

Mandatory arguments:
 . arch (fixed string)     : only select plugin available for the given architecture
 . os (fixed string)       : only select plugin available for the given operating system
 . name (fixed string)     : The name of the plugin to fetch

Return:
 . Redirects to the plugin's data file.

*/

require_once('pluginserver.php');

$arch=$_GET['arch'];
$os=$_GET['os'];
$name=$_GET['name'];
$tulip=$_GET['tulip'];

fetch_plugin($name,$os,$arch,$tulip);
