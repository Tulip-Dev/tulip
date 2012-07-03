<?php

function contains($str,$substring) {
  if (empty($substring))
    return true;
  return (stripos($str,$substring) !== false);
}

function is_available($path,$os,$arch) {
  return file_exists($path.'/data-'.$os.$arch.'.zip');
}

function data_url($name,$os,$arch,$tulip) {
  $plugindesc = find_plugin($name,$os,$arch,"",$tulip)[0];
  return 'http://'.$_SERVER['SERVER_ADDR'].dirname($_SERVER['REQUEST_URI']).'/'.$plugindesc['path'].'/data-'.$os.$arch.'.zip';
}

function find_plugin($name,$os,$arch,$category,$tulip) {
  $serverxml = simplexml_load_file('server.xml');
  $result = array();
  foreach($serverxml->xpath("plugins")[0] as $plugin) {
    $pluginname = (string)$plugin->attributes()['name'];
    // Check if plugin name matches query
    if (contains($pluginname,$name) === true) {
      $plugindesc = array();
      foreach($plugin->attributes() as $id => $value) {
        $plugindesc[$id]=(string)$value;
      }
      if (is_available($plugindesc['path'],$os,$arch) && contains($plugindesc['category'],$category) && $plugindesc['tulip'] === $tulip)
        array_push($result,$plugindesc);
    }
  }
  return $result;
}


function fetch_plugin($name,$os,$arch,$tulip) {
  header('Location: '.data_url($name,$os,$arch,$tulip));
}

?>
