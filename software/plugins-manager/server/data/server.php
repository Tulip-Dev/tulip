<?php

require_once("nusoap/lib/nusoap.php");

// Create the soap Object
$server = new soap_server;

// Register a method available for clients
$server->register('readCSV');
$server->register('getNbColumns');
$server->register('getNbRows');
$server->register('getNbElements');
$server->register('getVersion');
$server->register('getPluginMD5');
$server->register('getPluginForLinux');
$server->register('getBinary');
$server->register('getPluginXMLList');
$server->register('getPluginXMLInfo');
$server->register('getPluginXMLDoc');
$server->register('getServerName');
$server->register('downloadPlugin');
$server->register('connect');

// Return the results.
$server->service($HTTP_RAW_POST_DATA);

function scandirPHP4($dir) {
  $dirArray = array();
  if ($handle = opendir($dir)) {
    while (false !== ($file = readdir($handle))) {
      if ($file != "." && $file != "..") {
	array_push($dirArray,basename($file));
      }
    }
    closedir($handle);
  }
  return $dirArray;
}

function buildPluginsListForOneFilePHP5($outDoc,$outNode,$file){
  $linuxFileName=str_replace("xml","so",$file).".i386";
  $macFileName=str_replace("xml","so",$file).".mac";
  $windobFileName=str_replace("xml","dll",$file);

  $linuxVersion="false";
  $macVersion="false";
  $windowVersion="false";
  if(file_exists($linuxFileName)){
    $linuxVersion="true";
  }
  if(file_exists($macFileName)){
    $macVersion="true";
  }
  if(file_exists($windobFileName)){
    $windobVersion="true";
  }

  $doc = new DOMDocument();
  $doc->load($file);
  
  $nodes = $doc->getElementsByTagName("*");
  if(!$nodes)
    return "error in : ".$file; 
  $node = $nodes->item(0);

  $outNode->setAttribute("name",$node->getAttribute("name"));
  $outNode->setAttribute("version",$node->getAttribute("version"));
  $outNode->setAttribute("type",$node->getAttribute("type"));
  $outNode->setAttribute("displayType",$node->getAttribute("displayType"));
  $outNode->setAttribute("fileName",$node->getAttribute("fileName"));
  $outNode->setAttribute("linuxVersion",$linuxVersion);
  $outNode->setAttribute("macVersion",$macVersion);
  $outNode->setAttribute("windowsVersion",$windobVersion);

  $deps = $node->getElementsByTagname("dependency");
  foreach($deps as $dep) {
    $outDepNode = $outDoc->createElement("dependency");
    $outDepNode = $outNode->appendChild($outDepNode);
    $outDepNode->setAttribute("name",$dep->getAttribute("name"));
    $outDepNode->setAttribute("version",$dep->getAttribute("version"));
    $outDepNode->setAttribute("type",$dep->getAttribute("type"));
  }
  return "";
}

function buildPluginsListForOneFilePHP4($outDoc,$outNode,$file){
  $linuxFileName=str_replace("xml","so",$file).".i386";
  $macFileName=str_replace("xml","so",$file).".mac";
  $windobFileName=str_replace("xml","dll",$file);

  $linuxVersion="false";
  $macVersion="false";
  $windowVersion="false";
  if(file_exists($linuxFileName)){
    $linuxVersion="true";
  }
  if(file_exists($macFileName)){
    $macVersion="true";
  }
  if(file_exists($windobFileName)){
    $windobVersion="true";
  }

  if(!$doc = domxml_open_file($file)) {
    return "error in : ".$file;
  }
  $nodes = $doc->get_elements_by_tagname("pluginInfo");
  if(!$nodes)
    return "error in : ".$file; 
  $node = $nodes[0];
  
  $outNode->set_attribute("name",$node->get_attribute("name"));
  $outNode->set_attribute("version",$node->get_attribute("version"));
  $outNode->set_attribute("type",$node->get_attribute("type"));
  $outNode->set_attribute("displayType",$node->get_attribute("displayType"));
  $outNode->set_attribute("fileName",$node->get_attribute("fileName"));
  $outNode->set_attribute("linuxVersion",$linuxVersion);
  $outNode->set_attribute("macVersion",$macVersion);
  $outNode->set_attribute("windowsVersion",$windowVersion);

  $deps = $node->get_elements_by_tagname("dependency");
  foreach($deps as $key => $dep) {
    $outDepNode = $outDoc->create_element("dependency");
    $outDepNode = $outNode->append_child($outDepNode);
    $outDepNode->set_attribute("name",$dep->get_attribute("name"));
    $outDepNode->set_attribute("version",$dep->get_attribute("version"));
    $outDepNode->set_attribute("type",$dep->get_attribute("type"));
  }
  return "";
}

function buildPluginsListForOneDirPHP5($doc,$node,$currentDir) {
 
  $filesList = scandir($currentDir);

  foreach($filesList as $file) {
    if ($file != "." && $file != "..") {
      if(is_dir($currentDir.$file)) {
	$result = buildPluginsListForOneDirPHP5($doc,$node,$currentDir.$file."/");
	if($result!="")
	  return $result;
      }else{
	$pos = strstr($file,"xml");
	if($pos!==false) {
	  $tmp = $doc->createElement("plugin");
	  $tmp = $node->appendChild($tmp);
	  $result = buildPluginsListForOneFilePHP5($doc,$tmp,$currentDir.$file);
	  if($result!="") {
	    return $result;
	  }
	}
      }
    }
  }
  return "";
}

