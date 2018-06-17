<?php
	require_once("connectDB.php");

	//$page = $_POST['page'];
	$time = $_POST['time'];
	$timeU1 = $_POST['timeU1'];
	$timeU2 = $_POST['timeU2'];
	$lengthU = $_POST['lengthU'];
	$lengthV = $_POST['lengthV'];
	$sIP = $_POST['sourceIP'];
	$dIP = $_POST['destinationIP'];
	$protocol = $_POST['protocol'];
	$json = "[";
	$showWhat = "1";

	//$sql_All = "select f.time,e.type from frame as f left join ethernet as e on f.time=e.time";
	$sql_IPv4 = "select i.sourceIP,i.destinationIP,i.protocol,f.len,f.time from ipv4 as i left join frame as f on f.time=i.time where 1=1 ";
	$sql_ARP = "select a.sourceIP,a.destinationIP,f.len,f.time from arp as a left join frame as f on f.time=a.time where 1=1 ";
	
	if($time!=""){

	}

	if($lengthU!="n" && $lengthV!=""){
		$sql_IPv4 = $sql_IPv4 . "and f.len" . $lengthU . $lengthV;
		$sql_ARP  = $sql_ARP . "and f.len" . $lengthU . $lengthV;
	}

	if($sIP!=""){
		$sql_IPv4 = $sql_IPv4 . " and i.sourceIP='" . $sIP . "'";
		$sql_ARP  = $sql_ARP . " and a.sourceIP='" . $sIP . "'";
	}

	if($dIP!=""){
		$sql_IPv4 = $sql_IPv4 . " and i.destinationIP='" . $dIP . "'";
		$sql_ARP  = $sql_ARP . " and a.destinationIP='" . $dIP . "'";	
	}

	if($protocol!="ARP" && $protocol!=""){
		$sql_IPv4 = $sql_IPv4 . " and i.protocol='". $protocol . "'";
		$showWhat="2";
	}
	else if($protocol=="ARP")$showWhat="3";

	switch($showWhat){
		case "1":{
			$json = $json . buildIPv4($sql_IPv4,$db,$json);
			$json = $json . "," . buildARP($sql_ARP,$db,$json);
			break;
		}
		case "2":{
			$json = $json . buildIPv4($sql_IPv4,$db,$json);
			break;
		}
		case "3":{
			$json = $json . buildARP($sql_ARP,$db,$json);
			break;
		}
		default:{
			echo "error";
			break;
		}
	}

	$json = $json."]";
	echo $json;
?>

<?php
    function buildIPv4($sql_IPv4,$db){
    	$query_IPv4 = $db->prepare($sql_IPv4);
    	$query_IPv4->execute();
    	$tmp = "";

    	while($row = $query_IPv4->fetch()){
    		$tmp = $tmp . "{\"sIP\":\"" . $row['sourceIP'] . "\",\"dIP\":\"" . $row['destinationIP'] . "\",\"protocol\":\"". $row['protocol'] . "\",\"len\":\"". $row['len'] . "\",\"time\":\"". $row['time'] . "\"},";
    	}	
    	$json_IPv4 = ($tmp!=""?substr($tmp,0,strlen($tmp)-1):"");
    	return $json_IPv4;
    }
?>

<?php
    function buildARP($sql_ARP,$db){
    	$query_ARP = $db->prepare($sql_ARP);
    	$query_ARP->execute();
    	$tmp = "";

    	while($row = $query_ARP->fetch()){
    		$tmp = $tmp . "{\"sIP\":\"" . $row['sourceIP'] . "\",\"dIP\":\"" . $row['destinationIP'] . "\",\"protocol\":\"ARP\",\"len\":\"". $row['len'] . "\",\"time\":\"". $row['time'] . "\"},";
    	}
    	$json_ARP = ($tmp!=""?substr($tmp,0,strlen($tmp)-1):"");

    	return $json_ARP;
    }
?>