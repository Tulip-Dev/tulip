<?php

require_once("nusoap/lib/nusoap.php");

// Create the soap Object
$server = new soap_server;

// Register a method available for clients (V1 & V2)
$server->register('getVersion');
$server->register('connect');
$server->register('getServerName');
$server->register('downloadPlugin');

// Register a method available for clients Version 1
$server->register('getPluginXMLList');
$server->register('getPluginXMLInfo');
$server->register('getPluginXMLDoc');

// Register a method available for clients Version 2
$server->register('getPluginXMLList_v2');
$server->register('getPluginXMLInfo_v2');
$server->register('getPluginXMLDoc_v2');

// Return the results.
$server->service($HTTP_RAW_POST_DATA);

//***************************************************************************************************
// Function for server version 1 (tulip < 3.3.0)
//***************************************************************************************************
function buildPluginsListForOneFilePHP5($outDoc,$outNode,$file){
  $linuxFileName=str_replace("xml","so",$file).".i386";
  $macFileName=str_replace("xml","dylib",$file);
  $windobFileName=str_replace("xml","dll",$file);
  $i64FileName=str_replace("xml","so",$file).".i64";

  $linuxVersion="false";
  $macVersion="false";
  $windobVersion="false";
  $i64Version="false";
  if(file_exists($linuxFileName)){
    $linuxVersion="true";
  }
  if(file_exists($macFileName)){
    $macVersion="true";
  }
  if(file_exists($windobFileName)){
    $windobVersion="true";
  }
  if(file_exists($i64FileName)){
    $i64Version="true";
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
  $outNode->setAttribute("i64Version",$i64Version);

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

//***************************************************************************************************
// Function for server version 2 (tulip >= 3.3.0)
//***************************************************************************************************
function buildPluginsListForOneFilePHP5_v2($outDoc,$outNode,$dirName,$file){
  $doc = new DOMDocument();
  $doc->load($dirName.$file);
  
  $nodes = $doc->getElementsByTagName("*");
  if(!$nodes)
    return "error in : ".$file; 
  $node = $nodes->item(0);

  $linuxFileName=$node->getAttribute("fileName").".so";
  $macFileName=$node->getAttribute("fileName").".dylib";
  $windobFileName=$node->getAttribute("fileName").".dll";

  $linuxVersion="false";
  $macVersion="false";
  $macVersion_ppc="false";
  $macVersion_intel="false";
  $windobVersion="false";
  $linuxVersion_i34="false";
  $windobVersion_i64="false";
  if(file_exists($dirName."i386/".$linuxFileName)){
    $linuxVersion="true";
  }
  if(file_exists($dirName."i386/".$macFileName)){
    $macVersion="true";
  }
  if(file_exists($dirName."i386/".$macFileName)){
    $macVersion_ppc="true";
  }
  if(file_exists($dirName."i386/intel/".$macFileName)){
    $macVersion_intel="true";
  }
  if(file_exists($dirName."i386/".$windobFileName)){
    $windobVersion="true";
  }
  if(file_exists($dirName."i64/".$linuxFileName)){
    $linuxVersion_i64="true";
  }
  if(file_exists($dirName."i64/".$macFileName)){
    $macVersion_i64="true";
  }
  if(file_exists($dirName."i64/".$windobFileName)){
    $windobVersion_i64="true";
  }

  $outNode->setAttribute("name",$node->getAttribute("name"));
  $outNode->setAttribute("version",$node->getAttribute("version"));
  $outNode->setAttribute("type",$node->getAttribute("type"));
  $outNode->setAttribute("displayType",$node->getAttribute("displayType"));
  $outNode->setAttribute("fileName",$node->getAttribute("fileName"));
  $outNode->setAttribute("linuxVersion",$linuxVersion);
  $outNode->setAttribute("macVersion",$macVersion);
  $outNode->setAttribute("macVersion_ppc",$macVersion_ppc);
  $outNode->setAttribute("macVersion_intel",$macVersion_intel);
  $outNode->setAttribute("windowsVersion",$windobVersion);
  $outNode->setAttribute("linuxVersion_i64",$linuxVersion_i64);
  $outNode->setAttribute("macVersion_i64",false);
  $outNode->setAttribute("windowsVersion_i64",$windobVersion_i64);

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

function buildPluginsListForOneDirPHP5_v2($doc,$node,$currentDir) {
  $filesList = scandir($currentDir);

  foreach($filesList as $file) {
    if ($file != "." && $file != "..") {
      if(is_dir($currentDir.$file)) {
		$result = buildPluginsListForOneDirPHP5_v2($doc,$node,$currentDir.$file."/");
		if($result!="")
	  	return $result;
      }else{
		$pos = strstr($file,"xml");
		if($pos!==false) {
	  	  $tmp = $doc->createElement("plugin");
	  	  $tmp = $node->appendChild($tmp);
	  	  $result = buildPluginsListForOneFilePHP5_v2($doc,$tmp,$currentDir,$file);
	  	  if($result!="") {
	        return $result;
	  	  }
		}
      }
    }
  }
  return "";
}

function buildPluginsListPHP5_v2() {
  $doc = new DOMDocument;
  $doc->formatOutput = true;
  
  $node = $doc->createElement("pluginsList");
  $node = $doc->appendChild($node);

  $result = buildPluginsListForOneDirPHP5_v2($doc,$node,"./pluginsV2/");

  if($result!="")
    return $result;

  return $doc->saveXML();
}

function getPluginXMLList_v2(){
  $content=buildPluginsListPHP5_v2();
  return $content;
}

function getPluginXMLInfo_v2($pluginFileName,$pluginVersion){
  $fileName="./pluginsV2/".$pluginFileName.".".$pluginVersion."/".$pluginFileName.".xml";
  $content="File ".$fileName." doesn't exist";
  if(file_exists($fileName)){
    $handle = fopen($fileName, "r");
    $content="File exist";
    $content = fread($handle, filesize($fileName));
  }
  return $content;
}

function getPluginXMLDoc_v2($pluginFileName,$pluginVersion){
  $fileName="./pluginsV2/".$pluginFileName.".".$pluginVersion."/".$pluginFileName.".doc";
  $content="File ".$fileName." doesn't exist";
  if(file_exists($fileName)){
    $handle = fopen($fileName, "r");
    $content="File exist";
    $content = fread($handle, filesize($fileName));
  }
  return $content;
}


//***************************************************************************************************
// Function for server version 1 and version 2
//***************************************************************************************************
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

function getVersion(){
  return "Tulip plugins WS Version 0.0.1";
}

?>