function buildPluginsListForOneDirPHP4($doc,$node,$currentDir) {
  
  $filesList = scandirPHP4($currentDir);

  foreach($filesList as $key => $file) {
    if(is_dir($currentDir.$file)) {
      buildPluginsListForOneDirPHP4($doc,$node,$currentDir.$file."/");
    }else{
      $pos = strstr($file,"xml");
      if($pos!==false) {
	$tmp = $doc->create_element("plugin");
	$tmp = $node->append_child($tmp);
	$result = buildPluginsListForOneFilePHP4($doc,$tmp,$currentDir.$file);
	if($result!="") {
	  return $result;
	}
      }
    }
      //$filesList.fwrite($filesList,$currentDir.$file);
  }
  return "";
}

function buildPluginsListPHP5() {
  $doc = new DOMDocument;
  $doc->formatOutput = true;
  
  $node = $doc->createElement("pluginsList");
  $node = $doc->appendChild($node);

  $result = buildPluginsListForOneDirPHP5($doc,$node,"./plugins/");

  if($result!="")
    return $result;

  return $doc->saveXML();
}

function buildPluginsListPHP4() {
  $doc = domxml_new_doc('1.0');
  
  $node = $doc->create_element("pluginsList");
  $node = $doc->append_child($node);

  $result = buildPluginsListForOneDirPHP4($doc,$node,"./plugins/");

  if($result!="")
    return $result;

  return $doc->dump_mem();
}
  

// Check integrity of plugins list
function checkPluginsListIntegrity() {
  if(file_exists("filesList.txt")){
    $handle = fopen("filesList.txt", "r");
    while(!feof($handle)) {
      $line = stream_get_line($handle,1024,"\n");
      if(!file_exists($line)){
	return false;
      }
    }
  }else{
    return false;
  }
  return true;
}

// CSV Read
function readCSV($fileName, $row, $column){
  
  $handle = fopen($fileName, "r");
  
  $nbRowsRead = 1;
  $returnValue = "";

  while( ($data = fgetcsv($handle, 1000, ",")) !== FALSE){
    $nbColumns = count($data);
    
    if($nbRowsRead == $row
       && $column <= $nbColumns){
      $returnValue = $data[$column-1];
      break;
    }

    $nbRowsRead ++;
  }

  fclose($handle);
  return $returnValue;
}

// CSV nbColumns

function getNbColumns($fileName){
  $handle = fopen($fileName, "r");
  
  $data = fgetcsv($handle, 1000, ",");
  fclose($handle);  
  return count($data);
  
}



// CSV nbRows

function getNbRows($fileName){

  $handle = fopen($fileName, "r");
  
  $nbRowsRead = 0;

  while( ($data = fgetcsv($handle, 1000, ",")) !== FALSE){
    $nbRowsRead ++;
  }

  fclose($handle);
  return $nbRowsRead;
}


function getNbElements($fileName,$elementName){
  $handle = fopen($fileName, "r");
  
  $nbElementsRead = 0;

  while( ($data = fgetcsv($handle, 1000, ",")) !== FALSE){
    $arraySize=count($data);
    for($i=0;$i<$arraySize;$i++)
      if($data[$i]==$elementName)
	$nbElementsRead ++;	
  }

  fclose($handle);
  return $nbElementsRead;
}

function getVersion(){
  return "Tulip plugins WS Version 0.0.1";
}


// Plugin Binary

function getPluginMD5($pluginName){
  // :TODO: tmp : get filename in database, for pluginName
  $filename = "testFile.bin";

  return md5_file($filename);
}

function getPluginForWindows($pluginName){
  $completeName = "./plugins/Windows/" . $pluginName;
  return getBinary($completeName);
}

function getPluginForLinux($pluginName){
  $fileName = "./plugins/" . $pluginName;
  
  return getBinary($fileName);
}

function getPluginForMacOSX($pluginName){
  $completeName = "./plugins/MacOSX/" . $pluginName;
  return getBinary($completeName);
}

function getBinary($fileName){
  $contents="";
  if(file_exists($fileName)){
    $handle = fopen($fileName, "rb"); 
    $fileSize=filesize($fileName);
    for($i=0;$i<$fileSize;$i++){
      $contents .= bin2hex(fgetc($handle));
    }
    fclose($handle);
  }else{
    $contents.="File ". $fileName." doesn't exist !";
  }
  return $contents;
}

function getPluginXMLList(){
  $content=buildPluginsListPHP5();
  return $content;
}

function getPluginXMLInfo($pluginFileName,$pluginVersion){
  $fileName="./plugins/".$pluginFileName."/".$pluginFileName.".xml.".$pluginVersion;
  $content="File ".$fileName." doesn't exist";
  if(file_exists($fileName)){
    $handle = fopen($fileName, "r");
    $content="File exist";
    $content = fread($handle, filesize($fileName));
  }
  return $content;
}

function getPluginXMLDoc($pluginFileName,$pluginVersion){
  $fileName="./plugins/".$pluginFileName."/".$pluginFileName.".doc.".$pluginVersion;
  $content="File ".$fileName." doesn't exist";
  if(file_exists($fileName)){
    $handle = fopen($fileName, "r");
    $content="File exist";
    $content = fread($handle, filesize($fileName));
  }
  return $content;
}

function getServerName(){
  $fileName = "serverName.txt";
  $handle = fopen($fileName, "r");
  
  $data = fread($handle, filesize($fileName));
  fclose($handle); 
  return $data;
}

function downloadPlugin($name){
  $fileName="DownloadList.txt";
  $handle = fopen($fileName, "a+");
  fwrite($handle, $_SERVER["REMOTE_ADDR"]." : ".$name."\n");
  fclose($handle);
}

function connect() {
  $fileName="IPList.txt";
  $handle = fopen($fileName, "a+");
  fwrite($handle, date("c").";".$_SERVER["REMOTE_ADDR"]."\n");
  fclose($handle);
}
?>
