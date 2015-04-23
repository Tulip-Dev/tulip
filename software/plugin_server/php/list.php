<?php

/*
list.php

Mandatory arguments:
 . arch (fixed string)     : only select plugin available for the given architecture
 . os (fixed string) : only select plugin available for the given operating system

Optional arguments
 . category (fixed string) : filter on the plugin's category
 . name (regexp)           : filter on the plugin's name

Return:
 . A JSON list of matching plugins:

 Note: Where path is a relative path (starting from the server root) to where the plugin is located
*/

require_once('pluginserver.php');

$arch=$_GET['arch'];
$os=$_GET['os'];
$category=$_GET['category'];
$name=$_GET['name'];
$tulip=$_GET['tulip'];

$results = find_plugin($name,$os,$arch,$category,$tulip);
echo "[\n";
foreach($results as $r) {
  echo "\t{\n";
  foreach($r as $k => $v) {
    echo "\t\t".'"'.$k.'": "'.$v.'"';
    end($r);
    if ($k !== key($r)) {
      echo ",";
    }
    echo "\n";
  }
  echo "\t}";
  if ($r !== end($results)) {
    echo ',';
  }
  echo "\n";
}
echo "]";


?>
