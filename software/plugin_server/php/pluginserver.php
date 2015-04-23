<?php

function contains($str,$substring) {
  if (empty($substring))
    return true;
  return (stripos($str,$substring) !== false);
}

function is_available($path,$os,$arch) {
  return file_exists($path.'/data-'.$os.$arch.'.zip');
}

function find_plugin($name,$os,$arch,$category,$tulip) {
  $serverxml = simplexml_load_file('server.xml');
  $result = array();
  $pluginsxml = $serverxml->xpath("plugins");
  foreach($pluginsxml[0] as $plugin) {
	$attr = $plugin->attributes();
	$attr_name = $attr['name'];
	$pluginname = (string)$attr_name;
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

function data_url($name,$os,$arch,$tulip) {
  $find_results = find_plugin($name,$os,$arch,"",$tulip); 
  $plugindesc = $find_results[0];
  return 'http://tulip.labri.fr/'.dirname($_SERVER['REQUEST_URI']).'/'.$plugindesc['path'].'/data-'.$os.$arch.'.zip';
}

function fetch_plugin($name,$os,$arch,$tulip) {
  header('Location: '.data_url($name,$os,$arch,$tulip));
}

?>
